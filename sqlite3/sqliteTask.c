#include <stdio.h>  
#include <fcntl.h>  
#include <linux/ioctl.h>
#include "sqlite3.h"
#include "../debug.h"


static char *zErrMsg = NULL;
static char har_ver[] = "wr_01";
static char soft_ver[] = "v0.0.0";
static char soft_verbak[] = "v0.0.0";
static char shadow_name[] = "Thing_cardWriterV2_12467b16";
static char shadow_id[] = "12467b16";
static char main_m_name[] = "menu_lev1";
static char menu_type[] = "01";
static sqlite_int64  menu_ver = 1555572154506;
static sqlite_int64  new_menu_ver = 1555572154506;
static char address[] = "北京市朝阳区酒仙桥路美餐公司大食堂";
static int dev_status = 0;
static int menu_level = 4;
static int Indent_code = 215;
static char uuid[] = "";
static char sn[] = "";
static char JodID[] = "";
static char OtaResult[] = "ok";
static char v2id[] = "";
static sqlite_int64  restaurantid = 155;//餐厅ID

//基础配置数据库路径
#define base_config_path "/data/meican/base_config.db"

/*==================================================================================
* 函 数 名： sqlite_create_config_db
* 参    数： 
* 功能描述:  创建设备参数配置数据库
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int sqlite_create_config_db(void)
{
    int err;
    char tempdata[500];
    sqlite3 *config_db =NULL;

    /* 创建基础信息数据库 */
    err = sqlite3_open(base_config_path, &config_db);
    if( err ) {
        debug("Can't open database: %s\n", sqlite3_errmsg(config_db));
        sqlite3_close(config_db);
        return err;
    } 
    debug("You have opened a sqlite3 database named config_db successfully!\n");
    //硬件版本 har_ver 软件版本 soft_ver 字符编号 cha_encod 字库名称 font _name 主菜单表明 main_m_name 菜单类型版本 menu_type
    //菜单版本 menu_ver 食堂地址 address 设备状态 dev_status 菜单等级总数 menu_level 美餐识别码 Indent_code
    char *sql = "create table config (har_ver char, soft_ver char,shadow_name char,shadow_id char ,main_m_name char,\
    menu_type char,menu_ver INTEGER, new_menu_ver INTEGER, address char, dev_status INTEGER,menu_level INTEGER, \
    indent_code INTEGER,uuid char,sn char,v2id char,restaurantID INTEGER,softWare_bak char,jobId char,otaResult char);";

    sqlite3_exec(config_db,sql,NULL,NULL, &zErrMsg);
    sqlite3_close(config_db);
    //return err;   
    //插入数据
    err = sqlite3_open(base_config_path, &config_db);
    if( err ) {
        debug("Can't open database: %s\n", sqlite3_errmsg(config_db));
        sqlite3_close(config_db);
        return err;
    }
    sprintf(tempdata,"insert into config values('%s','%s','%s','%s','%s','%s',%lld,%lld,'%s',%d,%d,%d,'%s','%s',\
            '%s',%lld,'%s','%s','%s');",
            har_ver,soft_ver,shadow_name,shadow_id,main_m_name,menu_type,menu_ver,new_menu_ver,address,
            dev_status,menu_level,Indent_code,uuid,sn,v2id,restaurantid,soft_verbak,JodID,OtaResult);

    debug("config insertdata = %s\n", tempdata);
    err = sqlite3_exec(config_db,tempdata,NULL,NULL,&zErrMsg);
    sqlite3_close(config_db);
    return err;
}
