//
// Created by Administrator on 2022/4/26.
//

#include "AntiRoot.h"
#include <cstdio>
#include "../../base/file/File.h"
#include "../../base/str/Str.h"
#include <vector>
#include <cerrno>
#include "../../base/syscall/Syscall.h"
#include "../../base/shell/Shell.h"


//***************************************************************************************************



//***************************************************************************************************


UNEXPORT bool AntiRoot::check_su_for_which() {
    vector<char*> ret;
    if (Shell::run_shell("which su", ret) != -1){
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("check_su_for_which -> %s", tmp);
            if (Str::strstr(tmp, "/su") != nullptr){
                LOGI("check_su_for_which -> find su");
                return true;
            }
            free(ret[i]);
        }
    }
    return false;
}

//***************************************************************************************************
UNEXPORT bool AntiRoot::test_popen() {
    vector<char*> ret;
    if (Shell::run_shell("pm path com.yooha.antisdk", ret) != -1){
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("test_popen -> %s", tmp);
            free(ret[i]);
        }
    }
    return false;
}


//***************************************************************************************************

//bool AntiRoot::check_su_for_su() {
//    vector<string> ret;
//    if (Shell::run_shell("su || cat /system/build.prop", ret) != -1){
//        if (ret.size() == 0){
//            //LOGI("check_su_for_su -> find su");
//            return true;
//        }else{
//            return false;
//        }
//    }
//    return false;
//}

//***************************************************************************************************

bool AntiRoot::check_su_for_file() {
    char *(path[]) = {"/system/bin/su", "/system/xbin/su", "/system/sbin/su", "/sbin/su", "/vendor/bin/su", "/su/bin/su",
                      "/data/local/su", "/data/local/tmp/su", "/data/local/xbin/su", "/data/local/tmp/bin/su", "/system/app/Superuser.apk",
                      "/sbin/bin/su", "/system/bin/failsafe/su", "com.topjohnwu.magisk"};
    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
        if (Syscall::check_file_or_dir_exists(path[i])){
            LOGI("check_su_for_file %s file existing, find su", path[i]);
            return true; // true 表示有 root 风险
        }
    }
    return false;
}

//***************************************************************************************************

UNEXPORT bool AntiRoot::check_tags() {
    char tmp[64] = {0};
    File::read_property("ro.build.tags", reinterpret_cast<char *>(&tmp));
    LOGI("check_tags -> %s", tmp);
    if (Str::strcmp("test_keys", tmp) == 0) {  // ro.build.tags=release-keys
        LOGI("check_tags user=test_keys, find su");
        return true; //true 表示有 root 风险
    }
    return false;
}
//***************************************************************************************************
bool AntiRoot::check_type() {
    char tmp[64] = {0};
    File::read_property("ro.build.type", reinterpret_cast<char *>(&tmp));
    LOGI("check_type -> %s", tmp);
    if (Str::strcmp("user", tmp) == 0) {  // ro.build.type=user
        return false;
    }
    LOGI("check_type user=%s, find su");
    return true; //true 表示有 root 风险
}
//***************************************************************************************************
bool AntiRoot::check_secure() {
    char tmp[8] = {0};
    File::read_property("ro.secure", reinterpret_cast<char *>(&tmp));
    LOGI("check_secure -> %s", tmp);
    if (Str::strcmp("0", tmp) == 0) {   // ro.secure=0 代表是userdbg/eng版本
        LOGI("check_secure ro.secure=%d find su", tmp);
        return true; //true 表示有 root 风险
    }
    return false;
}
//***************************************************************************************************
bool AntiRoot::check_magisk() {
    string status = Syscall::readFile("/proc/self/mounts"); // 读取 /proc/self/mounts 文件
    if (status == "null"){
        return false;
    }
    size_t pos = status.find("\n");
    while (pos != status.npos)
    {
        string temp = status.substr(0, pos);
        if (Str::strstr(const_cast<char *>(temp.c_str()), (char*)"magisk") != nullptr ||
                Str::strstr(const_cast<char *>(temp.c_str()), (char*)"core/mirror") != nullptr ||
                Str::strstr(const_cast<char *>(temp.c_str()), (char*)"core/img") != nullptr){
            //TODO 发现装了magisk
            LOGI("check_magisk -> find magisk:%s", temp.c_str());
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        status = status.substr(pos + 1, status.size());
        pos = status.find("\n");
    }
    return false;
}

//另一种方式是执行adb shell mount | grep magisk 也能检测出magisk痕迹

//***************************************************************************************************
void AntiRoot::check_adb() {

}
//***************************************************************************************************
void AntiRoot::check_developer() {

}
//***************************************************************************************************





