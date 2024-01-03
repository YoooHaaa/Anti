//
// Created by Administrator on 2022/4/26.
//

#include "AntiSandBox.h"
#include <unistd.h>
#include <cstdio>
#include "../../base/call/Method.h"
#include "../../base/field/Field.h"
#include "../../base/new/NewObject.h"
#include "../../base/thread/Thread.h"
#include "../../base/syscall/Syscall.h"
#include "../../base/str/Str.h"
#include "../../base/env/Env.h"
#include "../../base/shell/Shell.h"

//*****************************************************************************************************

UNEXPORT bool AntiSandBox::scan_line_maps(char *map) {
    if (Str::strstr(map, "libepic.so") != NULL && Str::strstr(map, "libva++.so") != NULL){ //VirtualXposed
        return true;
    }
    if (Str::strstr(map, "app_ratel_runtime") != NULL && Str::strstr(map, "ratel_container-driver.jar") != NULL){ //Ratel
        return true;
    }
    return false;
}


UNEXPORT void AntiSandBox::read_line_scan_map(string str){
    char* split = (char*)"\n";
    string strs = str + split; // 在字符串末尾也加入分隔符，方便截取最后一段
    size_t pos = strs.find(split);
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        //LOGI("read_line_scan maps -> %s", temp.c_str());
        if (AntiSandBox::scan_line_maps(const_cast<char *>(temp.c_str()))){
            //TODO
            LOGI("---------------------------read_line_scan_map find VA -> %s", temp.c_str());
            return;
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
}

UNEXPORT void AntiSandBox::check_maps() {
    Thread::lock_mutex();
    Env::thread_share_switch_maps = true;
    Thread::unLock_mutex();
    string str = Syscall::readFile((char*)"/proc/self/maps");
    Thread::lock_mutex();
    Env::thread_share_switch_maps = false;
    Thread::unLock_mutex();
    if (str != "null"){
        AntiSandBox::read_line_scan_map(str);
    }
}

//*****************************************************************************************************

UNEXPORT void AntiSandBox::check_uid() {
    LOGI("------------------> 0 ");
    vector<char*> ret;
    char cmd[512] = {0};
    int count = 0;
    sprintf(cmd, "%s %s", "ps | grep", globalEnv->pkgname); //globalEnv->pkgname);
    LOGI("------------------> 1 ");
    if (Shell::run_shell(cmd, ret) != -1) {
        LOGI("------------------> 2 ");
        for (int i = 0; i < ret.size(); i++){
            LOGI("------------------> 3 ");
            if (Str::strstr(ret[i], globalEnv->pkgname) != nullptr
                && Str::strstr(ret[i], ":") == nullptr){
                count++;
                LOGI("check_uid -> %s count=%d", ret[i], count);
            }
            free(ret[i]);
        }
    }
    LOGI("------------------> 4 ");
    if (ret.size() == 0){
        // TODO 在VA进程中无法获取自己的ps信息，还和系统版本有关
    }
    if (count > 1){
        // TODO 有两个进程同uid
    }
}
//*****************************************************************************************************
UNEXPORT void AntiSandBox::check_file() {
//    char *(path[]) = {"/system/lib/libdroid4x.so",
//                      "/system/bin/windroyed",
//                      "/system/bin/microvirtd",
//                      "/system/bin/nox-prop",
//                      "/system/bin/ttVM-prop"};
//    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
//        if (Syscall::check_file_or_dir_exists(path[i])){
//            LOGI("%s file existing", path[i]);
//            return true;
//        }
//    }
//    return false;
}
//*****************************************************************************************************

UNEXPORT bool AntiSandBox::scan_line_so_path(char *map) {
    if (Str::strstr(map, globalEnv->pkgname) != NULL){
        if (Str::strstr(map, ".so") != NULL){
            if (Str::strstr(map, "/virtual/data/user/0/") != NULL){ //存在VA框架的虚拟路径
                return true;
            }
        }
    }
    return false;
}

UNEXPORT void AntiSandBox::read_line_scan_so_path(string str) {
    char* split = (char*)"\n";
    string strs = str + split; // 在字符串末尾也加入分隔符，方便截取最后一段
    size_t pos = strs.find(split);
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        //LOGI("read_line_scan maps -> %s", temp.c_str());
        if (AntiSandBox::scan_line_so_path(const_cast<char *>(temp.c_str()))){
            //TODO 发现VA
            LOGI("---------------------------read_line_scan_so_path find VA -> %s", temp.c_str());
            return;
        }
        // 去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
}

//*****************************************************************************************************
UNEXPORT void AntiSandBox::check_so_path() {
    Thread::lock_mutex();
    Env::thread_share_switch_maps = true;
    Thread::unLock_mutex();
    string str = Syscall::readFile((char*)"/proc/self/maps");
    Thread::lock_mutex();
    Env::thread_share_switch_maps = false;
    Thread::unLock_mutex();
    if (str != "null"){
        AntiSandBox::read_line_scan_so_path(str);
    }
}
//*****************************************************************************************************
UNEXPORT void AntiSandBox::check_ps() {
    vector<char*> ret;
    if (Shell::run_shell("ps -ef", ret) != -1) {
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("check_ps -> %s", tmp);

            if (i > 2){
                /*
                    check_ps -> UID            PID  PPID C STIME TTY          TIME CMD
                    check_ps -> u0_a759      22017   740 3 17:16:15 ?     00:00:00 com.serven.scorpion.bit64:helper
                    check_ps -> u0_a759      22132   740 60 17:16:31 ?    00:00:00 com.yooha.antisdk
                    check_ps -> u0_a759      22176 22132 2 17:16:32 ?     00:00:00 ps -ef
                 */

                // TODO 检测到有其他进程（必要时需要排除掉是否是自身进程）

            }
            free(ret[i]);
        }
    }
}
//*****************************************************************************************************
void AntiSandBox::check_dir(JNIEnv *env) {
    jobject context = AntiSandBox::getApplicationContext(env);
    LOGI("check_dir context %p", context);

    jobject objDir = Method::callMethodObject(env, context,
                                                           "android/content/Context",
                                                           "getFilesDir",
                                                           "()Ljava/io/File;");
    LOGI("check_dir objDir %p", objDir);

    jstring jsDir = static_cast<jstring>(Method::callMethodObject(env, objDir,
                                                                  "java/io/File",
                                                                  "getAbsolutePath",
                                                                  "()Ljava/lang/String;"));
    jboolean jb = false;
    const char* charPkg = (env->GetStringUTFChars(jsDir, &jb));
    LOGI("check_dir jsDir  ------------------------> < %s >", charPkg);

    string temp;
    temp.append(charPkg);
    size_t pos = temp.rfind("/");
    temp = temp.substr(0, pos);
    LOGI("check_dir tmp  ------------------------> < %s >", temp.c_str());

    AntiSandBox::access_dir(temp);
    AntiSandBox::canRead_dir(env, temp);
    env->ReleaseStringUTFChars(jsDir, charPkg);
}

