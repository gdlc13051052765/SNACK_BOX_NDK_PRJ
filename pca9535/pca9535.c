#include <stdio.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

#define PCA9535_I2C_ADDR 0x25
int i2c_fd = -1;

/*==================================================================================
* 函 数 名： pca9535_init
* 参    数： 
* 功能描述:  pca9535初始化
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2023-05-08
==================================================================================*/
int pca9535_init(void)
{
    uint8_t buffer[2];

    i2c_fd = open("/dev/i2c-3", O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open I2C device");
        return -1;
    }
    
    if (ioctl(i2c_fd, I2C_SLAVE, PCA9535_I2C_ADDR) < 0) {
        perror("Failed to set I2C slave address");
        close(i2c_fd);
        return -1;
    }
    perror("open iic ok \n");

    //默认配置成输出输入兼容模式
    buffer[0] = 0x06; // PCA9535 register address for output port 0
    buffer[1] = 0x00; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }
    buffer[0] = 0x07; // PCA9535 register address for output port 0
    buffer[1] = 0x00; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }

    // Set output values to 0xff
    buffer[0] = 0x02; // PCA9535 register address for output port 0
    buffer[1] = 0xff; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }
    buffer[0] = 0x03; // PCA9535 register address for output port 0
    buffer[1] = 0xff; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }

    return 0;
}

/*==================================================================================
* 函 数 名： pca9535_get_gpio_status
* 参    数： 
* 功能描述:  获取pca9535 gpio电平状态
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2023-05-08
==================================================================================*/
int pca9535_get_gpio_status(void)
{
    int status = 0;
    uint8_t buffer[2];

    // Read input values
    buffer[0] = 0x00; // PCA9535 register address for input port 0
    if (write(i2c_fd, buffer, 1) != 1) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }
    if (read(i2c_fd, buffer, 1) != 1) {
        perror("Failed to read from PCA9535");
        close(i2c_fd);
        return -1;
    }
    status = buffer[0];
    printf("Input port0 values: 0x%02x\n", buffer[0]);

    // Read input values
    buffer[0] = 0x01; // PCA9535 register address for input port 0
    if (write(i2c_fd, buffer, 1) != 1) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }
    if (read(i2c_fd, buffer, 1) != 1) {
        perror("Failed to read from PCA9535");
        close(i2c_fd);
        return -1;
    }
    status |= ((int)buffer[0])<<8;
    printf("Input port1 values: 0x%02x\n", buffer[0]);
    printf("gpio_status = %d\n", status);
    return status;
}

/*==================================================================================
* 函 数 名： pca9535_set_gpio_value
* 参    数： num 第几个脚(0-15),value,该脚的电平状态0或者1
* 功能描述:  获取pca9535 gpio电平状态
* 返 回 值： None
* 备    注： 创建成功返回0
* 作    者： lc
* 创建时间： 2023-05-08
==================================================================================*/
int pca9535_set_gpio_value(uint8_t num, uint8_t val)
{
    uint8_t buffer[2];
    int value = 0;
    int gpio_status = 0;
    uint8_t port_num = 0x02;

    gpio_status = pca9535_get_gpio_status();
    //条件判断0x03==port1;0x02==port0
   // buffer[0] = (num>7)?0x03:0x02;
    if(val>0)
    {
        gpio_status |= (1<<num);
    } else {
        gpio_status &= (~(0x0001<<num));
    }
    printf("gpio set value = %04x\n", gpio_status);
    // printf("buffer[0] = %02x set value = %d\n", buffer[0],value);
    buffer[0] = 0x02; // PCA9535 register address for output port 0
    buffer[1] = gpio_status; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    } 
    buffer[0] = 0x03; // PCA9535 register address for output port 0
    buffer[1] = gpio_status>>8; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    } 
}

int pca9535_test(void)
{
    int i2c_fd;
    uint8_t buffer[2];
    uint8_t gpio_value = 0;
    bool flag = false;
    
    i2c_fd = open("/dev/i2c-3", O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open I2C device");
        return -1;
    }
    
    if (ioctl(i2c_fd, I2C_SLAVE, PCA9535_I2C_ADDR) < 0) {
        perror("Failed to set I2C slave address");
        close(i2c_fd);
        return -1;
    }
    
    perror("open iic ok \n");

    buffer[0] = 0x06; // PCA9535 register address for output port 0
    buffer[1] = 0x00; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }

    buffer[0] = 0x07; // PCA9535 register address for output port 0
    buffer[1] = 0x00; // Output value
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("Failed to write to PCA9535");
        close(i2c_fd);
        return -1;
    }

    while(1)
    {
        if(flag == false)
        {
            flag = true;
            gpio_value = 0;
        } else {
            flag = false;
            gpio_value = 0xff;
        }
            
        // Set output values to 0x55
        buffer[0] = 0x02; // PCA9535 register address for output port 0
        buffer[1] = gpio_value; // Output value
        if (write(i2c_fd, buffer, 2) != 2) {
            perror("Failed to write to PCA9535");
            close(i2c_fd);
            return -1;
        }
        buffer[0] = 0x03; // PCA9535 register address for output port 0
        buffer[1] = gpio_value; // Output value
        if (write(i2c_fd, buffer, 2) != 2) {
            perror("Failed to write to PCA9535");
            close(i2c_fd);
            return -1;
        }

        // Read input values
        buffer[0] = 0x00; // PCA9535 register address for input port 0
        if (write(i2c_fd, buffer, 1) != 1) {
            perror("Failed to write to PCA9535");
            close(i2c_fd);
            return -1;
        }
        if (read(i2c_fd, buffer, 1) != 1) {
            perror("Failed to read from PCA9535");
            close(i2c_fd);
            return -1;
        }
        printf("Input port0 values: 0x%02x\n", buffer[0]);

        // Read input values
        buffer[0] = 0x01; // PCA9535 register address for input port 0
        if (write(i2c_fd, buffer, 1) != 1) {
            perror("Failed to write to PCA9535");
            close(i2c_fd);
            return -1;
        }
        if (read(i2c_fd, buffer, 1) != 1) {
            perror("Failed to read from PCA9535");
            close(i2c_fd);
            return -1;
        }
        printf("Input port1 values: 0x%02x\n", buffer[0]);

        sleep(2);
    } 
    close(i2c_fd);
    return 0;
}
