//
// Created by yangyao on 2022/3/14.
//
#pragma once
#ifndef DIPLOMA_PROJECT_GETFILE_H
#define DIPLOMA_PROJECT_GETFILE_H

#endif //DIPLOMA_PROJECT_GETFILE_H

#include <iostream>
#include<string>
#include <sys/stat.h>
#include <libgen.h>
#include "../system.h"
#include "../version/app_info.h"
#include "../file_info_struct.h"
#include "../file_handle/file_handle.h"
using namespace std;

#include <dirent.h>
#include<unistd.h>
#include "../error/error.h"
#include "../yzip_struct.h"
//获取普通文件
void get_file(YzipOptions &options, vector<FileInfo> &file_vec);
//遍历处理所有文件以及文件夹
void get_file_res(string &path, YzipOptions &options, vector<FileInfo> &file_vec);
//获取所有文件的文件名到一个vector里面
void get_files_names(vector<FileInfo> &file_vec,vector<string> &files);