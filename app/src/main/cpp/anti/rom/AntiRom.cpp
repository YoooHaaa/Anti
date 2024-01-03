//
// Created by Administrator on 2023/6/1.
//


#include "AntiRom.h"
#include "../../base/str/Str.h"
#include "../../base/file/File.h"
#include "../../base/syscall/Syscall.h"
#include "../../base/call/Method.h"
#include "../../base/shell/Shell.h"
#include <unistd.h>


UNEXPORT void AntiRom::check_prop() {
    char tmp[1024] = {0};

    // ro.build.display.id
    File::read_property("ro.build.display.id", reinterpret_cast<char *>(&tmp));
    LOGI("check_prop ro.build.display.id -> %s", tmp);
    Str::toLower(tmp);
    if (Str::strstr(tmp, "lineage") != nullptr || Str::strstr(tmp, "aosp") != nullptr){
        // TODO 发现 lineageOS Aosp
    }
}
UNEXPORT void AntiRom::check_prop_file() {
    string prop = Syscall::readFile("/dev/__properties__/u:object_r:exported_default_prop:s0");
    if (prop == "null"){
        return;
    }
    LOGI("check_prop_file -> %s", prop.c_str());
    if (Str::strstr(const_cast<char *>(prop.c_str()), "lineage") != nullptr ||
        Str::strstr(const_cast<char *>(prop.c_str()), "aosp") != nullptr){
        // TODO 发现 lineageOS Aosp
        LOGI("check_prop_file -> 发现 lineageOS Aosp");
    }
}
//***************************************************************************************************
UNEXPORT void AntiRom::check_file() {
    char *(path[]) = {
            "/system/addon.d"  // LineageOS特有文件
    };
    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
        if (Syscall::check_file_or_dir_exists(path[i])){
            LOGI("check_file  %s file existing", path[i]);
            // TODO 风险
        }
    }
}
//***************************************************************************************************
UNEXPORT void AntiRom::check_sensor(JNIEnv *env) {
    jobject context = AntiRom::getApplicationContext(env);
    LOGI("check_sensor context %p", context);
    jstring sensor = env->NewStringUTF("sensor");
    jobject objService = Method::callMethodObject(env, context,
                                                  "android/content/Context",
                                                  "getSystemService",
                                                  "(Ljava/lang/String;)Ljava/lang/Object;",
                                                  sensor);
    env->DeleteLocalRef(sensor);
    LOGI("check_sensor objService %p", objService);
    jobject sensorlist = Method::callMethodObject(env, objService,
                                                  "android/hardware/SensorManager",
                                                  "getSensorList",
                                                  "(I)Ljava/util/List;",
                                                  -1);
    LOGI("check_sensor sensorlist %p", sensorlist);
    jint len = Method::callMethodInt(env, sensorlist, "java/util/List", "size", "()I");
    LOGI("check_sensor sensorlist len=%d", len);

    //以下为获取传感器供应商信息，如果出现 Google aosp  等信息说明当前为 google 系列手机
    jboolean jb = false;
    for (int i = 0; i < len; i++){
        jobject sensorObj = Method::callMethodObject(env, sensorlist,
                                                     "java/util/List",
                                                     "get",
                                                     "(I)Ljava/lang/Object;",
                                                     i);
        LOGI("check_sensor sensorObj %p", sensorObj);
        jstring sensorName = static_cast<jstring>(Method::callMethodObject(env, sensorObj,
                                                                           "android/hardware/Sensor",
                                                                           "getVendor",
                                                                           "()Ljava/lang/String;"));

        const char* sensorChar = (env->GetStringUTFChars(sensorName, &jb));
        LOGI("check_sensor sensorChar ------------------------> < %s >", sensorChar);
        char tmp[1024] = {0};
        Str::strcpy(tmp, const_cast<char *>(sensorChar));
        Str::toLower(tmp);
        if (Str::strstr(tmp, "aosp") != nullptr || Str::strstr(tmp, "google") != nullptr){
            //TODO 发现 google 系列手机
        }
        env->ReleaseStringUTFChars(sensorName, sensorChar);
    }
}
//***************************************************************************************************
UNEXPORT jobject AntiRom::getApplication(JNIEnv *env) {
    return Method::callStaticMethodObject(env, "android/app/ActivityThread",
                                          "currentApplication",
                                          "()Landroid/app/Application;");
}
//***************************************************************************************************
UNEXPORT jobject AntiRom::getApplicationContext(JNIEnv *env) {
    jobject application = AntiRom::getApplication(env);
    jobject context = Method::callMethodObject(env, application, "android/app/Application", "getApplicationContext", "()Landroid/content/Context;");
    return context;
}
//***************************************************************************************************
UNEXPORT void AntiRom::get_all_prop() {
    vector<char*> ret;
    if (Shell::run_shell("getprop", ret) != -1) {
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("get_all_prop -> %s", tmp);
            sleep(0.01);
            free(ret[i]);
        }
    }
}
//***************************************************************************************************

