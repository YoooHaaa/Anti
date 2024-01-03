//
// Created by Administrator on 2022/4/26.
//

#ifndef ANTISDK_AES_H
#define ANTISDK_AES_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "base64.h"

#ifndef CBC
#define CBC 1
#endif

#ifndef ECB
#define ECB 1
#endif

static const unsigned  char HEX[16]={0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};

#if defined(ECB) && ECB

char* AES_ECB_PKCS7_Encrypt(const char *in, const uint8_t *key);
char* AES_ECB_PKCS7_Decrypt(const char *in, const uint8_t *key);

#endif // #if defined(ECB) && ECB

#if defined(CBC) && CBC

char *AES_CBC_PKCS7_Encrypt(const char *in, const uint8_t *key, const uint8_t *iv);
char *AES_CBC_PKCS7_Decrypt(const char *in, const uint8_t *key, const uint8_t *iv);
char *AES_CBC_ZREO_Encrypt(const char *in, const uint8_t *key, const uint8_t *iv);
char *AES_CBC_ZREO_Decrypt(const char *in, const uint8_t *key, const uint8_t *iv);

#endif // #if defined(CBC) && CBC

#endif //ANTISDK_AES_H
