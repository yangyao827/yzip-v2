//
// Created by yangyao on 2022/4/10.
//

#include "verbose_info.h"
void show_detail(VerboseInfo verbose_info,bool is_show){
    if(is_show){
        printf("TotalSize: %s ==> %s\n",verbose_info.size_bef,verbose_info.size_cur);
        printf("Time consuming: %.4fs\n",verbose_info.duration);
    }
}