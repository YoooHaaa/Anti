//
// Created by Administrator on 2022/6/30.
//

#include "AntiDump.h"
#include <cstdio>
#include "../../base/file/File.h"
#include "../../base/str/Str.h"
#include <dirent.h>
#include <cerrno>
#include "../../base/syscall/Syscall.h"
#include <vector>
#include "../../base/shell/Shell.h"
#include "../../base/thread/Thread.h"
#include "../../base/env/Env.h"
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
#include <sys/inotify.h>



//***************************************************************************************************
UNEXPORT char* get_mask(unsigned int mask){
    switch(mask){
        case 0x00000001:
            return "IN_ACCESS";
        case  0x00000002:
            return "IN_MODIFY";
        case  0x00000004:
            return "IN_ATTRIB";
        case  0x00000008:
            return "IN_CLOSE_WRITE";
        case  0x00000010:
            return "IN_CLOSE_NOWRITE";
        case  0x00000020:
            return "IN_OPEN";
        case  0x00000040:
            return "IN_MOVED_FROM";
        case  0x00000080:
            return "IN_MOVED_TO";
        case  0x00000100:
            return "IN_CREATE";
        case  0x00000200:
            return "IN_DELETE";
        case  0x00000400:
            return "IN_DELETE_SELF";
        case  0x00000800:
            return "IN_MOVE_SELF";
        case  0x00002000:
            return "IN_UNMOUNT";
        case  0x00004000:
            return "IN_Q_OVERFLOW";
        case  0x00008000:
            return "IN_IGNORED";
        case  0x00000018:
            return "IN_CLOSE";
        case  0x000000C0:
            return "IN_MOVE";
        case  0x01000000:
            return "IN_ONLYDIR";
        case  0x02000000:
            return "IN_DONT_FOLLOW";
        case  0x04000000:
            return "IN_EXCL_UNLINK";
        case  0x10000000:
            return "IN_MASK_CREATE";
        case  0x20000000:
            return "IN_MASK_ADD";
        case  0x40000000:
            return "IN_ISDIR";
        case  0x80000000:
            return "IN_ONESHOT";
    }
    return "NULL";
}


/**
 *  监控内存是否被修改事件, 用于监控/proc/pid/maps  /proc/pid/mem  /proc/pid/pagemap 的数据
 * @param arg
 * @return
 */
UNEXPORT void* thread_watch_intify_dump(void* arg)
{
    char dirName[NAME_MAX]={0};
    snprintf(dirName,NAME_MAX,"/proc/%d/%s", getpid(), arg);
    int fd = inotify_init(); //初始化inotify
    if (fd < 0){
        LOGI("inotify_init err");
        return nullptr;
    }
    int wd = inotify_add_watch(fd, dirName,IN_ALL_EVENTS);
    if (wd < 0){
        LOGI("inotify_add_watch error");
        close(fd);
        return nullptr;
    }
    const int buflen=sizeof(struct inotify_event) * 0x100;
    char buf[buflen]={0};
    fd_set readfds;
    while(1){
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);
        int iRet = select(fd+1, &readfds,0,0,0); // 此处阻塞,用于监视文件描述符，大于0：说明此文件描述符出现变化，-1：出错，0：超时
        if(-1 == iRet){
            break;
        }
        if (iRet){
            memset(buf,0,buflen);
            int len = read(fd,buf,buflen);
            int i=0;
            while(i < len){
                struct inotify_event *event = (struct inotify_event*)&buf[i];
                //LOGI("event->mask 的值为:%s", get_mask(event->mask));
                if( (event->mask==IN_OPEN) ){
                    bool open = false;
                    LOGI("thread_watch_intify_dump %s -> IN_OPEN  wd=%x  len=%d  name=%s", dirName, event->wd, event->len, event->name);
                    if (Str::strcmp(static_cast<char *>(arg), "maps") == 0){
                        Thread::lock_mutex();
                        open = Env::thread_share_switch_maps;
                        Thread::unLock_mutex();
                        if (open){
                            LOGI("thread_watch_intify_dump check thread open maps now");
                        }else{
                            LOGI("thread_watch_intify_dump unknow thread open maps now");
                            exit(0);
                        }
                    }else if (Str::strcmp(static_cast<char *>(arg), "mem") == 0){
                        Thread::lock_mutex();
                        open = Env::thread_share_switch_mem;
                        Thread::unLock_mutex();
                        if (open){
                            LOGI("thread_watch_intify_dump check thread open mem now");
                        }else{
                            LOGI("thread_watch_intify_dump unknow thread open mem now");
                            exit(0);
                        }
                    }else if (Str::strcmp(static_cast<char *>(arg), "pagemap") == 0){
                        Thread::lock_mutex();
                        open = Env::thread_share_switch_pagemap;
                        Thread::unLock_mutex();
                        if (open){
                            LOGI("thread_watch_intify_dump check thread open pagemap now");
                        }else{
                            LOGI("thread_watch_intify_dump unknow thread open pagemap now");
                            exit(0);
                        }
                    }
                }
                i += sizeof(struct inotify_event) + event->len;
            }
        }
    }
    inotify_rm_watch(fd,wd);
    close(fd);
    return nullptr;
}


void AntiDump::monitor_mem() {
    Thread::create_thread_func(thread_watch_intify_dump, (void*)"maps");
    Thread::create_thread_func(thread_watch_intify_dump, (void*)"mem");
    Thread::create_thread_func(thread_watch_intify_dump, (void*)"pagemap");
}
//**************************************************************************************************
void AntiDump::check_fart() {
    char *(path[]) = {"/sdcard/fart"};

    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
        LOGI("AntiFart::check_file -> %s", path[i]);
        if (Syscall::check_file_or_dir_exists(path[i])){
            LOGI("AntiFart::check_file  find file -> %s", path[i]);
            // TODO 发现fart相关痕迹

        }
    }
}
//**************************************************************************************************
void AntiDump::hook_write() {

}

//**************************************************************************************************

