//
// Created by Administrator on 2022/6/24.
//

#ifndef ANTISDK_CRASH_H
#define ANTISDK_CRASH_H

/**
 * 用于构造引起程序崩溃或退出的函数
 */
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

/**
 * 1 不可检测到风险立马崩溃，这样比较容易定位到检测点
 * 2 不可用同一种方式崩溃，可以每次都随机选用崩溃函数，让分析人员无法定位
 */
class Crash{
public:
    /**
     * Crash::crash_kill(getpid(), SIGKILL);
     * 调用此函数会导致进程被杀，然后会被zygote自动再次重启进程。此时frida会断开连接
     * @param pid
     * @param sig
     */
    static void crash_kill(int pid, int sig);

    /**
     * 利用svc调用
    *  利用exit函数退出进程
    */
    static void crash_exit();

    /**
    * 利用插入ret指令，让堆栈不平衡导致崩溃
    */
    static void crash_insert_ret();

    /**
     * 通过移动sp 造成栈不平衡
     */
    static void crash_mov_sp();

    /**
     * 通过移动LR 造成函数返回错误,必须将此函数设置为裸函数才行
     */
    static void crash_mov_LR();

    /**
    * 死循环崩溃,导致程序崩溃
    */
    static int crash_while();

    /**
    * 利用创建线程函数，传入空指针引起程序崩溃
    */
    static void crash_thread();

    /**
     *  通过内联汇编，让函数不返回，导致堆栈不平衡引起程序崩溃
     */
    static void crash_no_return();

     /**
      * 栈溢出崩溃
      */
     static void crash_by_stack_cross();
};

/*
if(isDebugged())
{
    //
    .....擦除backtrace
    //
    void* p = malloc(1);
    memset(p,0,0x1000);
    .....
}
*/

#endif //ANTISDK_CRASH_H
