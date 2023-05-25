#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <stdio.h>  
#include <fcntl.h>  
#include <string.h>

/*==================================================================================
* 函 数 名： make_udpsocket_connect
* 参    数：
* 功能描述:  工厂检测数据分析
* 返 回 值：创建成功返回0；
* 备    注： None
* 作    者： lc
* 创建时间： 2021/08/31
==================================================================================*/
int make_udpsocket_connect(void);

/*==================================================================================
* 函 数 名： udpsocket_receive_data
* 参    数：
* 功能描述:  udp socket 数据接收
* 返 回 值：创建成功返回0；
* 备    注： None
* 作    者： lc
* 创建时间： 2021/08/31
==================================================================================*/
void udpsocket_receive_data(void);

/*==================================================================================
* 函 数 名： udpsocket_send_data
* 参    数：
* 功能描述:  udp socket 发送数据
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
int udpsocket_send_data(char *data);

/*==================================================================================
* 函 数 名： udpsocket_set_weigh_uuid_data
* 参    数：
* 功能描述:  udp socket 设置当前重量用户卡信息
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
void udpsocket_set_weigh_uuid_data(char *data);

#endif
