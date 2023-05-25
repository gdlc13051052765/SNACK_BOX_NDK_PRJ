/*UVC 摄像头 层叠匹配操作
1)依次开关某个摄像头hub，查询此摄像头文件是否存在来判断此端口是否有UVC摄像头接入
2）有摄像头存在，通过识别二维码来判断自己再哪个层格
*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../perDevice/perDevice.h"
#include "uvcStackMate.h"

/*==================================================================================
* 函 数 名： uvc_camera_snck_mate
* 参    数：
* 功能描述:  UVC 摄像头 层叠匹配操作
* 返 回 值： 
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
int uvc_camera_snck_mate(void)
{
	int i =0;
	char *pid_num;
	struct uvcStackMateStr pStr;

	//先关闭所有的HUB供电
	for(i=0;i<MAX_HUB_NUM;i++)
	{
		open_close_usb_device(i, false);
	}
	sleep(2);
	for(i=0;i<MAX_HUB_NUM;i++)
	{
		open_close_usb_device(i, true);//依次打开某个HUB供电
		sleep(2);
		pid_num = read_uvc_camera_pid();
		if(strlen(pid_num)==0)
		{
			printf("hub %d no uvc camera\n", i);
		} else {//此HUB有摄像头接入
			printf("hub %d have uvc camera\n", i);
			//摄像头抓取二维码照片并识别层格号

			//二维码层格号识别完成写入配置数据库
			pStr.hub_num = i;
			pStr.box_num = 4;
			sprintf(pStr.pid,"%s",pid_num);
			sqlite_insert_uvc_snck_mate_db(pStr);
		}
		//关闭所有的HUB供电
		for(int j=0;j<MAX_HUB_NUM;j++)
		{
			open_close_usb_device(j, false);
		}
	}

	
}

