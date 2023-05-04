#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdint.h>

/*==================================================================================
* 函 数 名： pca9535_init
* 参    数： 
* 功能描述:  pca9535初始化
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int pca9535_init(void);
/*==================================================================================
* 函 数 名： pca9535_get_gpio_status
* 参    数： 
* 功能描述:  获取pca9535 gpio电平状态
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int pca9535_get_gpio_status(void);

/*==================================================================================
* 函 数 名： pca9535_set_gpio_value
* 参    数： num 第几个脚(0-15),value,该脚的电平状态0或者1
* 功能描述:  获取pca9535 gpio电平状态
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2021-05-25
==================================================================================*/
int pca9535_set_gpio_value(uint8_t num, uint8_t val);

int pca9535_test(void);

