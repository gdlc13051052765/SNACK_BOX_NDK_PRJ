#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#include <math.h>

#include "pca9535/pca9535.h"
#include "perDevice.h"


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
* 创建时间： 2023-05-08
==================================================================================*/
void open_close_door_rfid_pair(bool flag)
{
    if(flag==true){
        pca9535_set_gpio_value(DEV_FRID_CTL_BIT, 0);
        sleep(4);
        pca9535_set_gpio_value(DEV_FRID_CTL_BIT, 1);
    }else{
        pca9535_set_gpio_value(DEV_FRID_CTL_BIT, 1);
    }
}

/*==================================================================================
* 函 数 名： peripheral_device_init
* 参    数： 
* 功能描述:  外设初始化
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2023-05-08
==================================================================================*/
int peripheral_device_init(void)
{
    //pca9535初始化
    return pca9535_init();
    //关闭门配对
    open_close_door_rfid_pair(false);    
}

/*==================================================================================
* 函 数 名： open_box_lock
* 参    数： 
* 功能描述:  打开设备锁
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2023-05-08
==================================================================================*/
int open_box_lock(void)
{
    return pca9535_set_gpio_value(DEV_DOOR_OPEN_BIT, 0);
}

/*==================================================================================
* 函 数 名： close_box_lock
* 参    数： 
* 功能描述:  关闭设备锁
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2023-05-08
==================================================================================*/
int close_box_lock(void)
{
    return pca9535_set_gpio_value(DEV_DOOR_OPEN_BIT, 1);
}

/*==================================================================================
* 函 数 名： get_hub_perdevice_status
* 参    数： 
* 功能描述:  获取当前HUB外设开关状态
* 返 回 值：
* 备    注： 个状态返回值
* 作    者： lc
* 创建时间： 2023-05-08
-----------------------------------------------------------------------------------
HUB0|HUB1|HUB2|HUB3|HUB4|HUB5|HUB6|HUB7|
-----------------------------------------------------------------------------------
    |    |    |    |    |    |    |    |  
-----------------------------------------------------------------------------------
==================================================================================*/
int get_hub_perdevice_status(void)
{
    int b[16]={0};
    int status = 0;
    uint8_t hub_status = 0;

    status = pca9535_get_gpio_status();

    for(int i=DEV_HUB0_BIT; i<=DEV_HUB7_BIT; i++)
    {
        b[i] = ((status & (unsigned char)pow(2, i)) >> i);
        hub_status = (hub_status | b[i]<<i);
        printf("b[%d] = %d\n", i, b[i]);
    }

    printf("return hub_status = %02x \n", hub_status);
    return hub_status;
}

/*==================================================================================
* 函 数 名： get_led_perdevice_status
* 参    数： 
* 功能描述:  获取当前LED外设开关状态
* 返 回 值：
* 备    注： 个状态返回值
* 作    者： lc
* 创建时间： 2023-05-08
-----------------------------------------------------------------------------------
LED0|LED1|LED2|LED3|LED4|LED5|LED6|LED7|   
-----------------------------------------------------------------------------------
    |    |    |    |    |    |    |    |  
-----------------------------------------------------------------------------------
==================================================================================*/
int get_led_perdevice_status(void)
{
    int b[16];
    int status = 0;
    uint8_t led_status = 0x00;

    status = pca9535_get_gpio_status();

    for(int i=DEV_LED0_BIT; i<=DEV_LED3_BIT; i++)
    {
        b[i] = ((status & (unsigned char)pow(2, i)) >> i);
        led_status = (led_status | b[i]<<(i-DEV_LED0_BIT));
        printf("b[%d] = %d\n", i, b[i]);
    }

    printf("return led_status = %02x \n", led_status);
    return led_status;
}

