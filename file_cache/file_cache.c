#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <fcntl.h>
#include "file_cache.h"

#define MAX_FILESIZE	10240
int debug = 0;
#define PRINTF		if (debug) printf

// cache_entry structure
struct cache_entry {
	unsigned char		entry_dirty;
	unsigned long long	entry_pin;
	union entry_namestr {
		char		sname[32];
		char		*lname;
	}entry_name;
	unsigned int		entry_nlen;
	void			*entry_data;
	pthread_mutex_t		entry_mutex;
};

// short file name
#define entry_sname entry_name.sname

//long file name
#define entry_lname entry_name.lname

struct file_cache {
	unsigned long long	cache_total;
	unsigned long long	cache_avail;
	struct cache_entry	**cache_entpp;
	pthread_mutex_t		cache_mutex;
	pthread_cond_t		cache_cond;
};

// Constructor
struct file_cache *
file_cache_construct(
	int	 max_cache_entries)
{
	struct file_cache	*cp = NULL;
	struct cache_entry	**epp = NULL;
	int 			cond_init = 0;
	int 			mutex_init = 0;
	int			alloc_fail = -1;
	int			mutex_fail = -1;
	int			i = 0;

	cp = (struct file_cache *)malloc(sizeof (struct file_cache));
	if (cp == NULL) {
		return NULL;
	}
	if (pthread_cond_init(&cp->cache_cond, NULL)) {
		goto errout;
	}
	cond_init = 1;
	if (pthread_mutex_init(&cp->cache_mutex, NULL)) {
		goto errout;
	}
	mutex_init = 1;
	epp = (struct cache_entry **)malloc(max_cache_entries * sizeof (void  *));
	if (epp == NULL) {
		goto errout;
	}
	for (i = 0; i < max_cache_entries; i++) {
		epp[i] = (struct cache_entry *)calloc(1, sizeof (struct cache_entry));
		if (epp[i] == NULL) {
			alloc_fail = i-1;
			mutex_fail = i-1;
			goto errout;
		}
		if (pthread_mutex_init(&(epp[i]->entry_mutex), NULL)) {
			mutex_fail = i-1;
			alloc_fail = i;
			goto errout;
		}
	}
	cp->cache_entpp = epp;
	cp->cache_total = max_cache_entries;
	cp->cache_avail = max_cache_entries;
	return cp;

errout:
	if (cond_init) {
		(void) pthread_cond_destroy(&(cp->cache_cond));
	}
	if (mutex_init) {
		(void) pthread_mutex_destroy(&(cp->cache_mutex));
	}
	for (i = 0; i <= mutex_fail; i++) {
		(void) pthread_mutex_destroy(&(epp[i]->entry_mutex));
	}
	for (i = 0; i <= alloc_fail; i++) {
		free(epp[i]);
	}
	free(cp);
	return NULL;
}

// flushing a dirty cache entry
// mutex of the cache entry is locked on entry/exit
void
file_cache_flush_file(
	struct cache_entry *entp)
{
	char		*fname = NULL;
	int		len, fd;

	len = entp->entry_nlen;
	if (entp->entry_nlen > 32) {
		fname = entp->entry_lname;
	} else {
		fname = entp->entry_sname;
	}
	PRINTF("file_cache_flush_file: %s\n", fname);
	fd = open(fname, O_WRONLY|O_SYNC);
	if (fd) {
		write(fd, entp->entry_data, MAX_FILESIZE);
		close(fd);
	}
	return;
}

// Destructor
void
file_cache_destroy(
	struct file_cache	*cache)
{
	struct cache_entry 	*entp;
	int 	i;

	pthread_mutex_lock(&cache->cache_mutex);
	pthread_cond_broadcast(&cache->cache_cond);
	pthread_mutex_unlock(&(cache->cache_mutex));
	(void) pthread_cond_destroy(&cache->cache_cond);
	(void) pthread_mutex_destroy(&cache->cache_mutex);
	for (i = 0; i < cache->cache_total; i++) {
		entp = cache->cache_entpp[i];
		if (entp) {
			pthread_mutex_lock(&entp->entry_mutex);
			if (entp->entry_pin) {
				if (entp->entry_nlen > 32) {
					PRINTF("file_cache_destroy: %s\n", entp->entry_lname);
				} else {
					PRINTF("file_cache_destroy: %s\n", entp->entry_sname);
				}
				if (entp->entry_dirty) {
					file_cache_flush_file(entp);
					entp->entry_dirty = 0;
				}
				free(entp->entry_data);
			}
			if (entp->entry_nlen > 32) {
				free(entp->entry_lname);
			}
			pthread_mutex_unlock(&entp->entry_mutex);
			(void) pthread_mutex_destroy(&entp->entry_mutex);
			free(entp);
			cache->cache_entpp[i] = NULL;
		}
	}
	free(cache->cache_entpp);
	free(cache);
	return;
}

// searching a cache entry for a given file name
// returns a locked cache entry
struct cache_entry *
file_cache_search_file(
	struct file_cache	*cache,
	const char		*fname)
{
	struct cache_entry	*entp = NULL;
	char			*entname;
	int			i, len;

	for (i = 0; i < cache->cache_total; i++) {
		entp = cache->cache_entpp[i];
		if (entp) {
			pthread_mutex_lock(&entp->entry_mutex);
			len = entp->entry_nlen;
			entname = (len > 32) ? entp->entry_lname : entp->entry_sname;
			if (entp->entry_pin && 
			    !strncmp(entname, fname, len)) {
				return entp;
			}
			pthread_mutex_unlock(&entp->entry_mutex);
		}
	}
	return NULL;
}

