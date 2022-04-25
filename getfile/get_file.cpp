//
// Created by yangyao on 2022/3/14.
//
//递归获取文件夹下所有文件
#include "get_file.h"

string get_relative_path(const string path, const string &suffix) {
    string str_relative_path = path.substr(path.find(suffix), path.size());
    return str_relative_path;
}

void get_file(YzipOptions &options, vector<FileInfo> &file_vec) {
    for (auto item: options.operand) {
        struct stat buff{};
        if (lstat(item.data(), &buff) == 0) {
            if (S_ISREG(buff.st_mode)) {
                if (access(item.data(), R_OK) == 0) {
                    FileInfo fileInfo(item.c_str(), \
                    basename((strdup(item.c_str()))), buff.st_mode, basename(strdup(item.data())), buff.st_size);
                    file_vec.push_back(fileInfo);
                } else {
                    error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s: Permission denied"), item.data());
                }
            } else if (S_ISDIR(buff.st_mode))
                get_file_res(item, options, file_vec, IS_NOT_CUR_DICT);
            else
                error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s: Unsupported format"), item.data());
        } else {
            error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s: %s"), item.data(), strerror(errno));
        }
    }
}

void get_file_res(string &path, YzipOptions &options, vector<FileInfo> &file_vec, bool is_cur_dict) {
    if (options.is_res) {
        if (endswith(path, "/")) {/*去掉文件夹后面的‘/’*/
            path.erase(path.end() - 1);
        }

        static string path_head = basename(strdup(path.data()));//存储文件相对路径--test@2022.4.11
        if (!is_cur_dict) {//如果不是在当前目录下，重新获取相对目录名{
            path_head=basename(strdup(path.data()));
        }
        DIR *dir;
        if ((dir = opendir(path.data())) != nullptr) {/*如果文件夹打开成功*/
            struct dirent *stdir;
            while (true) {
                if ((stdir = readdir(dir)) == nullptr) {/*如果文件遍历完毕*/
                    break;
                } else {
                    if (stdir->d_type == DT_DIR) {/*如果是文件夹，递归遍历*/
                        string dir_name = stdir->d_name;
                        if (dir_name != "." && dir_name != "..") {
                            string cur_path = path + "/" + stdir->d_name;
                            get_file_res(cur_path, options, file_vec, IS_CUR_DICT);
                        }
                    } else {/*如果是文件*/
                        struct stat buf;
                        string cur_path = path + "/" + stdir->d_name;
                        lstat(cur_path.data(), &buf);
                        if (S_ISREG(buf.st_mode)) {   /*如果是普通文件  */
                            if (access(cur_path.data(), R_OK) == 0)/*如果有可读权限*/
                            {
                                string file_relative_path = get_relative_path(cur_path, path_head);//文件相对路径
                                FileInfo fileInfo(const_cast<char *>(cur_path.c_str()), stdir->d_name, buf.st_mode,
                                                  file_relative_path, buf.st_size);
                                file_vec.push_back(fileInfo);
                            } else {
                                cur_path = path + "/" + stdir->d_name;
                                error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s: Permission denied"), cur_path.data());;
                            }
                        } else
                            error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s: Unsupported format"), cur_path.data());

                    }
                }
            }
        } else {
            error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s: %s"), path.data(), strerror(errno));
        }
    } else
        error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s is a directory -- ignored"), path.data());
}

void get_files_names(vector<FileInfo> &file_vec, vector<string> &files) {
    for (auto &item: file_vec) {
        files.emplace_back(item.file_path);
    }
}

