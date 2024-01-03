//
// Created by Administrator on 2022/4/26.
//

#include "Thread.h"
#include <cerrno>
#include <string.h>

pthread_mutex_t Thread::mutex = PTHREAD_MUTEX_INITIALIZER;

//*************************************************************************************************
/**
 *  线程函数的一个例子,用于 create_thread_func_vm 调用
 * @param arg
 * @return
 */
UNEXPORT void* thread_func_vm(void* arg)
{
    LOGI("in new thread createThreadFunc_addr");
    PARAM_THREAD* param = (PARAM_THREAD*)arg;
    JavaVM* vm = param->vm;
    // TYPR_PARAM* p = (TYPR_PARAM*)param->param; 获取参数组成的结构体
    JNIEnv* threadenv = nullptr;
    if(vm->AttachCurrentThread(&threadenv, nullptr) == JNI_OK) {
        // do something
        jstring  str = threadenv->NewStringUTF("threadFunc test");
        const char* data = threadenv->GetStringUTFChars(str, nullptr);
        threadenv->ReleaseStringUTFChars(str, data);
    } else{
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
    }
    free(arg);
    vm->DetachCurrentThread();
    return nullptr;
}

/**
 *
 * @param vm JavaVM*
 * @param func 线程函数
 * @param params 参数结构体指针
 */
UNEXPORT void Thread::create_thread_func_vm(JavaVM *vm, void* (*func)(void*), void* params) {
    pthread_t thread;
    PARAM_THREAD* param = (PARAM_THREAD*)malloc(sizeof(PARAM_THREAD));
    param->vm = vm;
    param->param = params;
    if (pthread_create(&thread, nullptr, func, (void*)param) != 0){
        LOGI("create_thread_func_vm  error!");
    }else{
        pthread_detach(thread);
    }
}
//*************************************************************************************************

UNEXPORT void Thread::lock_mutex () {
    pthread_mutex_lock(&mutex);
}

UNEXPORT void Thread::unLock_mutex () {
    pthread_mutex_unlock(&mutex);
}



//*************************************************************************************************
/**
 * 线程函数的一个例子,用于 create_thread_func_reflect 调用
 * @param arg
 * @return
 */
UNEXPORT void* thread_func_reflect(void* arg){
    LOGI("in new thread createThreadCallJava_addr");
    PARAM_THREAD_CALL_JAVA* param = (PARAM_THREAD_CALL_JAVA*)arg;
    JavaVM* vm = param->vm;
    jobject javaObj = param->obj;
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) == JNI_EDETACHED){
        if (vm->AttachCurrentThread(&env, nullptr) != 0){
            LOGE("in createThreadCallJava_addr AttachCurrentThread error");
            return nullptr;
        }
    }

    jclass javaClass = env->GetObjectClass(javaObj);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("in new thread createThreadCallJava_addr GetObjectClass  Error");
        env->DeleteGlobalRef(javaObj);
        vm->DetachCurrentThread();
        return nullptr;
    }

    jmethodID javaCallId = env->GetMethodID(javaClass, "onDetected", "()I");
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("in new thread createThreadCallJava_addr GetMethodID  Error");
        env->DeleteGlobalRef(javaObj);
        vm->DetachCurrentThread();
        return nullptr;
    }

    int ret = env->CallIntMethod(javaObj, javaCallId);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        LOGE("in new thread createThreadCallJava_addr CallIntMethod  Error");
        env->DeleteGlobalRef(javaObj);
        vm->DetachCurrentThread();
        return nullptr;
    }

    env->DeleteGlobalRef(javaObj);
    vm->DetachCurrentThread();
    return nullptr;
}

/**
 *  //创建一个线程，该线程中能反射调用Java类，需将Java类对象封装进params里面
 * @param vm
 * @param env
 * @param func
 * @param javaObj  java类的实例
 * @param params   线程中需要传入的参数
 */
UNEXPORT void Thread::create_thread_func_reflect(JavaVM *vm, JNIEnv* env, void *(*func)(void *), jobject javaObj, void* params) {
    pthread_t thread;
    PARAM_THREAD_CALL_JAVA* param = (PARAM_THREAD_CALL_JAVA*)malloc(sizeof(PARAM_THREAD_CALL_JAVA));
    jobject _javaObj = env->NewGlobalRef(javaObj);
    param->vm = vm;
    param->obj = _javaObj;
    param->param = params;
    if (pthread_create(&thread, nullptr, func, (void*)param) != 0){
        LOGI("create_thread_func_reflect  error!");
    }else{
        pthread_detach(thread);
    }
}

//*************************************************************************************************

UNEXPORT void Thread::create_thread_func(void *(*func)(void *), void *params) {
    pthread_t thread;
    if (pthread_create(&thread, nullptr, func, (void*)params) != 0){
        LOGI("create_thread_func  error!");
    }else{
        pthread_detach(thread);
    }
}








