//
// Created by Park Yu on 2024/1/25.
//

#ifndef JIT_DEMO_EXECUTABLE_MEM_H
#define JIT_DEMO_EXECUTABLE_MEM_H

#ifdef __cplusplus

extern "C" void *createExecutableMemory(unsigned char *binary, int size);

extern "C" int releaseExecutableMemory(void *memory, int size);

#else

void *createExecutableMemory(unsigned char *binary, int size);

int releaseExecutableMemory(void *memory, int size);

#endif

#endif //JIT_DEMO_EXECUTABLE_MEM_H
