//
// Created by Park Yu on 2024/1/25.
//

#include "jit.h"

#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

int func(unsigned char *binary, int size, int a, int b) {
    void *mem = mmap(NULL, size, PROT_WRITE | PROT_EXEC,
                     MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    printf("mem=%p\n", mem);
    memset(mem, 0, size);
    memcpy(mem, binary, size);
    printf("---binary---\n");
    for (int i = 0; i < size; i++) {
        printf("0x%02X\n", binary[i]);
    }
    printf("---binary---\n");
    int (*fn)(int a, int b) = (int (*)(int, int)) mem;
    int result = fn(a, b);
    munmap(mem, size);
    return result;
}