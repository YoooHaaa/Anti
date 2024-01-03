//
// Created by Administrator on 2022/4/26.
//

#include "AntiUnidbg.h"
#include <cerrno>
#include <sys/utsname.h>
#include "../../base/str/Str.h"

UNEXPORT void AntiUnidbg::check_GetStaticMethodID(JNIEnv *env) {
    jmethodID id = getStaticMethodID(env, "com.yooha.antisdk.MainActivity", "getSig", "()Ljava/lang/String;");
    if (id != nullptr){
        // TODO 当前方法被模拟，说明存在于unidbg环境中
        LOGE("check_GetStaticMethodID -------------------> find unidbg");
    }
}

UNEXPORT jmethodID AntiUnidbg::getStaticMethodID(JNIEnv *env, const char *clsName, const char *methodName,
                                        const char *sig) {
    jclass cls = env->FindClass(clsName);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        LOGE("getStaticMethodID FindClass %s Error", clsName);
        return nullptr;
    }

    jmethodID  method = env->GetStaticMethodID(cls, methodName, sig);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe(); //调试阶段打开此开关
        env->ExceptionClear();
        LOGE("getStaticMethodID  name:%s sig:%s Error", methodName, sig);
        return nullptr;
    }
    return method;
}


UNEXPORT void AntiUnidbg::check_uname() {
    struct utsname sysinfo;

    if( uname( &sysinfo ) == -1 ) {
        LOGE("[%s %s %d] -> check_uname  error:%s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
        return;
    }

    if (Str::strstr(sysinfo.sysname, "Unidbg") != nullptr){
        // TODO Unidbg的uname系统调用表明了自己是Unidbg
        LOGE("check_uname -------------------> find unidbg");
    }

}

