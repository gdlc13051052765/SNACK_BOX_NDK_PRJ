#ifndef VIDEO_APP_H
#define VIDEO_APP_H

#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <poll.h>
#include <stdlib.h>
#include "yuv_to_jpg.h"


/*==================================================================================
* 函 数 名： init_camera_dev
* 参    数： 
* 功能描述:  初始化摄像头
* 返 回 值： 创建成功返回0
* 备    注： 
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
int init_camera_dev(char *fdname);

/*==================================================================================
* 函 数 名： get_jpg
* 参    数： 获取摄像头图片
* 功能描述:  jpg图片识别二维码
* 返 回 值： 创建成功返回0
* 备    注： 
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
int get_camera_jpg(char* name);

int mjpeg_test(void);

#endif