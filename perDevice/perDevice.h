#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdint.h>

/*==================================================================================
* 函 数 名： open_close_door_rfid_pair
* 参    数： 
* 功能描述:  true打开门rfid配对；false关闭门rfid配对
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
void open_close_door_rfid_pair(bool flag);

/*==================================================================================
* 函 数 名： peripheral_device_init
* 参    数： 
* 功能描述:  外设初始化
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int peripheral_device_init(void);
/*==================================================================================
* 函 数 名： open_box_door
* 参    数： 
* 功能描述:  打开设备门
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int open_box_door(void);
/*==================================================================================
* 函 数 名： close_box_door
* 参    数： 
* 功能描述:  关闭设备门
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int close_box_door(void);

/*==================================================================================
* 函 数 名： open_close_usb_device
* 参    数： num==usb设备号;flag==true打开设备
* 功能描述:  打开关闭USB
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int open_close_usb_device(uint8_t num, bool flag);

/*==================================================================================
* 函 数 名： open_close_led_device
* 参    数： num == led设备号;flag==true打开设备
* 功能描述:  打开关闭LED
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int open_close_led_device(uint8_t num, bool flag);