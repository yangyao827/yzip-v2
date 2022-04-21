//
// Created by yangyao on 2022/3/14.
//
#pragma once

#include "string"
#include "vector"
#include "verbose_info/verbose_info.h"
#define COMPRESSION 0
#define DECOMPRESSION 1
using namespace std;
//参数结构体
struct YzipOptions {
    int flag = COMPRESSION;//0压缩，1解压
    bool is_res = false;//是否递归目录
    vector<string> operand{}; //操作对象
    string password = {};//压缩密码
    bool show_info = false;//是否显示详细信息
    const char *out_path= nullptr;//输出文件路径
    VerboseInfo verbose_info{};//Verbose模式输出信息
};