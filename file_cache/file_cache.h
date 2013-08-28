
struct file_cache;

// Constructor. 'max_cache_entries' is the maximum number of files that can
// be cached at any time.
struct file_cache *file_cache_construct(int max_cache_entries);

// Destructor. Flushes all dirty buffers.
void file_cache_destroy(struct file_cache *cache);

// Pins the given files in array 'files' with size 'num_files' in the cache.
// If any of these files are not already cached, they are first read from the
// local filesystem. If the cache is full, then some existing cache entries may
// be evicted. If no entries can be evicted (e.g., if they are all pinned, or
// dirty), then this method will block until a suitable number of cache
// entries becomes available. It is OK for more than one thread to pin the
// same file, however the file should not become unpinned until both pins
// have been removed.
//
// Is is the application's responsibility to ensure that the files may
// eventually be pinned. For example, if 'max_cache_entries' is 5, an
// irresponsible client may try to pin 4 files, and then an additional 2
// files without unpinning any, resulting in the client deadlocking. The
// implementation *does not* have to handle this.
//
// If you are not comfortable with multi-threaded programming or
// synchronization, this function may be modified to return a boolean if
// the requested files cannot be pinned due to the cache being full. However,
// note that entries in 'files' may already be pinned and therefore even a
// full cache may add additional pins to files.
void file_cache_pin_files(struct file_cache *cache,
                          const char **files,
                          int num_files);

// Unpin one or more files that were previously pinned. It is ok to unpin
// only a subset of the files that were previously pinned using
// file_cache_pin_files(). It is undefined behavior to unpin a file that wasn't
// pinned.
void file_cache_unpin_files(struct file_cache *cache,
                            const char **files,
                            int num_files);

// Provide read-only access to a pinned file's data in the cache.
//
// It is undefined behavior if the file is not pinned, or to access the buffer
// when the file isn't pinned.
const char *file_cache_file_data(struct file_cache *cache, const char *file);

// Provide write access to a pinned file's data in the cache. This call marks
// the file's data as 'dirty'. The caller may update the contents of the file
// by writing to the memory pointed by the returned value.
//
// Multiple clients may have access to the data, however the cache *does not*
// have to worry about synchronizing the clients' accesses (you may assume
// the application does this correctly).
//
// It is undefined behavior if the file is not pinned, or to access the buffer
// when the file is not pinned.
char *file_cache_mutable_file_data(struct file_cache *cache, const char *file);

