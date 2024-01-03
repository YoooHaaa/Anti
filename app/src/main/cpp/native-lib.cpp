
/**
 *
 */


#include "./include/include.h"
#include "./include/global.h"
#include <string>
#include <android/log.h>
#include "./base/call/Method.h"
#include "./tools/base64/Base64.h"
#include "./tools/rc4/RC4.h"
#include "./tools/crc/CRC.h"
#include "./base/mem/Mem.h"
#include "./base/str/Str.h"
#include "./base/syscall/Syscall.h"
#include "./base/syscall/syscalls.h"
#include "./base/file/File.h"
#include "./base/field/Field.h"
#include "./base/env/Env.h"
#include "./base/thread/Thread.h"
#include "./anti/frida/AntiFrida.h"
#include "./anti/root/AntiRoot.h"
#include "./base/crash/Crash.h"
#include "./base/shell/Shell.h"
#include "./base/new/NewObject.h"
#include "./anti/debug/AntiDebugger.h"
#include "./anti/dump/AntiDump.h"
#include "./anti/emulator/AntiEmulator.h"
#include "./anti/xposed/AntiXposed.h"
#include "./anti/capture/AntiCapture.h"
#include "./anti/repack/AntiRepack.h"
#include "./anti/sandbox/AntiSandBox.h"
#include "./anti/hook/AntiHook.h"
#include "./anti/rom/AntiRom.h"
#include "./anti/unidbg/AntiUnidbg.h"
#include "./base/dlfcn/LocalDlfcn.h"
#include <sys/ptrace.h>
#include <cerrno>
#include <unistd.h>


#define NELEM(...) sizeof(__VA_ARGS__) / sizeof(JNINativeMethod)


// 此字段在check函数执行后才开始赋值（true值），在init中会第一次赋值（false值）
UNEXPORT std::string passValue = "you pass";
int testnum = 1;



INIT_ARRAY  void init_test() {
    LOGI("*********************************************start***************************************************************");
    LOGI("in init_test %s - %s - %d", __FILE__, __func__, __LINE__);
}


INIT_ARRAY  void init_base64() {
    LOGI("in init_base64");
    //Base64::test("test");
}

INIT_ARRAY  void init_md5() {
    LOGI("in init_md5");
    //MD5::test("test");
}

INIT_ARRAY  void init_rc4() {
//    LOGI("in init_rc4");
//    RC4 rc4;
//    string data = "zhengyaohua";
//    string key = "91888";
//    rc4.enc_dec(data, key);
//    LOGI("init_rc4 data -> %s", data.c_str());
//    rc4.enc_dec(data, key);
//    LOGI("init_rc4 data -> %s", data.c_str());
}

INIT_ARRAY  void init_rsa() { //1024位的加密，初始化产生公钥 私钥需要几十分钟
    LOGI("in init_rsa");
//    BigInt n, e, d;
//    vector<BigInt> encryption;
//    RSA::init(n, e, d, 16);
//    encryption = RSA::encrypt("test rsa", e, n);
//    string data;
//    string str;
//    for (const BigInt& m : encryption){
//        m.toString(data, 16);
//        str.append(data.c_str());
//    }
//    LOGI("encrypt -> %s", str.c_str());
//    string result = RSA::decrypt(encryption, d, n);
//    LOGI("decrypt -> %s", result.c_str());
}

INIT_ARRAY  void init_simple_rsa() {
    LOGI("in init_simple_rsa");
//    SimpleRsa r(1,1);
//    LOGE("init simple_rsa over");
//    r.generateKey();
//    LOGE("init generateKey over");
//    r.Encrypt("test simplersa");
//    LOGI("公钥加密后密文为：%s", r.ciphertext.c_str());
//    r.Decrypt(r.ciphertext);
//    LOGI("私钥解密后明文为：%s", r.plaintext.c_str());
}

INIT_ARRAY  void init_frida() {
    LOGI("in init_frida ");
//    AntiFrida::check_maps();
//    AntiFrida::check_fd();
//    AntiFrida::check_status();
//    AntiFrida::check_inlinehook();
//    AntiFrida::check_tcp();
//    AntiFrida::check_exec_section_checksum();
}

INIT_ARRAY  void init_root() {
    LOGI("in init_root ");
//    AntiRoot::check_tags();
//    AntiRoot::check_type();
//    AntiRoot::check_su_for_which();
//    AntiRoot::check_su_for_file();
//    AntiRoot::check_secure();
//    AntiRoot::check_magisk();
}

INIT_ARRAY  void init_debug() {
    LOGI("in init_debug ");
//    AntiDebugger::ptrace_self();
//    AntiDebugger::ptrace_multi_process();
//    AntiDebugger::check_ro_debuggable();
//    AntiDebugger::check_status_tracePid();
//    AntiDebugger::check_server();
//    AntiDebugger::check_thread_num();
//    AntiDebugger::check_stat();
//    AntiDebugger::check_wchan();
}

