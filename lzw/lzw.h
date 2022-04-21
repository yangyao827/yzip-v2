//
// Created by yangyao on 2022/3/18.
//
#include <string>
#include "unordered_map"
#include <iostream>
#include <climits>
#include "../bitfile/bitfile.h"
#include <csignal>
#include "../encryption/encryption.h"
#include <openssl/md5.h>
#include "../is_file_exist/is_file_exist.h"
#include "../package/pack.h"
#include "../encryption/aes_128_cbc.h"
#ifndef MYBIT_LZW_H
#define MYBIT_LZW_H

//#define CODE_LEN        12                  /* # bits in a code word */
#define MIN_CODE_LEN    9
#define MAX_CODE_LEN    16
#define FIRST_CODE      (1 << (MIN_CODE_LEN-1))     /* location of 1st string code */

#define MAX_CODES       (1 << MAX_CODE_LEN)
#define EMPTY           -1
#define DICT_SIZE       (MAX_CODES - FIRST_CODE)

#define CODE_MS_BITS    (CODE_LEN - CHAR_BIT)
#define MS_BITS_MASK    (UCHAR_MAX << (CHAR_BIT - CODE_MS_BITS))

//#if (CODE_LEN <= CHAR_BIT)
//#error Code words must be larger than 1 character
//#endif

#if (CODE_LEN > (2 * CHAR_BIT))
#error Code words larger than 2 characters require a re-write of GetCodeWord\
 and PutCodeWord
#endif

#if ((MAX_CODES - 1) > INT_MAX)
#error There cannot be more codes than can fit in an integer
#endif

#define CURRENT_MAX_CODES(bits)     ((unsigned int)((1 << (bits))-1))

void lzw_encoding(FILE *fp_in, const char *out_file, string passwd);//压缩
void lzw_decoding(const char *in_file, FILE *fp_out);//解压
void put_chars(bit_file_t *bf, const string& str);//把string的每个字符输出到文件
//为了列举出压缩包里的文件的解压，只解压头部信息，数据部分不解压
void lzw_decoding_for_list(const char *pkg_file_in,FILE * fp_tmp_out);
//压缩并加密
void lzw_decoding_encrypt(const char *in_file,FILE *fp_out);
//对加密文件提取信息
void lzw_decoding_for_list_encrypt(const char *pkg_file_in,FILE * fp_tmp_out);
//解密加密的压缩文件
void lzw_encoding_encrypt(FILE *fp_in, const char *out_file,string passwd);
#endif //MYBIT_LZW_H
