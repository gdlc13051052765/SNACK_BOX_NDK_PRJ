/*****************************************************************************************
 * 文件说明：
 * C线程任务管理
 * 为了减少CPU占有率，每个任务里面加入相应的延时
 *****************************************************************************************/

#include <stdio.h>  
#include <stdbool.h>  
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

#include "../udpSocket/udpsocket.h"
#include "../json/cJSON.h"
#include "quic/qrcodeApi.h"
#include "jpeg/video_app.h"

static pthread_t thread[10];  //两个线程

#define TASK_DELAY   1000 //线程间延时

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

/*==================================================================================
* 函 数 名： udp_socket_thread
* 参    数： Non
* 功能描述:  udp socket 任务
* 返 回 值：
* 备    注： 
* 作    者： lc
* 创建时间： 2021/4/16
==================================================================================*/
static void *udp_socket_thread(void *args) 
{
	int status = -1;

    if(!make_udpsocket_connect())
    {
        printf("udp socket make ok\n");
    }

	while(1) 
	{
		udpsocket_receive_data();
		//usleep(50000);
	}
}

/*==================================================================================
* 函 数 名： mjpg_qrcode_thread
* 参    数： Non
* 功能描述:  从uvc摄像头获取mjpg图片 线程任务
* 返 回 值：
* 备    注： 
* 作    者： lc
* 创建时间： 2021/4/16
==================================================================================*/
static void *mjpg_qrcode_thread(void *args) 
{
	char *tesult;
	int count = 0,i=0;;
	char name[256]={0};
	char cmd[256]={0};

	//初始化摄像头
	//init_camera_dev(argv[1]);
	while(1)
	{
		count++;
		char now[32];
		memset(now, 0, sizeof(now));
		app_get_local_time(now);
		memset(name,0,sizeof(name));
		//sprintf(name,"/data/meican/video%s/%s_%d.jpeg",argv[1],now,count);
		printf("jpg name = %s\n",name);
		//通过摄像头获取图片
		get_camera_jpg(name);
		printf("camera test program\n");
		for(i=0;i<3;i++)
		{
			//调用quirc识别采集的图像二维码
			tesult = qrcode_recognition(name);
			printf("qrcode tesult = %s\n",tesult);
			if(!strcmp("meican.cop",tesult))
			{	
				memset(cmd,0,sizeof(cmd));
				sprintf(cmd,"rm -rf %s",name);
				printf("cmd =  %s\n",cmd);
				printf("camera jpg ok\n");
				system(cmd);
				break;
			} else {
				printf("camera jpg error\n");
			}
		}
		usleep(50000);//200ms
	}
}

/*==================================================================================
* 函 数 名： time_check_door_lock_status_thread
* 参    数： Non
* 功能描述:  定时检测门跟锁的信号检测任务
* 返 回 值：
* 备    注： 
* 作    者： lc
* 创建时间： 2021/4/16
==================================================================================*/
static void *time_check_door_lock_status_thread(void *args) 
{
	while(1)
	{
		get_hub_perdevice_status();
		get_led_perdevice_status();
		get_door_perdevice_status();
		sleep(1);
	}
}

/*==================================================================================
* 函 数 名： app_wrplate_create_thread
* 参    数： Non
* 功能描述:  创建多线程任务
* 返 回 值：
* 备    注： 
* 作    者： lc
* 创建时间： 2021/5/6
==================================================================================*/
void app_bufei_create_thread(void)
{
	int temp;
	memset(&thread, 0, sizeof(thread));          

	if((temp = pthread_create(&thread[1], NULL, udp_socket_thread, NULL)) != 0)//udp socket线程任务
		printf("thread 1 make fail\n");
	else
		printf("thread 1 make ok\n");

		
	// if((temp = pthread_create(&thread[2], NULL, mjpg_qrcode_thread, NULL)) != 0)//从uvc摄像头获取mjpg图片 线程任务
	// 	printf("thread 1 make fail\n");
	// else
	// 	printf("thread 1 make ok\n");

	// if((temp = pthread_create(&thread[3], NULL, time_check_door_lock_status_thread, NULL)) != 0)//定时检测门锁信号线程任务
	// 	printf("thread 3 make fail\n");
	// else
	// 	printf("thread 3 make ok\n");

	pthread_join(thread[1],NULL);
	pthread_join(thread[3],NULL);
}
