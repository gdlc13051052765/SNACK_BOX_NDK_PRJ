#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include "pca9535/pca9535.h"

#define DOOR_NUM        8  //门开关对应的PCA9535的序号
#define RFID_PAIR_NUM   11 //rfid门配对PCA9535的序号

#define MAX_LED_NUM     4   //LED个数
#define LED1_NUM        12
#define LED2_NUM        13
#define LED3_NUM        14
#define LED4_NUM        15

/*==================================================================================
* 函 数 名： open_close_door_rfid_pair
* 参    数： 
* 功能描述:  true打开门rfid配对；false关闭门rfid配对
智能锁接通电源，通过拉低智能锁中的 RFID 卡配码线保持 3s 以上，让智能锁进入 RFID
卡配码状态。进入配码状态时，蓝色指示灯快速闪烁，此时拉高或悬空配码线，将 RFID 卡
放置在智能锁的 RFID 卡感应区即可实现配码。配码成功，蓝色指示灯停止闪烁，表示成功
配码
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
void open_close_door_rfid_pair(bool flag)
{
    if(flag==true){

        pca9535_set_gpio_value(RFID_PAIR_NUM, 0);
        sleep(4);
        pca9535_set_gpio_value(RFID_PAIR_NUM, 1);
    }else{
        pca9535_set_gpio_value(RFID_PAIR_NUM, 1);
    }
}

/*==================================================================================
* 函 数 名： peripheral_device_init
* 参    数： 
* 功能描述:  外设初始化
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int peripheral_device_init(void)
{
    //pca9535初始化
    return pca9535_init();
    //关闭门配对
    open_close_door_rfid_pair(false);    
}

/*==================================================================================
* 函 数 名： open_box_door
* 参    数： 
* 功能描述:  打开设备门
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int open_box_door(void)
{
    return pca9535_set_gpio_value(DOOR_NUM, 0);
}
/*==================================================================================
* 函 数 名： close_box_door
* 参    数： 
* 功能描述:  关闭设备门
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int close_box_door(void)
{
    return pca9535_set_gpio_value(DOOR_NUM, 1);
}

/*==================================================================================
* 函 数 名： open_close_usb_device
* 参    数： num==usb设备号;flag==true打开设备
* 功能描述:  打开关闭USB
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int open_close_usb_device(uint8_t num, bool flag)
{
    if(flag == true)
       return pca9535_set_gpio_value(num, 1);
    else
       return pca9535_set_gpio_value(num, 0);
}

/*==================================================================================
* 函 数 名： open_close_led_device
* 参    数： num == led设备号;flag==true打开设备
* 功能描述:  打开关闭led
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int open_close_led_device(uint8_t num, bool flag)
{
    uint8_t buf[MAX_LED_NUM] = {LED1_NUM, LED2_NUM, LED3_NUM, LED4_NUM};

    if(flag == true)
    {
        return pca9535_set_gpio_value(buf[num], 1);
    } else {
        return pca9535_set_gpio_value(buf[num], 0);
    }
}
