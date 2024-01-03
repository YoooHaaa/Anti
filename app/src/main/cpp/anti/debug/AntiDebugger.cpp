//
// Created by Administrator on 2022/4/26.
//

#include "AntiDebugger.h"
#include <cstdio>
#include "../../base/file/File.h"
#include "../../base/str/Str.h"
#include <dirent.h>
#include <cerrno>
#include "../../base/syscall/Syscall.h"
#include <vector>
#include "../../base/shell/Shell.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <cctype>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include<sys/prctl.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


//**************************************************************************************************

UNEXPORT bool AntiDebugger::check_ro_debuggable() {
    char tmp[8] = {0};
    File::read_property("ro.debuggable", reinterpret_cast<char *>(&tmp));
    LOGI("check_debuggable -> %s", tmp);
    if (Str::strcmp("1", tmp) == 0) {   // ro.debuggable=1 代表可调式
        LOGI("debuggable = 1");
        return true; //true 表示有 root 风险
    }
    return false;
}


//*************************************************************************************************

UNEXPORT bool AntiDebugger::check_status_tracePid() {
    string status = Syscall::readFile("/proc/self/status");
    if (status == "null"){
        return false; // false  TracerPid:0 未调试
    }
    size_t pos = status.find("\n");
    while (pos != status.npos)
    {
        string temp = status.substr(0, pos);
        if (Str::strstr(const_cast<char *>(temp.c_str()), (char*)"TracerPid:") != nullptr){
            LOGI("check_status_tracePid -> %s", temp.c_str());

            //方法一：TracerPid为调试应用的PID
//            int traceid = atoi(&temp.c_str()[10]);
//            if (traceid != 0){
//                char name[32];
//                sprintf(name, "/proc/%d/cmdline", traceid);
//                string cmdline = Syscall::readFile(name);
//                size_t cmdpos = cmdline.find("\n");
//                while (cmdpos != cmdline.npos){
//                    string cmdtmp = cmdline.substr(0, pos);
//                    if (Str::strstr(const_cast<char *>(cmdtmp.c_str()), (char*)"android_server:") != nullptr){ //IDA调试文件
//                        // TODO
//                        //pthread_kill(pid, 9);
//                        return true;
//                    }
//                    cmdline = cmdline.substr(pos + 1, cmdline.size());
//                    pos = cmdline.find("\n");
//                }
//            }

            //方法二：TracerPid固定为1
//            if (Str::strstr(const_cast<char *>(temp.c_str()), (char*)"1") != nullptr){
//                LOGI("check_status_tracePid -> find debug:%s", temp.c_str());
//                //TODO
//                return true; // true  TracerPid:1 正在调试
//            }
            return false;
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        status = status.substr(pos + 1, status.size());
        pos = status.find("\n");
    }
    return false;
}
//**************************************************************************************************

UNEXPORT bool AntiDebugger::check_server() {
    vector<char*> ret;
    if (Shell::run_shell("ps -A", ret) != -1) {
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("check_server -> %s", tmp);
            if (Str::strstr(tmp, "android_server") != nullptr ||
                Str::strstr(tmp, "gdbserver") != nullptr ||
                Str::strstr(tmp, "gdb") != nullptr){
                LOGI("check_server -> find server");
                return true;
            }
            free(ret[i]);
        }
    }
    return false;
}
//**************************************************************************************************

UNEXPORT void AntiDebugger::ptrace_self() {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0){
        //TODO 附加失败表示程序 可能被调试
        LOGI("PTRACE_TRACEME ptrace error:%s", strerror(errno));
    }else{
        LOGI("PTRACE_TRACEME ptrace success");
    }

}
//**************************************************************************************************
UNEXPORT void AntiDebugger::ptrace_multi_process() {
    pid_t child;
    pid_t parent;

    /**设置进程结束信号接受处理回调函数，不然子进程exit后会变成僵尸进程*/
    /***显示忽略SIGCHLD信号****/
    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
        return;
    }

    // 创建子进程
    child = fork();
    if (child){ //父进程
        LOGI("in parent process wait");
    }else{  //子进程
        // 获取父进程的pid
        parent = getppid();
        LOGI("child:%d  parent:%d", getpid(), parent);
        // ptrace附加父进程
        if (ptrace(PTRACE_ATTACH, parent, 0, 0) < 0){  //附加后父进程会挂起
            LOGI("ptrace error:%s", strerror(errno));
            //TODO 附加父进程失败，说明当前父进程可能被调试
        }else{
            usleep(100);
            LOGI("PTRACE_ATTACH success:%d", parent);
        }
        // 释放附加的进程
        ptrace(PTRACE_DETACH, parent, 0, 0);
        // 结束当前进程，此时会产生一个僵尸进程,需由signal来处理僵尸进程
        exit(0);
    }
}

