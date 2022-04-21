//
// Created by yangyao on 2022/3/22.
//

#include "encryption.h"

//判断是不是正确的密码
bool md5_compare(unsigned char correct_passwd[MD5_DIGEST_LENGTH], unsigned char passwd[MD5_DIGEST_LENGTH]) {
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        if (correct_passwd[i] != passwd[i]) {
            return false;
        }
    }
    return true;
}

//输入密码进行判断
string is_correct_passwd(bit_file_t *bf) {
    unsigned char correct_passwd[MD5_DIGEST_LENGTH];//正确的密码序列
    unsigned char passwd[MD5_DIGEST_LENGTH];//输入的密码序列
    string passwd_str{};
    if (is_encrypt(bf, correct_passwd)) {//如果加密了
        printf("This file is encrypted, please enter your password:\n");
        system("stty -echo");
        for (int i = 0; i < ERRORCOUNT; ++i) {/*最多试3次退出*/
            cin >> passwd_str;
            get_md5_from_file_encrypt(get_bitfile_fp(bf),passwd,passwd_str);//获取用当前密码输入密码解密的文件校验码
            if (md5_compare(correct_passwd, passwd)) {//密码正确
                system("stty echo");
                return passwd_str;//返回正确密码
            } else {
                if (i < 2)
                    printf("Incorrect password, please try again:\n");
            }
        }
        system("stty echo");
        error_msg(EXIT_FAILURE, PROGRAM_NAME, _("You've entered the wrong password too many times"), nullptr);
    }
}


//获取字符串的MD5
void md5code(unsigned char outmd[MD5_DIGEST_LENGTH], string &passwd) {
    //memset(outmd, 0, sizeof(outmd));
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, passwd.data(), passwd.size());
    MD5_Final(outmd, &ctx);
}

//判断是否加密了,并且提取出加密后的密码
bool is_encrypt(bit_file_t *bf, unsigned char md5[MD5_DIGEST_LENGTH]) {
    //memset(md5, 0, sizeof(md5));
    unsigned int flag;//判断是否加密，如果开头16位全1，就为加密，否则为未加密。
    int max_code = (1 << ENCRYPT_CODE_LEN);
    flag = GetCodeWord(bf, ENCRYPT_CODE_LEN);
    if (flag== (max_code - 1)) {
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            md5[i] = BitFileGetChar(bf);
        }
        return true;
    }
    return false;
}
//判断是否加密了
bool is_encrypt(const char *file_name) {
    unsigned int flag;//判断是否加密，如果开头8位全1，就为加密，否则为未加密。
    int max_code = (1 << ENCRYPT_CODE_LEN);
    FILE *file;
    file= fopen(file_name,"r");
    flag = fgetc(file);
    if (flag== (max_code - 1)) {
        fclose(file);
        return true;
    }
    fclose(file);
    return false;
}
//加密压缩文件，前8bit为标志，后面为文件的校验值
void encrypt_file(FILE *fp_in,bit_file_t *bf_out) {
    unsigned int flag = (2 << ENCRYPT_CODE_LEN) - 1;
    PutCodeWord(bf_out, flag, ENCRYPT_CODE_LEN);
    unsigned char outmd[MD5_DIGEST_LENGTH];
    get_file_md5_for_encrypt(fp_in,outmd);//获取未加密的数据的MD5校验值
    for (unsigned char i : outmd) {
        BitFilePutChar(i, bf_out);
    }
}

int get_offset(const char * file_name){
    if(is_encrypt(file_name)){
        return ENCRYPED_OFFSET;
    }
    return UNENCRYPED_OFFSET;
}


void get_file_md5(const char *file_name, unsigned char md5_value[MD5_DIGEST_LENGTH]) {
    std::ifstream file(file_name, std::ifstream::binary);
    if (!file) {
        error_msg(EXIT_FAILURE, PROGRAM_NAME, _("%s: %s"), file_name, strerror(errno));
    }
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    char buf[READ_BUF_SIZE];
    file.seekg(get_offset(file_name) + MD5_DIGEST_LENGTH);
    while (file.good()) {
        file.read(buf, sizeof(buf));
        MD5_Update(&md5Context, buf, file.gcount());
    }
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(md5_value, &md5Context);
    file.close();
}

void get_file_md5_for_encrypt(FILE *file, unsigned char md5_value[MD5_DIGEST_LENGTH]) {
    MD5_CTX md5Context;
    MD5_Init(&md5Context);
    char buff[READ_BUF_SIZE];
    int rest=0;
    while ((rest = fread(buff, sizeof(unsigned char), sizeof(buff), file)) != 0) {
        MD5_Update(&md5Context, buff, rest);
    }
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(md5_value, &md5Context);
}


bool
check_md5(const unsigned char file_md5_bef[MD5_DIGEST_LENGTH], const unsigned char file_md5_cur[MD5_DIGEST_LENGTH]) {
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        if (file_md5_bef[i] != file_md5_cur[i]) {
            return false;
        }
    }
    return true;
}


void get_md5_from_file(const char *file_name, unsigned char md5_value[MD5_DIGEST_LENGTH]) {
    //memset(md5_value, 0, sizeof(md5_value) * MD5_DIGEST_LENGTH);
    unsigned char passwd[MD5_DIGEST_LENGTH];
    bit_file_t *bf;
    bf = BitFileOpen(file_name, BF_READ);
    is_encrypt(bf, passwd);
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        md5_value[i] = BitFileGetChar(bf);
    }
    BitFileClose(bf);
}

void get_md5_from_file_encrypt(FILE *file,unsigned char md5_value[MD5_DIGEST_LENGTH],string passwd){
    FILE *fp_tmp=tmpfile();
    fseek(file,(ENCRYPT_CODE_LEN/CHAR_BIT)+MD5_DIGEST_LENGTH+MD5_DIGEST_LENGTH,SEEK_SET);//调整文件指针
    do_crypt(file,fp_tmp,0,passwd.data());//解密
    fseek(fp_tmp,0,SEEK_SET);
    get_file_md5_for_encrypt(fp_tmp,md5_value);
}

bool check_file_md5(const char *file_name) {
    unsigned char file_md5[MD5_DIGEST_LENGTH];//计算文件的md5
    unsigned char read_file_md5[MD5_DIGEST_LENGTH];//文件中写入的原md5
    memset(file_md5, 0, sizeof(file_md5));
    memset(read_file_md5, 0, sizeof(read_file_md5));
    get_file_md5(file_name, file_md5);
    get_md5_from_file(file_name, read_file_md5);
    if (check_md5(read_file_md5, file_md5)) {
        return true;
    }
    return false;
}

void check_file(const char *file_name, bool flag) {//flag用来控制是否输出无错误信息
    if (check_file_md5(file_name)) {
        if (flag) {
            printf("No errors were found in: %s\n", file_name);
            exit(1);
        }
    } else {
        printf("At least one error has been found in: %s\n", file_name);
        exit(0);
    }
}

void write_md5(const char *file_name, unsigned char md5[MD5_DIGEST_LENGTH]) {
    FILE *file;
    file = fopen(file_name, "r+");
    fseek(file, get_offset(file_name), 0);
    fwrite(md5, MD5_DIGEST_LENGTH, 1, file);
    fclose(file);
}

void file_md5_init(bit_file_t *bf) {
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        BitFilePutChar(0, bf);
    }
}

off_t adjust_file_ptr(bit_file_t *bf){
    fseek(get_bitfile_fp(bf),MD5_DIGEST_LENGTH,SEEK_CUR);
    return ftell(get_bitfile_fp(bf));
}