// reuse a cache entry for adding a new entry file with name fname
void
file_cache_reuse_entry(
	struct cache_entry	*entp,
	char			*fname)
{
	int			fd, len;

	len = entp->entry_nlen;
	if (len > 32) {
		free(entp->entry_lname);
	}
	entp->entry_nlen = strlen(fname) + 1;
	len = entp->entry_nlen;
	PRINTF("file_cache_reuse_entry:%s\n", fname);
	if (len > 32) {
		entp->entry_lname = (char *)calloc(1, len);
		strncpy(entp->entry_lname, fname, len);
	} else {
		bzero(entp->entry_sname, 32);
		strncpy(entp->entry_sname, fname, len);
	}
	entp->entry_data = (void *)malloc(MAX_FILESIZE);
	bzero(entp->entry_data, MAX_FILESIZE);

	// if file exists read it , otherwise create and write zeros
	fd = open(fname, O_RDWR|O_CREAT|O_EXCL);
	if (fd < 0) {
		fd = open(fname, O_RDWR);
		read(fd, entp->entry_data, MAX_FILESIZE);
	} else {
		write(fd, entp->entry_data, MAX_FILESIZE);
	}
	close(fd);
	entp->entry_pin = 1;
	entp->entry_dirty = 0;
	return;
}

// Pin the given file
void
file_cache_pin_file(
	struct file_cache 	*cache,
	const char 		*fname)
{
	struct cache_entry	*entp = NULL;
	struct cache_entry	*free_entp = NULL;
	char			*entname = NULL;
	int			i = 0, len, found = 0, reuse_failed = 0;

	pthread_mutex_lock(&cache->cache_mutex);
	entp = file_cache_search_file(cache, fname);
	if (entp) {
		//found a matching cache entry
		PRINTF("file_cache_pin_file: pinning already pinned %s\n", fname);
		entp->entry_pin++;
		pthread_mutex_unlock(&entp->entry_mutex);
	} else {
		//wait if cache is full
		while (cache->cache_avail == 0) {
			PRINTF("waiting in file_cache_pin_file for %s\n", fname);
			pthread_cond_wait(&cache->cache_cond, &cache->cache_mutex);
		}
		//reuse a unpinned , clean cache entry
		cache->cache_avail--;
		for (i = 0; i < cache->cache_total && found == 0; i++) {
			entp = cache->cache_entpp[i];
			pthread_mutex_lock(&entp->entry_mutex);
			if (entp->entry_pin == 0 && entp->entry_dirty != 1) {
				found = 1;
				file_cache_reuse_entry(entp, (char *)fname);
			}
			pthread_mutex_unlock(&entp->entry_mutex);
		}
	}
	pthread_mutex_unlock(&cache->cache_mutex);
	return;
}

//Pin the given files. Currently this pin sequentially.
//This can be improved to initiate parallel pin threads
void
file_cache_pin_files(
	struct file_cache 	 *cache,
	const char 		**files,
	int			 num_files)
{
	int			i = 0;

	for (i = 0; i < num_files; i++) {
		file_cache_pin_file(cache, files[i]);
	}
	return;
}

//Unpin a given file.  Flush if dirty and cleanup the cache entry for reuse
void
file_cache_unpin_file(
	struct file_cache	 *cache,
	const char		 *fname)
{
	struct cache_entry	*entp = NULL;

	pthread_mutex_lock(&cache->cache_mutex);
	entp = file_cache_search_file(cache, (char *)fname);
	if (entp) {
		entp->entry_pin--;
		PRINTF("file_cache_unpin_file:%s\n", fname);
		if (entp->entry_pin == 0) {
			PRINTF("file_cache_unpin_file: last unpin %s\n", fname);
			if (entp->entry_dirty) {
				file_cache_flush_file(entp);
			}
			if (entp->entry_nlen > 32) {
				free(entp->entry_lname);
			}
			free(entp->entry_data);
			cache->cache_avail++;
		}
		pthread_mutex_unlock(&entp->entry_mutex);
		pthread_cond_signal(&cache->cache_cond);
	}
	pthread_mutex_unlock(&cache->cache_mutex);
	return;
}

//Unpin the given files. Currently sequential
//Needs improvement
void
file_cache_unpin_files(
	struct file_cache	 *cache,
	const char		 **files,		
	int			 num_files)
{
	int			 i = 0;

	for (i = 0; i < num_files; i++) {
		file_cache_unpin_file(cache, files[i]);
	}
	return;
}

// Provide read-only access to cached data for a given file
const char *
file_cache_file_data(
	struct file_cache	*cache,
	const char		*file)
{
	struct cache_entry 	*entp = NULL;
	void			*data = NULL;

	pthread_mutex_lock(&cache->cache_mutex);
	entp = file_cache_search_file(cache, (char *)file);
	if (entp) {
		data = entp->entry_data;
		pthread_mutex_unlock(&entp->entry_mutex);
	}
	pthread_mutex_unlock(&cache->cache_mutex);
	return (const char *)data;
}

// Provide write access to a pinned fileś cache data.
// Mark the cache entry dirty. 
// What happens if the cache entry was already dirty?
char *
file_cache_mutable_file_data(
	struct file_cache	 *cache,
	const char		 *file)
{
	struct cache_entry	 *entp = NULL;
	void			 *data = NULL;

	pthread_mutex_lock(&cache->cache_mutex);
	entp = file_cache_search_file(cache, (char *)file);
	if (entp) {
		data = entp->entry_data;
		entp->entry_dirty = 1;
		pthread_mutex_unlock(&entp->entry_mutex);
	}
	pthread_mutex_unlock(&cache->cache_mutex);
	return (char *)data;
}
