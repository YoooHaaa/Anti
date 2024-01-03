//
// Created by Administrator on 2022/4/22.
//

#include "File.h"
#include <string>
#include "android/asset_manager.h"
#include "android/asset_manager_jni.h"
#include "../../include//include.h"
#include "../call/Method.h"
#include <sys/system_properties.h>
#include <cerrno>



UNEXPORT void File::readAssets(JNIEnv *env, jobject asset_manager, char* fileName, char* dst) {
    AAssetManager *nativeasset = AAssetManager_fromJava(env, asset_manager);
    AAsset *assetFile = AAssetManager_open(nativeasset, fileName, AASSET_MODE_BUFFER);
    size_t fileLength = AAsset_getLength(assetFile);
    LOGI("File::readAssets fileLength -> %d", fileLength);
    AAsset_read(assetFile, dst, fileLength);
    AAsset_close(assetFile);
    LOGI("File::readAssets read assets -> %s", dst);

}


UNEXPORT jstring File::readSharedPreferences(JNIEnv *env, jobject thiz, char* xmlName, char* key) {
    jobject obj_sharedPreferences = Method::callMethodObject(env,
                                                             thiz,
                                                             "android/content/Context",
                                                                "getSharedPreferences"
                                                                "(Ljava/lang/String;I)Landroid/content/SharedPreferences;",
                                                             xmlName,
                                                             0);

    jstring  key_value  = static_cast<jstring>(Method::callMethodObject(env,
                                                                        obj_sharedPreferences,
                                                                        "android/content/SharedPreferences",
                                                                             "getString"
                                                                             "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;",
                                                                        key,
                                                                        env->NewStringUTF(" ")));
    LOGI("read SharedPreferences %s  %s -> %s", xmlName, key, key_value);
    return key_value;
}

char *File::read_property(char *tags, char *buf) {
    if(0 != __system_property_get(tags, reinterpret_cast<char *>(buf))) {
        return buf;
    }
    const prop_info *pInfo = __system_property_find(tags);
    if(NULL != pInfo) {
        char name[20];
        if(0 != __system_property_read(pInfo, name, buf)){
            return buf;
        }
    }
    return buf;
}

