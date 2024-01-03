//
// Created by Administrator on 2023/5/29.
//
#include "../../base/str/Str.h"
#include "../../base/thread/Thread.h"
#include "../../base/env/Env.h"
#include "../../base/syscall/Syscall.h"
#include "../../base/load/LoadLibrary.h"
#include "AntiHook.h"
#include <unistd.h>
#include <cerrno>
#include <dlfcn.h>
#include <sys/mman.h>
#include "../../base/dlfcn/LocalDlfcn.h"
#include "../../tools/crc/CRC.h"

//***************************************************************************************************
UNEXPORT void AntiHook::check_gothook() {
    bool ret = compare_got("libc.so", "open");
    LOGI("check_gothook compare_got addr -> %p", compare_got);
}

UNEXPORT bool AntiHook::compare_got(const char *libpath, const char* symname) {
    //内存中libc.so的open地址
    void *dl_handle = dlopen(libpath, RTLD_NOW);
    void *symOpen = dlsym(dl_handle,symname);

    //本so在内存中got表中的open函数地址
    char line[1024];
    FILE *fp = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), fp)) {
        //LOGI("compare_got -> %s\n\n", line);
        if (Str::strstr(line, "/data/app/com.yooha.antisdk") != nullptr && Str::strstr(line, "base.apk") != nullptr) {
            // TODO 先研究透后续再写
            LOGI("compare_got find -> %s", line);

        }
    }
    return false;
}
//***************************************************************************************************
#ifdef __LP64__
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym  Elf64_Sym
#else
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym  Elf32_Sym
#endif

UNEXPORT void AntiHook::check_inlinehook(){
#ifdef __arm__
    const char *libc_path = "/apex/com.android.runtime/lib/bionic/libc.so";    // 安卓10下的路径
    const char *libart_path = "/apex/com.android.runtime/lib/libart.so";       // 安卓10下的路径
#elif defined(__aarch64__)
    const char *libc_path = "/apex/com.android.runtime/lib64/bionic/libc.so";  // 安卓10下的路径
    const char *libart_path = "/apex/com.android.runtime/lib64/libart.so";     // 安卓10下的路径
#else
#error "Arch unknown, please port me"
#endif
    if (compare_text_section_crc(libc_path)){
        // TODO 发现 inlinehook 框架

    }

    if (compare_text_section_crc(libart_path)){
        // TODO 发现 inlinehook 框架

    }
}

UNEXPORT bool AntiHook::compare_text_section_crc(const char *path) {
    FILE *maps;
    char buff[256];
    char* load_addr;
    int size;
    int fd = -1, found = 0;
    Elf_Ehdr *elf = static_cast<Elf_Ehdr *>(MAP_FAILED);
    char* shstr_offset;
    char* shoff;
    Elf_Shdr *sh;
    int crcFile = 0;
    int crcMemory = 0;
    char* name;
    long pageSize;

#define fatal(fmt,args...) do { LOGE(fmt,##args); goto err_exit; } while(0)

    maps = fopen("/proc/self/maps", "r");
    if(!maps){
        fatal("failed to open maps");
    }

    while(!found && fgets(buff, sizeof(buff), maps)){
        if(Str::strstr(buff, const_cast<char *>(path)) != nullptr && Str::strstr(buff,"r--p") != nullptr){
            found = 1;
        }
    }

    fclose(maps);

    if(!found){
        fatal("%s not found in my userspace", path);
    }

    if(sscanf(buff, "%lx", &load_addr) != 1){
        fatal("failed to read load address for %s", path);
    }

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        fatal("failed to open %s", path);
    }

    size = lseek(fd, 0, SEEK_END);
    if (size <= 0) {
        fatal("lseek() failed for %s", path);
    }

    elf = (Elf_Ehdr *) mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    fd = -1;

    if (elf == MAP_FAILED) {
        fatal("elf == MAP_FAILED");
    }

    shstr_offset = nullptr;
    shoff = ((char*) elf) + elf->e_shoff;
    for (int i = 0; i < elf->e_shstrndx; i++){
        shoff += elf->e_shentsize; //跳转到段字符串表位置
    }

    sh = (Elf_Shdr *) shoff;
    if (sh->sh_type == SHT_STRTAB) { // 段字符串表
        shstr_offset = ((char *) elf) + sh->sh_offset;
    }

    if (shstr_offset != nullptr){
        shoff = ((char *) elf) + elf->e_shoff;
        for (int k = 0; k < elf->e_shnum; k++, shoff += elf->e_shentsize) {
            Elf_Shdr *sh = (Elf_Shdr *) shoff;

            switch (sh->sh_type) {
                case SHT_PROGBITS:{
                    name = shstr_offset + sh->sh_name;
                    if (Str::strstr(name, ".text") != nullptr){ /** 找到 .text 段，对其求crc */
                        /** so本地文件中 .text 段的crc */
                        crcFile = CRC::calcCRC(reinterpret_cast<const unsigned char *>(((char *) elf) + sh->sh_offset), sh->sh_size);
                        LOGI("compare_text_section_crc %s find section -> %s, file crc=%d", path, name, crcFile);

                        /** so在内存中的 .text 段的crc */
                        pageSize = sysconf(_SC_PAGESIZE); /** .text段为不可读，需改内存属性 */
                        if (mprotect((void*)((long)(load_addr + sh->sh_offset) & -pageSize), (sh->sh_size / pageSize + 1) * pageSize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0){
                            fatal("mprotect failed -> %s", strerror(errno));
                        }
                        crcMemory = CRC::calcCRC(reinterpret_cast<const unsigned char *>(((char *) load_addr) + sh->sh_offset), sh->sh_size);
                        LOGI("compare_text_section_crc %s find section -> %s, memory crc=%d", path, name, crcMemory);

                        munmap(elf, size);
                        return crcFile == crcMemory;
                    }
                    break;
                }
            }
        }
    }

#undef fatal

    err_exit:
    if(fd >= 0) close(fd);
    if(elf != MAP_FAILED) munmap(elf, size);
    return false;
}

