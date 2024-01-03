//
// Created by Administrator on 2022/4/26.
//

#include "AntiXposed.h"
#include "../../base/call/Method.h"
#include <cstdio>
#include "../../base/thread/Thread.h"
#include "../../base/syscall/Syscall.h"
#include "../../base/str/Str.h"
#include "../../base/env/Env.h"
#include "../sandbox/AntiSandBox.h"
#include <dlfcn.h>

UNEXPORT void AntiXposed::check_load_xposed_class(JNIEnv *env) {
    jstring jsXposedHelpers = env->NewStringUTF("de.robv.android.xposed.XposedHelpers");
    jstring jsXposedBridge = env->NewStringUTF("de.robv.android.xposed.XposedBridge");
    jstring jsRatelBridge = env->NewStringUTF("com.virjar.ratel.api.rposed.RposedBridge");
    jstring jsRatelHelper = env->NewStringUTF("com.virjar.ratel.api.rposed.RposedHelpers");

    jobject jClassLoader = Method::callStaticMethodObject(env, "java/lang/ClassLoader",
                        "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    jobject clsXposedHelpers = Method::callMethodObject(env, jClassLoader, "java/lang/ClassLoader",
                        "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;", jsXposedHelpers);
    jobject clsXposedBridge = Method::callMethodObject(env, jClassLoader, "java/lang/ClassLoader",
                        "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;", jsXposedBridge);
    jobject clsRatelBridge = Method::callMethodObject(env, jClassLoader, "java/lang/ClassLoader",
                        "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;", jsRatelBridge);
    jobject clsRatelHelpers = Method::callMethodObject(env, jClassLoader, "java/lang/ClassLoader",
                        "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;", jsRatelHelper);

    LOGI("in load_xposed_class find XposedHelpers -> %p", clsXposedHelpers);
    LOGI("in load_xposed_class find XposedBridge -> %p", clsXposedBridge);
    LOGI("in load_xposed_class find RposedBridge -> %p", clsRatelBridge);
    LOGI("in load_xposed_class find RposedHelpers -> %p", clsRatelHelpers);
    if (clsXposedHelpers != NULL || clsXposedBridge != NULL || clsRatelBridge != NULL || clsRatelHelpers != NULL){
        LOGI("in load_xposed_class find xposed");
    }
    env->DeleteLocalRef(jsXposedHelpers);
    env->DeleteLocalRef(jsXposedBridge);
    env->DeleteLocalRef(jsRatelBridge);
    env->DeleteLocalRef(jsRatelHelper);
}

//*****************************************************************************************************

UNEXPORT bool AntiXposed::scan_line(char *map) {
    return Str::strstr(map, "XposedBridge") != NULL ||
            Str::strstr(map, "xposed") != NULL ||
            Str::strstr(map, "com.saurik.substrate") != NULL;
}


UNEXPORT void AntiXposed::read_line_scan(string str){
    char* split = (char*)"\n";
    string strs = str + split; // 在字符串末尾也加入分隔符，方便截取最后一段
    size_t pos = strs.find(split);
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        LOGI("read_line_scan maps -> %s", temp.c_str());
        if (scan_line(const_cast<char *>(temp.c_str()))){
            //TODO
            LOGI("---------------------------read_line_scan find xposed -> %s", temp.c_str());
            return;
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
}


UNEXPORT void AntiXposed::check_maps() {
    Thread::lock_mutex();
    Env::thread_share_switch_maps = true;
    Thread::unLock_mutex();
    string str = Syscall::readFile((char*)"/proc/self/maps");
    Thread::lock_mutex();
    Env::thread_share_switch_maps = false;
    Thread::unLock_mutex();
    if (str != "null"){
        read_line_scan(str);
    }
}



//*****************************************************************************************************

UNEXPORT void AntiXposed::load_xposed_class(JNIEnv *env, jclass thiz) {
    AntiXposed::check_load_xposed_class(env);
}



//*****************************************************************************************************

UNEXPORT void AntiXposed::check_env(JNIEnv *env) {
    jstring jsCLASSPATH = env->NewStringUTF("CLASSPATH");

    jstring jsEnv = static_cast<jstring>(Method::callStaticMethodObject(env,
                                                                     "java/lang/System",
                                                                     "getenv",
                                                                     "(Ljava/lang/String;)Ljava/lang/String;",
                                                                     jsCLASSPATH));
    LOGI("check_env jsEnv = %p", jsEnv);

    if (jsEnv != nullptr){
        jstring jsXposed = env->NewStringUTF("XposedBridge");
        jboolean jb = Method::callMethodBoolean(env, jsEnv,
                                                "java/lang/String",
                                                "contains",
                                                "(Ljava/lang/CharSequence;)Z",
                                                jsXposed);
        if (jb){
            LOGI("check_env find xposed");
        }else{
            LOGI("check_env con not find xposed");
        }
        env->DeleteLocalRef(jsXposed);
    }
    env->DeleteLocalRef(jsCLASSPATH);
}

UNEXPORT void AntiXposed::check_vxp(JNIEnv *env) {
    jstring jsvxp = env->NewStringUTF("vxp");

    jstring jsproperty = static_cast<jstring>(Method::callStaticMethodObject(env,
                                                                        "java/lang/System",
                                                                        "getProperty",
                                                                        "(Ljava/lang/String;)Ljava/lang/String;",
                                                                        jsvxp));
    LOGI("check_vxp jsproperty = %p", jsproperty);

    if (jsproperty != nullptr){
        LOGI("check_vxp find xposed");
    }else{
        LOGI("check_vxp con not find xposed");
    }
    env->DeleteLocalRef(jsvxp);
}

UNEXPORT void AntiXposed::check_symbols() {
    char *(path[]) = {
            "ZN3art6mirror9Artmethod16EnabIeXposedHookEP7_JNIEnvP8_jObject",
            "ZN3art9Artmethod16EnabIeXposedHookERNS_18ScopedObjectAccessEP8_jObject",
            "ZN3art6mirror9Artmethod16EnabIeXposedHookERNS_18ScopedObjectAccessEP8_jObject"
    };
    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
        if (dlsym(RTLD_DEFAULT, path[i]) != nullptr){
            LOGI("check_symbols find xposed");
            // TODO 找到xposed相关的函数

            break;
        }
    }
}