INIT_ARRAY  void init_dump() {
    LOGI("in init_dump ");
//    AntiDump::monitor_mem();
//    sleep(3);
//    AntiFrida::check_maps();
}

INIT_ARRAY  void init_capture() {
    LOGI("in init_capture ");
    //AntiCapture::check_proxy();
}

INIT_ARRAY  void init_VA() {
    LOGI("in init_VA ");
    //AntiSandBox::check_maps();
    //AntiSandBox::check_so_path();
}

INIT_ARRAY  void init_Emulator() {
    LOGI("in init_Emulator ");
//    AntiEmulator::check_Build();
//    AntiEmulator::check_prop();
//    AntiEmulator::check_file();
//    AntiEmulator::check_cpu_temp();
//    AntiEmulator::check_cpu();
}


void passTest(JNIEnv *env, jobject thiz){
    LOGI("in passTest ");
    LOGI("in passTest ");
    LOGI("in passTest ");
    Method::callMethodVoid(env, thiz, "com/yooha/antisdk/MainActivity", "passTo", "()V");
}



void check(JNIEnv *env, jclass cls, jstring key, jobject thiz) {
    //test
//    AntiFrida::check_exe();
//    AntiFrida::check_maps();
//    AntiFrida::check_fd();
//    AntiFrida::check_status();
//    AntiFrida::check_inlinehook();
//    AntiFrida::check_tcp();
    //检查环境

    //对passValue赋值（true值）

    //检查通过了则调用下面的函数
    passTest(env, thiz);
}

UNEXPORT jstring getPassValue(JNIEnv *env, jclass thiz) {
    return env->NewStringUTF(passValue.c_str());
}


UNEXPORT void init_Env(JNIEnv *env){
    globalEnv = (ENV_APP*)malloc(sizeof(ENV_APP));
    Env *app = new Env(env, globalEnv);
}
//***************************************************************************************************

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved){
    LOGI("in JNI_OnLoad");
    globalJvm = vm;
    JNIEnv *env = nullptr;
    globalJvm->GetEnv((void**)&env, JNI_VERSION_1_6);

    // 初始化环境，一定要在 JNI_OnLoad 开始就执行
    init_Env(env);
    LOGI("JNI_OnLoad ------------- get cpu -> %s", globalEnv->cpu);
    LOGI("JNI_OnLoad ------------- get pkgname -> %s", globalEnv->pkgname);


    // 测试xposed
    //AntiXposed::check_maps();
    //AntiXposed::check_env(env);

    // 测试AntiHook
    //AntiHook::check_gothook();


    //检测代理
    //AntiCapture::check_proxy(env);

    //重打包
    //AntiRepack::check_signature(env);
    //AntiRepack::check_dex_crc(env);
    //AntiRepack::check_classloader(env);


    //VA多开检测
    //AntiSandBox::check_uid();
    //AntiSandBox::check_ps();
    //AntiSandBox::check_dir(env);
    //AntiSandBox::check_proxy(env);
    //AntiSandBox::check_pkgName(env);
    //AntiSandBox::check_mInstrumentation(env);
    //AntiSandBox::check_pms(env);
    //AntiSandBox::check_ActivityThread_H_mCallback(env);



    //自定义rom检测
//    AntiRom::check_prop();
//    AntiRom::check_prop_file();
//    AntiRom::check_file();
//    AntiRom::check_sensor(env);


    //模拟器云手机检测
    //AntiEmulator::check_camera(env);
    //AntiEmulator::check_sensor(env);
    //AntiEmulator::get_all_prop();

    //AntiCapture::check_VPN(env);

    //GlobalString::teststr();



    //动态注册JNI函数
    jclass clsYooha = env->FindClass("com/yooha/antisdk/tools/YoohaUtil");

    if (env->ExceptionCheck()) {   //检测异常
        env->ExceptionClear();     //清除异常
        env->ExceptionDescribe();  //显示异常信息
        LOGE("JNI_OnLoad FindClass error");
        return JNI_ERR;
    }
    JNINativeMethod methods[] = {{"native_check", "(Ljava/lang/String;Ljava/lang/Object;)V", (void*)check} ,
                                  {"native_getPassValue", "()Ljava/lang/String;", (void*)getPassValue},
                                 {"native_load_xposed_class", "()V", (void*)AntiXposed::load_xposed_class}};

    if (env->RegisterNatives(clsYooha, methods, NELEM(methods)) < 0){
        LOGE("JNI_OnLoad RegisterNatives error");
    }

    LOGI("JNI_OnLoad over");
    return JNI_VERSION_1_6;
}
