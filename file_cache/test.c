#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "file_cache.h"

#define FILENAME 40

void
func(void *ptr)
{
	struct file_cache *cache = (struct file_cache *)ptr;
	char		*files[20];
	const char 	*rbuf = NULL;
	char	 	*wbuf = NULL;
	pthread_t	th = pthread_self();
	int		 i;

	for (i = 0; i < 20; i++) {
		files[i] = calloc(1,FILENAME);
		snprintf(files[i], FILENAME, "file-%d-%d", (int)th, i);
	}
	printf("pinning files from thread %d\n", (int)th);
	file_cache_pin_files(cache, (const char **)files, 20);
	for (i = 0; i < 20; i++) {
		printf("reading writing file %s pthread %d\n", files[i], (int)th);
		rbuf = file_cache_file_data(cache, files[i]);
		wbuf = file_cache_mutable_file_data(cache, files[i]);
	}
	printf("unpinning files from thread %d\n", (int)th);
	file_cache_unpin_files(cache, (const char **)files, 20);
	for (i = 0; i < 20; i++) {
		free(files[i]);
	}
	pthread_exit(0);
}

int main()
{
	struct file_cache *cache = NULL;
	pthread_t th[4];
	int i = 0;
	
	printf("simpla app\n");
	printf("creating cache\n");
	cache = file_cache_construct(60);
	for (i = 0; i < 4; i++) {
		pthread_create(&th[i], NULL, (void *) &func, (void *)cache);
	}
	for (i = 0; i < 4; i++) {
		pthread_join(th[i], NULL);
	}
	printf("destroying cache\n");
	file_cache_destroy(cache);
	return 0;
}
