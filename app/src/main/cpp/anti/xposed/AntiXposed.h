//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTIXPOSED_H
#define ANTITOOLS_ANTIXPOSED_H

#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiXposed{
public:
    /**
     * 检查内存中是否加载了xposed的相关类
     * @param env
     */
    static void check_load_xposed_class(JNIEnv *env);

    /**
     * 注入xposed后，在/proc/self/maps 中会出现xposed相关模块
     */
    static void check_maps();

    /**
     * 动态注册函数 native_load_xposed_class 的壳
     * @param env
     * @param thiz
     */
    static void load_xposed_class(JNIEnv *env, jclass thiz);

    /**
     *
     */
    static void check_env(JNIEnv *env);
    static void check_vxp(JNIEnv *env);


    /**
     *
     */
    static void check_symbols();

private:
    static bool scan_line(char *map);
    static void read_line_scan(string str);
};



#endif //ANTITOOLS_ANTIXPOSED_H



