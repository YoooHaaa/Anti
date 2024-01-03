//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTIREPACK_H
#define ANTITOOLS_ANTIREPACK_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class AntiRepack{
public:
    /**
     * 检查dex文件CRC值
     * @param env
     */
    static void check_dex_crc(JNIEnv *env);

    /**
     * 检查签名
     * @param env
     */
    static void check_signature(JNIEnv *env);

    /**
     * 很多攻击者会用Lspatch进行打包 ，对变量进行替换，这时候我们去检测这个变量的Classloader是不是系统ClassLoader
     * 系统的是bootclassloader, 用户创建的都是pathclassloader
     * @param env
     */
    static void check_classloader(JNIEnv *env);
private:
    static jobject getApplication(JNIEnv *env);
    static jobject getApplicationContext(JNIEnv *env);
};

#endif //ANTITOOLS_ANTIREPACK_H



//public void getSig()  {
//    try{
//        PackageManager pm = getApplicationContext().getPackageManager();
//        PackageInfo pi = pm.getPackageInfo("com.yooha.antisdk", PackageManager.GET_SIGNATURES);
//        Signature[] sn = pi.signatures;
//        Log.i("yooha-test", "signatures.toString -> " + sn[0].toString());
//        Log.i("yooha-test", "signatures.hashCode -> " + sn[0].hashCode());
//    } catch (Exception e){
//    }
//}
//
//public void getDexCRC(){
//    try{
//        String path = getApplicationContext().getPackageCodePath();
//        ZipFile zf = new ZipFile(path);
//        ZipEntry ze = zf.getEntry("classes.dex");
//        long crc = ze.getCrc();
//        Log.i("yooha-test", "classes.dex CRC -> " + crc);
//    }catch(Exception e){
//    }
//}