/**
 * 方法一
 */
void AntiSandBox::access_dir(string datadir) {
    if (access((datadir + "/../").c_str(), R_OK) == 0){
//        多开环境日志如下：
//        check_dir jsDir  ------------------------> < /data/user/0/com.serven.scorpion.bit64/virtual/data/user/0/com.yooha.antisdk/files >
//        check_dir tmp  ------------------------> < /data/user/0/com.serven.scorpion.bit64/virtual/data/user/0/com.yooha.antisdk > 06-05 11:32:00.972 18886 18886 I yooha-anti-ndk: access_dir  ------------------------> find sandbox
        // TODO 多开环境
        LOGI("access_dir  ------------------------> find sandbox");
    }
}
/**
 * 方法二
 */
void AntiSandBox::canRead_dir(JNIEnv *env, string datadir) {
    string path = datadir + (char*)"/..";
    LOGI("canRead_dir path = %s", path.c_str());

    jstring jsPath = env->NewStringUTF(path.c_str());
    jobject pathFile = NewObject::new_object(env, "java/io/File", "(Ljava/lang/String;)V", jsPath);
    LOGI("canRead_dir pathFile = %p", pathFile);

    jboolean read = Method::callMethodBoolean(env, pathFile,
                                              "java/io/File",
                                              "canRead",
                                              "()Z");
    if (read){
        // TODO 多开环境
        LOGI("canRead_dir  ------------------------> find sandbox");
    }
    env->DeleteLocalRef(jsPath);
}

