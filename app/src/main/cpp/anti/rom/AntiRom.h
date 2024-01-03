//
// Created by Administrator on 2023/6/1.
//

#ifndef ANTISDK_ANTIROM_H
#define ANTISDK_ANTIROM_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiRom{
public:

    /**
     * 检查prop中部分字段是否含有aosp google lineage等
     */
    static void check_prop();
    static void check_prop_file();

    /**
     * 检查一些自定义rom的特有文件
     */
    static void check_file();

    /**
     * 检查传感器的供应商
     */
    static void check_sensor(JNIEnv *env);

    /**
    * 对prop中的全字段进行获取，综合判断其是否为自定义rom
    */
    static void get_all_prop();
private:
    static jobject getApplication(JNIEnv *env);
    static jobject getApplicationContext(JNIEnv *env);
};


#endif //ANTISDK_ANTIROM_H
