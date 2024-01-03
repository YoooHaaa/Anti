//
// Created by Administrator on 2022/4/26.
//

#include "AntiFrida.h"
#include <cstdio>
#include <jni.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <android/log.h>
#include <unistd.h>
#include <cerrno>
#include "../../base/thread/Thread.h"
#include <dirent.h>

#include <asm/unistd.h>                   /* For syscall numbers. */
#include "../../base/syscall/Syscall.h"
#include "../../base/str/Str.h"
#include "../../base/crash/Crash.h"
#include "../../base/env/Env.h"
#include <csignal>
#include "../../base/load/LoadLibrary.h"
#include <dlfcn.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include "../../base/shell/Shell.h"

/**
register关键字只能作用于局部变量，不能作用于全局变量。
register关键字指明将局部变量存储于寄存器中。
register只是请求寄存器变量，但不一定请求成功。
register变量的必须是CPU寄存器可以接受的值。
*/


//********************************************************************************************************************
UNEXPORT char* AntiFrida::keyword[2] = {(char*)"LIBFRIDA", (char*)"\"frida:rpc\""};
//********************************************************************************************************************

UNEXPORT void AntiFrida::check_Server() {
    Thread::create_thread_func(check_Server_Thread, nullptr);
}

/**
 * 当前函数执行较耗时，需要放入子线程进行
 * @return  检查到frida痕迹，返回true，否则返回false
 */
UNEXPORT bool AntiFrida::check_Server_func() {
    LOGI("in AntiFrida::check_Server_func");
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
    for (int i = 10000; i <= 65535; i++) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1){
            LOGI("test socket fail:%s", strerror(errno));
        }
        sa.sin_port = htons(i);
        LOGI("check_Server_func test connect port:%d", i);
        if ( connect(sock, (struct sockaddr *) &sa, sizeof(sa)) >= 0) {
            LOGI("check_Server_func connect 127.0.0.1:%d", i);
            memset(res, 0, 7);
            send(sock, "\x00", 1, 0);
            send(sock, "AUTH\r\n", 6, 0);
            usleep(10);    // Give it some time to answer
            if ((recv(sock, res, 6, MSG_DONTWAIT)) != -1) {
                if (strcmp(res, "REJECT") == 0) {
                    close(sock);
                    LOGI("check_Server find frida for port:%d", i);
                    return true;
                }
            }
        }
        close(sock);
    }
    return false;
}


/**
 * 此函数必须作为线程函数被调用
 * @param param
 * @return
 */
UNEXPORT void* AntiFrida::check_Server_Thread(void* param) {
    check_Server_func();
    return nullptr;
}

//********************************************************************************************************************

UNEXPORT bool AntiFrida::find_mem_string(size_t start, size_t end, char *bytes, unsigned int len) {
    char *pmem = (char *) start;
    int matched = 0;
    while ((unsigned long) pmem < (end - len)) {
        if (*pmem == bytes[0]) {
            matched = 1;
            char *p = pmem + 1;
            while (*p == bytes[matched] && (unsigned long) p < end) {
                matched++;
                p++;
            }
            if (matched >= len) {
                LOGI("find keyword LIBFRIDA");
                return true;
            }
        }
        pmem++;
    }
    return false;
}

UNEXPORT int AntiFrida::wrap_endsWith(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    size_t lenA = strlen(str);
    size_t lenB = strlen(suffix);
    if (lenB > lenA)
        return 0;
    return strncmp(str + lenA - lenB, suffix, lenB) == 0;
}

UNEXPORT int AntiFrida::elf_check_header(uintptr_t base_addr) {
    ElfW(Ehdr) *ehdr = (ElfW(Ehdr) *) base_addr;
    if (0 != memcmp(ehdr->e_ident, ELFMAG, SELFMAG)) return 0;
#if defined(__LP64__)
    if(ELFCLASS64 != ehdr->e_ident[EI_CLASS]) return 0;
#else
    if (ELFCLASS32 != ehdr->e_ident[EI_CLASS]) return 0;
#endif
    if (ELFDATA2LSB != ehdr->e_ident[EI_DATA]) return 0;
    if (EV_CURRENT != ehdr->e_ident[EI_VERSION]) return 0;
    if (ET_EXEC != ehdr->e_type && ET_DYN != ehdr->e_type) return 0;
    if (EV_CURRENT != ehdr->e_version) return 0;
    return 1;
}

