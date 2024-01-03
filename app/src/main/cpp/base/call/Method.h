//
// Created by Administrator on 2022/4/18.
//

#ifndef ANTITOOLS_CALLMETHOD_H
#define ANTITOOLS_CALLMETHOD_H

#pragma once
#include <jni.h>

class Method{
public:
    static jobject callMethodObject(JNIEnv *env,
                                    jobject obj,
                                    const char* clsName,
                                    const char* methodName,
                                    const char* sig,
                                    ...);
    static jobject callStaticMethodObject(JNIEnv *env,
                                    const char* clsName,
                                    const char* methodName,
                                    const char* sig,
                                    ...);

    static void callMethodVoid(JNIEnv *env,
                                    jobject obj,
                                    const char* clsName,
                                    const char* methodName,
                                    const char* sig,
                                    ...);
    static void callStaticMethodVoid(JNIEnv *env,
                               const char* clsName,
                               const char* methodName,
                               const char* sig,
                               ...);

    static int callMethodInt(JNIEnv *env,
                               jobject obj,
                               const char* clsName,
                               const char* methodName,
                               const char* sig,
                               ...);
    static int callStaticMethodInt(JNIEnv *env,
                             const char* clsName,
                             const char* methodName,
                             const char* sig,
                             ...);

    static jboolean callMethodBoolean(JNIEnv *env,
                             jobject obj,
                             const char* clsName,
                             const char* methodName,
                             const char* sig,
                             ...);
    static jboolean callStaticMethodBoolean(JNIEnv *env,
                                      const char* clsName,
                                      const char* methodName,
                                      const char* sig,
                                      ...);

    static jlong callMethodLong(JNIEnv *env,
                                 jobject obj,
                                 const char* clsName,
                                 const char* methodName,
                                 const char* sig,
                                 ...);
    static jlong callStaticMethodLong(JNIEnv *env,
                                const char* clsName,
                                const char* methodName,
                                const char* sig,
                                ...);

    static jbyte callMethodByte(JNIEnv *env,
                                 jobject obj,
                                 const char* clsName,
                                 const char* methodName,
                                 const char* sig,
                                 ...);
    static jbyte callStaticMethodByte(JNIEnv *env,
                                    const char* clsName,
                                    const char* methodName,
                                    const char* sig,
                                    ...);

    static jchar callMethodChar(JNIEnv *env,
                                jobject obj,
                                const char* clsName,
                                const char* methodName,
                                const char* sig,
                                ...);
    static jchar callStaticMethodChar(JNIEnv *env,
                                      const char* clsName,
                                      const char* methodName,
                                      const char* sig,
                                      ...);

    static jdouble callMethodDouble(JNIEnv *env,
                                jobject obj,
                                const char* clsName,
                                const char* methodName,
                                const char* sig,
                                ...);
    static jdouble callStaticMethodDouble(JNIEnv *env,
                                      const char* clsName,
                                      const char* methodName,
                                      const char* sig,
                                      ...);

    static jfloat callMethodFloat(JNIEnv *env,
                                    jobject obj,
                                    const char* clsName,
                                    const char* methodName,
                                    const char* sig,
                                    ...);
    static jfloat callStaticMethodFloat(JNIEnv *env,
                                          const char* clsName,
                                          const char* methodName,
                                          const char* sig,
                                          ...);

    static jshort callMethodShort(JNIEnv *env,
                                  jobject obj,
                                  const char* clsName,
                                  const char* methodName,
                                  const char* sig,
                                  ...);
    static jshort callStaticMethodShort(JNIEnv *env,
                                        const char* clsName,
                                        const char* methodName,
                                        const char* sig,
                                        ...);
};


#endif //ANTITOOLS_CALLMETHOD_H
