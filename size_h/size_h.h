//
// Created by yangyao on 2022/4/9.
//

#ifndef DIPLOMA_PROJECT_SIZE_H_H
#define DIPLOMA_PROJECT_SIZE_H_H
#include <cstdlib>
#include <cstdio>
struct FileSizeHumanReadable{
     char size_h[20];//文件大小
     char unit[5];//文件单位
};

//把byte转换为容易理解的、容易显示的大小
FileSizeHumanReadable size_h(const char * size_b);
#endif //DIPLOMA_PROJECT_SIZE_H_H
