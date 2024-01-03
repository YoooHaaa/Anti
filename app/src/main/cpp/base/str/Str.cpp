//
// Created by Administrator on 2022/6/22.
//

#include "Str.h"
#include <stdio.h>
#include <ctype.h>
#include "../../base/mem//Mem.h"
#include "../../include//include.h"


State Str::state = INVALID;

UNEXPORT INLINE
char* Str::strstr(char *p1, char *p2) {
    assert(p1 != NULL);//断言该指针不为空指针
    assert(p2 != NULL);
    char*s1 = NULL;//创建一个空指针；
    char*s2 = NULL;
    char*cur = p1;
    if (*p2 == '\0')
    {
        return (char*)p1;                    //如果我们所要找的子串p2为'\0',返回总串的首元素地址
    }
    while (*cur)//cur代表指向的是子串第一次出现的首元素地址
    {
        s1 = cur;                      //把cur指向字符串的首元素地址赋给s1
        s2 = (char*)p2;                    //把p2指向的首元素地址赋给s2，每一次循环都要从p2的首元素地址开始重新比较
        while ((*s1 != 0) && (*s2 != 0) && *s1 == *s2)           //只有在s1指向的首元素地址，s2指向的首元素地址都不为'\0'的时候，才可以在总串中找子串的首地址第一次出现的时候
        {                                            // 比较s1，s2指向的地址所对应的字符是否相等，若相等，各自指向的地址向后跳一个字节比较下一位是否相等在满足以上的条件下不断循环，若不满足，
            s1++;                                    //开始一下比较
            s2++;
        }
        if (*s2 == '\0')             //若经过上面的比较，s2指向的内容恰好是'\0',代表找到子串首次出现的地址
        {                              //若不满足以上情况的比较，开始以下比较
            return (char*)cur;
        }
        if (*s1 == '\0')
        {
            return nullptr;            //若在s2指向的内容不为'\0'时，s1指向的内容率先为0，代表总串长度比子串长度短，必不存在子串，返回一个空指针
        }
        cur++;//代表当前cur指向的并不是子串第一次出现的首元素地址，指向的地址要往后跳一个字节
    }
    return nullptr;//如果经过以上比较都不能得出结果，代表找不到子串，要返回空指针
}

UNEXPORT INLINE
int Str::strcmp(char *src, char *dst) {
    int i = 0;
    while(*(src+i) == *(dst+i) && *(src + i) != 0 && *(dst+i) != 0){
        i++;
    }
    if(*(src + i) > *(dst + i)){
        return 1;
    }else if(*(src + i) < *(dst + i)){
        return -1;
    }
    else {
        return 0;
    }
}


UNEXPORT INLINE
void Str::strcpy(char *dst, char *src)
{
    while(*src != '\0')
    {
        *dst = *src;
        dst++;
        src++;
    }
    *dst = '\0';
}

UNEXPORT INLINE
size_t Str::strlen(char *src) {
    size_t len = 0;
    while(*src++) len++;
    return len;
}

/**
 * 字符串转化为数字 "123" -> 123
 * @param src
 * @return
 */
UNEXPORT INLINE
int Str::atoi(char *str) {
    int negative = 0;//0表示正数，1表示负数
    long long ret = 0;
    //首先判断空
    if (str == NULL){
        return 0;
    }
    //跳过空格--atoi只会跳过空格
    while(!isdigit(*str)){
        if('0' == *str){
            return 0;
        }
        else if('-' == *str){
            negative = 1;
            str++;
            break;
        }
        else if('+' == *str){
            negative = 0;
            str++;
            break;
        }
        else if(isdigit(*str)){
            negative = 0;
            break;
        }
        else{
            str++;
        }
    }
    while(isdigit(*str)){
        //计算结果，*str - '0'是通过ASCII码计算相对应的数值
        ret = ret*10 + *str - '0';
        //如果发现结果大于INT_MAX或者小于INE_MIN，则溢出，返回最值
        if(ret >(negative?-(long long)INT_MIN:INT_MAX)){
            return negative?INT_MIN:INT_MAX;
        }
        str++;
    }
    return negative?-ret:ret;
}

UNEXPORT INLINE
int Str::strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0)
        return (0);
    do {
        if (*s1 != *s2++)
            return (*(unsigned char *)s1 - *(unsigned char *)--s2);
        if (*s1++ == 0)
            break;
    } while (--n != 0);
    return 0;
}

UNEXPORT INLINE
size_t Str::strlcpy(char *dst, const char *src, size_t siz) {
    char *d = dst;
    const char *s = src;
    size_t n = siz;
    if (n != 0) {
        while (--n != 0) {
            if ((*d++ = *s++) == '\0')
                break;
        }
    }
    if (n == 0) {
        if (siz != 0)
            *d = '\0';
        while (*s++)
            ;
    }
    return(s - src - 1);
}

UNEXPORT INLINE
void Str::toLower(char *src) {
    int i = 0;
    while(src[i]){
        src[i] = tolower(src[i]);
        i++;
    }
}

UNEXPORT INLINE
void Str::toUper(char *src) {
    int i = 0;
    while(src[i]){
        src[i] = toupper(src[i]);
        i++;
    }
}
