rm *.o
rm *.a
rm test
gcc -c file_cache.c -o file_cache.o
ar rcs libfile_cache.a file_cache.o
gcc -o test -g test.c -L. -lfile_cache -lpthread
