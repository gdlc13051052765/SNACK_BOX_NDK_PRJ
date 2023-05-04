 
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include<sys/ioctl.h>
#include <setjmp.h>
#include <unistd.h>
 
#define CAM_DEV "/dev/video1"
// #define   WIDTH   640                       // 图片的宽度
// #define   HEIGHT  480                       // 图片的高度

#define   WIDTH   2000                       // 图片的宽度
#define   HEIGHT  2000                       // 图片的高度
#define  NB_BUFFER  4   //memory block number
struct pic_data 
{
    unsigned char *tmpbuffer[NB_BUFFER];
    unsigned int tmpbytesused[NB_BUFFER];
}pic;
 
int cam_fd;
 
/*==================================================================================
* 函 数 名： init_camera_dev
* 参    数： 
* 功能描述:  初始化摄像头
* 返 回 值： 创建成功返回0
* 备    注： 
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
int init_camera_dev(char *fdname)
{
    int i;
    int ret = 0;
	char buf[200]={0};

	//number = 2*number;
	/*1. 初始化摄像头设备*/
	sprintf(buf,"/dev/video%s",fdname);
    // 1、Open camera device+
    if((cam_fd = open(buf,O_RDWR)) == -1)
    {
        perror("ERROR opening V4L interface.");
        return -1;
    }
 
    // 2、Judge if the device is a camera device 
    struct v4l2_capability cam_cap;
    if(ioctl(cam_fd,VIDIOC_QUERYCAP,&cam_cap) == -1)
    {
        perror("Error opening device %s: unable to query device.");
        return -1;
    }
    if((cam_cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) 
    {
        perror("ERROR video capture not supported.");
        return -1;
    }
 
    // 3、Setting output parameter.
    struct v4l2_format v4l2_fmt;
    v4l2_fmt.type = V4L2_CAP_VIDEO_CAPTURE;
    v4l2_fmt.fmt.pix.width = WIDTH;
    v4l2_fmt.fmt.pix.height = HEIGHT;
    v4l2_fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    if (ioctl (cam_fd, VIDIOC_S_FMT, &v4l2_fmt) == -1) 
    {   
        perror("ERROR camera VIDIOC_S_FMT Failed.");
        return -1;
    }
    // 4、Check whether the parameters are set successfully 
    if (ioctl (cam_fd, VIDIOC_G_FMT, &v4l2_fmt) == -1) 
    {
        perror("ERROR camera VIDIOC_G_FMT Failed.");
        return -1;
    }
    if (v4l2_fmt.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG)
    {
        printf("Set VIDIOC_S_FMT sucessful\n");
    }
    // 5、Require buffer to store image data
    struct v4l2_requestbuffers v4l2_req;
    v4l2_req.count = NB_BUFFER;
    v4l2_req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_req.memory = V4L2_MEMORY_MMAP;
    if (ioctl (cam_fd, VIDIOC_REQBUFS, &v4l2_req) == -1) 
    {
        perror("ERROR camera VIDIOC_REQBUFS Failed.");
        return -1;
    } 
    // 6、Start memory map
    struct v4l2_buffer v4l2_buf;
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
      for(i = 0; i < NB_BUFFER; i++) 
   {
        v4l2_buf.index = i;
        if(ioctl(cam_fd, VIDIOC_QUERYBUF, &v4l2_buf) < 0)
        {
            perror("Unable to query buffer.");
            return -1;
        }
 
        pic.tmpbuffer[i] = (unsigned char*)mmap(NULL, v4l2_buf.length, PROT_READ, MAP_SHARED, cam_fd, v4l2_buf.m.offset);
        if(pic.tmpbuffer[i] == MAP_FAILED)
        {
             perror("Unable to map buffer.");
             return -1;
        }
        if(ioctl(cam_fd, VIDIOC_QBUF, &v4l2_buf) < 0)
        {
            perror("Unable to queue buffer.");
            return -1;
        }
   }
    //7、Open stream input 
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(cam_fd, VIDIOC_STREAMON, &type) < 0)
    {
        perror("Unable to start capture.");
        return -1;
    }
    return 0;
}
 
/*
Description.:Get a jpeg image and save.
*/

#define conter_pre    97
int get_camera_jpg(char *name,int cont)
{
    //8、Get a image 
    struct v4l2_buffer buff;
    buff.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buff.memory = V4L2_MEMORY_MMAP;
    if(ioctl(cam_fd, VIDIOC_DQBUF, &buff) < 0)
    {
        printf("camera VIDIOC_DQBUF Failed.\n");
        usleep(1000*1000);
        return -1;
    }
 
    pic.tmpbytesused[buff.index] = buff.bytesused;
    printf("size : %d\n",pic.tmpbytesused[buff.index]);  

    if(cont%conter_pre==0){
    //9、Save image.
        int jpg_fd = open(name,O_RDWR|O_CREAT,00700);
        if(jpg_fd == -1)
        {
            printf("open ipg Failed!\n ");
            return -1;      
        }
        int writesize = write(jpg_fd, pic.tmpbuffer[buff.index], pic.tmpbytesused[buff.index]);
        printf("Write successfully size : %d\n",writesize);
        close(jpg_fd);

    }         
 
    
    
    //10、Queue the buffers.
    if(ioctl(cam_fd, VIDIOC_QBUF, &buff) < 0)
    {
        printf("camera VIDIOC_QBUF Failed.");
        usleep(1000*1000);
        return -1;
    }
    return 0;
}
 
/*
Description.:Release resource
*/
int v4l2_close(void)
{
    // int i;
    // for(i=0; i<NB_BUFFER; i++)
    //     munmap(pic[0].tmpbuffer[i],pic[0].tmpbytesused[i]);
    // close(cam_fd);
    // return 0;
}
 
/*
Description.:main
*/
int mjpeg_test(char *fdname,char *jpegName)
{
    //v4l2_init(fdname);
   // while(1)
    {        
        get_camera_jpg(jpegName,1);
    }
   // v4l2_close();            
    return 0;
}