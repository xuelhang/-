#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus
extern "C"
{
#endif

//include
#include <main.h>


//两个宏定义，求出三个值的最大最小值
#define min3(x, y, z) ({ \
typeof(x) _min1 = (x); \
typeof(y) _min2 = (y); \
typeof(z) _min3 = (z); \
(void) (&_min1 == &_min2); \
(void) (&_min1 == &_min3); \
_min1 < _min2 ? (_min1 < _min3 ? _min1 : _min3) : \
(_min2 < _min3 ? _min2 : _min3); })

#define max3(x, y, z) ({ \
typeof(x) _max1 = (x); \
typeof(y) _max2 = (y); \
typeof(z) _max3 = (z); \
(void) (&_max1 == &_max2); \
(void) (&_max1 == &_max3); \
_max1 > _max2 ? (_max1 > _max3 ? _max1 : _max3) : \
(_max2 > _max3 ? _max2 : _max3); })

//串口属性结构体
struct uart_attribute
{
    char *name;
    char *path;     //路径
    int fd;         //文件描述符
    int len;        //报文长度
    int buf[MAX_MSG_LENGTH];//报文缓冲区
};

//报文属性结构体,为射频模块接收报文的抽象
struct node_attribute
{
    unsigned int address[4];//节点地址
     int funcode;   //功能码
    unsigned int datalen;   //数据字段长度
    unsigned int frame_num[2]; //帧序列号
    int data[12];              //数据
    int RSSI[2];               //信号强度
    
};

void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen );
char *getNowtime(void);
int ogmsg_log_write (char *buf, int length, char *uart_name);
void msg_recv_handle(struct uart_attribute *uart);
struct node_attribute *msg_data_prase(int *buf, int len);














#ifdef __cplusplus
}
#endif

#endif