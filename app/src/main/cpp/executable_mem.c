//
// Created by Park Yu on 2024/1/25.
//

#include "executable_mem.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

void *createExecutableMemory(unsigned char *binary, int size) {
    void *mem = mmap(NULL, size, PROT_WRITE | PROT_EXEC,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    printf("mem=%p\n", mem);
    memset(mem, 0, size);
    memcpy(mem, binary, size);
    printf("---binary---\n");
    for (int i = 0; i < size; i++) {
        printf("%02X\n", binary[i]);
    }
    printf("---binary---\n");
    return mem;
}

int releaseExecutableMemory(void *memory, int size) {
    return munmap(memory, size);
}