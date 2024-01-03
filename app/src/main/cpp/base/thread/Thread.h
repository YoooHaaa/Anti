//
// Created by Administrator on 2022/4/26.
//


#ifndef ANTISDK_THREAD_H
#define ANTISDK_THREAD_H
#pragma once
#include <jni.h>
#include <pthread.h>
#include <semaphore.h>
#include "../../include/include.h"
#include <stdlib.h>

class Thread{
public:
    //互斥体不能跨进程
    static pthread_mutex_t mutex;
    static void lock_mutex ();
    static void unLock_mutex ();

public:
    //信号量可以跨进程
    static sem_t bin_sem;
    static void init_sem(){
        int res = sem_init(&bin_sem, 0, 0);
        if (res != 0){
            LOGE("Thread -> init_sem -> sem_init : error");
            return;
        }
    }
//    nRet = sem_init(&g_sem1, 0 , 0);  //初始化信号量
//    nRet = sem_post(&g_sem3);         //投第一个信号，信号数+1
//    sem_wait(&g_sem3);                //等待1个信号，阻塞状态
//    sem_trywait(&g_sem3);             //等待信号，不阻塞
//    sem_close                         //关闭信号

public:
    static void create_thread_func(void* (*func)(void*), void* params);
    static void create_thread_func_vm(JavaVM* vm, void* (*func)(void*), void* params);
    static void create_thread_func_reflect(JavaVM* vm, JNIEnv* env, void* (*func)(void*), jobject javaObj, void* params);
};







#endif //ANTISDK_THREAD_H
