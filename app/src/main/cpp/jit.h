//
// Created by Park Yu on 2024/1/25.
//

#ifndef JIT_DEMO_JIT_H
#define JIT_DEMO_JIT_H

#ifdef __cplusplus
extern "C" int func(unsigned char *binary, int size, int a, int b);
#else
int func(unsigned char *binary, int size, int a, int b);
#endif

#endif //JIT_DEMO_JIT_H
