//
// Created by yangyao on 2022/3/14.
//
//对文件的后续处理
#include "file_handle.h"

using namespace std;

//处理压缩文件
void file_handle_encoding(FILE *fp, YzipOptions &options) {
    string out_path_str = options.out_path;
    struct stat buff{};
    if ((lstat(out_path_str.data(), &buff) == 0)) {
        if (S_ISDIR(buff.st_mode)) {//如果是文件夹
            error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"), out_path_str.data(), "is a directory");
        }
        if (!endswith(out_path_str, FILE_NAME_SUFFIX)) {//如果名字不是以“.pkg.yz结尾的”
            out_path_str = out_path_str + FILE_NAME_SUFFIX;//加上后缀名
        }
        if (is_overwrite(out_path_str.data())) {//如果文件存在，但是不覆盖
            exit(0);//退出
        }
        //压缩
        if (options.password == "") {//不加密
            lzw_encoding(fp, out_path_str.data(), options.password);
        } else {//加密压缩
            lzw_encoding_encrypt(fp, out_path_str.data(), options.password);
        }
    } else {//打开失败，文件不存在
        if (!endswith(out_path_str, FILE_NAME_SUFFIX)) {//如果名字不是以“.pkg.yz结尾的”
            out_path_str = out_path_str + FILE_NAME_SUFFIX;//加上后缀名
        }
        if (is_overwrite(out_path_str.data())) {//如果文件存在，但是不覆盖
            exit(0);//退出
        }
        //压缩
        if (options.password == "") {//不加密
            lzw_encoding(fp, out_path_str.data(), options.password);
        } else {//加密压缩
            lzw_encoding_encrypt(fp, out_path_str.data(), options.password);
        }
    }
    if (options.show_info) {//填入压缩后的大小
        struct stat buf{};
        FileSizeHumanReadable file_info{};
        char size_cur[20];
        if (lstat(out_path_str.data(), &buf) != 0)
            error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"), out_path_str.data(), strerror(errno));
        sprintf(size_cur, "%ld", buf.st_size);
        file_info = size_h(size_cur);
        strcat(file_info.size_h, file_info.unit);
        strcpy(options.verbose_info.size_cur, file_info.size_h);
    }
}

//处理解压文件
void file_handle_dncoding(FILE *fp, YzipOptions &options) {
    string out_path_str = options.out_path;
    if (options.operand.size() > 1) {//参数太多
        error_msg(EXIT_FAILURE, PROGRAM_NAME,
                  _("Too many parameters you have provided"), nullptr);
    }
    if(access(options.operand[0].data(),R_OK)!=0){//文件不存在或打不开
        error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"), options.operand[0].data(), strerror(errno));
    }
    struct stat buff{};
    if ((lstat(out_path_str.data(), &buff) == 0)) {//打开成功
        if (!S_ISDIR(buff.st_mode)) {//如果不是文件夹
            error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"), out_path_str.data(), "is not a directory");
        }
        if (!is_encrypt(options.operand[0].data()))//未加密
            lzw_decoding(options.operand[0].data(), fp);//解压压缩包到临时文件
        else//加密了
            lzw_decoding_encrypt(options.operand[0].data(), fp);
    } else {//打开文件失败
        error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"), out_path_str.data(), strerror(errno));
    }
    if (options.show_info) {//获取输出详情的相关信息
        if ((lstat(options.operand[0].data(), &buff) != 0))
            error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"), out_path_str.data(), strerror(errno));
        char size_bef[20];
        sprintf(size_bef, "%ld", buff.st_size);
        FileSizeHumanReadable file_size_info{};
        file_size_info = size_h(size_bef);
        strcat(file_size_info.size_h, file_size_info.unit);
        strcpy(options.verbose_info.size_bef, file_size_info.size_h);
    }
}

//列举文件
void list_pkg_file(const char *pkg_file_in) {
    FILE *fp_tmp = tmpfile();//创建一个临时文件
    if (!is_encrypt(pkg_file_in))//不加密
        lzw_decoding_for_list(pkg_file_in, fp_tmp);//解压出头部信息
    else {
        lzw_decoding_for_list_encrypt(pkg_file_in, fp_tmp);
    }
    print_all_head(fp_tmp);
    fclose(fp_tmp);
    exit(0);
}

//处理文件
void file_handle(YzipOptions &options) {
    if (options.out_path == nullptr) {//如果输出文件为空
        error_msg(EXIT_FAILURE, PROGRAM_NAME,
                  _("Please enter the out path"), nullptr);
    }
    string out_path_str = options.out_path;
    if (endswith(out_path_str, "/")) {//如果路径本身带有‘/’，去掉
        out_path_str.erase(end(out_path_str) - 1);
        options.out_path = out_path_str.data();
    }
    if (options.flag == 0) {//如果是压缩
        clock_t start, end;//开始，结束时间
        start = clock();//开始时间
        vector<FileInfo> files{};
        vector<string> file_paths{};
        get_file(options, files);//筛选出符合要求的文件
        get_files_names(files, file_paths);//获取所有文件的路径
        FILE *fp_tmp = tmpfile();//创建一个临时文件
        char sum_size[20];
        get_sum_size(file_paths, sum_size);//获取文件总的大小
        strcpy(options.verbose_info.size_bef, sum_size);//填入文件压缩前总大小
        write_file(files, fp_tmp, options.show_info);//把文件写入临时的pkg包
        fseek(fp_tmp, 0, SEEK_SET);//文件指针指向头部
        file_handle_encoding(fp_tmp, options);//压缩
        fclose(fp_tmp);
        end = clock();
        options.verbose_info.duration = (double) (end - start) / CLOCKS_PER_SEC;//消耗时间
        show_detail(options.verbose_info, options.show_info);//显示详情
    } else {//如果是解压缩
        clock_t start, end;//开始，结束时间
        start = clock();
        FILE *fp_tmp = tmpfile();//创建一个临时文件
        file_handle_dncoding(fp_tmp, options);//解压
        fseek(fp_tmp, 0, SEEK_SET);//文件指针指向头部
        get_file_from_pkg(options, fp_tmp);//提取出文件
        fclose(fp_tmp);
        end = clock();
        options.verbose_info.duration = (double) (end - start) / CLOCKS_PER_SEC;//消耗时间
        show_detail(options.verbose_info, options.show_info);//显示详情
    }
}