//
// Created by Administrator on 2022/4/22.
//



#ifndef ANTISDK_MEM_H
#define ANTISDK_MEM_H
#pragma once
#include <string>
class Mem{
public:
    static void mem_copy(char** dst, std::string src);
    static void memset(void *buf, int value, size_t len);
    static void memmove(void* dest, void* src, size_t num);
    static void memcopy(void* dest, void* src, size_t num);
};




#endif //ANTISDK_MEM_H
