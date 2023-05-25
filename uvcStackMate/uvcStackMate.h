#ifndef UVCSTACKMATE_H
#define UVCSTACKMATE_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


//uvc摄像头层格匹配关系结构体
struct uvcStackMateStr
{
    int hub_num;//hub板上的hub号
    int box_num;//零食柜层格号
    char pid[128];//uvc摄像头pid号
};

/*==================================================================================
* 函 数 名： uvc_camera_snck_mate
* 参    数：
* 功能描述:  UVC 摄像头 层叠匹配操作
* 返 回 值： 
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
int uvc_camera_snck_mate(void);


#endif 		//_SWITCH_CONFIG_H__
