//
// Created by yangyao on 2022/4/20.
//

#ifndef DIPLOMA_PROJECT_AES_128_CBC_H
#define DIPLOMA_PROJECT_AES_128_CBC_H
#include <cstdio>
#include <openssl/evp.h>
#include <cstring>

int do_crypt(FILE *in, FILE *out, int do_encrypt,const char *passwd);
#endif //DIPLOMA_PROJECT_AES_128_CBC_H
