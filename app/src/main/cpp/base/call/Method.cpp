//
// Created by Administrator on 2022/4/18.
//
#include "Method.h"
#include "../../include//include.h"



UNEXPORT jobject Method::callMethodObject(JNIEnv *env,
                                 jobject obj,
                                 const char* clsName,
                                 const char* methodName,
                                 const char* sig,
                                 ...){
    LOGI("in callMethodObject %s -> %s %s", clsName, methodName, sig);
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();//调试阶段打开此开关
        env->ExceptionClear();
        LOGE("callMethodObject FindClass %s Error", clsName);
        return NULL;
    }
    LOGI("callMethodObject FindClass %p", cls);
    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();//调试阶段打开此开关
        env->ExceptionClear();
        LOGE("callMethodObject GetMethodID name:%s sig:%s Error", methodName, sig);
        return NULL;
    }
    LOGI("callMethodObject GetMethodID %p", method);
    va_list args;
    va_start(args, method);
    jobject ret = env->CallObjectMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();//调试阶段打开此开关
        env->ExceptionClear();
        LOGE("callMethodObject name:%s sig:%s Error", methodName, sig);
        return NULL;
    }
    LOGI("callMethodObject success");
    return ret;
}


UNEXPORT jobject Method::callStaticMethodObject(JNIEnv *env,
                                       const char* clsName,
                                       const char* methodName,
                                       const char* sig,
                                       ...){
    //LOGI("in callStaticMethodObject %s -> %s %s", clsName, methodName, sig);
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe(); //调试阶段打开此开关
        env->ExceptionClear();
        LOGE("callStaticMethodObject FindClass %s Error", clsName);
        return NULL;
    }
    //LOGI("callStaticMethodObject cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe(); //调试阶段打开此开关
        env->ExceptionClear();
        LOGE("callStaticMethodObject GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return NULL;
    }

    //LOGI("callStaticMethodObject method=%p", method);

    va_list args;
    va_start(args, method);
    jobject ret = env->CallStaticObjectMethodV(cls, method, args);
    va_end(args);

    //LOGI("callStaticMethodObject ret=%p", method);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodObject name:%s sig:%s Error", methodName, sig);
        return NULL;
    }

    return ret;
}



UNEXPORT void Method::callMethodVoid(JNIEnv *env,
                            jobject obj,
                            const char* clsName,
                            const char* methodName,
                            const char* sig,
                            ...){
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodVoid FindClass %s Error", clsName);
        return;
    }
    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodVoid GetMethodID name:%s sig:%s Error", methodName, sig);
        return;
    }
    va_list args;
    va_start(args, method);
    env->CallVoidMethodV(obj, method, args);
    va_end(args);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodObject name:%s sig:%s Error", methodName, sig);
        return;
    }
}

UNEXPORT void Method::callStaticMethodVoid(JNIEnv *env, const char *clsName, const char *methodName,
                                  const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodVoid FindClass %s Error", clsName);
        return;
    }
    LOGI("callStaticMethodVoid cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodVoid GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return;
    }

    LOGI("callStaticMethodVoid method=%p", method);

    va_list args;
    va_start(args, method);
    env->CallStaticVoidMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodVoid name:%s sig:%s Error", methodName, sig);
        return;
    }
}


UNEXPORT int Method::callMethodInt(JNIEnv *env,
                          jobject obj,
                          const char *clsName,
                          const char *methodName,
                          const char *sig,
                          ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodInt FindClass %s Error", clsName);
        return -1;
    }
    LOGI("callMethodInt cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodInt GetMethodID name:%s sig:%s Error", methodName, sig);
        return -1;
    }

    LOGI("callMethodInt method=%p", method);

    va_list args;
    va_start(args, method);
    jint ret = env->CallIntMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodInt name:%s sig:%s Error", methodName, sig);
        return -1;
    }

    return ret;
}

UNEXPORT int Method::callStaticMethodInt(JNIEnv *env, const char *clsName, const char *methodName,
                                const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodInt FindClass %s Error", clsName);
        return -1;
    }
    LOGI("callStaticMethodInt cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodInt GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return -1;
    }

    LOGI("callStaticMethodInt method=%p", method);

    va_list args;
    va_start(args, method);
    jint ret = env->CallIntMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodInt name:%s sig:%s Error", methodName, sig);
        return -1;
    }

    return ret;
}


