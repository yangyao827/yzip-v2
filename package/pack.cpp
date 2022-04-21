#include <libgen.h>
#include "pack.h"

PackageHead get_file_head(FileInfo &file) {
    PackageHead pkg{};
    memset(&pkg, 0, sizeof(pkg));
    strcpy(pkg.file_name, file.relative_path.data());//装入文件名
    sprintf(pkg.size, "%ld", file.file_size);//装入文件大小
    sprintf(pkg.permission,"%u",file.permissions);//装入文件权限
    get_cur_time(pkg.mtime);
    return pkg;
}

void write_file_to_pkg(const char *file_name, FILE *file) {
    FILE *fp = fopen(file_name, "r");//打开待读取的文件
    fseek(file, 0, SEEK_END);//文件指针指向文件尾
    unsigned char buff[1024];//读入数据缓冲区
    memset(buff, 0, sizeof(buff));
    int rest = 0;
    while ((rest = fread(buff, sizeof(unsigned char), sizeof(buff), fp)) != 0) {
        fwrite(buff, sizeof(unsigned char), rest, file);
    }
    fclose(fp);
}

void write_file(vector<FileInfo> &files, FILE *fp_out,bool is_show_info) {
    if (files.size() > MAX_FILE_COUNT) {
        error_msg(EXIT_FAILURE, PROGRAM_NAME,
                  _("Too much files"), nullptr);
    }
    if (files.empty()) {//如果没有文件
        exit(0);//直接退出程序
    }
    vector<PackageHead> pkg_vec;
    fill_head_vec(pkg_vec, files);
    uint16_t file_count = files.size();
    bit_file_t *bf = BitFileOpen(fp_out, BF_WRITE);
    PutCodeWord(bf, file_count, FILE_COUNT_LEN * CHAR_BIT);//写入文件个数到头部前
    for (int i = 0; i < file_count; ++i) {
        fwrite(&pkg_vec[i], HEAD_SIZE, 1, fp_out);//写入文件的头
    }
    for (int i = 0; i < file_count; ++i) {
        if(is_show_info){
            FileSizeHumanReadable file_info{};
            file_info= size_h(pkg_vec[i].size);
            strcat(file_info.size_h,file_info.unit);
            printf("Adding: %s\t%s\n",files[i].file_path.data(),file_info.size_h);
        }
        write_file_to_pkg(files[i].file_path.data(), fp_out);//写入文件
    }
}

void fill_head_vec(vector<PackageHead> &pkg_vec, vector<FileInfo> &files) {
    for (auto item: files) {
        pkg_vec.push_back(get_file_head(item));//获取文件头部信息
    }
    //计算文件的偏移量
    for (int i = 0; i < pkg_vec.size(); ++i) {
        if (i == 0) {//如果是第一个文件，偏移量等于（FILE_COUNT_LEN+文件数*头部大小）
            long int offset = FILE_COUNT_LEN + pkg_vec.size() * HEAD_SIZE;
            sprintf(pkg_vec[i].offset, "%ld", offset);
        } else {//如果不是第一个文件，则偏移量等于前一个偏移量+前一个大小
            long int offset = atol(pkg_vec[i - 1].offset) + atol(pkg_vec[i - 1].size);
            sprintf(pkg_vec[i].offset, "%ld", offset);
        }
    }
}

void get_head_from_buff(char buff[HEAD_SIZE], PackageHead &pkg) {
    strncpy(pkg.file_name, buff + FILE_NAME_OFFSET, FILE_NAME_LEN);//获取文件名
    strncpy(pkg.size, buff + SIZE_OFFSET, SIZE_LEN);//获取文件大小
    strncpy(pkg.offset, buff + OFFSET_OFFSET, OFFSET_LEN);//获取文件偏移量
    strncpy(pkg.permission, buff + PERMISSION_OFFSET, PERMISSION_LEN);//获取文件偏移量
    strncpy(pkg.mtime, buff + MTIME_OFFSET, TIME_LEN);//获取添加时间
}

