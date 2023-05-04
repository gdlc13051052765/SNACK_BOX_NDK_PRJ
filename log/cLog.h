#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

/*
获得当前时间字符串
@param buffer [out]: 时间字符串
@return 空
*/
void get_local_time(char* buffer);
/*==================================================================================
* 函 数 名： LogWrite
* 参    数： filename==log文件名字；buffer==写入内容
* 功能描述:  日志写入
* 返 回 值： 创建成功返回0
* 备    注： 
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
void app_Log_Write(char* filename, char* buffer);
int write_log_test(void);

#endif /* __LOG_H__ */