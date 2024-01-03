# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("antitools")

# 设置项目路径变量，便于后面引用
set(CMAKE_CURRENT_SOURCE_DIR .)

# 编译器堆栈保护 设置此项会增大so包体积
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all")

# 开启汇编
set(can_use_assembler TRUE)
enable_language(ASM)

# 编译警告
add_definitions(-w) #忽略所有警告
#add_definitions(-Wwritable-strings)
#add_definitions(-Wno-format-security)

# 查找当前目录下的所有源文件，并保存到 DIR_LIB_SRCS 变量
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}  DIR_LIB_SRCS)
aux_source_directory(include  DIR_LIB_SRCS)
aux_source_directory(check  DIR_LIB_SRCS)
aux_source_directory(tools/base64  DIR_LIB_SRCS)
aux_source_directory(tools/md5  DIR_LIB_SRCS)
aux_source_directory(tools/rsa DIR_LIB_SRCS)
aux_source_directory(tools/aes DIR_LIB_SRCS)
aux_source_directory(tools/rc4 DIR_LIB_SRCS)
aux_source_directory(tools/crc DIR_LIB_SRCS)
aux_source_directory(base/dlfcn DIR_LIB_SRCS)
aux_source_directory(tools/bigint DIR_LIB_SRCS)
aux_source_directory(base/load DIR_LIB_SRCS)
aux_source_directory(base/call DIR_LIB_SRCS)
aux_source_directory(base/mem  DIR_LIB_SRCS)
aux_source_directory(base/str  DIR_LIB_SRCS)
aux_source_directory(base/syscall DIR_LIB_SRCS)
aux_source_directory(base/file DIR_LIB_SRCS)
aux_source_directory(base/field DIR_LIB_SRCS)
aux_source_directory(base/env DIR_LIB_SRCS)
aux_source_directory(base/thread DIR_LIB_SRCS)
aux_source_directory(base/crash DIR_LIB_SRCS)
aux_source_directory(base/shell DIR_LIB_SRCS)
aux_source_directory(base/new DIR_LIB_SRCS)
aux_source_directory(anti DIR_LIB_SRCS)
aux_source_directory(anti/frida DIR_LIB_SRCS)
aux_source_directory(anti/root DIR_LIB_SRCS)
aux_source_directory(anti/debug DIR_LIB_SRCS)
aux_source_directory(anti/dump DIR_LIB_SRCS)
aux_source_directory(anti/emulator DIR_LIB_SRCS)
aux_source_directory(anti/xposed DIR_LIB_SRCS)
aux_source_directory(anti/capture DIR_LIB_SRCS)
aux_source_directory(anti/repack DIR_LIB_SRCS)
aux_source_directory(anti/sandbox DIR_LIB_SRCS)
aux_source_directory(anti/hook DIR_LIB_SRCS)
aux_source_directory(anti/rom DIR_LIB_SRCS)
aux_source_directory(anti/unidbg DIR_LIB_SRCS)

add_library( # Sets the name of the library.
        yooha

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
#        native-lib.cpp
#        Check.cpp
#        ./tools/base64/base64.cpp
#        ./tools/call/Method.cpp)
#        src/main/cpp/anti/syscall/asm.S
        ${DIR_LIB_SRCS})



# 指定头文件目录
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}
#        ${CMAKE_CURRENT_SOURCE_DIR}/anti/
#        ${CMAKE_CURRENT_SOURCE_DIR}/tools/base64/
#        ${CMAKE_CURRENT_SOURCE_DIR}/tools/call/
#        ${CMAKE_CURRENT_SOURCE_DIR}/tools/md5/)
#include_directories(.)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tools/call/)

#target_include_directories(yooha PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/tools/call/
#        )

#target_include_directories(yooha PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/anti/)
#target_include_directories(yooha PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/base/)
#target_include_directories(yooha PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
#target_include_directories(yooha PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tools/)
target_include_directories(yooha PUBLIC arch/${ANDROID_ABI}) # cpp文件直接 #include "syscall_arch.h" 即可使用

find_library( # Sets the name of the path variable.
        log-lib
        log
        android # 链接下面的库
        #include "android/asset_manager.h"
        #include "android/asset_manager_jni.h"
        )



target_link_libraries( # Specifies the target library.
        yooha

        android
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib} )