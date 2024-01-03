//
// Created by Administrator on 2022/4/22.
//

#ifndef ANTISDK_SYSCALL_H
#define ANTISDK_SYSCALL_H
#pragma once
#include "../../include/include.h"
#include <string>
using namespace std;

class Syscall{
public:
    /**
     * svc读取文件
     * @param filePath
     * @return
     */
    static std::string readFile(char* filePath);
    static int  my_openat(int __dir_fd, const void* __path, int __flags, int __mode );
    static ssize_t my_read(int __fd, void* __buf, size_t __count);
    static int my_close(int __fd);
    static string  read_file_for_svc(char* path);
    static off_t my_lseek(int __fd, off_t __offset, int __whence);
    /**
     * kill函数
     * @param pid
     * @param sig
     */
    static void kill(int pid, int sig);

    /**
     * 检查路径或文件是否存在
     * @param path
     * @return
     */
    static bool check_file_or_dir_exists(char *path);


    static long readlink(const char* path, char* const  buf, size_t size);

    static void socket();
    static void connect();

};
#endif //ANTISDK_SYSCALL_H
