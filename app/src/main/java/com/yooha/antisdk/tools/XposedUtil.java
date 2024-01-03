package com.yooha.antisdk.tools;

import android.util.Log;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Enumeration;
import java.util.Objects;

import dalvik.system.BaseDexClassLoader;
import dalvik.system.DexFile;

public class XposedUtil {
    public static String mTag = "yooha-anti-java";

    /**
     * 判断是否加载了xposed的相关类
     */
    public static void is_load_xposed_class() {
        YoohaUtil.load_xposed_class();
    }

    /**
     * 打印栈查看是否有xposed的栈信息
     */
    public static void is_xposed_stack() {
        try {
            throw new Exception("yooha");
        } catch (Exception e) {
            for (StackTraceElement stackTraceElement : e.getStackTrace()) {
                if (stackTraceElement.getClassName().contains(".xposed.")) {
                    //TODO
                    Log.i(mTag, stackTraceElement.getClassName());
                }

            }
        }
    }

    /**
     * xposed框架中有一个静态字段，若该值为true则表示关闭了Xposed框架Hook的总开关
     */
    public static void close_xposed() {
        try {
            Field declaredField = ClassLoader.getSystemClassLoader().loadClass("de.robv.android.xposed.XposedBridge").getDeclaredField("disableHooks");
            declaredField.setAccessible(true);
            declaredField.set(null, true);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 查看是否加载了xposed相关的jar文件
     */
    public static void is_load_xposed_dex(ClassLoader loader) {
        try {
            Class<?> cls = Class.forName("dalvik.system.DexPathList");
            Method method = Class.forName("dalvik.system.DexPathList$Element").getMethod("toString");
            Field declaredField = cls.getDeclaredField("dexElements");
            declaredField.setAccessible(true);
            Field declaredField2 = BaseDexClassLoader.class.getDeclaredField("pathList");
            declaredField2.setAccessible(true);
            Object[] objArr = (Object[]) declaredField.get(declaredField2.get(loader));
            for (Object obj : objArr) {
                try {
                    String str2 = (String) method.invoke(obj, new Object[0]); //dex文件名
                    Log.i(mTag, "pathList -> " + str2);
                    if (str2 != null && str2.contains("XposedBridge.jar")) {
                        //TODO

                    }
                } catch (Exception e2) {
                }
            }
        } catch (Exception e3) {
        }
    }



    /**
     *  通过检查指定方法是否为native方法，来判断当前应用是否被hook
     * @param clsName    类名
     * @param methodName 方法名
     * @param sig        方法参数类数组
     */
    public static void is_native_method(String clsName, String methodName, Class[] sig){
        try {
            // 判断方法是不是用native修饰的
            boolean bool = Modifier.isNative(Class.forName(clsName)
                    .getDeclaredMethod(methodName, sig).getModifiers());
            if (bool) {
                //TODO 发现native方法，该方法应该被hook

            }
        }
        catch (Exception paramString1) {
            paramString1.printStackTrace();
        }
    }
}
