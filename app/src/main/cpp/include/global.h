//
// Created by Administrator on 2023/6/6.
//

#ifndef ANTISDK_GLOBAL_H
#define ANTISDK_GLOBAL_H
#pragma once
#include "include.h"
#include <string>
using namespace std;


typedef struct entrypt_string{
    string str;
    int key[16];
}ENCRYPTSTRING;


/**
 * 1 这是一个全局的字符串表类，用于加解密整个应用native层中的所有字符串
 * 2 字符串的加密和解密都由统一的函数进行，但是加密解密的key的长度是可以变化的，且每个字符串的解密key都不同
 * 3 设计的初衷是不让字符串暴露出来，增加逆向分析难度，所以关键点在于如何隐藏加解密函数，因为逆向分析人员一旦找到了加解密函数，只需要hook该函数的返回值即可获取到明文
 * 4 还有一种设计思路是：每个字符串都匹配一个加解密函数，key的长度不由实例中key的len决定，而是由函数决定。key的len可以作为迷惑分析人员的存在，其len并不是都为有效部分
 */
class GlobalString{

public:
    static void teststr();

// *********************  class -> AntiCapture  ************************
public:
    static ENCRYPTSTRING str_AntiCapture_proxyHost;            // http.proxyHost
    static ENCRYPTSTRING str_AntiCapture_proxyPort;            // http.proxyPort
    static ENCRYPTSTRING str_AntiCapture_System;               // java/lang/System
    static ENCRYPTSTRING str_AntiCapture_getProperty;          // getProperty
    static ENCRYPTSTRING str_AntiCapture_SiggetProperty;       // (Ljava/lang/String;)Ljava/lang/String;
    static ENCRYPTSTRING str_AntiCapture_NetworkInterface;     // java/net/NetworkInterface
    static ENCRYPTSTRING str_AntiCapture_getNetworkInterfaces; // getNetworkInterfaces
    static ENCRYPTSTRING str_AntiCapture_Enumeration;          // ()Ljava/util/Enumeration;
    static ENCRYPTSTRING str_AntiCapture_Collections;          // java/util/Collections
    static ENCRYPTSTRING str_AntiCapture_list;                 // list
    static ENCRYPTSTRING str_AntiCapture_Siglist;              // (Ljava/util/Enumeration;)Ljava/util/ArrayList;
    static ENCRYPTSTRING str_AntiCapture_ArrayList;            // java/util/ArrayList
    static ENCRYPTSTRING str_AntiCapture_size;                 // size
    static ENCRYPTSTRING str_AntiCapture_Sigsize;              // ()I
    static ENCRYPTSTRING str_AntiCapture_get;                  // get
    static ENCRYPTSTRING str_AntiCapture_Object;               // (I)Ljava/lang/Object;
    static ENCRYPTSTRING str_AntiCapture_isUp;                 // isUp
    static ENCRYPTSTRING str_AntiCapture_SigisUp;              // ()Z
    static ENCRYPTSTRING str_AntiCapture_getInterfaceAddresses;// getInterfaceAddresses
    static ENCRYPTSTRING str_AntiCapture_List;                 // ()Ljava/util/List;
    static ENCRYPTSTRING str_AntiCapture_getName;              // getName
    static ENCRYPTSTRING str_AntiCapture_String;               // ()Ljava/lang/String;
    static ENCRYPTSTRING str_AntiCapture_tun0;                 // tun0
    static ENCRYPTSTRING str_AntiCapture_ppp0;                 // ppp0

// *********************  class -> AntiDebugger  **************************




//************************************************************************************
public:
    static const char* decrypt(ENCRYPTSTRING str);
    static const char* encrypt(ENCRYPTSTRING str);
};


#endif //ANTISDK_GLOBAL_H
