//
// Created by Administrator on 2022/4/26.
//

#include "AntiCapture.h"
#include "../../base/file/File.h"
#include "../../base/str/Str.h"
#include "../../base/call/Method.h"
#include "../../include/global.h"


//***************************************************************************************************
UNEXPORT void AntiCapture::check_proxy(JNIEnv *env) {
    jstring jsproxyHost = env->NewStringUTF(GlobalString::encrypt(GlobalString::str_AntiCapture_proxyHost));
    jstring jsproxyPort = env->NewStringUTF(GlobalString::encrypt(GlobalString::str_AntiCapture_proxyPort));

    jstring proxyHost = static_cast<jstring>(Method::callStaticMethodObject(env, GlobalString::encrypt(GlobalString::str_AntiCapture_System),
                                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_getProperty),
                                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_SiggetProperty),
                                                                            jsproxyHost));
    jstring proxyPort = static_cast<jstring>(Method::callStaticMethodObject(env, GlobalString::encrypt(GlobalString::str_AntiCapture_System),
                                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_getProperty),
                                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_SiggetProperty),
                                                                            jsproxyPort));


    char* cproxyHost = const_cast<char *>(env->GetStringUTFChars(proxyHost, nullptr));
    char* cproxyPort = const_cast<char *>(env->GetStringUTFChars(proxyPort, nullptr));

    LOGI("native http.proxyHost = %s", cproxyHost);
    LOGI("native http.proxyPort = %s", cproxyPort);

    if (Str::strlen(const_cast<char *>(cproxyHost)) >= 2 && Str::strlen(const_cast<char *>(cproxyPort)) >= 2){
        //TODO 开启代理
        LOGI("check_proxy find proxy");
    }

    env->ReleaseStringUTFChars(proxyHost, cproxyHost);
    env->ReleaseStringUTFChars(proxyPort, cproxyPort);
    env->DeleteLocalRef(jsproxyHost);
    env->DeleteLocalRef(jsproxyPort);
}

//***************************************************************************************************
UNEXPORT void AntiCapture::check_VPN(JNIEnv *env) {
    jobject objEnumeration = Method::callStaticMethodObject(env,
                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_NetworkInterface),
                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_getNetworkInterfaces),
                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_Enumeration));
    LOGI("check_VPN objEnumeration %p", objEnumeration);
    if (objEnumeration != 0){
        jobject objArrayList = Method::callStaticMethodObject(env,
                                                              GlobalString::encrypt(GlobalString::str_AntiCapture_Collections),
                                                              GlobalString::encrypt(GlobalString::str_AntiCapture_list),
                                                              GlobalString::encrypt(GlobalString::str_AntiCapture_Siglist),
                                                              objEnumeration);
        LOGI("check_VPN objArrayList %p", objArrayList);

        jint len = Method::callMethodInt(env, objArrayList,
                                         GlobalString::encrypt(GlobalString::str_AntiCapture_ArrayList),
                                         GlobalString::encrypt(GlobalString::str_AntiCapture_size),
                                         GlobalString::encrypt(GlobalString::str_AntiCapture_Sigsize));
        LOGI("check_VPN len %d", len);

        for (int i = 0; i < len; i++){
            jobject objNetworkInterface = Method::callMethodObject(env, objArrayList,
                                                                   GlobalString::encrypt(GlobalString::str_AntiCapture_ArrayList),
                                                                   GlobalString::encrypt(GlobalString::str_AntiCapture_get),
                                                                   GlobalString::encrypt(GlobalString::str_AntiCapture_Object),
                                                                   i);
            LOGI("check_VPN objNetworkInterface %p  i=%d", objNetworkInterface, i);

            jboolean bIsUp = Method::callMethodBoolean(env, objNetworkInterface,
                                                       GlobalString::encrypt(GlobalString::str_AntiCapture_NetworkInterface),
                                                       GlobalString::encrypt(GlobalString::str_AntiCapture_isUp),
                                                       GlobalString::encrypt(GlobalString::str_AntiCapture_SigisUp));
            if (!bIsUp){
                continue;
            }

            jobject objInterfaceAddresses = Method::callMethodObject(env, objNetworkInterface,
                                                                     GlobalString::encrypt(GlobalString::str_AntiCapture_NetworkInterface),
                                                                     GlobalString::encrypt(GlobalString::str_AntiCapture_getInterfaceAddresses),
                                                                     GlobalString::encrypt(GlobalString::str_AntiCapture_List));
            LOGI("check_VPN objInterfaceAddresses %p", objInterfaceAddresses);

            jint size = Method::callMethodInt(env, objInterfaceAddresses,
                                                GlobalString::encrypt(GlobalString::str_AntiCapture_List),
                                                GlobalString::encrypt(GlobalString::str_AntiCapture_size),
                                                GlobalString::encrypt(GlobalString::str_AntiCapture_Sigsize));
            LOGI("check_VPN size %d", size);
            if (size == 0){
                continue;
            }

            jstring jsName = static_cast<jstring>(Method::callMethodObject(env,
                                                                            objNetworkInterface,
                                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_NetworkInterface),
                                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_getName),
                                                                            GlobalString::encrypt(GlobalString::str_AntiCapture_String)));
            LOGI("check_VPN jsName %p", jsName);

            jboolean jb = false;
            const char* charName = (env->GetStringUTFChars(jsName, &jb));
            LOGI("check_VPN charName  ------------------------> < %s >", charName);

            if (Str::strcmp(const_cast<char *>(charName), const_cast<char *>(GlobalString::encrypt(GlobalString::str_AntiCapture_tun0))) == 0 ||
                    Str::strcmp(const_cast<char *>(charName), const_cast<char *>(GlobalString::encrypt(GlobalString::str_AntiCapture_ppp0))) == 0){
                // TODO 发现开启VPN
                LOGI("check_VPN   ----------------------------------> 发现开启VPN");
            }

            env->ReleaseStringUTFChars(jsName, charName);
        }
    }
    LOGI("check_VPN over");
}


