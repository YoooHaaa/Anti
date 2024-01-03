//
// Created by Administrator on 2023/5/22.
//
#include "LoadLibrary.h"

#include <cstdio>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <android/log.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>


#ifdef __arm__
#define Elf_Ehdr Elf32_Ehdr
#define Elf_Shdr Elf32_Shdr
#define Elf_Sym  Elf32_Sym
#elif defined(__aarch64__)
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym  Elf64_Sym
#else
#error "Arch unknown, please port me"
#endif

struct ctx {
    void *load_addr;
    void *dynstr;
    void *dynsym;
    int nsyms;
    off_t bias;
};

UNEXPORT int LoadLibrary::fake_dlclose(void *handle)
{
    if(handle) {
        struct ctx *ctx = (struct ctx *) handle;
        if(ctx->dynsym) free(ctx->dynsym);
        if(ctx->dynstr) free(ctx->dynstr);
        free(ctx);
    }
    return 0;
}


UNEXPORT void* LoadLibrary::fake_dlopen(const char *libpath, int flags)
{
    FILE *maps;
    char buff[256];
    struct ctx *ctx = 0;
    off_t load_addr, size;
    int k, fd = -1, found = 0;
    void *shoff;
    Elf_Ehdr *elf = static_cast<Elf_Ehdr *>(MAP_FAILED); //MAP_FAILED

#define fatal(fmt,args...) do { LOGE(fmt,##args); goto err_exit; } while(0)

    maps = fopen("/proc/self/maps", "r");
    if(!maps){
        fatal("failed to open maps");
    }

    while(!found && fgets(buff, sizeof(buff), maps))
        if(strstr(buff,"r--p") && strstr(buff,libpath)) found = 1; // libc libart等so是以r--p开头，第三方so是以r-xp开头

    fclose(maps);

    if(!found) fatal("%s not found in my userspace", libpath);

    if(sscanf(buff, "%lx", &load_addr) != 1)
        fatal("failed to read load address for %s", libpath);

    LOGI("%s loaded in Android at 0x%08lx", libpath, load_addr);

    fd = open(libpath, O_RDONLY);

    if(fd < 0) fatal("failed to open %s", libpath);

    size = lseek(fd, 0, SEEK_END);
    if(size <= 0) fatal("lseek() failed for %s", libpath);

    elf = (Elf_Ehdr *) mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

    close(fd);
    fd = -1;

    if(elf == MAP_FAILED) fatal("mmap() failed for %s", libpath);

    ctx = (struct ctx *) calloc(1, sizeof(struct ctx));
    if(!ctx) fatal("no memory for %s", libpath);

    ctx->load_addr = (void *) load_addr;
    shoff = reinterpret_cast<void *>((uintptr_t) elf + elf->e_shoff);

    for(k = 0; k < elf->e_shnum; k++, shoff = reinterpret_cast<void *>((uintptr_t) shoff +
                                                                       elf->e_shentsize))  {
        Elf_Shdr *sh = (Elf_Shdr *) shoff;

        switch(sh->sh_type) {

            case SHT_DYNSYM:
                if(ctx->dynsym) fatal("%s: duplicate DYNSYM sections", libpath);   /* .dynsym */
                ctx->dynsym = malloc(sh->sh_size);
                if(!ctx->dynsym) fatal("%s: no memory for .dynsym", libpath);
                memcpy(ctx->dynsym, reinterpret_cast<const void *>((uintptr_t) elf + sh->sh_offset), sh->sh_size);
                ctx->nsyms = (sh->sh_size/sizeof(Elf_Sym)) ;
                break;

            case SHT_STRTAB:
                if(ctx->dynstr) break;	/* .dynstr is guaranteed to be the first STRTAB */
                ctx->dynstr = malloc(sh->sh_size);
                if(!ctx->dynstr) fatal("%s: no memory for .dynstr", libpath);
                memcpy(ctx->dynstr, reinterpret_cast<const void *>((uintptr_t) elf + sh->sh_offset), sh->sh_size);
                break;

            case SHT_PROGBITS:
                if(!ctx->dynstr || !ctx->dynsym) break;
                /* won't even bother checking against the section name */
                ctx->bias = (off_t) sh->sh_addr - (off_t) sh->sh_offset;
                k = elf->e_shnum;  /* exit for */
                break;
        }
    }

    munmap(elf, size);
    elf = 0;

    if(!ctx->dynstr || !ctx->dynsym) fatal("dynamic sections not found in %s", libpath);

#undef fatal
    LOGI("%s: ok, dynsym = %p, dynstr = %p", libpath, ctx->dynsym, ctx->dynstr);
    return ctx;

    err_exit:
    if(fd >= 0) close(fd);
    if(elf != MAP_FAILED) munmap(elf, size);
    fake_dlclose(ctx);
    return 0;
}

UNEXPORT void* LoadLibrary::fake_dlsym(void *handle, const char *name)
{
    int k;
    struct ctx *ctx = (struct ctx *) handle;
    Elf_Sym *sym = (Elf_Sym *) ctx->dynsym;
    char *strings = (char *) ctx->dynstr;
    LOGI("handle = %p, name = %s, strings = %s", handle, name, strings);
    for(k = 0; k < ctx->nsyms; k++, sym++)
        if(strstr(strings + sym->st_name, name) != NULL) {

            void *ret = reinterpret_cast<void *>((uintptr_t)ctx->load_addr + (sym->st_value -
                                                 ctx->bias));
            LOGI("%s found at %p funcname=%s", name, ret, strings + sym->st_name);
            return ret;
        }
    return 0;
}


