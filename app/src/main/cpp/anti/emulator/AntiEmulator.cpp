//
// Created by Administrator on 2022/4/26.
//

#include "AntiEmulator.h"
#include "../../base/field/Field.h"
#include "../../base/str/Str.h"
#include "../../base/file/File.h"
#include "../../base/syscall/Syscall.h"
#include <dirent.h>
#include <vector>
#include "../../base/shell/Shell.h"
#include "../../base/call/Method.h"
#include <unistd.h>


UNEXPORT bool AntiEmulator::check_Build(JNIEnv *env) {
    char* sig = "Ljava/lang/String;";
    Field* field = new Field("android/os/Build", env);
    bool ret = false;

    // HARDWARE
    jstring HARDWARE = field->get_static_jstring_field("HARDWARE", sig);
    char *cHARDWARE = const_cast<char *>(env->GetStringUTFChars(HARDWARE, NULL));
    if (Str::strstr(cHARDWARE, "vbox86") != nullptr ||
        Str::strstr(cHARDWARE, "nox") != nullptr ||
        Str::strstr(cHARDWARE, "ttVM_x86") != nullptr ) {//是模拟器 不通过
        LOGI("check_Build => check FINGERPRINT find emulator => FINGERPRINT:%s", cHARDWARE);
        ret =  true;
    }
    LOGI("HARDWARE:%s", cHARDWARE);

    // FINGERPRINT
    jstring FINGERPRINT = field->get_static_jstring_field("FINGERPRINT", sig);
    char *cFINGERPRINT = const_cast<char *>(env->GetStringUTFChars(FINGERPRINT, NULL));
    if (Str::strstr(cFINGERPRINT, "Android") != nullptr || Str::strstr(cFINGERPRINT, "unknown") != nullptr ||
            Str::strstr(cFINGERPRINT, "generic/sdk/generic") != nullptr ||
            Str::strstr(cFINGERPRINT, "generic_x86/sdk_x86/generic_x86") != nullptr ||
            Str::strstr(cFINGERPRINT, "Andy") != nullptr ||
            Str::strstr(cFINGERPRINT, "ttVM_Hdragon") != nullptr ||
            Str::strstr(cFINGERPRINT, "generic/google_sdk/generic") != nullptr ||
            Str::strstr(cFINGERPRINT, "vbox86p") != nullptr ||
            Str::strstr(cFINGERPRINT, "generic/vbox86p/vbox86p") != nullptr) {//是模拟器 不通过
        LOGI("check_Build => check FINGERPRINT find emulator => FINGERPRINT:%s", cFINGERPRINT);
        ret =  true;
    }
    LOGI("FINGERPRINT:%s", cFINGERPRINT);

    // MODEL
    jstring MODEL = field->get_static_jstring_field("MODEL", sig);
    char *cMODEL = const_cast<char *>(env->GetStringUTFChars(MODEL, NULL));
    if (Str::strstr(cMODEL, "google_sdk") != nullptr || Str::strstr(cMODEL, "Emulator") != nullptr ||
                Str::strstr(cMODEL, "Droid4X") != nullptr ||
                Str::strstr(cMODEL, "TiantianVM") != nullptr ||
                Str::strstr(cMODEL, "Andy") != nullptr ||
                Str::strstr(cMODEL, "Android SDK built for x86_64") != nullptr ||
                Str::strstr(cMODEL, "Android SDK built for x86") != nullptr) {//是模拟器 不通过
        LOGI("check_Build => check MODEL find emulator => MODEL:%s", cMODEL);
        ret = true;
    }
    LOGI("MODEL:%s", cMODEL);

    // MANUFACTURER
    jstring MANUFACTURER = field->get_static_jstring_field("MANUFACTURER", sig);
    char *cMANUFACTURER = const_cast<char *>(env->GetStringUTFChars(MANUFACTURER, NULL));
    if (Str::strstr(cMANUFACTURER, "Genymotion") != nullptr ||
            Str::strstr(cMANUFACTURER, "Andy") != nullptr ||
            Str::strstr(cMANUFACTURER, "nox") != nullptr ||
            Str::strstr(cMANUFACTURER, "TiantianVM") != nullptr) {//是模拟器 不通过
        LOGI("check_Build => check MANUFACTURER find emulator => MANUFACTURER:%s", cMANUFACTURER);
        ret = true;
    }
    LOGI("MANUFACTURER:%s", cMANUFACTURER);

    // PRODUCT
    jstring PRODUCT = field->get_static_jstring_field("PRODUCT", sig);
    char *cPRODUCT = const_cast<char *>(env->GetStringUTFChars(PRODUCT, NULL));
    if (Str::strstr(cPRODUCT, "sdk") != nullptr ||
            Str::strstr(cPRODUCT, "Andy") != nullptr ||
            Str::strstr(cPRODUCT, "Droid4X") != nullptr ||
            Str::strstr(cPRODUCT, "nox") != nullptr ||
            Str::strstr(cPRODUCT, "vbox86p") != nullptr ) {//是模拟器 不通过
        LOGI("check_Build => check PRODUCT find emulator => PRODUCT:%s", cPRODUCT);
        ret = true;
    }
    LOGI("PRODUCT:%s", cPRODUCT);

    // BRAND   DEVICE
    jstring BRAND = field->get_static_jstring_field("BRAND", sig);
    char *cBRAND = const_cast<char *>(env->GetStringUTFChars(BRAND, NULL));
    jstring DEVICE = field->get_static_jstring_field("DEVICE", sig);
    char *cDEVICE = const_cast<char *>(env->GetStringUTFChars(DEVICE, NULL));
    if ((Str::strstr(cBRAND, "generic") != nullptr && Str::strstr(cDEVICE, "generic") == cDEVICE) ||
            Str::strstr(cBRAND, "Andy") != nullptr ||
            Str::strstr(cBRAND, "Droid4X") != nullptr ||
            Str::strstr(cBRAND, "nox") != nullptr ||
            Str::strstr(cBRAND, "vbox86p") != nullptr ) {//是模拟器 不通过
        LOGI("check_Build => check BRAND find emulator => BRAND:%s", cBRAND);
        LOGI("check_Build => check DEVICE find emulator => DEVICE:%s", cDEVICE);
        ret = true;
    }
    LOGI("BRAND:%s", cBRAND);
    LOGI("DEVICE:%s", cDEVICE);
    env->ReleaseStringUTFChars(HARDWARE, cHARDWARE);
    env->ReleaseStringUTFChars(FINGERPRINT, cFINGERPRINT);
    env->ReleaseStringUTFChars(MODEL, cMODEL);
    env->ReleaseStringUTFChars(MANUFACTURER, cMANUFACTURER);
    env->ReleaseStringUTFChars(PRODUCT, cPRODUCT);
    env->ReleaseStringUTFChars(BRAND, cBRAND);
    env->ReleaseStringUTFChars(DEVICE, cDEVICE);
    return ret;
}