//***************************************************************************************************
UNEXPORT jobject AntiSandBox::getApplication(JNIEnv *env) {
    return Method::callStaticMethodObject(env, "android/app/ActivityThread",
                                          "currentApplication",
                                          "()Landroid/app/Application;");
}
//***************************************************************************************************
UNEXPORT jobject AntiSandBox::getApplicationContext(JNIEnv *env) {
    jobject application = AntiSandBox::getApplication(env);
    jobject context = Method::callMethodObject(env, application, "android/app/Application", "getApplicationContext", "()Landroid/content/Context;");
    return context;
}
//***************************************************************************************************

UNEXPORT void AntiSandBox::check_proxy(JNIEnv *env) {
    Field* field = new Field("android/app/ActivityManager", env);
    jobject objIActivityManagerSingleton = field->get_static_object_field("IActivityManagerSingleton", "Landroid/util/Singleton;");
    LOGI("check_proxy objIActivityManagerSingleton %p", objIActivityManagerSingleton);

    jobject objSingleton = Method::callMethodObject(env, objIActivityManagerSingleton,
                                              "android/util/Singleton",
                                              "get",
                                              "()Ljava/lang/Object;");
    LOGI("check_proxy objSingleton %p", objSingleton);

    jclass clsProxy = env->FindClass("java/lang/reflect/Proxy");
    LOGI("check_proxy clsProxy %p", clsProxy);

    jboolean jb = Method::callMethodBoolean(env, clsProxy,
                                                    "java/lang/Class",
                                                    "isInstance",
                                                    "(Ljava/lang/Object;)Z",
                                                        objSingleton);
    if (jb){
        // TODO 正常的环境，AM不可能是一个代理实例。通过判断AM是否是Proxy，便可直接判断环境是否正常
        LOGI("check_proxy js true ------------------------> find sandbox");
    }
}
//***************************************************************************************************
UNEXPORT void AntiSandBox::check_pkgName(JNIEnv *env) {
    jobject context = AntiSandBox::getApplicationContext(env);
    LOGI("check_pkgName context %p", context);

    jstring jsBasePackageName = static_cast<jstring>(Method::callMethodObject(env, context,
                                                                              "android/content/Context",
                                                                              "getBasePackageName",
                                                                              "()Ljava/lang/String;"));
    LOGI("check_pkgName jsBasePackageName %p", jsBasePackageName);

    jboolean jb = false;
    const char* charBasePackageName = (env->GetStringUTFChars(jsBasePackageName, &jb));
    LOGI("check_pkgName charBasePackageName  ------------------------> < %s >", charBasePackageName);

    jstring jsOpPackageName = static_cast<jstring>(Method::callMethodObject(env, context,
                                                                              "android/content/Context",
                                                                              "getOpPackageName",
                                                                              "()Ljava/lang/String;"));
    LOGI("check_pkgName jsOpPackageName %p", jsOpPackageName);

    const char* charOpPackageName = (env->GetStringUTFChars(jsOpPackageName, &jb));
    LOGI("check_pkgName charOpPackageName  ------------------------> < %s >", charOpPackageName);

    if (Str::strcmp("com.yooha.antisdk", const_cast<char *>(charBasePackageName)) != 0 || Str::strcmp("com.yooha.antisdk", const_cast<char *>(charOpPackageName)) != 0){
        // TODO 检测到双开框架的包名
        LOGI("check_pkgName  ------------------------> find sandbox");
    }
    env->ReleaseStringUTFChars(jsBasePackageName, charBasePackageName);
    env->ReleaseStringUTFChars(jsOpPackageName, charOpPackageName);
}
//***************************************************************************************************
void AntiSandBox::check_mInstrumentation(JNIEnv *env) {
    Field* field = new Field("android/app/ActivityThread", env);
    jobject objActivityThread = field->get_static_object_field("sCurrentActivityThread", "Landroid/app/ActivityThread;");
    LOGI("check_mInstrumentation objActivityThread %p", objActivityThread);

    jobject objInstrumentation = Method::callMethodObject(env, objActivityThread,
                                                    "android/app/ActivityThread",
                                                    "getInstrumentation",
                                                    "()Landroid/app/Instrumentation;");
    LOGI("check_mInstrumentation objInstrumentation %p", objInstrumentation);

    jobject clsInstrumentation = Method::callMethodObject(env, objInstrumentation, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
    LOGI("check_mInstrumentation clsInstrumentation %p", clsInstrumentation);

    jstring clsInstrumentationName = static_cast<jstring>(Method::callMethodObject(env,
                                                                                   clsInstrumentation,
                                                                                   "java/lang/Class",
                                                                                   "getName",
                                                                                   "()Ljava/lang/String;"));
    LOGI("check_mInstrumentation clsInstrumentationName %p", clsInstrumentationName);

    jboolean jb = false;
    const char* charclsInstrumentationName = (env->GetStringUTFChars(clsInstrumentationName, &jb));
    LOGI("check_mInstrumentation charclsInstrumentationName %s", charclsInstrumentationName);

    if (Str::strcmp("android.app.Instrumentation", const_cast<char *>(charclsInstrumentationName)) != 0 ){
        // TODO 检测到双开框架的包名
        LOGI("check_mInstrumentation  ------------------------> find sandbox");
    }

    env->ReleaseStringUTFChars(clsInstrumentationName, charclsInstrumentationName);
}
//***************************************************************************************************
void AntiSandBox::check_pms(JNIEnv *env) {
    Field* field = new Field("android/app/ActivityThread", env);
    jobject objIPackageManager = field->get_static_object_field("sPackageManager", "Landroid/content/pm/IPackageManager;");
    LOGI("check_pms objIPackageManager %p", objIPackageManager);

    jobject clsIPackageManager = Method::callMethodObject(env, objIPackageManager, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
    LOGI("check_pms clsIPackageManager %p", clsIPackageManager);

    jstring clsIPackageManagerName = static_cast<jstring>(Method::callMethodObject(env,
                                                                                   clsIPackageManager,
                                                                                   "java/lang/Class",
                                                                                   "getName",
                                                                                   "()Ljava/lang/String;"));
    LOGI("check_pms clsIPackageManagerName %p", clsIPackageManagerName);

    jboolean jb = false;
    const char* charclsIPackageManagerName = (env->GetStringUTFChars(clsIPackageManagerName, &jb));
    LOGI("check_pms charclsIPackageManagerName %s", charclsIPackageManagerName);

    if (Str::strcmp("android.content.pm.IPackageManager$Stub$Proxy", const_cast<char *>(charclsIPackageManagerName)) != 0 ){
        // TODO 检测到双开框架的包名
        LOGI("check_pms  ------------------------> find sandbox");
    }

    env->ReleaseStringUTFChars(clsIPackageManagerName, charclsIPackageManagerName);
}
//***************************************************************************************************
void AntiSandBox::check_ActivityThread_H_mCallback(JNIEnv *env) {
    Field* field = new Field("android/app/ActivityThread", env);
    jobject objActivityThread = field->get_static_object_field("sCurrentActivityThread", "Landroid/app/ActivityThread;");
    LOGI("check_ActivityThread_H_mCallback objActivityThread %p", objActivityThread);

    jobject objHandler = Method::callMethodObject(env, objActivityThread,
                                                          "android/app/ActivityThread",
                                                          "getHandler",
                                                          "()Landroid/os/Handler;");
    LOGI("check_ActivityThread_H_mCallback objHandler %p", objHandler);

    Field* fieldHandler = new Field("android/os/Handler", env);
    jobject objmCakkback = fieldHandler->get_object_field(objHandler, "mCallback", "Landroid/os/Handler$Callback;");
    LOGI("check_ActivityThread_H_mCallback objmCakkback %p", objmCakkback);

    if (objmCakkback != 0){
        // TODO 正常情况下 ActivityThread$H.mCallback = null
        LOGI("check_ActivityThread_H_mCallback ------------------------> find sandbox");
    }
}
//***************************************************************************************************
void AntiSandBox::check_service_list() {
    vector<char*> ret;
    if (Shell::run_shell("service list", ret) != -1){
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("check_service_list -> %s", tmp);
            // TODO 回传service列表到后端进行判断
//            if (Str::strstr(tmp, "xiaomi") != nullptr){
//                LOGI("check_service_list -> find xiaomi service");
//                return;
//            }
            free(ret[i]);
        }
    }
}
//***************************************************************************************************


