#include "io.h"
#include "memory.h"
#include "multiboot.h"

struct memory_area safe_memory_areas[8];
int num_safe_memory_areas = 0;
int bytes_free = 0;

struct memory_area allocation_table[256];
int num_areas_allocated = 0;

void init_memory(multiboot_info_t *mb) {
  // for each region in the multiboot memory map
  for (
    multiboot_memory_map_t *e = (multiboot_memory_map_t*)mb->mmap_addr;
    (unsigned int) e < mb->mmap_addr + mb->mmap_length;
    e++
  ) {
    /* printn((unsigned long) e->addr); */
    /* print(";"); */
    /* printn((unsigned long) e->type); */
    /* print("\r\n"); */
    if (e->type != MULTIBOOT_MEMORY_AVAILABLE) {
      continue;
    }
    // memory below 1MB is not safe
    if (e->addr < 0x100000) {
      continue;
    }
    if (e->addr > 4294967295) {
      continue;
    }
    unsigned int length;
    // truncate length if needed
    if (e->addr + e->len > 4294967295) {
      length = 4294967295 - e->addr;
    } else {
      length = e->len;
    }
    // for the sake of argument, the first 64KB after 1MB is not safe
    if (e->addr == 0x100000) {
      safe_memory_areas[num_safe_memory_areas].addr = 0x110000;
      safe_memory_areas[num_safe_memory_areas].len = length - 0x10000;
    } else {
      safe_memory_areas[num_safe_memory_areas].addr = e->addr;
      safe_memory_areas[num_safe_memory_areas].len = length;
    }
    bytes_free += safe_memory_areas[num_safe_memory_areas].len;
    num_safe_memory_areas++;
  }
}

void* malloc(int len) {
  unsigned long addr;
  // if no blocks have been allocated yet
  if (num_areas_allocated == 0) {
    // memory should be allocated at the start of the first safe memory area
    addr = safe_memory_areas[0].addr;
  } else {
    // memory should be allocated after the end of the last allocated block
    addr = allocation_table[num_areas_allocated - 1].addr + allocation_table[num_areas_allocated - 1].len;
  }
  allocation_table[num_areas_allocated].addr = addr;
  allocation_table[num_areas_allocated].len = len;
  num_areas_allocated++;
  return (void*) addr;
}

/* void free(void *ptr) { */
/* } */
