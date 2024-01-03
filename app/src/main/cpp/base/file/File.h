//
// Created by Administrator on 2022/4/22.
//


#ifndef ANTISDK_FILE_H
#define ANTISDK_FILE_H
#pragma once
#include <jni.h>

class File{
public:
    static void readAssets(JNIEnv *env, jobject asset_manager, char* fileName, char* dst);
    static jstring readSharedPreferences(JNIEnv *env, jobject thiz, char* xmlName, char* key);
    static char* read_property(char* tags, char *buf);
};


#endif //ANTISDK_FILE_H
