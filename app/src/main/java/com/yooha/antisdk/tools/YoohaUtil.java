package com.yooha.antisdk.tools;

import android.content.Context;
import android.util.Log;

public class YoohaUtil {
    public static String mTag = "yooha-anti-java";

    static {
        System.loadLibrary("yooha");
    }
    public static void init(){
        Log.i(mTag, "in YoohaUtil init");
    }

    public static void check(String key, Object mainActivity){
        native_check(key, mainActivity);
    }

    public static String getPassValue(){
        return native_getPassValue();
    }

    public static void load_xposed_class(){
        native_load_xposed_class();
    }



    public static native void native_check(String key, Object mainActivity);
    public static native String native_getPassValue();
    public static native void native_load_xposed_class();
}
