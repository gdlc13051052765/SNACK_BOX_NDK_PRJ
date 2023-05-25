#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../cutString/cutString.h"

#define FILE_NAME 	"/sys/class/video4linux/video0/device/modalias"
#define BUF_LEN		1024

//uvc摄像头vid 型号
static char uvc_vid[] = "1BCF";

/*==================================================================================
* 函 数 名： read_uvc_camera_pid
* 参    数：
* 功能描述:  读取uvccamera pid
* 返 回 值： -1 没有此文件代表次usb端口没有摄像头,正常读取返回pid（int）
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
char * read_uvc_camera_pid(void)
{
	int fd = -1;
	int rv = -1;
	char buf[BUF_LEN]={0};
    char *newBuf;
    char tempBuf[50] = {0};
    
	memset(buf, 0, sizeof(buf));
	//创建hello.txt文件
	if ((fd = open(FILE_NAME, O_RDONLY)) < 0)
	{
		printf("Open file %s failure: %s \n", FILE_NAME, strerror(errno));
       // memcpy(newBuf,tempBuf,sizeof(tempBuf));
		return tempBuf;
	}
	printf("Open file %s fd[%d] successfully\n", FILE_NAME, fd);
	
	//从hello.txt中读数据
	lseek(fd, 0, SEEK_SET);
	if ((rv = read(fd, buf, sizeof(buf))) < 0)
	{
		printf("Read data from fd[%d] failure: %s\n", fd, strerror(errno));
		goto cleanup;
	}
    // usb:v1BCFp1001d0001dcEFdsc02dp01ic0Eisc01ip00in00
	printf("Read %luB data from fd[%d]: %s\n", strlen(buf), fd, buf);
    newBuf = cutString_before_num(buf,14) ;
    printf("newBuf = %s\n", newBuf);
    newBuf = cutString_behind_num(newBuf,4);
    memcpy(tempBuf,newBuf,strlen(newBuf));
    printf("uvc pid  = %s\n", tempBuf);
	
cleanup:
	close(fd);

	return tempBuf;
}

