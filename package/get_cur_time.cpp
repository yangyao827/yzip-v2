//
// Created by yangyao on 2022/4/5.
//

#include "get_cur_time.h"

void get_cur_time(char mtime[TIME_LEN]) {
    time_t tmpcal_ptr;
    struct tm *tmp_ptr = NULL;
    time(&tmpcal_ptr);
    tmp_ptr = localtime(&tmpcal_ptr);
    sprintf(mtime,"%d-%d-%d %02d:%02d", (1900 + tmp_ptr->tm_year), (1 + tmp_ptr->tm_mon),\
    tmp_ptr->tm_mday,tmp_ptr->tm_hour,tmp_ptr->tm_min);
    //printf("%s\n",mtime);
}