UNEXPORT bool AntiFrida::scan_executable_segments(char *map) {
    size_t start, end;
    char perm[5];
    unsigned long offset;
    char path[256];

    if (sscanf(map, "%x-%lx %4s %lx %*s %*s %s", &start, &end, perm, &offset, path) != 5) {
        return false;
    }

    if (Str::strstr(map, (char*)"libgadget") != NULL){
        LOGI("find libgadget");
        return true;
    }

    if (perm[0] != 'r' ||
            perm[3] != 'p' ||
            0 != offset ||
            strlen(path) == 0 ||
            '[' == path[0] ||
            end - start <= 1000000 ||
            wrap_endsWith(path, ".oat") ||
            elf_check_header(start) != 1){
        return false;
    }

    for (char* key : keyword){
        if (find_mem_string(start, end, key, strlen(key))){
            return true;
        }
    }
    return false;
}

UNEXPORT bool AntiFrida::read_line_scan(string str){
    char* split = (char*)"\n";
    string strs = str + split; // 在字符串末尾也加入分隔符，方便截取最后一段
    size_t pos = strs.find(split);
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        if (scan_executable_segments(const_cast<char *>(temp.c_str()))){
            return true;
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
    return false;
}

UNEXPORT bool AntiFrida::check_maps() {
    Thread::lock_mutex();
    Env::thread_share_switch_maps = true;
    Thread::unLock_mutex();
    string str = Syscall::readFile((char*)"/proc/self/maps"); // 读取maps查找子串LIBFRIDA
    Thread::lock_mutex();
    Env::thread_share_switch_maps = false;
    Thread::unLock_mutex();
    if (str != "null"){
        return read_line_scan(str);
    }
    return false;
}

//********************************************************************************************************************

UNEXPORT bool AntiFrida::check_fd() {
    DIR *dir = NULL;
    struct dirent *entry;
    char link_name[100];
    char buf[100];
    bool ret = false;
    if ((dir = opendir("/proc/self/fd/")) == NULL) {
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
    } else {
        entry = readdir(dir);
        while (entry) {
            switch (entry->d_type) {
                case DT_LNK:
                    sprintf(link_name, "%s/%s", "/proc/self/fd/", entry->d_name);
                    readlink(link_name, buf, sizeof(buf));
                    if (strstr(buf, "frida") || strstr(buf, "gum-js-loop") || strstr(buf, "gmain") ||
                        strstr(buf, "-gadget") || strstr(buf, "linjector")) {
                        LOGI("check_fd -> find frida:%s", buf);
                        ret = true;
                    }
                    break;
                default:
                    break;
            }
            entry = readdir(dir);
        }
    }
    closedir(dir);
    return ret;
}


//********************************************************************************************************************

UNEXPORT bool AntiFrida::check_status() {
    DIR *dir = NULL;
    struct dirent *entry;
    char status_path[128];
    if ((dir = opendir("/proc/self/task/")) == NULL) {
        LOGE(" %s - %d  error:%s", __FILE__, __LINE__, strerror(errno));
    } else {
        entry = readdir(dir);
        while (entry) {
            switch (entry->d_type) {
                case DT_DIR:
                    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                        break;
                    }
                    sprintf(status_path, "%s/%s/status", "/proc/self/task", entry->d_name);
                    string status = Syscall::readFile(status_path); // 读取 /proc/self/task/tid/status 文件
                    if (status == "null"){
                        break;
                    }
                    size_t pos = status.find("\n");
                    while (pos != status.npos)
                    {
                        string temp = status.substr(0, pos);
                        if (Str::strstr(const_cast<char *>(temp.c_str()), (char*)"Name:") != nullptr){
                            if (Str::strstr(const_cast<char *>(temp.c_str()), (char*)"gmain") != nullptr ||
                                    Str::strstr(const_cast<char *>(temp.c_str()), (char*)"gum-js-loop") != nullptr ||
                                    Str::strstr(const_cast<char *>(temp.c_str()), (char*)"pool-frida") != nullptr ||
                                    Str::strstr(const_cast<char *>(temp.c_str()), (char*)"gdbus") != nullptr){
                                LOGI("check_status -> find frida thread:%s", temp.c_str());
                                // TODO 发现frida
                                //Crash::crash_no_return();
                                //Crash::crash_kill(getpid(), SIGKILL); //进程被杀后会重启，然后frida会断开
                                return true;
                            }
                            break;
                        }
                        //去掉已分割的字符串,在剩下的字符串中进行分割
                        status = status.substr(pos + 1, status.size());
                        pos = status.find("\n");
                    }
                    break;
            }
            entry = readdir(dir);
        }
    }
    closedir(dir);
    return false;
}

