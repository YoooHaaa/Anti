//
// Created by Administrator on 2023/1/17.
//

#ifndef ANTISDK_LOCALDLFCN_H
#define ANTISDK_LOCALDLFCN_H
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include "../../include/include.h"

#ifdef __cplusplus
extern "C" {
#endif

struct local_dlfcn_handle{
    char *elf;
    size_t size;
    char *dynsym;
    int nsyms;
    char *dynstr;
};
//此方法的功能为获取手机中本地（非应用内存中）so文件中函数的偏移
void *local_dlopen(const char *lib_path);
off_t local_dlsym(void *handle, const char *sym_name);
void local_dlclose(void *handle);

#ifdef __cplusplus
}
#endif



#endif //ANTISDK_LOCALDLFCN_H
/*
 * 此类的功能为获取手机中so文件函数的物理内存信息，用法如下

#ifdef __LP64__
   const char *lib_path = "/system/lib64/libc.so";
#else
   const char *lib_path = "/system/lib/libc.so";
#endif
#define CMP_COUNT 8
   const char *sym_name = "open";

   struct local_dlfcn_handle *handle = static_cast<local_dlfcn_handle *>(local_dlopen(lib_path));

   off_t offset = local_dlsym(handle,sym_name);

   FILE *fp = fopen(lib_path,"rb");
   char file_bytes[CMP_COUNT] = {0};
   if(fp != NULL){
       fseek(fp,offset,SEEK_SET);
       fread(file_bytes,1,CMP_COUNT,fp);
       fclose(fp);
  }

   void *dl_handle = dlopen(lib_path,RTLD_NOW);
   void *sym = dlsym(dl_handle,sym_name);

   int is_hook = memcmp(file_bytes,sym,CMP_COUNT) != 0;

   local_dlclose(handle);
   dlclose(dl_handle);

   char text[128] = {0};
   snprintf(text,128,"Function \"%s\" is Hook: %s",sym_name,is_hook ? "true" : "false");

 * */