//
// Created by Administrator on 2022/4/22.
//

#include "Syscall.h"
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
//#include "../call/Method.h"
#include <cerrno>
#include <sys/types.h>


#ifdef __arm__
#define OUT : "=r"(r0)
#elif defined(__aarch64__)
#define OUT : "=r"(x0)
#endif

#define __asm_syscall(...) do { \
	__asm__ __volatile__ ( "svc 0" \
    OUT : __VA_ARGS__  : "memory", "cc"); \
	return x0; \
	} while (0)

#define __asm_syscall64(...) do { \
	__asm__ __volatile__ ( "svc 0" \
	: "=r"(x0) : __VA_ARGS__ : "memory", "cc"); \
	return x0; \
	} while (0)

#define __asm_syscall32(...) do { \
	__asm__ __volatile__ ( "svc 0" \
	: "=r"(r0) : __VA_ARGS__ : "memory", "cc"); \
	return r0; \
	} while (0)

UNEXPORT INLINE
static inline long __syscall_close(long n, long a)
{
#if defined(__arm__)
    register long r7 __asm__("r7") = n;
    register long r0 __asm__("r0") = a;
    __asm_syscall32("r"(r7), "0"(r0));
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    __asm_syscall64("r"(x8), "0"(x0));
#endif
}

UNEXPORT INLINE
static inline long __syscall_read(long n, long a, long b, long c)
{
#if defined(__arm__) //编译器会根据宏来选择arm32还是arm64
    register long r7 __asm__("r7") = n;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    __asm_syscall32("r"(r7), "0"(r0), "r"(r1), "r"(r2));
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    __asm_syscall64("r"(x8), "0"(x0), "r"(x1), "r"(x2));
#endif
}

