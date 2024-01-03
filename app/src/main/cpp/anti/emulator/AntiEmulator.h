//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTIEMULATOR_H
#define ANTITOOLS_ANTIEMULATOR_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiEmulator{
public:

    /**
     *  通过 android/os/Build 类检测虚拟机,此方法要在JNI_OnLoad后才能开始检测，因为需要JNIEnv
     * @param env
     * @return true-检测到虚拟机 false-未检测到虚拟机
     */
    static bool check_Build(JNIEnv *env);

    /**
     *  通过检查prop.build文件中的字段来判断是否为模拟器
     *  ro.kernel.qemu：    模拟器中为1，通常在正常手机中没有该属性
     *  ro.build.tags：     部分模拟器中为test-keys，通常在正常手机中它的值为release-keys（不准确）
     *  ro.product.model：  模拟器中为sdk，通常在正常手机中它的值为手机的型号
     *  ro.product.cpu.abi：x86架构在真机中极少，后续若出现单独适配
     * @return true-检测到虚拟机 false-未检测到虚拟机
     */
    static bool check_prop();

    /**
     * 检查是否存在一些模拟器框架文件
     */
    static void check_file();

    /**
     * 检测cpu温度，一般模拟器没有此文件:/sys/class/thermal/thermal_zone...
     */
    static void check_cpu_temp();

    /**
     * 读取 /proc/cpuinfo，真机一般会获取到hardware
     */
    static void check_cpu();

    /**
    * 读取 /proc/version，模拟器一般有qemu，tencent
    */
    static void check_version();

    /**
     * 检查传感器个数，少于20个认为是模拟器
     */
     static void check_sensor(JNIEnv *env);

    /**
    * 检查摄像头个数
    */
    static void check_camera(JNIEnv *env);

    /**
    * 检查mounts文件，检测里面是否包含docker关键字 ，防止一些云手机搞虚拟化，通过使用docker进行挂载
    */
    static void check_mounts();

    /**
     * 对prop中的全字段进行获取，综合判断其是否为模拟器或云手机
     */
    static void get_all_prop();
private:
    static jobject getApplication(JNIEnv *env);
    static jobject getApplicationContext(JNIEnv *env);
};
#endif //ANTITOOLS_ANTIEMULATOR_H
