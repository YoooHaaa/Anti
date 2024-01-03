//
// Created by Administrator on 2022/6/22.
//

#ifndef ANTISDK_STR_H
#define ANTISDK_STR_H
#pragma once
#include <stdlib.h>

enum State
{
    INVALID,//0 //非法字符串返回此值
    VALID//1
};


class Str{
public:
    static char* strstr(char* src, char* dst);
    static int strcmp(char* src, char* dst);
    static void strcpy(char *dst, char *src);
    static size_t strlen(char *src);
    static int atoi(char* src);
    static int strncmp(const char *s1, const char *s2, size_t n);
    static size_t strlcpy(char *dst, const char *src, size_t siz);
    static void toLower(char* src);
    static void toUper(char* src);
private:
    static enum State state;
};


#endif //ANTISDK_STR_H