UNEXPORT INLINE
static inline long __syscall_open(long n, long a, long b, long c, long d)
{
#if defined(__arm__) //编译器会根据宏来选择arm32还是arm64
    register long r7 __asm__("r7") = n;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    register long r3 __asm__("r3") = d;
    __asm_syscall32("r"(r7), "0"(r0), "r"(r1), "r"(r2), "r"(r3));
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    register long x3 __asm__("x3") = d;
    __asm_syscall64("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3));
#endif
}

UNEXPORT INLINE
static inline long __syscall_lseek(long n, long a, long b, long c)
{
#if defined(__arm__) //编译器会根据宏来选择arm32还是arm64
    register long r7 __asm__("r7") = n;
    register long r0 __asm__("r0") = a;
    register long r1 __asm__("r1") = b;
    register long r2 __asm__("r2") = c;
    __asm_syscall32("r"(r7), "0"(r0), "r"(r1), "r"(r2));
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = n;
    register long x0 __asm__("x0") = a;
    register long x1 __asm__("x1") = b;
    register long x2 __asm__("x2") = c;
    __asm_syscall64("r"(x8), "0"(x0), "r"(x1), "r"(x2));
#endif
}

UNEXPORT INLINE
int  Syscall::my_openat(int __dir_fd, const void* __path, int __flags, int __mode ){
    return (int)__syscall_open(__NR_openat, __dir_fd, (long)__path, __flags, __mode);
}

UNEXPORT INLINE
ssize_t Syscall::my_read(int __fd, void* __buf, size_t __count){
    return __syscall_read(__NR_read, __fd, (long)__buf, (long)__count);
}

UNEXPORT INLINE
int Syscall::my_close(int __fd){
    return (int)__syscall_close(__NR_close, __fd);
}

UNEXPORT INLINE
off_t Syscall::my_lseek(int __fd, off_t __offset, int __whence){
    return (off_t)__syscall_lseek(__NR_lseek, __fd, __offset, __whence);
}

UNEXPORT INLINE
string  Syscall::read_file_for_svc(char* path){
    long fd = my_openat(AT_FDCWD, path, O_RDONLY | O_CLOEXEC, 0);
    if (fd > 0) {
        char buffer[8];
        memset(buffer, 0, 8);
        std::string str;
        //失败 -1；成功：>0 读出的字节数  =0文件读完了
        while (my_read(fd, buffer, 1) != 0) {
            //LOGI("读取文件内容  %s" ,buffer);
            str.append(buffer);
        }
        my_close(fd);
        //LOGI("read_file_for_svc %s success " , path);
        return str;
    } else{
        LOGE("[%s %s %d] -> open %s error%d:%s", __FILE__, __FUNCTION__, __LINE__, path, fd, strerror(errno));
        return "null";
    }
}

UNEXPORT
string Syscall::readFile(char *filePath) {
    return read_file_for_svc(filePath);
}

//************************************************************************************

UNEXPORT INLINE
static inline long __syscall_kill(long flag, long pid, long sig)
{
#if defined(__arm__) //编译器会根据宏来选择arm32还是arm64
    register long r7 __asm__("r7") = flag;
    register long r0 __asm__("r0") = pid;
    register long r1 __asm__("r1") = sig;
    __asm_syscall32("r"(r7), "0"(r0), "r"(r1));
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = flag;
    register long x0 __asm__("x0") = pid;
    register long x1 __asm__("x1") = sig;
    __asm_syscall64("r"(x8), "0"(x0), "r"(x1));
#endif
}

UNEXPORT INLINE
static inline long  kill_for_svc(int __pid, int __sig ){
    return (int)__syscall_kill(__NR_kill, __pid, __sig);
}

UNEXPORT
void Syscall::kill(int pid, int sig) {
    kill_for_svc(pid, sig);
}

//**************************************************************************************************
UNEXPORT INLINE
static inline long __syscall_stat(long flag, int dirfd, long path, long buf, int flags)
{
#if defined(__arm__)
    register long r7 __asm__("r7") = flag;
    register long r0 __asm__("r0") = dirfd;
    register long r1 __asm__("r1") = path;
    register long r2 __asm__("r2") = buf;
    register long r3 __asm__("r3") = flags;
    __asm_syscall32("r"(r7), "0"(r0), "r"(r1), "r"(r2), "r"(r3));
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = flag;
    register long x0 __asm__("x0") = dirfd;
    register long x1 __asm__("x1") = path;
    register long x2 __asm__("x2") = buf;
    register long x3 __asm__("x3") = flags;
    __asm_syscall64("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3));
#endif
}

UNEXPORT INLINE
static inline int  stat_for_svc(int dirfd, char* path, void* buf, int flags){
    return (int)__syscall_stat(80, dirfd, (long)path, (long)buf, flags); //__NR_newfstatat
}

UNEXPORT
bool Syscall::check_file_or_dir_exists(char *path) {
    struct stat buf{};
    if (stat_for_svc(AT_FDCWD, path, &buf, 0) == 0 ){
        return true;
    }
    return false;
}
//**************************************************************************************************
UNEXPORT INLINE
static inline long __syscall_readlink(long flag, long __dir_fd, long path, long buf, long size)
{
#if defined(__arm__)
    register long r7 __asm__("r7") = flag;
    register long r0 __asm__("r0") = __dir_fd;
    register long r1 __asm__("r1") = path;
    register long r2 __asm__("r2") = buf;
    register long r3 __asm__("r3") = size;
    __asm_syscall32("r"(r7), "0"(r0), "r"(r1), "r"(r2), "r"(r3));
#elif defined(__aarch64__)
    register long x8 __asm__("x8") = flag;
    register long x0 __asm__("x0") = __dir_fd;
    register long x1 __asm__("x1") = path;
    register long x2 __asm__("x2") = buf;
    register long x3 __asm__("x3") = size;
    __asm_syscall64("r"(x8), "0"(x0), "r"(x1), "r"(x2), "r"(x3));
#endif
}

UNEXPORT INLINE
static inline long  readlink_for_svc(const char *path, char *const buf, size_t size){
    return (long)__syscall_readlink(__NR_readlinkat, AT_FDCWD, (long)path, (long)buf, (long)size);
}

UNEXPORT
long Syscall::readlink(const char *path, char *const buf, size_t size) {
    return readlink_for_svc(path, buf, size);
}
//**************************************************************************************************




