void print_all_head(FILE *fp) {
    vector<PackageHead> pkg_vec{};
    get_all_head(fp, pkg_vec);
    size_t total_size=0;
    FileSizeHumanReadable file_size_info{};
    //打印全部头部信息
    printf("%-10s   %-10s       %-10s   %-s\n", "Permission","CreatTime","Size","FileName");
    printf("------------------------------------------------------\n");
    for (auto it: pkg_vec) {
        file_size_info= size_h(it.size);
        strcat(file_size_info.size_h,file_size_info.unit);
        char permission[20];
        mode_to_letters(atol(it.permission),permission);
        printf("%-10s  %-16s  %-10s  %-s\n", permission, it.mtime, file_size_info.size_h,basename(it.file_name));
        total_size+=atol(it.size);
    }
    printf("------------------------------------------------------\n");
    char total_size_str[20];
    sprintf(total_size_str,"%ld",total_size);
    file_size_info=size_h(total_size_str);
    printf("TotalSize: %s%s\n",file_size_info.size_h,file_size_info.unit);
}

void get_all_head(FILE *fp, vector<PackageHead> &pkg_vec) {
    fseek(fp,0,SEEK_SET);
    PackageHead pkg{};//存放单个头部信息
    char buff[HEAD_SIZE];
    unsigned int head_count;//存文件（头）个数
    bit_file_t *bf = BitFileOpen(fp, BF_READ);
    memset(buff, 0, sizeof(buff));
    head_count = GetCodeWord(bf, FILE_COUNT_LEN * CHAR_BIT);//读取前2个字节（文件长度）
    for (int i = 0; i < head_count; ++i) {
        fread(buff, sizeof(pkg), 1, fp);//读入单个头部信息到buff中
        get_head_from_buff(buff, pkg);//从buff中解析出头部信息
        pkg_vec.push_back(pkg);
        memset(buff, 0, sizeof(buff));//清空buff
    }
}

void get_file_from_pkg(YzipOptions &options, FILE *fp_in) {
    vector<PackageHead> pkg_vec{};//装头部信息
    get_all_head(fp_in, pkg_vec);//获取所有头部信息
    FileSizeHumanReadable file_size_info{};
    size_t total_size=0;
    for (auto &it: pkg_vec) {
        total_size+= atol(it.size);
        char file_out[1024];
        sprintf(file_out, "%s/", options.out_path);
        strcat(file_out, it.file_name);//把路径和文件名连接成文件路径
        if(options.show_info){
            memset(&file_size_info,0,sizeof(file_size_info));
            file_size_info= size_h(it.size);
            strcat(file_size_info.size_h,file_size_info.unit);
            printf("Unpack: %s\t%s\n",file_out,file_size_info.size_h);
        }
        if (is_overwrite(file_out)) {//文件存在，但是不覆盖
            continue;//略过当前文件
        }
        FILE *fp_out;
        char dir_name[256];
        strcpy(dir_name,dirname(strdup(file_out)));//获取文件路径
        char cmd[256]="mkdir -p ";
        strcat(cmd,dir_name);
        if(access(dir_name,F_OK)!=0){//如果文件夹不存在，创建文件夹
            system(cmd);
        }
        if ((fp_out = fopen(file_out, "w")) == nullptr) {
            error_msg(EXIT_SUCCESS, PROGRAM_NAME, _("%s: %s"), file_out, strerror(errno));
            continue;//如果打开失败，略过
        }
        long int size_t = atol(it.size);
        long int count = 0;//计数
        fseek(fp_in, atol(it.offset), SEEK_SET);//调整文件指针到文件前
        unsigned char c;
        while (count < size_t) {//把数据写入文件
            c = fgetc(fp_in);
            fputc(c, fp_out);
            count++;
        }
        fclose(fp_out);
        chmod(file_out, atol(it.permission));//修改权限
    }
    if(options.show_info){
        char sum_size[20];//总文件大小
        memset(&file_size_info,0,sizeof(file_size_info));
        sprintf(sum_size,"%ld",total_size);
        file_size_info= size_h(sum_size);
        strcat(file_size_info.size_h,file_size_info.unit);
        strcpy(options.verbose_info.size_cur,file_size_info.size_h);
    }
}

void get_sum_size(const vector<string>& file_paths,char * sum_size){
    struct stat buf{};
    size_t total_size=0;
    for(auto &item:file_paths){
        if(lstat(item.data(),&buf)!=0){//打开失败
            error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"),item.data(), strerror(errno));
        }
        total_size+=buf.st_size;
    }
    FileSizeHumanReadable file_info{};
    sprintf(sum_size,"%ld",total_size);
    file_info= size_h(sum_size);
    strcpy(sum_size,file_info.size_h);
    strcat(sum_size,file_info.unit);
}
