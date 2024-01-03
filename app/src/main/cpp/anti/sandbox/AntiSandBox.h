//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTIVA_H
#define ANTITOOLS_ANTIVA_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

// 应用级沙箱的检测：VA Ratel 等
class AntiSandBox{
public:

    /**
     * 多开app会加载一些自己的so文件,
     * VirtualXposed:  libepic.so          libva++.so
     * Ratel:          app_ratel_runtime   ratel_container-driver.jar    ratel_SandHooker
     */
    static void check_maps();
private:
    static bool scan_line_maps(char *map);
    static void read_line_scan_map(string str);

public:
    /**
     * 多开app会将应用的so加载到自己的应用目录下，所以检测so的路径就能发现其路径在多开框架的路径下
     * 当前检测方式需要获取本包名，故不能放在init_array中
     */
    static void check_so_path();
private:
    static void read_line_scan_so_path(string str);
    static bool scan_line_so_path(char *map);

public:
    /**
     * 检测多开
     * 软多开，一般绕不过uid，还是用宿主的uid,如果满足同一uid下的两个进程对应的包名
     * 此方法需在初始化ENV之后调用
     */
    static void check_uid();

    /**
     * 检查是否存在一些VA框架文件
     */
    static void check_file();
    static void check_ps();

    /**
     * 通过检测datadir是否可访问来判断当前是否为双开
     */
    static void check_dir(JNIEnv *env);
private:
    static void access_dir(string datadir);
    static void canRead_dir(JNIEnv *env, string datadir);

public:
    /**
     * 正常的环境，AM不可能是一个代理实例。通过判断AM是否是Proxy，便可直接判断环境是否正常
     */
    static void check_proxy(JNIEnv *env);
    static void check_pkgName(JNIEnv *env);
    static void check_service_list();
    static void check_mInstrumentation(JNIEnv *env);
    static void check_pms(JNIEnv* env);
    static void check_ActivityThread_H_mCallback(JNIEnv* env);

private:
    static jobject getApplication(JNIEnv *env);
    static jobject getApplicationContext(JNIEnv *env);
};

#endif //ANTITOOLS_ANTIVA_H