UNEXPORT bool AntiEmulator::check_prop() {
    char tmp[1024] = {0};

    // ro.kernel.qemu
    File::read_property("ro.kernel.qemu", reinterpret_cast<char *>(&tmp));
    if (Str::strcmp(tmp, "1") == 0){
        return true;
    }
    LOGI("check_prop ro.kernel.qemu -> %s", tmp);

    // ro.product.model
    File::read_property("ro.product.model", reinterpret_cast<char *>(&tmp));
    if (Str::strstr(tmp, "sdk") != nullptr || Str::strstr(tmp, "Android SDK") != nullptr){
        return true;
    }
    LOGI("check_prop ro.product.model -> %s", tmp);

    // ro.product.cpu.abi
    File::read_property("ro.product.cpu.abi", reinterpret_cast<char *>(&tmp));
    if (Str::strcmp(tmp, "x86") == 0){
        return true;
    }
    LOGI("check_prop ro.product.cpu.abi -> %s", tmp);
    return false;
}

UNEXPORT void AntiEmulator::check_file() {
    char *(path[]) = {
            "/system/bin/androVM-prop",   //检测androidVM
            "/system/bin/microvirt-prop", //检测逍遥模拟器--新版本找不到特征
            "/system/lib/libdroid4x.so",  //检测海马模拟器
            "/system/bin/windroyed",      //检测文卓爷模拟器
            "/system/bin/nox-prop",       //检测夜神模拟器--某些版本找不到特征
            "/system/lib/libnoxspeedup.so",//检测夜神模拟器
            "/system/bin/ttVM-prop",      //检测天天模拟器
            "/data/.bluestacks.prop",     //检测bluestacks模拟器  51模拟器
            "/system/bin/duosconfig",     //检测AMIDuOS模拟器
            "/system/etc/xxzs_prop.sh",   //检测星星模拟器
            "/system/etc/mumu-configs/device-prop-configs/mumu.config", //网易MuMu模拟器
            "/system/priv-app/ldAppStore",   //雷电模拟器
            "/system/bin/ldinit",             //雷电模拟器
            "/system/bin/ldmountsf",          //雷电模拟器
            "/system/app/AntStore",          //小蚁模拟器
            "/system/app/AntLauncher",       //小蚁模拟器
            "vmos.prop",                     //vmos虚拟机
            "fstab.titan",                   //光速虚拟机
            "init.titan.rc",                 //光速虚拟机
            "x8.prop",                       //x8沙箱和51虚拟机
            "/system/lib/libc_malloc_debug_qemu.so", //AVD QEMU
            "/system/bin/microvirtd",
            "/dev/socket/qemud",
            "/dev/qemu_pipe"};
    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
        if (Syscall::check_file_or_dir_exists(path[i])){
            LOGI("check_file  %s file existing", path[i]);
            // TODO 风险

        }
    }
}

