//
// Created by yangyao on 2022/3/10.
//
//程序版本信息
#include "version_etc.h"
using namespace std;

void
version_etc(string program_name, string code_time, string version, string authors) {
    printf("%s %s\n\n", program_name.data(), version.data());
    printf("This program was written in %s\n\n", code_time.data());
    printf("Written by %s\n", authors.data());
    printf("Contact with me: %s\n",AUTHOR_EMAIL);
}
