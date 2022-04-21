//
// Created by yangyao on 2022/4/7.
//

#ifndef DIPLOMA_PROJECT_IS_FILE_EXIST_H
#define DIPLOMA_PROJECT_IS_FILE_EXIST_H
#include <cstdlib>
#include <csignal>
#include <cstdio>
#include <termios.h>

//判断文件是否存在，以及是否替换掉已经存在的文件
bool is_overwrite(const char *out_file);
//取消getchar()缓冲区
void set_keypress();
//恢复getchar()缓冲区
void reset_keypress();
#endif //DIPLOMA_PROJECT_IS_FILE_EXIST_H
