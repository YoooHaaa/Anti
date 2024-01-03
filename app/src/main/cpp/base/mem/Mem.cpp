//
// Created by Administrator on 2022/4/22.
//

#include "Mem.h"
#include "../../include//include.h"

/**
 * 将string copy 到 char[]
 * @param dst
 * @param src
 */
INLINE UNEXPORT
void Mem::mem_copy(char** dst, std::string src) {
    *dst = (char*)malloc(sizeof(char) * src.length() + 1);
    memset(*dst, 0, sizeof(char) * src.length() + 1);
    memmove(*dst, (void *) src.c_str(), sizeof(char) * src.length());
}

INLINE UNEXPORT
void Mem::memset(void *buf, int value, size_t len) {
    if (buf == nullptr){
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, "NullPointer Exception");
        return;
    }
    char* tmp = (char*)buf;
    while(len--)
    {
        tmp[len] = (char)value;
    }
}

INLINE UNEXPORT
void Mem::memmove(void *dest, void *src, size_t num) {
    if (dest == nullptr || src == nullptr){
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, "NullPointer Exception");
        return;
    }
    unsigned char* pdst = (unsigned char*)dest;
    unsigned char* psrc = (unsigned char*)src;
    if (pdst > psrc)
    {

        for (size_t i = num; i > 0; i--)
        {
            *(pdst + i - 1) = *(psrc + i - 1);
        }
    }
    else
    {
        for (size_t i = 0; i < num; i++)
        {
            *(pdst + i - 1) = *(psrc + i - 1);
        }
    }
}

INLINE UNEXPORT
void Mem::memcopy(void *dest, void *src, size_t num) {
    memmove(dest, src, num);
}