//********************************************************************************************************************
UNEXPORT bool AntiFrida::check_exe() {
    char buf[1024] = { 0 };
    int n;

    n = readlink("/proc/self/exe" , buf , sizeof(buf));
    if( n > 0 && n < sizeof(buf)){
        LOGI("check_exe /proc/self/exe -> %s\n" , buf);
    }
    LOGI("check_exe over" );
    return true;
}
//********************************************************************************************************************
UNEXPORT bool AntiFrida::check_tcp() {
    vector<char*> ret;
    if (Shell::run_shell("cat /proc/net/tcp", ret) != -1) {
        LOGI("check_tcp -> %d", ret.size());
        for (int i = 0; i < ret.size(); i++){
            char* tmp = ret[i];
            LOGI("check_tcp -> %s", tmp);
            if (Str::strstr(tmp, ":69A2") != nullptr ||
                Str::strstr(tmp, ":69A3") != nullptr ||
                Str::strstr(tmp, ":5D8A") != nullptr){
                // TODO 发现风险

                return false;
            }
            free(ret[i]);
        }
    }
    return true;
}
//********************************************************************************************************************

UNEXPORT bool AntiFrida::check_inlinehook(){
    bool find = false;
#ifdef __arm__
    string st_PrettyMethod = get_method_code("/apex/com.android.runtime/lib/libart.so", "ArtMethod12PrettyMethod");
#elif defined(__aarch64__)
    string st_PrettyMethod = get_method_code("/apex/com.android.runtime/lib64/libart.so", "ArtMethod12PrettyMethod");
    if (strstr(st_PrettyMethod.c_str(), "50 00 00 58 00 02 1F D6 00 9E 23 ED 78 00 00 00") != NULL){
        LOGI("check inlinehook find frida patchcode PrettyMethod -> %s", st_PrettyMethod.c_str());
        find = true;
    }else{
        LOGI("check inlinehook cant find frida patchcode PrettyMethod -> %s", st_PrettyMethod.c_str());
    }
#else
#error "Arch unknown, please port me"
#endif

    return find;
}

UNEXPORT string AntiFrida::get_method_code(const char* lib, const char* method){
    string st;

    void* handle = LoadLibrary::fake_dlopen(lib, RTLD_NOW);
    if (handle == nullptr){
        LOGE("dlopen failed -> %s", strerror(errno));
        return st;
    }
    void* op = (void*)LoadLibrary::fake_dlsym(handle, method);
    if (op == nullptr){
        LOGE("dlsym failed -> %s", strerror(errno));
        LoadLibrary::fake_dlclose(handle);
        return st;
    }

    long pageSize = sysconf(_SC_PAGESIZE);
    void* start = (void*)((long)op & -pageSize);
    if (mprotect(start, pageSize * 2, PROT_READ | PROT_WRITE | PROT_EXEC) == 0){
        LOGI("mprotect success");
    }else{
        LOGE("mprotect failed -> %s", strerror(errno));
    }
    char tmp[128] = {0};
    sprintf(tmp, "%s %s -> %p\n", lib, method, op);
    LOGI("tmp = %s ", tmp);
    //st.append(tmp);
    unsigned char*code = (unsigned char*)op;
    for (int i = 0; i < 16; i++){
        if (code[i] < 0x10){
            sprintf(tmp, "0%X ",code[i]);
        }else{
            sprintf(tmp, "%X ",code[i]);
        }
        st.append(tmp);
    }
    LOGI("get_method_code -> %s", st.c_str());
    LoadLibrary::fake_dlclose(handle);
    return st;
}
//********************************************************************************************************************
void AntiFrida::check_processName() {
    // getRunningProcess
}

