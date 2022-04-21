//
// Created by yangyao on 2022/3/22.
//

#ifndef DIPLOMA_PROJECT_ENCRYPTION_H
#define DIPLOMA_PROJECT_ENCRYPTION_H

#include "../bitfile/bitfile.h"
#include "iostream"
#include <openssl/md5.h>
#include "cstring"
#include "../error/error.h"
#include "../version/app_info.h"
#include "../system.h"
#include <fstream>
#include "./aes_128_cbc.h"
#define ENCRYPT_CODE_LEN CHAR_BIT
#define ERRORCOUNT 3//输错密码的次数
#define ENCRYPED_OFFSET 17
#define UNENCRYPED_OFFSET 1
#define READ_BUF_SIZE 1024 * 16
using namespace std;

bool is_encrypt(bit_file_t *bf, unsigned char md5[MD5_DIGEST_LENGTH]);
void encrypt_file(FILE *fp_in,bit_file_t *bf_out);
void md5code(unsigned char outmd[], string &passwd);
bool is_encrypt(const char *file_name);
string is_correct_passwd(bit_file_t *bf);
//获取加密的字节数，未加密为1字节，加密后为17字节
int get_offset(const char *file_name);
//校验文件的md5与写入的md5值是否一致,bef为之前写入的md5,cur为现在的md5
bool check_md5(const unsigned char file_md5_bef[MD5_DIGEST_LENGTH],const unsigned char file_md5_cur[MD5_DIGEST_LENGTH]);
//检测压缩包是否完整，是返回true,反之返回false
bool check_file_md5(const char *file_name);
void get_md5_from_file(const char*file_name, unsigned char md5_value[MD5_DIGEST_LENGTH]);
void get_file_md5(const char *file_name, unsigned char md5_value[MD5_DIGEST_LENGTH]);
//向文件中写入文件md5值
void write_md5(const char * file_name,unsigned char md5[MD5_DIGEST_LENGTH]);
//在压缩文件中预留md5的位置
void file_md5_init(bit_file_t *bf);
//校验文件，并且输出错误以及正确信息
void check_file(const char *file_name,bool flag);
//调整文件指针，使其指向数据部分，并返回当前文件指针位置
off_t adjust_file_ptr(bit_file_t *bf);
//
void get_file_md5_for_encrypt(FILE *file, unsigned char md5_value[MD5_DIGEST_LENGTH]);
//从压缩且加密的文件中获取文件的MD5值
void get_md5_from_file_encrypt(FILE *file,unsigned char md5_value[MD5_DIGEST_LENGTH],string passwd);
#endif //DIPLOMA_PROJECT_ENCRYPTION_H