//**************************************************************************************************

UNEXPORT bool AntiDebugger::check_port() {
    int sock;
    struct sockaddr_in sa;
    bzero(&sa,sizeof(sa));
    sa.sin_family = AF_INET;
    inet_aton("127.0.0.1", &(sa.sin_addr));
    if( inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr) < 0){    //set ip address
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
        return false;
    }
    char res[7];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        LOGI("test socket fail:%s", strerror(errno));
    }
    sa.sin_port = htons(23946);
    if ( connect(sock, (struct sockaddr *) &sa, sizeof(sa)) >= 0) {
        LOGI("check_port connect 127.0.0.1:23946");
        return true;
    }
    LOGI("check_port connect 127.0.0.1:23946 error");
    close(sock);
    return false;
}
//**************************************************************************************************

UNEXPORT bool AntiDebugger::check_thread_num() {
    char* str="/proc/self/task";
    // 打开目录:
    DIR* pdir = opendir(str);
    if (!pdir){
        LOGE("check_thread_num opendir(%s) fail", str);
        return false;
    }
// 查看目录下文件个数:
    struct dirent* pde=NULL;
    int Count=0;
    while ((pde = readdir(pdir))){
        // 字符过滤
        if ((pde->d_name[0] <= '9') && (pde->d_name[0] >= '0')){
            ++Count;
            LOGI("%d 线程ID:%s\n",Count,pde->d_name);
        }
    }
    LOGI("线程个数为：%d",Count);
    if(2 >= Count){
        // 此处判定为调试状态.
        LOGE("当前调试状态!\n");
        return true;
    }
    return false;
}
//**************************************************************************************************

UNEXPORT bool AntiDebugger::check_stat() { // 也可以检查task下面每个线程的flag
    string status = Syscall::readFile("/proc/self/stat");
    if (status == "null"){
        return false;
    }
    LOGI("check_stat -> %s", status.c_str());
    pid_t pid;
    char pName[36] = {0};
    char flag;
    sscanf(status.c_str(), "%d %s %c", &pid, pName, &flag);
    LOGI("check_stat pid -> %d", pid);
    LOGI("check_stat pName -> %s", pName);
    LOGI("check_stat flag -> %c", flag);
    if (flag == 't'){
        LOGI("check_stat -> debugging");
        return true;
    }
    return false;
}
//**************************************************************************************************

UNEXPORT bool AntiDebugger::check_wchan() {
    string status = Syscall::readFile("/proc/self/wchan");
    if (status == "null"){
        return false;
    }
    LOGI("check_wchan -> %s", status.c_str());
    if (Str::strstr(const_cast<char *>(status.c_str()), "ptrace_stop") != nullptr){
        LOGI("check_wchan -> debugging");
        return true;
    }
    return false;
}
//**************************************************************************************************
UNEXPORT void AntiDebugger::check_fd_num() {
    DIR *dir = NULL;
    struct dirent *entry;
    char link_name[100];
    char buf[100];
    int count = 0;
    if ((dir = opendir("/proc/self/fd/")) == NULL) {
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
    } else {
        entry = readdir(dir);
        while (entry) {
            switch (entry->d_type) {
                case DT_LNK:
                    ++count;
                    break;
                default:
                    break;
            }
            entry = readdir(dir);
        }
    }
    closedir(dir);

    if (count < 20){
        // TODO 小于20认为是在被调试（本应用正常情况下fd > 30）
    }
}
//**************************************************************************************************
UNEXPORT int AntiDebugger::debugger_signal = -1;
UNEXPORT void AntiDebugger::signal_handler(int signum){
    debugger_signal = 0;
    signal(SIGTRAP, SIG_IGN);  // SIGTRAP->调试信号  SIG_IGN->忽视信号
}
/**
    IDA总是先于我们的应用程序截获信号
    signal(SIGTRAP, myhandler);   SIGTRAP：调试信号  myhandler：信号处理函数(自己实现的)
    信号处理函数 可以设置一个全局变量
    终止进程方式可以kill进程 或者 sleep()
    先给程序设置signal 并实现信号处理函数
    在关键函数里或者开一个线程 隔时 发送信号 即 raise()
    若信号未收到 则程序被调试
    信号 消息机制 被捕获就会消失 一次性
 */
UNEXPORT void AntiDebugger::check_signal() {
    if (-1 == debugger_signal) {
        debugger_signal = 1;
        signal(SIGTRAP, AntiDebugger::signal_handler);
        raise(SIGTRAP);
    }
}

/**
 *
 * @return : 0  表示未被调试
 * @return : 1  表示被调试
 * @return : -1 表示检测函数被patch
 */
UNEXPORT int AntiDebugger::get_signal() { //返回值不为0，则表示被IDA调试
    return debugger_signal;
}
//**************************************************************************************************








