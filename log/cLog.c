
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>		// linux下头文件
#endif
 
#define FILE_MAX_SIZE (1024*1024)
 
/*
获得当前时间字符串
@param buffer [out]: 时间字符串
@return 空
*/
void get_local_time(char* buffer)
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
 
/*
获得文件大小
@param filename [in]: 文件名
@return 文件大小
*/
long get_file_size(char* filename)
{
	long length = 0;
	FILE *fp = NULL;
 
	fp = fopen(filename, "rb");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
	}
 
	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
 
	return length;
}
 
/*
写入日志文件
@param filename [in]: 日志文件名
@param max_size [in]: 日志文件大小限制
@param buffer [in]: 日志内容
@param buf_size [in]: 日志内容大小
@return 空
*/
void write_log_file(char* filename, long max_size, char* buffer, unsigned buf_size)
{
	if (filename != NULL && buffer != NULL)
	{
		// 文件超过最大限制, 删除
		long length = get_file_size(filename);
 
		if (length > max_size)
		{
			unlink(filename); // 删除文件
		}
 
		// 写日志
		{
			FILE *fp;
			fp = fopen(filename, "at+");
			if (fp != NULL)
			{
				char now[32];
				memset(now, 0, sizeof(now));
				get_local_time(now);
				fwrite(now, strlen(now)+1, 1, fp);
				fwrite(buffer, buf_size, 1, fp);
 
				fclose(fp);
				fp = NULL;
			}
		}
	}
}

/*==================================================================================
* 函 数 名： LogWrite
* 参    数： filename==log文件名字；buffer==写入内容
* 功能描述:  日志写入
* 返 回 值： 创建成功返回0
* 备    注： 
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
void app_Log_Write(char* filename, char* buffer)
{
    write_log_file(filename, FILE_MAX_SIZE, buffer, strlen(buffer));
}

int write_log_test(void)
{
	int i;
	for (i=0; i<10; ++i)
	{
		char buffer[32];
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "====> %s\n", "write_log_test\n");
		app_Log_Write("log.txt",  buffer);
        usleep(500000);	// 500m秒
	}
	// system("pause");
	return 0;
}
