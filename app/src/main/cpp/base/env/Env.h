//
// Created by Administrator on 2022/4/23.
//

#ifndef ANTISDK_ENV_H
#define ANTISDK_ENV_H

#pragma once
#include "../../include/include.h"

class Env{
private:
    JNIEnv *mEnv;

public:
    Env(JNIEnv *env, ENV_APP* env_p){
        mEnv = env;
        getEnv(env_p);
    }

    void getEnv(ENV_APP* env_p);


    /**
     * 读取maps文件的标志
     * 任何函数若要读取maps文件需先设置此值为true，读取完毕再设置为false，否则会被检测为进程读取maps，导致程序闪退
     */
    static bool thread_share_switch_maps;
    static bool thread_share_switch_mem;
    static bool thread_share_switch_pagemap;
private:
    void setCPU(char** cpu);
    void setPKG(char** pkg);
    jobject getApplication();

};
#endif //ANTISDK_ENV_H
