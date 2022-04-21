//
// Created by yangyao on 2022/4/7.
//
#include "is_file_exist.h"
static struct termios stored_settings;
void set_keypress()
{
    termios new_settings{};
    tcgetattr(0,&stored_settings);    // 存储旧的模式
    new_settings = stored_settings;
    /* Disable canonical mode, and set buffer size to 1 byte */
    new_settings.c_lflag &= (~ICANON)&(~ECHO); //~ICANON:取消行缓冲，～ECHO:禁用输入回显示
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(0,TCSANOW,&new_settings);
}
void reset_keypress()
{
    tcsetattr(0,TCSANOW,&stored_settings); // 恢复旧模式
}


bool is_overwrite(const char *out_file){//true：存在，但是不删（不覆盖），false：不存在或者存在且删除（覆盖）
    //判断输出文件是否存在，以及时候具有写权限
    if (access(out_file, F_OK) == 0) {//输出文件已经存在
        char c;
        printf("The %s already exists, do you want to overwrite it?(y or n)",out_file);
        set_keypress();
        while ((c=getchar())!= EOF) {
            if (c == 'n') {
                reset_keypress();
                printf("\n");
                return true;
            }
            if (c == 'y') {
                remove(out_file);//删除输出文件
                reset_keypress();
                printf("\n");
                return false;
            }
        }
    }
    return false;//不存在
}