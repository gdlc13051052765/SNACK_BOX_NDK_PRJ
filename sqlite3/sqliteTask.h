#include <stdio.h>  
#include <fcntl.h>  
#include <linux/ioctl.h>
#include "sqlite3.h"

/*==================================================================================
* 函 数 名： sqlite_create_config_db
* 参    数： 
* 功能描述:  创建设备参数配置数据库
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int sqlite_create_config_db(void);