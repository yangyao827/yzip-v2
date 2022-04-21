//
// Created by yangyao on 2022/4/9.
//

#include <cstring>
#include "size_h.h"
FileSizeHumanReadable size_h(const char * size_b){
    FileSizeHumanReadable file_info{};
    if(atol(size_b)<=(1<<10)){//1KB以下，不转换
        strcpy(file_info.size_h,size_b);
        strcpy(file_info.unit,"B");
        return file_info;
    }
    if(atol(size_b)<=(1<<20)){//1K-1MB之间，转换为K
        sprintf(file_info.size_h,"%.1f",(double)atol(size_b)/(1<<10));
        strcpy(file_info.unit,"K");
        return file_info;
    }
    if(atol(size_b)<=(1<<30)){//1M-1GB之间，转换为M
        sprintf(file_info.size_h,"%.1f",(double)atol(size_b)/(1<<20));
        strcpy(file_info.unit,"M");
        return file_info;
    } else{//1G-1T之间，转换为G
        sprintf(file_info.size_h,"%.1f",(double)atol(size_b)/(1<<30));
        strcpy(file_info.unit,"G");
        return file_info;
    }
}
