#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *memory_start;
void *memory_end;
int is_intialized = 0;

struct mem_cblk {
	int size;
	int is_avail;
};

void initialize() {
	if (!is_intialized) {
		memory_start = memory_end = sbrk(0);
		is_intialized = 1;
	}
	return;
}

void free(void *addr) {
	struct mem_cblk *cblk = (struct mem_cblk *)(addr - sizeof(struct mem_cblk));
	cblk->is_avail = 1;
	return;
}

void* alloc(int size) {
	int actualsize = size + sizeof (struct mem_cblk);
	void *memory_cur = memory_start;
	struct mem_cblk *memcblk;
	while (memory_cur < memory_end) {
		memcblk = (struct mem_cblk *)memory_cur;
		if (!memcblk->is_avail) {
			memory_cur = memory_cur + memcblk->size + sizeof (struct mem_cblk);
		} else {
			if (size > memcblk->size) {
				 memory_cur = memory_cur + memcblk->size + sizeof (struct mem_cblk);
			} else {
				memcblk->is_avail = 0;
				return (memory_cur + sizeof (struct mem_cblk));
			}
		}
	}
	memory_cur = memory_end;
	memory_end = sbrk(actualsize);
	memcblk = (struct mem_cblk *)memory_cur;
	memcblk->size = size;
	memcblk->is_avail = 0;
	return (memory_cur + sizeof (struct mem_cblk));
}

