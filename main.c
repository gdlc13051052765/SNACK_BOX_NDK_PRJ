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


#define argument_count  camera_count+1
#define camera_count    5 //摄像头数量
int camera_fd[camera_count+1]={0};



int main(int argc, char **argv)
{
	char *tesult;
	int count = 0,i=0;;
	char name[256]={0};
	char cmd[256]={0};
	int temp_fd = -1;

	if(argc!=argument_count)
	{
		printf("parameter error");
		return 0;
	}
	//初始化摄像头
	for(int i=1;i<=camera_count;i++)
	{
		camera_fd[i] = init_camera_dev(i,argv[i]);
		if(camera_fd[i]>0)
		{
			printf("open camera %d camera_fd[%d] fd = %d ok\n",i,i,camera_fd[i]);
		} else {
			printf("open %s err\n",argv[i]);
		}
	}
	
	// mjpeg_test();

	while(1)
	{
		
		char now[32];
		memset(now, 0, sizeof(now));
		app_get_local_time(now);
		// memset(name,0,sizeof(name));
		// sprintf(name,"/data/meican/video%s/%s_%d.jpeg",argv[count%camera_count+1],now,count);
		// printf("jpg name = %s\n",name);
		//通过摄像头获取图片
		for(int i=1;i<=camera_count;i++)
		{
			if(open_stream_dev(i,camera_fd[i])==0)
			{
				count++;
				memset(name,0,sizeof(name));
				sprintf(name,"/data/meican/video%s/%s_%d.jpeg",argv[count%camera_count+1],now,count);
				printf("jpg name = %s\n",name);
				printf("open camera %d temp_fd = %d ok\n",i,camera_fd[i]);
				get_camera_jpg(camera_fd[i],i,name,count);
				//v4l2_close(camera_fd[i]);
			} 
			else {	
				printf("open %s err\n",argv[i]);
				//v4l2_close(camera_fd[i]);
			}	
			//sleep(1);
		}
		
		//get_camera_jpg(camera_fd[count%camera_count+1],name,count);
		// printf("camera test program\n");
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
