#include <jni.h>
#include "executable_mem.h"


#include <pthread.h>
#include <unistd.h>
#include <android/log.h>
#include <stdio.h>
#include "getting-started.h"
#include "tcc/libtcc.h"
#include "tcc/tcc.h"

// Start threads to redirect stdout and stderr to logcat.
int pipe_stdout[2];
int pipe_stderr[2];
pthread_t thread_stdout;
pthread_t thread_stderr;
const char *NATIVE_TAG = "jit_native";

void *thread_stderr_func(void *) {
    ssize_t redirect_size;
    char buf[2048];
    while ((redirect_size = read(pipe_stderr[0], buf, sizeof buf - 1)) > 0) {
        //__android_log will add a new line anyway.
        if (buf[redirect_size - 1] == '\n')
            --redirect_size;
        buf[redirect_size] = 0;
        __android_log_write(ANDROID_LOG_ERROR, NATIVE_TAG, buf);
    }
    return 0;
}

void *thread_stdout_func(void *) {
    ssize_t redirect_size;
    char buf[2048];
    while ((redirect_size = read(pipe_stdout[0], buf, sizeof buf - 1)) > 0) {
        //__android_log will add a new line anyway.
        if (buf[redirect_size - 1] == '\n')
            --redirect_size;
        buf[redirect_size] = 0;
        __android_log_write(ANDROID_LOG_INFO, NATIVE_TAG, buf);
    }
    return 0;
}

int start_redirecting_stdout_stderr() {
    //set stdout as unbuffered.
    setvbuf(stdout, 0, _IONBF, 0);
    pipe(pipe_stdout);
    dup2(pipe_stdout[1], STDOUT_FILENO);

    //set stderr as unbuffered.
    setvbuf(stderr, 0, _IONBF, 0);
    pipe(pipe_stderr);
    dup2(pipe_stderr[1], STDERR_FILENO);

    if (pthread_create(&thread_stdout, 0, thread_stdout_func, 0) == -1)
        return -1;
    pthread_detach(thread_stdout);

    if (pthread_create(&thread_stderr, 0, thread_stderr_func, 0) == -1)
        return -1;
    pthread_detach(thread_stderr);

    return 0;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
    //Start threads to show stdout and stderr in logcat.
    if (start_redirecting_stdout_stderr() == -1) {
        __android_log_write(ANDROID_LOG_ERROR, NATIVE_TAG,
                            "Couldn't start redirecting stdout and stderr to logcat.\n");
    }
    printf("started redirecting stdout and stderr to logcat.\n");
    return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL
Java_com_park_jitdemo_NativeBridge_executeBinary(JNIEnv *env, jclass clazz, jbyteArray native_codes,
                                                 jint a, jint b) {
    unsigned char *codes = (unsigned char *) (*env)->GetByteArrayElements(env, native_codes,
                                                                          JNI_FALSE);
    int codeSize = (*env)->GetArrayLength(env, native_codes);
    int paramA = a;
    int paramB = b;
    void *execMem = createExecutableMemory(codes, codeSize);
    int (*fn)(int a, int b) = (int (*)(int, int)) execMem;
    int result = fn(paramA, paramB);
    releaseExecutableMemory(execMem, codeSize);
    (*env)->ReleaseByteArrayElements(env, native_codes, (jbyte *) (codes), JNI_FALSE);
    return result;
}

JNIEXPORT void JNICALL
Java_com_park_jitdemo_NativeBridge_testVixl(JNIEnv *env, jclass clazz) {
    testVixil();
}

JNIEXPORT jint JNICALL
Java_com_park_jitdemo_NativeBridge_testCompiler(JNIEnv *env, jclass clazz, jstring c_source_code,
                                                jint a, jint b) {
    char *c_code = (*env)->GetStringUTFChars(env, c_source_code, JNI_FALSE);
    printf("source=%s\n", c_code);
    TCCState *s;
    s = tcc_new();
    s->verbose = 1;
    s->nostdinc = 1;
    s->nostdlib = 1;
    s->nocommon = 1;
    s->do_debug = 0;
    tcc_set_output_type(s, TCC_OUTPUT_OBJ);
    s->ppfp = stdout;
    int compile_result = tcc_compile_string(s, c_code);
    if (compile_result != 0) {
        return compile_result;
    }
    (*env)->ReleaseStringUTFChars(env, c_source_code, c_code);

    Section *section;
    int i, size;
    int result = 0;
    int (*add)(int, int) = NULL;
    for (i = 1; i < s->nb_sections; i++) {
        section = s->sections[i];
        if (section->sh_type != SHT_NOBITS) {
            size = section->data_offset;
            if (size && strcmp(".text", section->name) == 0) {
                printf("write data:%s, addr:%p size:%d\n", section->name, section->data, size);
                add = createExecutableMemory((unsigned char *) section->data, size);
                result = add(a, b);
                printf("fun(%d, %d) = %d\n", a, b, result);
                return result;
            }
        }
    }
    return compile_result;
}