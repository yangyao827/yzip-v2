//
// Created by yangyao on 2022/4/10.
//

#ifndef DIPLOMA_PROJECT_VERBOSE_INFO_H
#define DIPLOMA_PROJECT_VERBOSE_INFO_H
#include <cstdio>
struct VerboseInfo{
    char  size_bef[20];//压缩前的大小
    char  size_cur[20];//压缩后的大小
    double duration=0;//耗时
};
//输出详情
void show_detail(VerboseInfo verbose_info,bool is_show);
#endif //DIPLOMA_PROJECT_VERBOSE_INFO_H
