//
// Created by Administrator on 2022/6/28.
//

#include "Shell.h"
#include <cstdio>
#include "../file/File.h"
#include "../str/Str.h"
#include <cerrno>

/**
 * 执行一个shell命令，输出结果逐行存储在resvec中，并返回行数
 * 若有多条shell命令需要执行，而且命令之间有关系，不能通过多个popen执行，因为会在不同进程下执行，可以通过； && || 在同一个popen里面 执行
 * 使用者要对容器中的指针进行释放
 * @param cmd
 * @param resvec
 * @return
 */
UNEXPORT int Shell::run_shell(const char *cmd, vector<char*> &resvec){
    LOGI("Shell::run_shell exec -> %s", cmd);
    resvec.clear();
    FILE *pp = popen(cmd, "r"); //建立管道
    if (!pp) {
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
        return -1;
    }
    char tmp[1024] = {0}; //设置一个合适的长度，以存储每一行输出
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        if (tmp[strlen(tmp) - 1] == '\n') {
            tmp[strlen(tmp) - 1] = '\0'; //去除换行符
        }
        //LOGI("run_shell get line -> %s", tmp);
        char* pStr = (char*)malloc(strlen(tmp) + 1);
        strcpy(pStr, tmp);
        resvec.push_back(pStr);
    }
    pclose(pp); //关闭管道
    return resvec.size();
}


