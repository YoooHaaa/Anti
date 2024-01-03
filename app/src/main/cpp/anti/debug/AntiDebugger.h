//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTIDEBUGGER_H
#define ANTITOOLS_ANTIDEBUGGER_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

/**
 * 反调试，反IDA
 */
class AntiDebugger{
public:

    /**
     * 检测是否打开了全局可调试开关
     * @return true(打开), false(未打开)
     */
    static bool check_ro_debuggable();

    /**
     * 检测/proc/self/status 文件,获取TracerPid字段值，若调试则为调试进程的pid，未调试为0
     * @return  true(调试), false(未调试)
     */
    static bool check_status_tracePid();

    /**
     *  遍历进程，查找类似android_server，gdbserver，gdb等调试器进程（高版本失效）
     * @return
     */
    static bool check_server();

    /**
     * 检查23946端口是否能连接上
     * @return  // false  未调试
     */
    static bool check_port();

    /**
     * 正常apk进程一般会有十几个线程在运行,比如会有jdwp线程,自己写可执行文件加载so一般只有一个线程
     * @return  // false  未调试
     */
    static bool check_thread_num();

    /**
     * /proc/pid/stat 和 /proc/pid/task/pid/stat：调试状态下，括号后面的第一个字母应该为t,如：1234 (com.tencant.mm) t
     * @return  // false  未调试
     */
    static bool check_stat();

    /**
     * /proc/pid/wchan 和 /proc/pid/task/pid/wchan：调试状态下，里面内容为 ptrace_stop
     * @return // false  未调试
     */
    static bool check_wchan();

    /**
     *  会失败，提示Permission denied, 进程不能被zygote trace,只能被子进程trace
     */
    static void ptrace_self();

    /**
     *  创建子进程trace父进程，如果能被trace，则未被调试
     */
    static void ptrace_multi_process();

    /**
     *  即使进行ida调试了 程序的大部分功能依然没有运行 所以fd文件较正常偏少
     */
    static void check_fd_num();

    /**
     *   IDA总是先于我们的应用程序截获信号
     */
    static void check_signal();
    static int get_signal();
private:
    static int debugger_signal;
    static void signal_handler(int signum);
};


#endif //ANTITOOLS_ANTIDEBUGGER_H









