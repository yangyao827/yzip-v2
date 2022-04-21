//
// Created by yangyao on 2022/3/14.
//
#pragma once
#ifndef DIPLOMA_PROJECT_FILEHANDLE_H
#define DIPLOMA_PROJECT_FILEHANDLE_H
#include "string"
#include "../file_info_struct.h"
#include "../yzip_struct.h"
#include "../lzw/lzw.h"
#include "cstring"
#include "sys/stat.h"
#include "../endswith/endswith.h"
#include "../error/error.h"
#include "../version/app_info.h"
#include "../system.h"
#include "../is_file_exist/is_file_exist.h"
#include "../getfile/get_file.h"
#include "../package/pack.h"
#include <ctime>
//文件标准后缀
#define FILE_NAME_SUFFIX ".pkg.yz"
using namespace std;

/*处理文件，压缩或者解压文件*/
void file_handle(YzipOptions &options);
//列举出文件里面的文件信息
void list_pkg_file(const char *pkg_file_in);
#endif //DIPLOMA_PROJECT_FILEHANDLE_H

