#ifndef MEMORY_H
#define MEMORY_H

#include "multiboot.h"

struct memory_area {
  unsigned long addr;
  unsigned int len;
};
extern struct memory_area safe_memory_areas[8];

extern int num_safe_memory_areas;
extern int bytes_free;

void init_memory(multiboot_info_t *mb);

void* malloc(int len);
/* extern unsigned int my_memory_available; */

#endif
