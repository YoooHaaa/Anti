//
// Created by Administrator on 2022/8/11.
//

#include "RC4.h"


UNEXPORT void RC4::enc_dec(string &data, string key) {
    init_sbox(key);
    unsigned int datalen = data.length();
    unsigned char k, i = 0, j = 0, t;
    for (unsigned int h = 0; h < datalen; h++)
    {
        i = (i + 1) % 256;
        j = (j + s[i]) % 256;
        swap(s[i], s[j]);
        t = (s[i] + s[j]) % 256;
        k = s[t];
        data[h] ^= k;
    }
}

UNEXPORT void RC4::swap(unsigned char &a, unsigned char &b) {
    unsigned char tmp = a;
    a = b;
    b = tmp;
}

UNEXPORT void RC4::init_sbox(string key) {
    for (unsigned int i = 0; i < 256; i++)	//初始化s盒
        s[i] = i;
    unsigned char T[256] = { 0 };
    unsigned keylen = key.length();
    for (int i = 0; i < 256; i++)
        T[i] = key[i % keylen];				//根据密钥初始化t表
    for (int j = 0, i = 0; i < 256; i++)
    {
        j = (j + s[i] + T[i]) % 256;		//打乱s盒
        swap(s[i], s[j]);
    }
}

