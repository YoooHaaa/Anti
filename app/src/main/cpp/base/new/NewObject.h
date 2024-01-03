//
// Created by Administrator on 2022/7/4.
//

#ifndef ANTISDK_NEWOBJECT_H
#define ANTISDK_NEWOBJECT_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class NewObject{
public:
    /**
     * new对象
     * @param env
     * @param clsName
     * @param sig
     * @param ...
     * @return
     */
    static jobject new_object(JNIEnv *env, const char* clsName, const char* sig, ...);
};

#endif //ANTISDK_NEWOBJECT_H
