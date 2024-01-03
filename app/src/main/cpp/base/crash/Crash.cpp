//
// Created by Administrator on 2022/6/24.
//

#include "Crash.h"
#include "../syscall/Syscall.h"
#include <pthread.h>
//#include <semaphore.h>


/**
 * 利用kill函数退出进程
 */
UNEXPORT void Crash::crash_kill(int pid, int sig) {
    Syscall::kill(pid, sig);
}


UNEXPORT void Crash::crash_exit() {

}


UNEXPORT void Crash::crash_insert_ret() {
#if defined(__arm__) //编译器会根据宏来选择arm32还是arm64
    __asm__ __volatile__(  //__volatile__避免被优化
        "LDR R15, [R0]\r\n"
    );
#elif defined(__aarch64__)
    __asm__ __volatile__(  //__volatile__避免被优化
    "RET\r\n"
    );
#endif
}


UNEXPORT int Crash::crash_while() {
    int a = 0x2022;
    int b = 1;
    while(1){
        a = a ^ 0xFFFF;
        if (b > a){
            break;
        }
    }
    return a + b;
}


UNEXPORT void Crash::crash_thread() {
    pthread_t thread;
    pthread_create(&thread, nullptr, nullptr, (void*)nullptr);
}

//*******************************************************
UNEXPORT __attribute__((always_inline))
static inline long no_return_func()
{
    long result;
#if defined(__arm__)
    __asm__ __volatile__(
        "mov r0, r0\r\n"
    );
#elif defined(__aarch64__)
    __asm__ __volatile__(
        "mov x0, x0\r\n"
    );
#endif
}

UNEXPORT void Crash::crash_no_return() {
    no_return_func();
}

//*******************************************************
UNEXPORT void Crash::crash_by_stack_cross() {
    char tmp[10] = {0};
    // TODO 无用代码
    int count = 0;
    for (int i = 0; i < 15; i++){
        count += i;
        count += count << 2;
        count += count ^ 0x11;
        count += count | 0x23;
        count += count & 0x15;
    }
    for (int i = 0; i < 0x100; i++){
        tmp[i] = ((i >> 1) + (count & 0xF));
    }
}

UNEXPORT void Crash::crash_mov_sp() {
    __asm__ __volatile__(  //__volatile__避免被优化
    "SUB SP, SP, #0x900\r\n"
    );
}

UNEXPORT NAKED void Crash::crash_mov_LR() {
#if defined(__arm__)
    __asm__ __volatile__(  //__volatile__避免被优化
        "MOV R0, R0\r\n"
    );
#elif defined(__aarch64__)
    __asm__ __volatile__(
    "SUB SP, SP, #0x20\r\n"
    "ADD X28, SP, #0x10\r\n"
    "MOV W0, #0x11\r\n"
    "LDR X9, [X8, #0x25]\r\n"
    "SUBS X9, X9, X10\r\n"
    "SUB LR, LR, #0x900\r\n"
    "RET\r\n"
    );
#endif
}





