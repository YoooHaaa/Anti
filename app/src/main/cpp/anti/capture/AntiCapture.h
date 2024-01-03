//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTICAPTURE_H
#define ANTITOOLS_ANTICAPTURE_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiCapture{
public:
    /**
    * 检查当前是否开启了代理
    * @param env
    */
    static void check_proxy(JNIEnv *env);

    /**
     * 检测VPN
     */
    static void check_VPN(JNIEnv *env);

};


#endif //ANTITOOLS_ANTICAPTURE_H
