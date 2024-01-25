#include <jni.h>
#include "jit.h"


#include <pthread.h>
#include <unistd.h>
#include <android/log.h>
#include <stdio.h>
#include "getting-started.h"

// Start threads to redirect stdout and stderr to logcat.
int pipe_stdout[2];
int pipe_stderr[2];
pthread_t thread_stdout;
pthread_t thread_stderr;
const char *QUICKJS_TAG = "jit_native";

void *thread_stderr_func(void *) {
    ssize_t redirect_size;
    char buf[2048];
    while ((redirect_size = read(pipe_stderr[0], buf, sizeof buf - 1)) > 0) {
        //__android_log will add a new line anyway.
        if (buf[redirect_size - 1] == '\n')
            --redirect_size;
        buf[redirect_size] = 0;
        __android_log_write(ANDROID_LOG_ERROR, QUICKJS_TAG, buf);
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
        __android_log_write(ANDROID_LOG_INFO, QUICKJS_TAG, buf);
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
        __android_log_write(ANDROID_LOG_ERROR, QUICKJS_TAG,
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
    int len = (*env)->GetArrayLength(env, native_codes);
    int paramA = a;
    int paramB = b;
    int result = func(codes, len, paramA, paramB);
    (*env)->ReleaseByteArrayElements(env, native_codes, (jbyte *) (codes), JNI_FALSE);
    return result;
}

JNIEXPORT void JNICALL
Java_com_park_jitdemo_NativeBridge_testVixl(JNIEnv *env, jclass clazz) {
    main();
}