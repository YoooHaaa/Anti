//
// Created by Administrator on 2022/4/22.
//

#ifndef ANTISDK_INCLUDE_H
#define ANTISDK_INCLUDE_H
#pragma once
#include <android/log.h>
#include <jni.h>
#include <math.h>

#define LOG_TAG "yooha-anti-ndk"


#ifdef __cplusplus
extern "C"{
#endif

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#ifdef  __cplusplus
}  // extern C
#endif

#define UNEXPORT __attribute__ ((visibility("hidden")))
#define INIT_ARRAY UNEXPORT __attribute__ ((constructor))

// 裸函数,编译器不会生成任何函数入口代码和退出代码
#define NAKED __attribute__((naked))

#define INLINE  __attribute__((always_inline))

typedef struct env_app{
    char* cpu = nullptr;
    char* pkgname = nullptr;
}ENV_APP;

//用于线程函数传参的结构体
typedef struct thread_param{
    JavaVM* vm;
    void* param;
}PARAM_THREAD;

//用于线程函数传参的结构体,该线程会反射调用JAVA，需将java类对象传递进来
typedef struct thread_param_call_java{
    JavaVM* vm;
    jobject obj;
    void* param;
}PARAM_THREAD_CALL_JAVA;

extern ENV_APP* globalEnv;
extern JavaVM *globalJvm;



// 反IDA的内联汇编花指令，只需要调用此函数即可
INLINE
static void Junk_code_001() {
#ifdef __aarch64__

    __asm__ __volatile__ ("b 0xc\n"
                          ".long 12345678\n"
                          ".long 12345678\n"
    );

#elif __arm__

    __asm__ __volatile__ (
            "movw r0, #1001 \t\n"
                 "movw r12, #2020 \t\n"
                 "add r0, r0, r12 \t\n"
                 "bx  lr" );

#endif
}

INLINE
static void Junk_code_002() {
#ifdef __aarch64__
    __asm__ __volatile__(
    "mov x8,#0x1\n"
    "adr x9, #0x10\n"
    "mul x8, x9, x8\n"
    ".long 0x12345678\n"
    "br x8\n"
    );
#endif
}

INLINE
static void Junk_code_003() {
#ifdef __aarch64__
    __asm__ __volatile__(
    "adr x8,#0xc\n"
    "mov x30,x8\n"
    "ret\n" );
#endif
}

INLINE
static void junk_code_004() {
#ifdef __aarch64__
    int a = 3;
    int b = 4;
    int c = 6;
    if (c < sqrt(a*a+b*b)) {
        __asm__ __volatile__(
        ".long 12345678\n"
        ".long 12345678\n"
        );
    }
#endif
}

INLINE
static int count_bits(int x)
{
    register int xx=x;
    xx=xx-((xx>>1)&0x55555555);
    xx=(xx&0x33333333)+((xx>>2)&0x33333333);
    Junk_code_001();
    xx=(xx+(xx>>4))&0x0f0f0f0f;
    xx=xx+(xx>>8);
    return (xx+(xx>>16)) & 0xff;
}


//#if defined(__arm__) //插入无效代码 改变栈结构
//    __asm__(
//    "B codelabel\n"
//    "mov r0, r3\n"
//    "add sp, #0x50\n"
//    "codelabel:\n"
//    );
//    LOGI("in init_test_junk_code %d", num);
//#elif defined(__aarch64__)
//    __asm__(
//    "B codelabel\n"
//    "mov x0, x3\n"
//    "add sp, #0x50\n"
//    "codelabel:\n"
//    );
//    LOGI("in init_test_junk_code %d", num);
//#endif


#endif //ANTISDK_INCLUDE_H
