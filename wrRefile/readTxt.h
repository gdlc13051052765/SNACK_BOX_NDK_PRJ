#ifndef READTXT_H
#define READTXT_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


/*==================================================================================
* 函 数 名： read_uvc_camera_pid
* 参    数：
* 功能描述:  读取uvccamera pid
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
char *  read_uvc_camera_pid(void);

#endif