//***************************************************************************************************
UNEXPORT bool AntiHook::check_open() {
#ifdef __arm__
    const char *lib_path = "/apex/com.android.runtime/lib/bionic/libc.so";    // 安卓10的路径
#elif defined(__aarch64__)
    const char *lib_path = "/apex/com.android.runtime/lib64/bionic/libc.so";  // 安卓10的路径
#else
#error "Arch unknown, please port me"
#endif
#define CMP_COUNT 8

    //文件中的open
    struct local_dlfcn_handle *handle = static_cast<local_dlfcn_handle *>(local_dlopen(lib_path));
    off_t offset = local_dlsym(handle,"open");
    FILE *fp = fopen(lib_path,"rb");
    char file_bytes[CMP_COUNT] = {0};
    if(fp != nullptr){
        fseek(fp,offset,SEEK_SET);
        fread(file_bytes,1,CMP_COUNT,fp);
        fclose(fp);
    }else{
        LOGE("fopen failed -> %s", strerror(errno));
    }

    //内存中的open
    void *dl_handle = dlopen(lib_path,RTLD_NOW);
    void *sym = dlsym(dl_handle,"open");
    long pageSize = sysconf(_SC_PAGESIZE);
    void* start = (void*)((long)sym & -pageSize);
    if (mprotect(start, pageSize * 2, PROT_READ | PROT_WRITE | PROT_EXEC) == 0){
        bool is_hook = memcmp(file_bytes,sym,CMP_COUNT) != 0;
        local_dlclose(handle);
        dlclose(dl_handle);
        return is_hook;
    }else{
        LOGE("mprotect failed -> %s", strerror(errno));
    }
    local_dlclose(handle);
    dlclose(dl_handle);
    return false;
}
//***************************************************************************************************
/**
 *  此code适配安卓10
 */
UNEXPORT bool AntiHook::check_PrettyMethod() {
#ifdef __arm__
    string st_PrettyMethod = get_method_code("/apex/com.android.runtime/lib/libart.so", "ArtMethod12PrettyMethod");
    if (Str::strstr(const_cast<char *>(st_PrettyMethod.c_str()), "B5 85 B0 04 46 51 48 0D 46 78 44 07 68 38 68 04") == nullptr){
        // TODO 发现PrettyMethod二进制代码被修改
        LOGI("check_inlinehook PrettyMethod 被 patch");
        return true;
    }
#elif defined(__aarch64__)
    string st_PrettyMethod = get_method_code("/apex/com.android.runtime/lib64/libart.so", "ArtMethod12PrettyMethod");
    if (Str::strstr(const_cast<char *>(st_PrettyMethod.c_str()), "FF 43 01 D1 F6 57 02 A9 F4 4F 03 A9 FD 7B 04 A9") == nullptr){
        // TODO 发现PrettyMethod二进制代码被修改
        LOGI("check_inlinehook PrettyMethod 被 patch");
        return true;
    }
#else
#error "Arch unknown, please port me"
#endif
    return false;
}

UNEXPORT string AntiHook::get_method_code(const char* lib, const char* method, int len){
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
    for (int i = 0; i < len; i++){
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
//***************************************************************************************************
UNEXPORT bool AntiHook::scan_line(char *map) {
    char *(path[]) = {
            "libarthook_native.so",
            "libsandhook.edxp.so",
            "libsandhook-native.so",
            "libsandhook.so",
            "libxposed_art.so",
            "libfrida-gadget.so",
            "libmemtrack_real.so",
            "frida-agent-64.so",
            "libva++.so",
            "librfbinder-cpp.so",
            "frida-agent-32.so",
            "libva-native.so",
            "libwhale.edxp.so",
            "libriru_edxp.so",
            "libriru_snet-tweak-riru.so",
            "libriru_edxposed.so"
    };
    for (int i = 0; i < sizeof(path) / sizeof(char*); i++){
        if (Str::strstr(map, path[i]) != nullptr){
            return true;
        }
    }
    return false;
}


UNEXPORT void AntiHook::read_line_scan(string str){
    char* split = (char*)"\n";
    string strs = str + split; // 在字符串末尾也加入分隔符，方便截取最后一段
    size_t pos = strs.find(split);
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        LOGI("read_line_scan maps -> %s", temp.c_str());
        if (scan_line(const_cast<char *>(temp.c_str()))){
            //TODO 找到 hook 框架
            LOGI("---------------------------read_line_scan find hook -> %s", temp.c_str());
            return;
        }
        //去掉已分割的字符串,在剩下的字符串中进行分割
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(split);
    }
}


UNEXPORT void AntiHook::check_maps() {
    Thread::lock_mutex();
    Env::thread_share_switch_maps = true;
    Thread::unLock_mutex();
    string str = Syscall::readFile((char*)"/proc/self/maps");
    Thread::lock_mutex();
    Env::thread_share_switch_maps = false;
    Thread::unLock_mutex();
    if (str != "null"){
        read_line_scan(str);
    }
}
//***************************************************************************************************
UNEXPORT void AntiHook::check_svc_hook() {

}








//***************************************************************************************************


