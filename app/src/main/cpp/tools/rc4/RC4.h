//
// Created by Administrator on 2022/8/11.
//

#ifndef ANTISDK_RC4_H
#define ANTISDK_RC4_H

// 原理：https://blog.51cto.com/u_15301988/3089450

#include "../../include/include.h"
#include <string>
using namespace std;

class RC4{
public:
    /**
     * 加密解密用同一函数
     */
    void enc_dec(string& data,string key);

private:
    unsigned char s[256];
    void swap(unsigned char& a, unsigned char& b);
    void init_sbox(string key);
};


#endif //ANTISDK_RC4_H


