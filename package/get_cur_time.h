//
// Created by yangyao on 2022/4/5.
//

#ifndef PACK_GET_CUR_TIME_H
#define PACK_GET_CUR_TIME_H
#include "time.h"
#include "stdio.h"
#define TIME_LEN 17
typedef long time_t; /* time value */
#define _TIME_T_DEFINED /* avoid multiple def's of time_t */
void get_cur_time(char mtime[TIME_LEN]);
#endif //PACK_GET_CUR_TIME_H
