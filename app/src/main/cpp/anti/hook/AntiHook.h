//
// Created by Administrator on 2023/5/29.
//

#ifndef ANTISDK_ANTIHOOK_H
#define ANTISDK_ANTIHOOK_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiHook{
public:
    /**
     * 检查maps中是否存在inlinehook、gothook框架的so文件
     */
    static void check_maps();

    /**
     * gothook 框架检测
     * 检测原理：检测系统lib库的so文件的指定函数地址与本进程so的导入函数地址是否相等。比如libc库的open函数。这种方式需要先计算好got起始地址，导入表函数位置。
     */
    static void check_gothook();
private:
    static bool compare_got(const char *libpath, const char* symname);

    /**
     * inlinehook 框架检测
     * 原理：.text段的内容在ELF加载前后没有发生变化，对比内存中的.text段和文件中的.text的crc是否相等
     */
    static void check_inlinehook();
private:
    /**
     * 获取text段的crc值
     */
    static bool compare_text_section_crc(const char* path);
public:
    /**
     * svc hook 检测：
     * 原理：针对SVC Hook目前就seccomp的bpf最常用，这种就是设定filter过滤器规则，拦截到指定的系统调用号后会产生一个信号量，这种信号量是你设定的，
       然后就会执行事先使用sigaction注册的函数。我们可以这样检测：注册一个 sigaction 处理逻辑，然后主动使用 sigqueue函数发送信号量，再接收，
       如果没有接收到说明被seccomp的bpf规则拦截了，SVC已经被Hook了。
     */
    static void check_svc_hook();

    /**
     * 检查PrettyMethod函数是否被hook
     */
    static bool check_PrettyMethod();

    /**
     * 检查open函数是否被hook
     */
    static bool check_open();
private:

    static string get_method_code(const char* lib, const char* method, int len=16);
    static bool scan_line(char *map);
    static void read_line_scan(string str);
};
#endif //ANTISDK_ANTIHOOK_H
