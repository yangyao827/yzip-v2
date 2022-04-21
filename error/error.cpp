//
// Created by yangyao on 2022/3/10.
//
//错误、异常处理
#include "error.h"
#include "stdarg.h"
#include "../system.h"
#include "/usr/include/bash/include/gettext.h"

void error_msg(int status, std::string program_name, ...) {
    va_list vaList;
    va_start(vaList, program_name);
    const char *st1 = va_arg(vaList, char *);
    const char *st2 = va_arg(vaList, char *);
    const char *st3 = va_arg(vaList, char *);
    printf("%s: ", program_name.c_str());
    printf(_(st1), st2, st3);
    printf("\n");
    if (status)
        exit(status);
}
