//
// Created by Administrator on 2022/4/26.
//

#include "AntiRepack.h"
#include "../../base/call/Method.h"
#include "../../base/field/Field.h"
#include <cstdio>
#include "../../base/thread/Thread.h"
#include "../../base/syscall/Syscall.h"
#include "../../base/str/Str.h"
#include "../../base/env/Env.h"
#include "../../base/file/File.h"
#include "../../base/new/NewObject.h"


UNEXPORT void AntiRepack::check_signature(JNIEnv *env) {
    jstring pkgname = env->NewStringUTF("com.yooha.antisdk"); //1 包名可以从配置文件中读取  2 包名可以在ENV初始化之后获得

    jobject application = AntiRepack::getApplication(env);
    jobject pm = Method::callMethodObject(env, application, "android/app/Application",
                                          "getPackageManager", "()Landroid/content/pm/PackageManager;");

    LOGI("check_signature pm = %p", pm);
    jobject packageinfo = Method::callMethodObject(env, pm, "android/content/pm/PackageManager",
                                        "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;",
                                                   pkgname, 64);
    LOGI("check_signature packageinfo = %p", packageinfo);
    Field* field = new Field("android/content/pm/PackageInfo", env);
    jobjectArray signatures = static_cast<jobjectArray>(field->get_object_field(packageinfo,
                                                                                "signatures",
                                                                                "[Landroid/content/pm/Signature;"));
    LOGI("check_signature signatures = %p", signatures);
    jobject signature = env->GetObjectArrayElement(signatures, 0);
    LOGI("check_signature signatures[0] = %p", signature);
    jint hashCode = Method::callMethodInt(env, signature, "android/content/pm/Signature", "hashCode",
                                            "()I");
    LOGI("check_signature hashCode = %d", hashCode);

    // TODO 与资源目录中预存的hashcode进行对比
    jobject assetsManager = Method::callMethodObject(env, application, "android/app/Application",
                                          "getAssets", "()Landroid/content/res/AssetManager;");
    LOGI("check_signature assetsManager = %p", assetsManager);
    char assets[32] = {0};
    File::readAssets(env, assetsManager, "12.dat", assets);
    LOGI("check_signature assets = %s", assets);
    int assetsHashcode = Str::atoi(assets);
    LOGI("check_signature assetsHashcode = %d", assetsHashcode);
    if (assetsHashcode != hashCode){
        // TODO 签名不一致
        LOGI("check_signature 签名不一致");
    }
    env->DeleteLocalRef(pkgname);
}

UNEXPORT void AntiRepack::check_dex_crc(JNIEnv *env) {
    jobject application = AntiRepack::getApplication(env);
    jstring path = static_cast<jstring>(Method::callMethodObject(env, application,
                                                                 "android/app/Application",
                                                                 "getPackageCodePath",
                                                                 "()Ljava/lang/String;"));
    LOGI("check_dex_crc path = %p", path);

    jobject zipFile = NewObject::new_object(env, "java/util/zip/ZipFile", "(Ljava/lang/String;)V", path);
    LOGI("check_dex_crc zipFile = %p", zipFile);
    jstring dex = env->NewStringUTF("classes.dex");
    LOGI("check_dex_crc dex = %p", dex);
    jobject  zipEntry = Method::callMethodObject(env, zipFile,
                                        "java/util/zip/ZipFile",
                                    "getEntry",
                                            "(Ljava/lang/String;)Ljava/util/zip/ZipEntry;",
                                                 dex);
    LOGI("check_dex_crc zipEntry = %p", zipEntry);
    jlong dexCrc = Method::callMethodLong(env, zipEntry, "java/util/zip/ZipEntry", "getCrc", "()J");
    LOGI("check_dex_crc dexCrc = %ld", dexCrc);

    env->DeleteLocalRef(dex);

    // TODO 与资源目录中预存的 dex CRC 进行对比
    jobject assetsManager = Method::callMethodObject(env, application, "android/content/Context",
                                                     "getAssets", "()Landroid/content/res/AssetManager;");
    LOGI("check_dex_crc assetsManager = %p", assetsManager);
    char assets[32] = {0};
    File::readAssets(env, assetsManager, "18.dat", assets);
    LOGI("check_dex_crc assets = %s", assets);
    int assetsHashcode = Str::atoi(assets);
    LOGI("check_dex_crc assetsHashcode = %d", assetsHashcode);
    if (assetsHashcode != dexCrc){
        // TODO Dex CRC不一致
        LOGI("check_dex_crc Dex CRC 不一致");
    }
}
//***************************************************************************************************
UNEXPORT jobject AntiRepack::getApplication(JNIEnv *env) {
    return Method::callStaticMethodObject(env, "android/app/ActivityThread",
                                          "currentApplication",
                                          "()Landroid/app/Application;");
}