UNEXPORT void AntiEmulator::check_cpu_temp() {
    DIR *dirptr = NULL; //当前手机的温度检测，手机下均有thermal_zone文件
    int count = 0;
    struct dirent *entry;
    if ((dirptr = opendir("/sys/class/thermal/")) != NULL) {
        while (entry = readdir(dirptr)) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }
            char *tmp = entry->d_name;
            if (Str::strstr(tmp, "thermal_zone") != NULL) {
                count++;
            }
        }
        closedir(dirptr);
    } else {
        LOGE("AntiEmulator::check_cpu_temp open thermal fail");
    }
    if (count == 0){
        // TODO 此时为模拟器
    }
    LOGI("AntiEmulator::check_cpu_temp count=%d", count);
}

UNEXPORT void AntiEmulator::check_cpu() {
    string str = Syscall::readFile((char*)"/proc/cpuinfo");
    char* split = (char*)"\n";
    string strs = str + split; // 在字符串末尾也加入分隔符，方便截取最后一段
    size_t pos = strs.find(split);
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        if (Str::strstr(const_cast<char *>(temp.c_str()), "Hardware") != NULL){
            LOGI("AntiEmulator::check_cpu find %s", temp.c_str());
            return;
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
    // TODO 没找到 Hardware，说明为模拟器

}
//***************************************************************************************************
UNEXPORT void AntiEmulator::check_version() {
    string str = Syscall::readFile((char*)"/proc/version");
    char* split = (char*)"\n";
    string strs = str + split; // 在字符串末尾也加入分隔符，方便截取最后一段
    size_t pos = strs.find(split);
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        if (Str::strstr(const_cast<char *>(temp.c_str()), "qemu") != NULL ||
            Str::strstr(const_cast<char *>(temp.c_str()), "qemu") != NULL){
            // TODO 发现模拟器
            LOGI("AntiEmulator::check_version find %s", temp.c_str());
            return;
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
}



//***************************************************************************************************
UNEXPORT jobject AntiEmulator::getApplication(JNIEnv *env) {
    return Method::callStaticMethodObject(env, "android/app/ActivityThread",
                                          "currentApplication",
                                          "()Landroid/app/Application;");
}

//***************************************************************************************************
UNEXPORT jobject AntiEmulator::getApplicationContext(JNIEnv *env) {
    jobject application = AntiEmulator::getApplication(env);
    jobject context = Method::callMethodObject(env, application, "android/app/Application", "getApplicationContext", "()Landroid/content/Context;");
    return context;
}
//***************************************************************************************************
UNEXPORT void AntiEmulator::check_sensor(JNIEnv *env) {
    jobject context = AntiEmulator::getApplicationContext(env);
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
    if (len < 20){
        // todo 传感器少于20 认为当前环境为非真机环境
    }

}
//***************************************************************************************************
UNEXPORT void AntiEmulator::check_camera(JNIEnv *env) {
    jobject context = AntiEmulator::getApplicationContext(env);
    LOGI("check_camera context %p", context);
    jstring camera = env->NewStringUTF("camera");
    jobject objService = Method::callMethodObject(env, context,
                                                                 "android/content/Context",
                                                                 "getSystemService",
                                                                 "(Ljava/lang/String;)Ljava/lang/Object;",
                                                                    camera);
    env->DeleteLocalRef(camera);
    LOGI("check_camera objService %p", objService);
    jobjectArray cameralist = static_cast<jobjectArray>(Method::callMethodObject(env, objService,
                                                                                 "android/hardware/camera2/CameraManager",
                                                                                 "getCameraIdList",
                                                                                 "()[Ljava/lang/String;"));
    LOGI("check_camera cameralist %p", cameralist);
    int len = env->GetArrayLength(cameralist);
    LOGI("check_camera cameralist len %d", len);
    if (len < 2){
        // todo 摄像头少于2 认为当前环境为非真机环境

    }
//    for (int i = 0; i < len + 1; i++){
//        jobject ca = env->GetObjectArrayElement(cameralist, i);
//        LOGI("check_camera camera -> %d : %p", i, ca);
//    }

}
//***************************************************************************************************
UNEXPORT void AntiEmulator::check_mounts() {
    char *(path[]) = {"/proc/mounts", "/proc/self/mountstats", "/proc/self/mountinfo"};
    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
        string status = Syscall::readFile(path[i]);
        if (status == "null"){
            break;
        }
        size_t pos = status.find("\n");
        while (pos != status.npos)
        {
            string temp = status.substr(0, pos);
            if (Str::strstr(const_cast<char *>(temp.c_str()), (char*)"docker") != nullptr){
                //TODO 发现docker, 可能存在云手机挂载docker

            }
            //去掉已分割的字符串,在剩下的字符串中进行分割
            status = status.substr(pos + 1, status.size());
            pos = status.find("\n");
        }
    }
}
//***************************************************************************************************


UNEXPORT void AntiEmulator::get_all_prop() {
    vector<char*> ret;
    if (Shell::run_shell("getprop", ret) != -1) {
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("get_all_prop -> %s", tmp);
            // TODO 回传本地
            sleep(0.01);
            free(ret[i]);
        }
    }
}
//***************************************************************************************************
