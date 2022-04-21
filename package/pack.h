//
// Created by yangyao on 2022/4/5.
//

#ifndef PACK_PACK_H
#define PACK_PACK_H
#include<iostream>
#include<vector>
#include <cstring>
#include <sys/stat.h>
#include <climits>
#include <cstring>
#include "get_cur_time.h"
#include "mode_to_letters.h"
#include "../error/error.h"
#include "../version/app_info.h"
#include "../is_file_exist/is_file_exist.h"
#include "../bitfile/bitfile.h"
#include "../size_h/size_h.h"
#include "../yzip_struct.h"
#include "../file_info_struct.h"
#define HEAD_SIZE 147
#define FILE_NAME_LEN 100
#define SIZE_LEN 12
#define TIME_LEN 17
#define OFFSET_LEN 12
#define PERMISSION_LEN 6
#define FILE_NAME_OFFSET 0
#define SIZE_OFFSET 100
#define OFFSET_OFFSET 112
#define PERMISSION_OFFSET 124
#define MTIME_OFFSET 130
#define FILE_COUNT_LEN 2//标志文件数量,Byte
#define MAX_FILE_COUNT 4096//最多处理4096个文件
using namespace std;
struct PackageHead{
                                                    /*Byte offset*/
    char file_name[FILE_NAME_LEN];//文件名                 0
    char size[SIZE_LEN];//文件大小(Byte)                   100
    char offset[OFFSET_LEN];//文件偏移量                   112
    char permission[PERMISSION_LEN];//文件权限             124
    char mtime[TIME_LEN];//文件添加时间                    130
    //                                                    147

};
//获取文件的头部信息
PackageHead get_file_head(FileInfo &file);
//把文件以及文件头写入归档中
void write_file(vector<FileInfo> &files, FILE * fp_out,bool is_show_info);
//填入所有文件的文件头到vector中
void fill_head_vec(vector<PackageHead> &pkg_vec,vector<FileInfo> &files);
//把文件追加到打包文件中文件头的后面
void write_file_to_pkg(const char *file_name,FILE *file);
//获取文件头部信息
void get_all_head(FILE *fp,vector<PackageHead> &pkg_vec);
//打印头部信息
void print_all_head(FILE *fp);
//从buff数组中获取头部信息
void get_head_from_buff(char buff[HEAD_SIZE],PackageHead &pkg);
//解开归档，提取出文件
void get_file_from_pkg(YzipOptions &options, FILE *fp_in);
//获取所有文件的大小和
void get_sum_size(const vector<string>& file_paths,char * sum_size);
#endif //PACK_PACK_H
