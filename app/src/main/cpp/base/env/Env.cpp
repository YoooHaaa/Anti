//
// Created by Administrator on 2022/4/23.
//




#include <jni.h>
#include <string>
#include "Env.h"
#include "../call/Method.h"
#include "../mem/Mem.h"
#include "../../base/file/File.h"

bool Env::thread_share_switch_maps = false;
bool Env::thread_share_switch_mem = false;
bool Env::thread_share_switch_pagemap = false;


UNEXPORT  void Env::getEnv(ENV_APP* env_p) {
    setCPU(&env_p->cpu);
    setPKG(&env_p->pkgname);
}

UNEXPORT void Env::setCPU(char** cpu) {
    jstring jsArch = mEnv->NewStringUTF("os.arch");
    jstring arch = static_cast<jstring>(Method::callStaticMethodObject(mEnv, "java/lang/System",
                                                                            "getProperty",
                                                                            "(Ljava/lang/String;)Ljava/lang/String;",
                                                                            jsArch));
    char* cArch = "";
    if (arch != NULL){
        cArch = const_cast<char *>(mEnv->GetStringUTFChars(arch, NULL));
    }
    LOGI("setCPU os.arch = %s", cArch);

    *cpu = (char*)malloc(strlen(cArch) + 1);
    memcpy(*cpu, cArch, strlen(cArch) + 1);

    mEnv->DeleteLocalRef(jsArch);
    if (arch != NULL){
        mEnv->ReleaseStringUTFChars(arch, cArch);
    }
}


UNEXPORT void Env::setPKG(char **pkg) {
    jobject application = getApplication();
    jstring pkgname = static_cast<jstring>(Method::callMethodObject(mEnv, application,
                                                                    "android/app/Application",
                                                                    "getPackageName",
                                                                    "()Ljava/lang/String;"));
    char* cPkg = const_cast<char *>(mEnv->GetStringUTFChars(pkgname, NULL));
    *pkg = (char*)malloc(strlen(cPkg) + 1);
    memcpy(*pkg, cPkg, strlen(cPkg) + 1);
    mEnv->ReleaseStringUTFChars(pkgname, cPkg);
}


UNEXPORT jobject Env::getApplication() {
    return Method::callStaticMethodObject(mEnv, "android/app/ActivityThread",
                                          "currentApplication",
                                          "()Landroid/app/Application;");
}