//********************************************************************************************************************

const char *AntiFrida::libstocheck[NUM_LIBS] = {"yooha.so", LIBC};
execSection *AntiFrida::elfSectionArr[NUM_LIBS] = {nullptr};

UNEXPORT INLINE
inline unsigned long AntiFrida::checksum(void *buffer, size_t len) {
    unsigned long seed = 0;
    uint8_t *buf = (uint8_t *) buffer;
    size_t i;
    for (i = 0; i < len; ++i)
        seed += (unsigned long) (*buf++);
    return seed;
}
#include "../../base/syscall/syscalls.h"
UNEXPORT INLINE
inline void AntiFrida::parse_proc_maps_to_fetch_path(char **filepaths) {
    int fd = 0;
    char map[MAX_LINE];
    int counter = 0;
    if ((fd = my_openat(AT_FDCWD, PROC_MAPS, O_RDONLY | O_CLOEXEC, 0)) != 0) {
        while ((read_one_line(fd, map, MAX_LINE)) > 0) {
            for (int i = 0; i < NUM_LIBS; i++) {
                if (Str::strstr(map, const_cast<char *>(libstocheck[i])) != NULL) {
                    char tmp[MAX_LENGTH] = "";
                    char path[MAX_LENGTH] = "";
                    char buf[5] = "";
                    sscanf(map, "%s %s %s %s %s %s", tmp, buf, tmp, tmp, tmp, path);
                    if (buf[2] == 'x') {
                        size_t size = Str::strlen(path) + 1;
                        filepaths[i] = static_cast<char *>(malloc(size));
                        Str::strlcpy(filepaths[i], path, size);
                        counter++;
                    }
                }
            }
            if (counter == NUM_LIBS)
                break;
        }
        Syscall::my_close(fd);
    }
}

UNEXPORT INLINE
inline bool AntiFrida::fetch_checksum_of_library(const char *filePath, execSection **pTextSection) {
    Elf_Ehdr ehdr;
    Elf_Shdr sectHdr;
    int fd;
    int execSectionCount = 0;
    fd = Syscall::my_openat(AT_FDCWD, filePath, O_RDONLY | O_CLOEXEC, 0);
    if (fd < 0) {
        return NULL;
    }
    Syscall::my_read(fd, &ehdr, sizeof(Elf_Ehdr));
    Syscall::my_lseek(fd, (off_t) ehdr.e_shoff, SEEK_SET);
    unsigned long memsize[2] = {0};
    unsigned long offset[2] = {0};

    for (int i = 0; i < ehdr.e_shnum; i++) {
        Mem::memset(&sectHdr, 0, sizeof(Elf_Shdr));
        Syscall::my_read(fd, &sectHdr, sizeof(Elf_Shdr));

        if (sectHdr.sh_flags & SHF_EXECINSTR) {
            offset[execSectionCount] = sectHdr.sh_offset;
            memsize[execSectionCount] = sectHdr.sh_size;
            execSectionCount++;
            if (execSectionCount == 2) {
                break;
            }
        }
    }
    if (execSectionCount == 0) {
        LOGI("No executable section found. Suspicious");
        Syscall::my_close(fd);
        return false;
    }

    *pTextSection = static_cast<execSection *>(malloc(sizeof(execSection)));
    (*pTextSection)->execSectionCount = execSectionCount;
    (*pTextSection)->startAddrinMem = 0;
    for (int i = 0; i < execSectionCount; i++) {
        Syscall::my_lseek(fd, offset[i], SEEK_SET);
        uint8_t *buffer = static_cast<uint8_t *>(malloc(memsize[i] * sizeof(uint8_t)));
        Syscall::my_read(fd, buffer, memsize[i]);
        (*pTextSection)->offset[i] = offset[i];
        (*pTextSection)->memsize[i] = memsize[i];
        (*pTextSection)->checksum[i] = checksum(buffer, memsize[i]);
        free(buffer);
    }
    Syscall::my_close(fd);
    return true;
}

