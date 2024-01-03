//
// Created by Administrator on 2022/4/15.
//

#ifndef ANTITOOLS_ANTIFRIDA_H
#define ANTITOOLS_ANTIFRIDA_H
#pragma once
#include "../../include/include.h"
#include <string>
#include <elf.h>
#include <link.h>
#include "../../base/mem/Mem.h"
using namespace std;


#if defined(__arm__) //编译器会根据宏来选择arm32还是arm64
typedef Elf32_Ehdr Elf_Ehdr;
typedef Elf32_Shdr Elf_Shdr;
#elif defined(__aarch64__)
typedef Elf64_Ehdr Elf_Ehdr;
typedef Elf64_Shdr Elf_Shdr;
#endif



#define NUM_LIBS 2
#define MAX_LINE 512
#define MAX_LENGTH 256
static const char *APPNAME = "DetectFrida";
static const char *FRIDA_THREAD_GUM_JS_LOOP = "gum-js-loop";
static const char *FRIDA_THREAD_GMAIN = "gmain";
static const char *FRIDA_NAMEDPIPE_LINJECTOR = "linjector";
static const char *PROC_MAPS = "/proc/self/maps";
static const char *PROC_STATUS = "/proc/self/task/%s/status";
static const char *PROC_FD = "/proc/self/fd";
static const char *PROC_TASK = "/proc/self/task";
#define LIBC "libc.so"

typedef struct stExecSection {
    int execSectionCount;
    unsigned long offset[2];
    unsigned long memsize[2];
    unsigned long checksum[2];
    unsigned long startAddrinMem;
} execSection;

class AntiFrida{
public:
    /**
     * 遍历运行的进程，对比进程名是否为fridaserver （只需修改server的名称即可绕过，太小儿科）
     */
    static void check_processName();

    /**
     * 通过遍历端口来检查frida server  10000~65535
     */
    static void check_Server();
private:
    static void* check_Server_Thread(void* param);
    static bool check_Server_func();


public:
    /**
     * 注入frida后，在/proc/self/maps 中会出现libgadget.so ,或frida-agent-32.so,frida-agent.so等模块
     * @return
     */
    static bool check_maps();
private:
    static char* keyword[2];
    static bool scan_executable_segments(char *map);
    static bool find_mem_string(size_t start, size_t end, char *bytes, unsigned int len);
    static bool read_line_scan(string str);
    static int wrap_endsWith(const char *str, const char *suffix);
    static int elf_check_header(uintptr_t base_addr);

public:
    /**
     * fd里面包含着当前进程打开的每一个文件的描述符,差不多就是路径，这些文件描述符是指向实际文件的一个符号连接
     * @return 找到特征返回true
     */
    static bool check_fd();


public:
    /**
     * 遍历 /proc/self/task/tid/status 文件，获取Name字段的值（线程名），并判断
     * @return
     */
    static bool check_status();

public:
    /**
     *
     * @return
     */
    static bool check_exe();


public:
    /**
     * 读取/proc/net/tcp 检查 27042（69A2）， 23946 （5D8A）
     * @return
     */
    static bool check_tcp();

public:
    /**
     * 检查frida inlinehook 的代码：frida注入过程中，会在对libart的PrettyMethod函数进行hook，该函数头的代码会发生改变
     * @return
     */
    static bool check_inlinehook();
private:
    static string get_method_code(const char*, const char*);


public:
    /**
     * 比较内存和文件中section节的可执行节区的checksum是否一致
     */
    static void check_exec_section_checksum();
private:
    static const char *libstocheck[NUM_LIBS];
    static execSection *elfSectionArr[NUM_LIBS];
    static void detect_frida_memdiskcompare();
    static unsigned long checksum(void *buffer, size_t len);
    static void parse_proc_maps_to_fetch_path(char **filepaths);
    static bool fetch_checksum_of_library(const char *filePath, execSection **pTextSection);
    static bool compare_executable_segments(char *map, execSection *pElfSectArr, const char *libraryName);
    static ssize_t read_one_line(int fd, char *buf, unsigned int max_len);
};
#endif
