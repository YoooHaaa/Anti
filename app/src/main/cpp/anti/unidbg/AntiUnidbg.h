//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTIUNIDBG_H
#define ANTITOOLS_ANTIUNIDBG_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

/**
>> anti unidbg/unicorn：
1.在真机上，使用GetStaticMethodID寻找一个不存在的方法，系统会返回一个错误。在unidbg中使用GetStaticMethodID，无论存不存在都会模拟一个jmethodID返回给so。
2.Unidbg的uname系统调用表明了自己是Unidbg。
3.上文环境缺失(JAVA环境缺失/Native环境缺失)，Unidbg尚未实现的常用Linux相关特性，如多线程、信号等机制 Unidbg尚未实现的JNI函数以及JNI相关问题
4.int setenv(const char *name, const char *value, int overwrite); 添加系统环境变量后做比较
5.Java 存在继承情况，子类方法id与父类一致，但是unidbg对Java一无所知，会以两个类处理，返回两个方法ID，子类重写了父类的方法，但通过父类获取 methodID，让子类Call 这个MethodID。用GetMethodID方法构造一个隐蔽的初始化函数创造陷阱。
6.使用unidbg未实现的jni方法。
7.Unidbg中jmethodID是是方法签名这个字符串本身的hashCode
8.加载unidbg不支持的依赖库。
9.QEMU以及基于它的Unicorn不存在字节对齐。
10.执行耗时，inlinehook检测
 */
class AntiUnidbg{
public:
    /**
     * 通过获取一个不存在的方法的methodid来判断当前是否在unidbg中，因为unidbg会模拟环境，所以一定会模拟一个jmethodid出来
     */
    static void check_GetStaticMethodID(JNIEnv *env);
private:
    static jmethodID getStaticMethodID(JNIEnv *env,
                                       const char* clsName,
                                       const char* methodName,
                                       const char* sig);

public:
    static void check_uname();
};
#endif //ANTITOOLS_ANTIUNIDBG_H