//***************************************************************************************************
UNEXPORT jobject AntiRepack::getApplicationContext(JNIEnv *env) {
    jobject application = AntiRepack::getApplication(env);
    jobject context = Method::callMethodObject(env, application, "android/app/Application", "getApplicationContext", "()Landroid/content/Context;");
    return context;
}
//***************************************************************************************************
/**
 *   // 系统的是bootclassloader
 *   // 用户创建的都是pathclassloader
 *   // 如果相等则认为系统的被替换
 */
UNEXPORT void AntiRepack::check_classloader(JNIEnv *env) {
    Field* field = new Field("android/content/pm/PackageInfo", env);
    jobject CREATOR = field->get_static_object_field("CREATOR", "Landroid/os/Parcelable$Creator;");
    LOGI("check_classloader CREATOR %p", CREATOR);
    jobject cls = Method::callMethodObject(env, CREATOR, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
    LOGI("check_classloader cls %p", cls);
    jobject creatorClassloader = Method::callMethodObject(env, cls, "java/lang/Class", "getClassLoader", "()Ljava/lang/ClassLoader;");
    LOGI("check_classloader creatorClassloader %p", creatorClassloader);
    jobject creatorClassloadercls = Method::callMethodObject(env, creatorClassloader, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
    LOGI("check_classloader creatorClassloadercls %p", creatorClassloadercls);
    jstring creatorClassloadername = static_cast<jstring>(Method::callMethodObject(env,
                                                                                   creatorClassloadercls,
                                                                                   "java/lang/Class",
                                                                                   "getName",
                                                                                   "()Ljava/lang/String;"));
    LOGI("check_classloader creatorClassloadername %p", creatorClassloadername);
    jboolean jb = false;
    const char* creatorClassloadernamechar = (env->GetStringUTFChars(creatorClassloadername, &jb));
    LOGI("check_classloader creatorClassloadernamechar %s", creatorClassloadernamechar);


    jobject sysClassloader = Method::callStaticMethodObject(env, "java/lang/ClassLoader", "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    LOGI("check_classloader sysClassloader %p", sysClassloader);
    jobject sysClassloadercls = Method::callMethodObject(env, sysClassloader, "java/lang/Object", "getClass", "()Ljava/lang/Class;");
    LOGI("check_classloader sysClassloadercls %p", sysClassloadercls);
    jstring sysClassloadername = static_cast<jstring>(Method::callMethodObject(env,
                                                                               sysClassloadercls,
                                                                                   "java/lang/Class",
                                                                                   "getName",
                                                                                   "()Ljava/lang/String;"));
    LOGI("check_classloader sysClassloadername %p", sysClassloadername);
    const char* sysClassloadernamechar = (env->GetStringUTFChars(sysClassloadername, &jb));
    LOGI("check_classloader sysClassloadernamechar %s", sysClassloadernamechar);

    if (Str::strcmp(reinterpret_cast<char *>(creatorClassloadername),
                reinterpret_cast<char *>(sysClassloadername)) == 0){
        //todo
        LOGI("check_classloader 检测到签名方法被替换");
    }

    env->ReleaseStringUTFChars(creatorClassloadername, creatorClassloadernamechar);
    env->ReleaseStringUTFChars(sysClassloadername, sysClassloadernamechar);
}


//***************************************************************************************************
















