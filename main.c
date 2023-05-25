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

#include "sqlite3/sqliteTask.h"
#include "pca9535/pca9535.h"
#include "perDevice/perDevice.h"
#include "log/cLog.h"
#include "cAppTask/cAppTask.h"
#include "wrRefile/readTxt.h"
#include "uvcStackMate/uvcStackMate.h"

int main(int argc, char **argv)
{
	//创建uvc匹配关系数据库
	//sqlite_create_uvc_snck_mate_db();

	//初始化设备外设
	peripheral_device_init();

	uvc_camera_snck_mate();
	//创建线程任务
	app_bufei_create_thread();

	return 0;
}
