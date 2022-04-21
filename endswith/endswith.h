//
// Created by yangyao on 2022/3/21.
//

#ifndef DIPLOMA_PROJECT_ENDSWITH_H
#define DIPLOMA_PROJECT_ENDSWITH_H

#include<iostream>
#include <cstring>

using namespace std;
#define ADJUST_INDICES(start, end, len)     \
    if (end > len)                          \
        end = len;                          \
    else if (end < 0) {                     \
        end += len;                         \
        if (end < 0)                        \
        end = 0;                            \
    }                                       \
    if (start < 0) {                        \
        start += len;                       \
        if (start < 0)                      \
        start = 0;                          \
    }

//匹配函数：endswith与startwith的内部调用函数
int _string_tailmatch(const std::string &self, const std::string &substr, int start, int end, int direction);

bool endswith(const std::string &str, const std::string &suffix, int start = 0, int end = INT32_MAX);

#endif //DIPLOMA_PROJECT_ENDSWITH_H
