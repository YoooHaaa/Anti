//
// Created by Administrator on 2022/4/22.
//



#ifndef ANTISDK_FIELD_H
#define ANTISDK_FIELD_H
#pragma once
#include <jni.h>
#include "../../include//include.h"
#include <climits>

class Field{
public:
    Field(char* name, JNIEnv *env);

public:
    jstring get_static_jstring_field(const char *fieldName, const char *sig);
    jobject get_object_field(jobject obj, const char *fieldName, const char *sig);
    jobject get_static_object_field(const char *fieldName, const char *sig);
    void set_static_boolean_field(const char *fieldName, const char *sig, int value);


private:
    void findClass();
    void setFieldInt(jobject obj, const char *fieldName, const char *sig, int value);
    void setStaticFieldInt(const char *fieldName, const char *sig, int value);

    int getFieldInt(jobject obj, const char *fieldName, const char *sig);
    int getStaticFieldInt(const char *fieldName, const char *sig);
    jstring getStaticFieldString(const char *fieldName, const char *sig);
    jobject getFieldObject(jobject obj, const char *fieldName, const char *sig);
    jobject getStaticFieldObject(const char *fieldName, const char *sig);
private:
    char mClsName[NAME_MAX] = {0};
    JNIEnv *mEnv = nullptr;
    jclass mJclass = nullptr;
};
#endif //ANTISDK_FIELD_H
