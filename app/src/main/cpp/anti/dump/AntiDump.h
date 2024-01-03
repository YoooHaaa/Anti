//
// Created by Administrator on 2022/6/30.
//

#ifndef ANTISDK_ANTIDUMP_H
#define ANTISDK_ANTIDUMP_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiDump{
public:
    /**
     * 监控/proc/pid/maps  /proc/pid/mem  /proc/pid/pagemap 的数据变化
     */
    static void monitor_mem();

    /**
     * 检查fart的缓存目录
     */
    static void check_fart();

    /**
     * 集成dobby框架，hook write函数，检查是否在写 dex/so 文件
     * dex: dex.035 -> 0x64, 0x65, 0x78, 0x0A, 0x30, 0x33, 0x35, 0x00
     * elf: .ELF    -> 0x7F, 0x45, 0x4C, 0x46
     */
    static void hook_write();
};



#endif //ANTISDK_ANTIDUMP_H