UNEXPORT INLINE
inline bool AntiFrida::compare_executable_segments(char *map, execSection *pElfSectArr, const char *libraryName) {
    unsigned long start, end;
    char buf[MAX_LINE] = "";
    char path[MAX_LENGTH] = "";
    char tmp[100] = "";

    sscanf(map, "%lx-%lx %s %s %s %s %s", &start, &end, buf, tmp, tmp, tmp, path);

    if (buf[2] == 'x') {
        if (buf[0] == 'r') {
            uint8_t *buffer = NULL;
            buffer = (uint8_t *) start;
            for (int i = 0; i < pElfSectArr->execSectionCount; i++) {
                if (start + pElfSectArr->offset[i] + pElfSectArr->memsize[i] > end) {
                    if (pElfSectArr->startAddrinMem != 0) {
                        buffer = (uint8_t *) pElfSectArr->startAddrinMem;
                        pElfSectArr->startAddrinMem = 0;
                        break;
                    }
                }
            }
            for (int i = 0; i < pElfSectArr->execSectionCount; i++) {
                unsigned long output = checksum(buffer + pElfSectArr->offset[i],
                                                pElfSectArr->memsize[i]);
                if (output != pElfSectArr->checksum[i]) {
                    LOGI("Executable Section Manipulated, maybe due to Frida or other hooking framework. Act Now!!!");
                }
            }
        } else {
            char ch[10] = "", ch1[10] = "";
            __system_property_get("ro.build.version.release", ch);
            __system_property_get("ro.system.build.version.release", ch1);
            int version = Str::atoi(ch);
            int version1 = Str::atoi(ch1);
            if (version < 10 || version1 < 10) {
                LOGI("Suspicious to get XOM in version < Android10");
            } else {
                if (0 == Str::strncmp(libraryName, LIBC, Str::strlen(LIBC))) {
                    LOGI("LIBC Executable Section not readable! ");
                } else {
                    LOGI("Suspicious to get XOM for non-system library on Android 10 and above");
                }
            }
        }
        return true;
    } else {
        if (buf[0] == 'r') {
            pElfSectArr->startAddrinMem = start;
        }
    }
    return false;
}

UNEXPORT INLINE
inline ssize_t AntiFrida::read_one_line(int fd, char *buf, unsigned int max_len) {
    char b;
    ssize_t ret;
    ssize_t bytes_read = 0;
    Mem::memset(buf, 0, max_len);
    do {
        ret = Syscall::my_read(fd, &b, 1);
        if (ret != 1) {
            if (bytes_read == 0) {
                return -1;
            } else {
                return bytes_read;
            }
        }
        if (b == '\n') {
            return bytes_read;
        }
        *(buf++) = b;
        bytes_read += 1;
    } while (bytes_read < max_len - 1);
    return bytes_read;
}

UNEXPORT INLINE
inline void AntiFrida::detect_frida_memdiskcompare() {
    int fd = 0;
    char map[MAX_LINE];

    if ((fd = Syscall::my_openat(AT_FDCWD, PROC_MAPS, O_RDONLY | O_CLOEXEC, 0)) != 0) {
        while ((read_one_line(fd, map, MAX_LINE)) > 0) {
            for (int i = 0; i < NUM_LIBS; i++) {
                if (Str::strstr(map, const_cast<char *>(libstocheck[i])) != NULL) {
                    if (true == compare_executable_segments(map, elfSectionArr[i], libstocheck[i])) {
                        break;
                    }
                }
            }
        }
    } else {
        LOGI("Error opening /proc/self/maps. That's usually a bad sign.");

    }
    Syscall::my_close(fd);
}

UNEXPORT
void AntiFrida::check_exec_section_checksum() {
    char *filePaths[NUM_LIBS] = {nullptr};
    parse_proc_maps_to_fetch_path(filePaths);
    for (int i = 0; i < NUM_LIBS; i++) {
        fetch_checksum_of_library(filePaths[i], &elfSectionArr[i]);
        if (filePaths[i] != NULL){
            free(filePaths[i]);
        }
    }
    detect_frida_memdiskcompare();
}

//********************************************************************************************************************


