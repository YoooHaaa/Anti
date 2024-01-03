//
// Created by Administrator on 2023/5/22.
//

#ifndef ANTISDK_LOADLIBRARY_H
#define ANTISDK_LOADLIBRARY_H
#pragma once
#include "../../include/include.h"
using namespace std;

/**
 * 用于打开内存中(/proc/self/maps)so文件，获取其函数地址（libart无法直接用dlopen打开，但是能通过此类打开）
 */
class LoadLibrary {
public:
    static void *fake_dlopen(const char *libpath, int flags);// 此函数返回的并不是dlopen的handle，而是一个构造的结构体
    static void *fake_dlsym(void *handle, const char *name);
    static int fake_dlclose(void *handle);
};
#endif //ANTISDK_LOADLIBRARY_H
