//
// Created by Administrator on 2022/4/22.
//

#include "Field.h"
#include "../str/Str.h"



UNEXPORT Field::Field(char *name, JNIEnv *env) {
    Str::strcpy(mClsName, name);
    mEnv = env;
    this->findClass();
}

UNEXPORT void Field::findClass() {
    mJclass = mEnv->FindClass(mClsName);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("findClass %s Error", mClsName);
    }
}

UNEXPORT  void Field::setFieldInt(jobject obj, const char *fieldName,const char *sig, int value) {
    jfieldID fieldid = mEnv->GetFieldID(mJclass, fieldName, sig);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("setFieldInt GetFieldID name:%s sig:%s Error", fieldName, sig);
        return;
    }
    mEnv->SetIntField(obj, fieldid, value);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("setFieldInt name:%s sig:%s Error", fieldName, sig);
        return;
    }
}

UNEXPORT  void Field::setStaticFieldInt(const char *fieldName, const char *sig, int value) {
    jfieldID fieldid = mEnv->GetStaticFieldID(mJclass, fieldName, sig);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("setStaticFieldInt GetStaticFieldID name:%s sig:%s Error", fieldName, sig);
        return;
    }
    mEnv->SetStaticIntField(mJclass, fieldid, value);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("setStaticFieldInt name:%s sig:%s Error", fieldName, sig);
        return;
    }
}

UNEXPORT int Field::getFieldInt(jobject obj, const char *fieldName, const char *sig) {
    jfieldID fieldid = mEnv->GetFieldID(mJclass, fieldName, sig);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("setFieldInt GetFieldID name:%s sig:%s Error", fieldName, sig);
        return 0;
    }
    jint value = mEnv->GetIntField(obj, fieldid);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("setFieldInt name:%s sig:%s Error", fieldName, sig);
        return 0;
    }
    return value;
}

UNEXPORT int Field::getStaticFieldInt(const char *fieldName, const char *sig) {
    jfieldID fieldid = mEnv->GetStaticFieldID(mJclass, fieldName, sig);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("getStaticFieldInt GetStaticFieldID name:%s sig:%s Error", fieldName, sig);
        return 0;
    }
    jint value = mEnv->GetStaticIntField(mJclass, fieldid);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("getStaticFieldInt name:%s sig:%s Error", fieldName, sig);
        return 0;
    }
    return value;
}

UNEXPORT jstring Field::getStaticFieldString(const char *fieldName, const char *sig) {
    jfieldID fieldid = mEnv->GetStaticFieldID(mJclass, fieldName, sig);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("getStaticFieldString GetStaticFieldID name:%s sig:%s Error", fieldName, sig);
        return nullptr;
    }
    jstring value = static_cast<jstring>(mEnv->GetStaticObjectField(mJclass, fieldid));
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("getStaticFieldInt name:%s sig:%s Error", fieldName, sig);
        return nullptr;
    }
    return value;
}

UNEXPORT jstring Field::get_static_jstring_field(const char *fieldName, const char *sig) {
    return getStaticFieldString(fieldName, sig);
}

UNEXPORT void Field::set_static_boolean_field(const char *fieldName, const char *sig, int value) {

}

UNEXPORT jobject Field::getFieldObject(jobject obj, const char *fieldName, const char *sig) {
    jfieldID fieldid = mEnv->GetFieldID(mJclass, fieldName, sig);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionDescribe();//调试阶段打开此开关
        mEnv->ExceptionClear();
        LOGE("getFieldObject  GetFieldID  name:%s sig:%s Error", fieldName, sig);
        return nullptr;
    }
    jobject value = static_cast<jobject>(mEnv->GetObjectField(obj, fieldid));
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionDescribe();//调试阶段打开此开关
        mEnv->ExceptionClear();
        LOGE("getFieldObject GetObjectField name:%s sig:%s Error", fieldName, sig);
        return nullptr;
    }
    return value;
}

UNEXPORT jobject Field::get_object_field(jobject obj, const char *fieldName, const char *sig) {
    return getFieldObject(obj, fieldName, sig);
}


UNEXPORT jobject Field::getStaticFieldObject(const char *fieldName, const char *sig) {
    jfieldID fieldid = mEnv->GetStaticFieldID(mJclass, fieldName, sig);
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("getStaticFieldObject name:%s sig:%s Error", fieldName, sig);
        return nullptr;
    }
    jobject value = static_cast<jobject>(mEnv->GetStaticObjectField(mJclass, fieldid));
    if (mEnv->ExceptionCheck()) {
        mEnv->ExceptionClear();
        LOGE("getStaticFieldObject name:%s sig:%s Error", fieldName, sig);
        return nullptr;
    }
    return value;
}

UNEXPORT jobject Field::get_static_object_field(const char *fieldName, const char *sig) {
    return getStaticFieldObject(fieldName, sig);
}