/*==================================================================================
* 函 数 名： get_door_perdevice_status
* 参    数： 
* 功能描述:  获取当前门外设开关状态
* 返 回 值：
* 备    注： 个状态返回值
* 作    者： lc
* 创建时间： 2023-05-08
-----------------------------------------------------------------------------------
DOOR0|DOOR1|DOOR2|DOOR3|DOOR4|DOOR5|DOOR6|DOOR7|  
-----------------------------------------------------------------------------------
     |     |     |     |     |     |     |     |   
-----------------------------------------------------------------------------------
==================================================================================*/
int get_door_perdevice_status(void)
{
    int b[16];
    int status = 0;
    uint8_t door_status = 0x00;

    status = pca9535_get_gpio_status();

    for(int i=DEV_DOOR_OPEN_BIT; i<=DEV_FRID_CTL_BIT; i++)
    {
        b[i] = ((status & (unsigned char)pow(2, i)) >> i);
        door_status = (door_status | b[i]<<(i-DEV_DOOR_OPEN_BIT));
        printf("b[%d] = %d\n", i, b[i]);
    }
    printf("return door_status = %02x\n", door_status);
    return door_status;
}

/*==================================================================================
* 函 数 名： get_lock_perdevice_status
* 参    数： 
* 功能描述:  获取门跟锁的状态
* 返 回 值： 0代表门打开
* 备    注： 个状态返回值
* 作    者： lc
* 创建时间： 2023-05-08
4.3.5:RFID 信号/门状态/锁舌状态在正常控制时的三者关系
对应关系
RFID 信号（门侦信号） 锁舌状态（锁侦信号） 门状态
有 有 关好
无 有 未关好
有 无 未关好
无 无 未关
==================================================================================*/
int get_lock_perdevice_status(void)
{
    int status = 0;

    status = pca9535_get_gpio_status();
    if(status & (0x0001<<(DEV_GATE_IN_BIT-1)))
    {
        printf("door close\n");
    } else {
        printf("door open\n");
    }
    printf("door status: %d, lock status: %d\n", status, status&(0x0001<<(DEV_GATE_IN_BIT-1)));

    return status&(0x0001<<(DEV_GATE_IN_BIT-1));
} 

/*==================================================================================
* 函 数 名： get_all_perdevice_status
* 参    数： 
* 功能描述:  获取所有外设的设备状态
* 返 回 值：
* 备    注： 个状态返回值
* 作    者： lc
* 创建时间： 2023-05-08
-----------------------------------------------------------------------------------*/
struct devStatusRes get_all_perdevice_status(void)
{
    struct devStatusRes devStr;
    int status = 0;

    status = get_hub_perdevice_status();
    for(int i = 0; i < MAX_HUB_NUM ;i++)
    {
        devStr.hub_status[i] = status &((0x01<<i));
    }
    status = get_led_perdevice_status();
    for(int i = 0; i < MAX_LED_NUM ;i++)
    {
        devStr.led_status[i] = status &((0x01<<i));
    }
    status = get_door_perdevice_status();
    for(int i = 0; i < MAX_BOX_DOOR_NUM ;i++)
    {
        devStr.door_status[i] = status &((0x01<<i));
    }
    status = get_lock_perdevice_status();
    for(int i = 0; i < MAX_BOX_DOOR_NUM ;i++)
    {
        devStr.door_status[i] = status &((0x01<<i));
    }

    return devStr;
}

/*==================================================================================
* 函 数 名： open_close_usb_device
* 参    数： num==usb设备号;flag==true打开设备
* 功能描述:  打开关闭USB
* 返 回 值：
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2023-05-08
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
* 创建时间： 2023-05-08
==================================================================================*/
int open_close_led_device(uint8_t num, bool flag)
{
    uint8_t buf[MAX_LED_NUM] = {DEV_LED0_BIT, DEV_LED1_BIT, DEV_LED2_BIT, DEV_LED3_BIT};

    if(flag == true)
    {
        return pca9535_set_gpio_value(buf[num], 1);
    } else {
        return pca9535_set_gpio_value(buf[num], 0);
    }
}
