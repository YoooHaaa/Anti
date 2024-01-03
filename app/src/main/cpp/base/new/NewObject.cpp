//
// Created by Administrator on 2022/7/4.
//

#include "NewObject.h"

UNEXPORT jobject NewObject::new_object(JNIEnv *env, const char *clsName, const char *sig, ...) {
    LOGI("in NewObject::new_object %s -> %s", clsName, sig);
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe(); //调试阶段打开此开关
        env->ExceptionClear();
        LOGE("NewObject::new_object FindClass %s Error", clsName);
        return NULL;
    }
    LOGI("new_object FindClass cls=%p", cls);

    jmethodID  constructor = env->GetMethodID(cls, "<init>", sig);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe(); //调试阶段打开此开关
        env->ExceptionClear();
        LOGE("NewObject::new_object GetMethodID name:<init> sig:%s Error", sig);
        return NULL;
    }
    LOGI("new_object GetMethodID jmethodID=%p", constructor);

    va_list args;
    va_start(args, constructor);
    jobject ret = env->NewObjectV(cls, constructor, args);
    va_end(args);
    LOGI("NewObject::new_object NewObject ret=%p", ret);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe(); //调试阶段打开此开关
        env->ExceptionClear();
        LOGE("NewObject::new_object NewObject name:%s sig:%s Error", clsName, sig);
        return NULL;
    }

    return ret;
}

