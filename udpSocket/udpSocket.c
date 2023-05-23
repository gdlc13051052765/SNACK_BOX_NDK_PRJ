#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include "../debug.h"
#include "udpSocket.h"
#include "../json/cJSON.h"

#include "../perDevice/perDevice.h"

//udp命令列表 利用enum实现switch case 字符串功能
enum choice {getDevStatus, setUvcCamera, setled, setLock, adapterLock, getDoorStatus, getLockStatus, ota, endCmd};
const char *choices[] = {"getDevStatus", "setUvcCamera", "setled", "setLock", "adapterLock", "getDoorStatus", \
                         "getLockStatus", "ota", "endCmd"};

#define RCV_PORT 6666//接收端口
#define SEN_PORT 8888//发送端口
#define MAXDATASIZE 2048  
#define SERVER_IP "127.0.0.1" 
//#define SERVER_IP "192.168.31.182" 
int javaSocketFd, numbytes;  
struct sockaddr_in server_addr; 
struct sockaddr_in cliaddr;
char ipbuf[64];
int len = sizeof(cliaddr);
char udpRcvBuf[1024]={};
char udpSendBuf[1024]={};//udp发送数据缓存

//安卓 udp 命令解析
static void udpsocket_data_processing(char *data);

//创建UDP——CLIENT跟JAVA UDP——server通讯；利用本地回环地址的形式
/*==================================================================================
* 函 数 名： make_udpsocket_connect
* 参    数：
* 功能描述:  
* 返 回 值：创建成功返回0；
* 备    注： None
* 作    者： lc
* 创建时间： 2021/08/31
==================================================================================*/
int make_udpsocket_connect(void)
{
    debug("\n======================java client initialization======================\n"); 
    if ((javaSocketFd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    { 
        perror("socket 创建失败");
        return FAIL;
    }
    debug("socket connect ok\n");

    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(RCV_PORT); 
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); 
    //server_addr.sin_addr.s_addr = INADDR_ANY;

    int res = bind(javaSocketFd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(res < 0) 
    {
        perror("绑定失败");
        close(javaSocketFd);
        return FAIL;
    }  
    return SUCCESS ;
}

/*==================================================================================
* 函 数 名： udpsocket_receive_data
* 参    数：
* 功能描述:  udp socket 数据接收
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
void udpsocket_receive_data(void)
{
    memset(udpRcvBuf, 0, sizeof(udpRcvBuf));
    int rlen = recvfrom(javaSocketFd, udpRcvBuf, sizeof(udpRcvBuf), 0, (struct sockaddr*)&cliaddr, &len);
    if(rlen>0)
    {
        //LogWrite("INFO",buf);
        usleep(1000);
        udpsocket_data_processing(udpRcvBuf);
       // udpsocket_send_data(udpSendBuf);
    }
}

/*==================================================================================
* 函 数 名： udpsocket_send_data
* 参    数：
* 功能描述:  udp socket 发送数据
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
int udpsocket_send_data(char *data)
{
    printf("client info, ip: %s, port: %d\n",
        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
        ntohs(cliaddr.sin_port));

    //发送到指定端口,不指定的话就发送到接收数据端口
    cliaddr.sin_port = htons(SEN_PORT);
    sendto(javaSocketFd, data, strlen(data), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    return 0;
}

/*==================================================================================
* 函 数 名： udpsocket_ack_err_code
* 参    数：
* 功能描述:  udp socket 发送错误应答
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
static void udpsocket_ack_err_code(int status, char *string)
{
    //创建根节点JSON(最外面大括号的JSON对象)
    cJSON *json_root=cJSON_CreateObject();
    cJSON_AddBoolToObject(json_root, "success", status);
    cJSON_AddStringToObject(json_root, "message", string);
    char *data = cJSON_Print(json_root);
    cJSON_Delete(json_root); 
    printf("udp send data = %s\n", data);
    udpsocket_send_data(data);
}

/*==================================================================================
* 函 数 名： udpsocket_ack_cmd_code
* 参    数：
* 功能描述:  udp socket 应答
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
static void udpsocket_ack_cmd_code(int status, char *string, char *dataStr)
{
    //创建根节点JSON(最外面大括号的JSON对象)
    cJSON *json_root=cJSON_CreateObject();
    cJSON_AddBoolToObject(json_root, "success", status);
    cJSON_AddStringToObject(json_root, "message", string);
    cJSON_AddStringToObject(json_root, "data", dataStr);
    char *data = cJSON_Print(json_root);
    cJSON_Delete(json_root); 
    printf("udp send data = %s\n", data);
    udpsocket_send_data(data);
}

/*==================================================================================
* 函 数 名： data_json_processing
* 参    数：
* 功能描述:  解析data域json数据
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
static void data_json_processing(cJSON *dataJson)
{

}

/*==================================================================================
* 函 数 名： udpsocket_data_processing
* 参    数：
* 功能描述:  udp socket 接收数据处理
* 返 回 值： 创建成功返回0
* 备    注： None
* 作    者： lc
* 创建时间： 2023/03/30
==================================================================================*/
static void udpsocket_data_processing(char *json_string)
{
    int i =0,num=0;
    int status;
    char *buf =NULL;
    char data[128] = {};
    char temp_str[256] = {};
    char numStr[256] = {};
    struct devStatusRes dev_status = {0};
    cJSON *rootJson =NULL;
    cJSON *dataJson =NULL;
    cJSON *actionJson =NULL;
    cJSON *cmdJson =NULL;

    //设备状态json对象
    cJSON *cjson_devStatus = NULL;
    cJSON *cjson_data = NULL;
    cJSON *cjson_uvc = NULL;
    cJSON *cjson_led = NULL;
    cJSON *cjson_door = NULL;
    cJSON *cjson_lock = NULL;

    char testbuf[] = "afafsafasfasfasfasfasfasfasfasfasf";


    printf("json_string = %s\n",json_string);
    //JSON字符串到cJSON格式
    rootJson = cJSON_Parse(json_string); 
    //判断cJSON_Parse函数返回值确定是否打包成功
    if(rootJson == NULL){
        printf("json pack into cjson error...");
        goto err;
    }
    else{//打包成功调用cJSON_Print打印输出
        //cJSON_Print(rootJson);
        printf("json pack into cjson ok...");
    }
    //获取字段值
    //cJSON_GetObjectItemCaseSensitive返回的是一个cJSON结构体所以我们可以通过函数返回结构体的方式选择返回类型！
    cmdJson = cJSON_GetObjectItem(rootJson, "cmd");
    if(cmdJson==NULL)
    {
        goto err;
    }
    char *cmdStr = cJSON_GetObjectItemCaseSensitive(rootJson,"cmd")->valuestring;
    printf("cmd  = %s\n", cmdStr);
    
    // str = cJSON_PrintUnformatted(actionJson);
    // printf("actionJson  = %s\n", str);
    
    //命令解析
    for(i=getDevStatus; i<endCmd; i++)
    {
        if(strcmp(cmdStr, choices[i]) == 0)
        {
            break;
        }
    }
    if(i==endCmd)
    {
        printf("错误指令\n");
        goto err;
    }
    switch(i)
    {
        case getDevStatus://安卓获取当前设备各个硬件模块(HUB,LED,门,锁)的当前状态
            /* 创建一个JSON数据对象(链表头结点) */
            cjson_devStatus=cJSON_CreateObject();
            cJSON_AddBoolToObject(cjson_devStatus, "success", cJSON_True);
            cJSON_AddStringToObject(cjson_devStatus, "message", "success");
            /* 添加一个嵌套的JSON数据（添加一个链表节点） */
            cjson_data = cJSON_CreateObject();
            cjson_uvc  = cJSON_CreateObject();
            cjson_led  = cJSON_CreateObject();
            cjson_door = cJSON_CreateObject();
            cjson_lock = cJSON_CreateObject();
                    
            dev_status = get_all_perdevice_status();
            for(int j=0;j<MAX_HUB_NUM;j++)
            {
                printf("udp get hub[%d] status = %d\n", j, dev_status.hub_status[j]);
                memset(temp_str,0,sizeof(temp_str)); 
                sprintf(temp_str, "%d",j);
                if(dev_status.hub_status[j])
                {
                    cJSON_AddStringToObject(cjson_uvc, temp_str, "open");
                } else {
                    cJSON_AddStringToObject(cjson_uvc, temp_str, "close");
                } 
            }        
            for(int j=0;j<MAX_LED_NUM;j++)
            {
                printf("udp get led[%d] status = %d\n", j, dev_status.led_status[j]);
                memset(temp_str,0,sizeof(temp_str)); 
                sprintf(temp_str, "%d",j);
                if(dev_status.led_status[j])
                {
                    cJSON_AddStringToObject(cjson_led, temp_str, "open");
                } else {
                    cJSON_AddStringToObject(cjson_led, temp_str, "close");
                } 
            }       
            for(int j=0;j<MAX_BOX_DOOR_NUM;j++)
            {
                printf("udp get door[%d] status = %d\n", j, dev_status.door_status[j]);
                memset(temp_str,0,sizeof(temp_str)); 
                sprintf(temp_str, "%d",j);
                if(dev_status.door_status[j])
                {
                    cJSON_AddStringToObject(cjson_door, temp_str, "open");
                } else {
                    cJSON_AddStringToObject(cjson_door, temp_str, "close");
                } 
            }      
            for(int j=0;j<MAX_BOX_DOOR_NUM;j++)
            {
                printf("udp get lock[%d] status = %d\n", j, dev_status.lock_status[j]);
                memset(temp_str,0,sizeof(temp_str)); 
                sprintf(temp_str, "%d",j);
                if(dev_status.lock_status[j])
                {
                    cJSON_AddStringToObject(cjson_lock, temp_str, "open");
                } else {
                    cJSON_AddStringToObject(cjson_lock, temp_str, "close");
                } 
            }
            cJSON_AddItemToObject(cjson_data, "uvc", cjson_uvc);
            cJSON_AddItemToObject(cjson_data, "led", cjson_led);
            cJSON_AddItemToObject(cjson_data, "door", cjson_door);
            cJSON_AddItemToObject(cjson_data, "lock", cjson_lock);
            cJSON_AddItemToObject(cjson_devStatus, "data", cjson_data);
            //delete cjson
            cJSON_Delete(rootJson);
            char *dev_data = cJSON_Print(cjson_devStatus);
            printf("dev status return data = %s\n",dev_data);
            cJSON_Delete(cjson_devStatus); 
            udpsocket_send_data(dev_data);
            return;
        break;

        case setUvcCamera: //打开某个USB-UVC 摄像头
            printf("open UVC CAMREA cmd\n");
            dataJson = cJSON_GetObjectItem(rootJson,"data");
            if(dataJson==NULL)
            {
                goto err;
            } 
            //json数据转换成字符串并打印输出
            char *str = cJSON_PrintUnformatted(dataJson);
            printf("dataJson  = %s\n", str);

            actionJson = cJSON_GetObjectItem(dataJson, "uvc");
            if(actionJson==NULL)
            {
                printf("data uvc camera cmd err\n");
                goto err;
            } 
            str = cJSON_PrintUnformatted(actionJson);
            //获取元素个数
            num =  cJSON_GetArraySize(actionJson);
            printf("actionJson  = %s number = %d\n", str,num);
            for(i=1;i<MAX_HUB_NUM+1;i++)
            {
                memset(numStr,0,sizeof(numStr));
                sprintf(numStr,"%d",i);
                /*判断是否有key键值是string的项，如果有返回1，否则返回0*/
                if(cJSON_HasObjectItem(actionJson, numStr) > 0)
                {
                   // printf("hub%d have value\n", i);
                    cJSON  *numJson = cJSON_GetObjectItem(actionJson, numStr);
                   // printf("numJson = %s\n", numJson->valuestring);  
                    if(strcmp("open", numJson->valuestring) == 0)
                    {
                        printf("UVC-CAMERA-%d open\n",i);
                        open_close_usb_device(i-1, true); 
                    } else {
                        printf("UVC-CAMERA-%d close\n",i);
                        open_close_usb_device(i-1, false);
                    }
                }
            } 
            //delete cjson
            cJSON_Delete(rootJson);
            memset(data, 0, sizeof(data));
            sprintf(data, "%s","success");
            udpsocket_ack_err_code(cJSON_True,data);
            return;
        break;

        case setled://打开某个LED
            printf("open led cmd\n");
            dataJson = cJSON_GetObjectItem(rootJson,"data");
            if(dataJson==NULL)
            {
                goto err;
            } 
            //json数据转换成字符串并打印输出
            str = cJSON_PrintUnformatted(dataJson);
            printf("dataJson  = %s\n", str);
            actionJson = cJSON_GetObjectItem(dataJson, "led");
            if(actionJson==NULL)
            {
                printf("data led cmd err\n");
                goto err;
            } 
            str = cJSON_PrintUnformatted(actionJson);
            //获取元素个数
            num =  cJSON_GetArraySize(actionJson);
            printf("actionJson  = %s number = %d\n", str,num);
            for(i=1;i<MAX_LED_NUM+1;i++)
            {
                memset(numStr,0,sizeof(numStr));
                sprintf(numStr,"%d",i);
                /*判断是否有key键值是string的项，如果有返回1，否则返回0*/
                if(cJSON_HasObjectItem(actionJson, numStr) > 0)
                {
                    cJSON  *numJson = cJSON_GetObjectItem(actionJson, numStr); 
                    if(strcmp("open", numJson->valuestring) == 0)
                    {
                        printf("LED-%d open\n",i);
                        open_close_led_device(i-1,true); 
                    } else {
                        printf("LED-%d close\n",i);
                        open_close_led_device(i-1,false); 
                    }
                }
            }  
            //delete cjson
            cJSON_Delete(rootJson);
            memset(data,0,sizeof(data)); 
            sprintf(data, "%s","success");
            udpsocket_ack_err_code(cJSON_True,data);
            return;
        break;

        case setLock://打开锁
            printf("open lock cmd\n");
            dataJson = cJSON_GetObjectItem(rootJson,"data");
            if(dataJson==NULL)
            {
                goto err;
            } 
            //json数据转换成字符串并打印输出
            str = cJSON_PrintUnformatted(dataJson);
            printf("dataJson  = %s\n", str);
            actionJson = cJSON_GetObjectItem(dataJson, "lock");
            if(actionJson==NULL)
            {
                printf("data open lock cmd err\n");
                goto err;
            } 
            str = cJSON_PrintUnformatted(actionJson);
            //获取元素个数
            num =  cJSON_GetArraySize(actionJson);
            printf("actionJson  = %s number = %d\n", str,num);
            for(i=1;i<MAX_BOX_DOOR_NUM+1;i++)
            {
                memset(numStr,0,sizeof(numStr));
                sprintf(numStr,"%d",i);
                /*判断是否有key键值是string的项，如果有返回1，否则返回0*/
                if(cJSON_HasObjectItem(actionJson, numStr) > 0)
                {
                    cJSON  *numJson = cJSON_GetObjectItem(actionJson, numStr);
                    if(strcmp("open", numJson->valuestring) == 0)
                    {
                        printf("LOCK-%d open\n",i);
                        open_box_lock(); 
                    } else {
                        printf("LOCK-%d close\n",i);
                        close_box_lock(); 
                    }
                }
            }  
            //delete cjson
            cJSON_Delete(rootJson);
            memset(data,0,sizeof(data)); 
            sprintf(data, "%s","success");
            udpsocket_ack_err_code(cJSON_True,data);
            return;
        break;

        case adapterLock://RFID 卡与智能锁配码
            printf("rfid lock cmd\n");
            actionJson = cJSON_GetObjectItem(dataJson, "adapterLock");
            if(actionJson==NULL)
            {
                printf("data rfid lock cmd err\n");
                goto err;
            } 
            str = cJSON_PrintUnformatted(actionJson);
            //获取元素个数
            num =  cJSON_GetArraySize(actionJson);
            printf("actionJson  = %s number = %d\n", str,num);
            for(i=1;i<MAX_BOX_DOOR_NUM+1;i++)
            {
                memset(numStr,0,sizeof(numStr));
                sprintf(numStr,"%d",i);
                /*判断是否有key键值是string的项，如果有返回1，否则返回0*/
                if(cJSON_HasObjectItem(actionJson, numStr) > 0)
                {
                    cJSON  *numJson = cJSON_GetObjectItem(actionJson, numStr); 
                    if(strcmp("open", numJson->valuestring) == 0)
                    {
                        printf("RFID-DOOR-%d open\n",i);
                        open_close_door_rfid_pair(true);
                    } else {
                        printf("RFID-DOOR-%d close\n",i);
                        open_close_door_rfid_pair(false); 
                    }
                }
            }  
            //delete cjson
            cJSON_Delete(rootJson);
            memset(data,0,sizeof(data)); 
            sprintf(data, "%s","success");
            udpsocket_ack_err_code(cJSON_True,data);
            return;
        break;
        
        case getDoorStatus://获取门状态
            status = get_door_perdevice_status();
            /* 创建一个JSON数据对象(链表头结点) */
            cjson_devStatus=cJSON_CreateObject();
            cJSON_AddBoolToObject(cjson_devStatus, "success", cJSON_True);
            cJSON_AddStringToObject(cjson_devStatus, "message", "success");
            /* 添加一个嵌套的JSON数据（添加一个链表节点） */
            cjson_data = cJSON_CreateObject();
            cjson_door = cJSON_CreateObject();
             for(int j=0;j<MAX_BOX_DOOR_NUM;j++)
            {
                printf("udp get door[%d] status = %d\n", j, status);
                memset(temp_str,0,sizeof(temp_str)); 
                sprintf(temp_str, "%d",j);
                if(status)
                {
                    cJSON_AddStringToObject(cjson_door, temp_str, "open");
                } else {
                    cJSON_AddStringToObject(cjson_door, temp_str, "close");
                } 
            }
         
            cJSON_AddItemToObject(cjson_data, "door", cjson_door);
            cJSON_AddItemToObject(cjson_devStatus, "data", cjson_data);
            //delete cjson
            cJSON_Delete(rootJson);
            char *doorData = cJSON_Print(cjson_devStatus);
            printf("get door status return data = %s\n",doorData);
            cJSON_Delete(cjson_devStatus); 
            udpsocket_send_data(doorData);
            return;
        break;

        case getLockStatus://获取锁状态
            status = get_lock_perdevice_status();
            /* 创建一个JSON数据对象(链表头结点) */
            cjson_devStatus=cJSON_CreateObject();
            cJSON_AddBoolToObject(cjson_devStatus, "success", cJSON_True);
            cJSON_AddStringToObject(cjson_devStatus, "message", "success");
            /* 添加一个嵌套的JSON数据（添加一个链表节点） */
            cjson_data = cJSON_CreateObject();
            cjson_lock = cJSON_CreateObject();
            for(int j=0;j<MAX_BOX_DOOR_NUM;j++)
            {
                printf("udp get lock[%d] status = %d\n", j, status);
                memset(temp_str,0,sizeof(temp_str)); 
                sprintf(temp_str, "%d",j);
                if(status)
                {
                    cJSON_AddStringToObject(cjson_lock, temp_str, "open");
                } else {
                    cJSON_AddStringToObject(cjson_lock, temp_str, "close");
                } 
            } 
            cJSON_AddItemToObject(cjson_data, "lock", cjson_lock);
            cJSON_AddItemToObject(cjson_devStatus, "data", cjson_data);
            //delete cjson
            cJSON_Delete(rootJson);
            char *lockData = cJSON_Print(cjson_devStatus);
            printf("get lock status return data = %s\n",lockData);
            cJSON_Delete(cjson_devStatus); 
            udpsocket_send_data(lockData);
            return;
        break;

        case ota://OTA升级
            //delete cjson
            cJSON_Delete(rootJson);
            memset(data,0,sizeof(data)); 
            sprintf(data, "%s","success");
            udpsocket_ack_err_code(cJSON_True,data);
            return;
        break;

        default:
            goto err;
        break;
    }
    err://json错误应答
        //delete cjson
        cJSON_Delete(rootJson);
        //udp socket 应答
        memset(data,0,sizeof(data));
        sprintf(data, "%s","err json cmd");
        udpsocket_ack_err_code(cJSON_False,data);
        return;
}
