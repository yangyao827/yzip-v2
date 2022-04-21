//
// Created by yangyao on 2022/3/21.
//

#include "endswith.h"

//匹配函数：endswith与startwith的内部调用函数
int _string_tailmatch(const std::string &self, const std::string &substr, int start, int end, int direction) {
    int selflen = (int) self.size();
    int slen = (int) substr.size();

    const char *str = self.c_str();
    const char *sub = substr.c_str();

    //对输入的范围进行校准
    ADJUST_INDICES(start, end, selflen);

    //字符串头部匹配（即startswith）
    if (direction < 0) {
        if (start + slen > selflen)
            return 0;
    }
        //字符串尾部匹配（即endswith）
    else {
        if (end - start < slen || start > selflen)
            return 0;
        if (end - slen > start)
            start = end - slen;
    }
    if (end - start >= slen)
        //mcmcmp函数用于比较buf1与buf2的前n个字节
        return !std::memcmp(str + start, sub, slen);
    return 0;

}

bool endswith(const std::string &str, const std::string &suffix, int start, int end) {
    int result = _string_tailmatch(str, suffix, start, end, +1);
    return static_cast<bool>(result);
}