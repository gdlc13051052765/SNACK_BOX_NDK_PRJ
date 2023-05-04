#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/time.h>       // included for 'gettimeofday()'
#include <time.h>           // included for 'localtime()'
#include <setjmp.h>
#include <time.h>
#include <stdbool.h>
#include "quic/qrcodeApi.h"
#include "jpeg/video_app.h"
#include "sqlite3/sqliteTask.h"
#include "pca9535/pca9535.h"
#include "perDevice/perDevice.h"
#include "log/cLog.h"


/*
获得当前时间字符串
@param buffer [out]: 时间字符串
@return 空
*/
static void app_get_local_time(char* buffer)
{
	struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t sec = tv.tv_sec;
    suseconds_t usec = tv.tv_usec;

    struct tm *lt = localtime(&sec);

    printf("sys time %d-%02d-%02d %02d:%02d:%02d.%03d\n", 
            lt->tm_year + 1900,
            lt->tm_mon + 1,
            lt->tm_mday,
            lt->tm_hour,
            lt->tm_min,
            lt->tm_sec,
            (int)(usec / 1000));
	sprintf(buffer,"%d_%02d_%02d_%02d_%02d_%02d", 
            lt->tm_year + 1900,
            lt->tm_mon + 1,
            lt->tm_mday,
            lt->tm_hour,
            lt->tm_min,
            lt->tm_sec,sizeof(lt));
}

#define argument_count  camera_count+1
#define camera_count    5
int main(int argc, char **argv)
{
	char *tesult;
	int count = 0,i=0;;
	char name[256]={0};
	char cmd[256]={0};

	if(argc!=argument_count)
	{
		printf("parameter error");
		return 0;
	}
	//初始化摄像头
	init_camera_dev(argv[1]);
	init_camera_dev(argv[2]);
	init_camera_dev(argv[3]);
	init_camera_dev(argv[4]);
	init_camera_dev(argv[5]);

	// mjpeg_test();

	while(1)
	{
		count++;
		char now[32];
		memset(now, 0, sizeof(now));
		app_get_local_time(now);
		memset(name,0,sizeof(name));
		sprintf(name,"/data/meican/video%s/%s_%d.jpeg",argv[count%camera_count+1],now,count);
		printf("jpg name = %s\n",name);
		//通过摄像头获取图片
		get_camera_jpg(name,count);
		printf("camera test program\n");
		// for(i=0;i<3;i++)
		// {
		// 	//调用quirc识别采集的图像二维码
		// 	tesult = qrcode_recognition(name);
		// 	printf("qrcode tesult = %s\n",tesult);
		// 	if(!strcmp("meican.cop",tesult))
		// 	{	
		// 		memset(cmd,0,sizeof(cmd));
		// 		sprintf(cmd,"rm -rf %s",name);
		// 		printf("cmd =  %s\n",cmd);
		// 		printf("camera jpg ok\n");
		// 		system(cmd);
		// 		break;
		// 	} else {
		// 		printf("camera jpg error\n");
		// 	}
		// }
		//usleep(50000);//200ms
	}
	return 0;
}