UNEXPORT jboolean Method::callMethodBoolean(JNIEnv *env, jobject obj, const char *clsName, const char *methodName,
                                   const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodBoolean FindClass %s Error", clsName);
        return false;
    }
    LOGI("callMethodBoolean cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodBoolean GetMethodID name:%s sig:%s Error", methodName, sig);
        return false;
    }

    LOGI("callMethodBoolean method=%p", method);

    va_list args;
    va_start(args, method);
    jboolean ret = env->CallBooleanMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodBoolean name:%s sig:%s Error", methodName, sig);
        return false;
    }

    return ret;
}
UNEXPORT jboolean Method::callStaticMethodBoolean(JNIEnv *env, const char *clsName, const char *methodName,
                                         const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodBoolean FindClass %s Error", clsName);
        return false;
    }
    LOGI("callStaticMethodBoolean cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodBoolean GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return false;
    }

    LOGI("callStaticMethodBoolean method=%p", method);

    va_list args;
    va_start(args, method);
    jboolean ret = env->CallStaticBooleanMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodBoolean name:%s sig:%s Error", methodName, sig);
        return false;
    }

    return ret;
}

UNEXPORT jlong Method::callMethodLong(JNIEnv *env, jobject obj, const char *clsName, const char *methodName,
                             const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodLong FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callMethodLong cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodLong GetMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callMethodLong method=%p", method);

    va_list args;
    va_start(args, method);
    jlong ret = env->CallLongMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodLong name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jlong Method::callStaticMethodLong(JNIEnv *env, const char *clsName,
                                   const char *methodName, const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodBoolean FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callStaticMethodBoolean cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodBoolean GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callStaticMethodBoolean method=%p", method);

    va_list args;
    va_start(args, method);
    jlong ret = env->CallStaticLongMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodBoolean name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jbyte Method::callMethodByte(JNIEnv *env, jobject obj, const char *clsName, const char *methodName,
                             const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodByte FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callMethodByte cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodByte GetMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callMethodByte method=%p", method);

    va_list args;
    va_start(args, method);
    jbyte ret = env->CallByteMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodByte name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jbyte Method::callStaticMethodByte(JNIEnv *env, const char *clsName, const char *methodName,
                                   const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodByte FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callStaticMethodByte cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodByte GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callStaticMethodByte method=%p", method);

    va_list args;
    va_start(args, method);
    jbyte ret = env->CallStaticByteMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodByte name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jchar Method::callMethodChar(JNIEnv *env, jobject obj, const char *clsName, const char *methodName,
                             const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodChar FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callMethodChar cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodChar GetMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callMethodChar method=%p", method);

    va_list args;
    va_start(args, method);
    jchar ret = env->CallCharMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodChar name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jchar Method::callStaticMethodChar(JNIEnv *env, const char *clsName, const char *methodName,
                                   const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodChar FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callStaticMethodChar cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodChar GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callStaticMethodChar method=%p", method);

    va_list args;
    va_start(args, method);
    jchar ret = env->CallStaticCharMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodChar name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jdouble Method::callMethodDouble(JNIEnv *env, jobject obj, const char *clsName, const char *methodName,
                         const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodDouble FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callMethodDouble cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodDouble GetMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callMethodDouble method=%p", method);

    va_list args;
    va_start(args, method);
    jdouble ret = env->CallDoubleMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodDouble name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jdouble Method::callStaticMethodDouble(JNIEnv *env, const char *clsName, const char *methodName,
                                       const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodDouble FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callStaticMethodDouble cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodDouble GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callStaticMethodDouble method=%p", method);

    va_list args;
    va_start(args, method);
    jdouble ret = env->CallStaticDoubleMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodDouble name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jfloat Method::callMethodFloat(JNIEnv *env, jobject obj, const char *clsName, const char *methodName,
                               const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodFloat FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callMethodFloat cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodFloat GetMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callMethodFloat method=%p", method);

    va_list args;
    va_start(args, method);
    jfloat ret = env->CallFloatMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodFloat name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jfloat Method::callStaticMethodFloat(JNIEnv *env, const char *clsName, const char *methodName,
                                     const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodFloat FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callStaticMethodFloat cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodFloat GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callStaticMethodFloat method=%p", method);

    va_list args;
    va_start(args, method);
    jfloat ret = env->CallStaticFloatMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodFloat name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jshort Method::callMethodShort(JNIEnv *env, jobject obj, const char *clsName, const char *methodName,
                        const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodShort FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callMethodShort cls=%p", cls);

    jmethodID  method = env->GetMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodShort GetMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callMethodShort method=%p", method);

    va_list args;
    va_start(args, method);
    jshort ret = env->CallShortMethodV(obj, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callMethodShort name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}

UNEXPORT jshort Method::callStaticMethodShort(JNIEnv *env, const char *clsName, const char *methodName,
                                     const char *sig, ...) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodShort FindClass %s Error", clsName);
        return 0;
    }
    LOGI("callStaticMethodShort cls=%p", cls);

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodShort GetStaticMethodID name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    LOGI("callStaticMethodShort method=%p", method);

    va_list args;
    va_start(args, method);
    jshort ret = env->CallStaticShortMethodV(cls, method, args);
    va_end(args);

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("callStaticMethodShort name:%s sig:%s Error", methodName, sig);
        return 0;
    }

    return ret;
}












