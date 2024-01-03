//
// Created by Administrator on 2022/4/15.
//



#ifndef ANTITOOLS_ANTIROOT_H
#define ANTITOOLS_ANTIROOT_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiRoot{
public:
    /**
     * 通过：adb shell/which su 检查是否存在su可执行文件
     * @return
     */
    static bool check_su_for_which();

    static bool test_popen();

    /**
     * 通过执行su命令，判断是否存在rot权限,此方法会弹窗提示要申请超级用户，用户体验差，不建议使用
     * @return
     */
    //static bool check_su_for_su();

    /**
     * 通过打开文件，查看是否存在su文件
     * @return
     */
    static bool check_su_for_file();

    /**
     * 通过获取 ro.build.tags 字段的值，并判断是否为test_keys来判断当前系统是否为测试版,此方法不太靠谱，有些厂商出厂设备也是test_keys
     * @return
     */
    static bool check_tags();

    /**
     * 检查 ro.build.type 字段的值，判断是否user版
     * @return
     */
    static bool check_type();

    /**
     * ro.secure=0 代表是userdbg/eng版本
     * @return
     */
    static bool check_secure();

    /**
     * ro.debuggable=1 表示可调式
     * @return
     */
    static bool check_debuggable();

    /**
     * 安装magisk后，会改变/proc/self/mounts文件的内容，出现标志性字符串
     * @return
     */
    static bool check_magisk();

    /**
     * 检测adb连接
     */
    static void check_adb();

    /**
     * 检测开发者选项调试是否打开
     */
    static void check_developer();

};

#endif //ANTITOOLS_ANTIROOT_H
