//
// Created by yangyao on 2022/4/5.
//

#include "mode_to_letters.h"
void mode_to_letters( int mode, char str[] )
{
    strcpy( str, "----------" ); /* default=no perms */

    if ( S_ISDIR(mode) ) str[0] = 'd'; /* directory? */
    if ( S_ISCHR(mode) ) str[0] = 'c'; /* char devices */
    if ( S_ISBLK(mode) ) str[0] = 'b'; /* block device */

    if ( mode & S_IRUSR ) str[1] = 'r'; /* 3 bits for user */
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( (mode & S_ISUID) && (mode & S_IXUSR) )
        str[3] = 's';
    else if ( (mode & S_ISUID) && !(mode & S_IXUSR) )
        str[3] = 'S';
    else if ( mode & S_IXUSR )
        str[3] = 'x';

    if ( mode & S_IRGRP ) str[4] = 'r'; /* 3 bits for group */
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( (mode & S_ISGID) && (mode & S_IXGRP) )
        str[6] = 's';
    else if ( (mode & S_ISGID) && !(mode & S_IXGRP) )
        str[6] = 'S';
    else if ( mode & S_IXGRP )
        str[6] = 'x';

    if ( mode & S_IROTH ) str[7] = 'r'; /* 3 bits for other */
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( (mode & S_ISVTX) && (mode & S_IXOTH) )
        str[9] = 't';
    else if ( (mode & S_ISVTX) && !(mode & S_IXOTH) )
        str[9] = 'T';
    else if ( mode & S_IXOTH )
        str[9] = 'x';
}