//
// Created by yangyao on 2022/3/18.
//

#ifndef DIPLOMA_PROJECT_FILE_INFO_STRUCT_H
#define DIPLOMA_PROJECT_FILE_INFO_STRUCT_H

#include "string"
#include "cstring"

using namespace std;

/*存储文件信息*/
struct FileInfo {
    string file_path;//文件绝对路径
    string file_name;//文件名
    unsigned int permissions;//权限
    string relative_path;//文件相对路径
    size_t file_size;//文件大小
    FileInfo(string file_path, string file_name, unsigned int permissions,string relative_path,size_t file_size) {
        this->file_path = file_path;
        this->file_name = file_name;
        this->permissions = permissions;
        this->relative_path=relative_path;
        this->file_size=file_size;
    }
};

#endif //DIPLOMA_PROJECT_FILE_INFO_STRUCT_H
