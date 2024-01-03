//
// Created by Administrator on 2022/6/28.
//

#ifndef ANTISDK_SHELL_H
#define ANTISDK_SHELL_H
#pragma once
#include "../../include/include.h"
#include <cstring>
#include "../../base/syscall/Syscall.h"
#include <vector>
using namespace std;

class Shell{
public:
    static int run_shell(const char *cmd, vector<char*> &resvec);
};


#endif //ANTISDK_SHELL_H
