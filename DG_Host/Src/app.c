#include <app.h>

//字节流转换为十六进制字符串,并加上冒号
void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )
{
    int  i;
    char szTmp[4];
 
    for( i = 0; i < nSrcLen; i++ )
    {
        sprintf( szTmp, "%02X", (unsigned char) sSrc[i] );
        if (i < nSrcLen - 1)
        {
            sprintf(&szTmp[2], ":");
        }
        memcpy( &sDest[i * 3], szTmp, 3 );
    }
    return ;
}


//获取系统时间并返回
char *getNowtime(void)
{
    static char s[30] = {0};
    char YMD[15] = {0};
    char HMS[10] = {0};
    time_t current_time;
    struct tm *now_time;

    char *cur_time = (char *)malloc(21 * sizeof(char));
    time(&current_time);
    now_time = localtime(&current_time);

    strftime(YMD, sizeof(YMD), "%F ", now_time);
    strftime(HMS, sizeof(HMS), "%T", now_time);

    strncat(cur_time, YMD, 11);
    strncat(cur_time, HMS, 8);

    // printf("\nCurrent time: %s\n\n", cur_time);
    memcpy(s, cur_time, strlen(cur_time) + 1);
    free(cur_time);

    cur_time = NULL;

    return s;
}


//将报文写入日志文件
int ogmsg_log_write (char *buf, int length, char *uart_name)
{

    FILE *fp;
    char *fbuf;
    char tbuf[MAX_MSG_LENGTH] = {0};

    //为文件缓冲区申请内存
    fbuf = (char *)malloc(MAX_MSG_LENGTH);
    //初始化文件指针，打开日志文件，“a”：追加模式
    fp = fopen("OriginMessage_log.txt", "a");
    if (NULL == fp)
    {
        printf("Fail to Open File\n");
        return 0;
    }

    Hex2Str(buf, tbuf, length);//将入参的报文转为字符串
    //存入fbuf中
    sprintf(fbuf, "\n-----------------------------\nport: %s\ntime: %s\nmessage: %s\n", uart_name,(char *)getNowtime(), tbuf);
    // printf("%s", fbuf);//test
    //写入文件
    fwrite(fbuf, 1, strlen(fbuf), fp);
    //将缓冲区的数据全部写入文件中
    fflush(fp);
    //关闭文件
    fclose(fp);

    //释放指针fbuf
    free(fbuf);
    fbuf = NULL;

    return 0;
}


//函数功能：读取指定路径的串口设备数据，并写入日志
void msg_recv_handle(struct uart_attribute *uart)
{


    struct node_attribute *node;//报文数据实例化指针



    uart->fd = open(uart->path, O_RDWR);
    if (uart->fd < 0)
    {
        perror(uart->path);
        exit(-1);
    }
    //将串口设备设为非阻塞状态
    fcntl(uart->fd, F_SETFL, O_NONBLOCK);

        uart->buf[0] =  0x68;
        uart->buf[1] =  0xE4;
        uart->buf[2] =  0x00;
        uart->buf[3] =  0x10;
        uart->buf[4] =  0x11;
        uart->buf[5] =  0x42;
        uart->buf[6] =  0x08;
        uart->buf[7] =  0x00;
        uart->buf[8] =  0x02;
        uart->buf[9] =  0x0B;
        uart->buf[10] =  0x29;
        uart->buf[11] =  0x0C;
        uart->buf[12] =  0xEF;
        uart->buf[13] =  0x1C;
        uart->buf[14] =  0x1F;
        uart->buf[15] =  0xFF;
        uart->buf[16] =  0xCB;
        uart->buf[17] =  0x85;
        uart->buf[18] =  0x16;
        
        

    //读取串口设备数据
    uart->len = read(uart->fd, uart->buf, MAX_MSG_LENGTH);
    uart->len = 19; 
    if (uart->len > 0)
    {
        // printf("from %s,len = %d\n",uart->name, uart->len);//调试信息       
        ogmsg_log_write(uart->buf, uart->len, uart->name);

        //此处buf和长度传进来，传进一个函数里，记录进内存中
        node = msg_data_prase(uart->buf,uart->len);
        //此处写应答操作
        // write(uart3.fd, buf, strlen(buf));//应答
        free(node);

        memset(uart->buf, 0, 1024);//操作结束后清空缓冲区和长度
        uart->len = 0;
    }
    close(uart->fd);
    return ;
}
//ADD8校验和
   int sumCheck8(int *buf, int len){
        int sum = 0;
        int temp = 0;
        for(int i = 0; i < len; i++){
            sum = sum + buf[i+1];
        }
        return (int) (sum & 0xff);
    }

void  addr_datalen_fn(struct node_attribute *msg,int *buf ){
              //地址
            for (size_t i = 0; i < 4; i++)
            {
                 msg->address[i] = buf[i+1];
                    printf(" address %d = %d\n",i,msg->address[i]);
            }
            //数据长度
            msg->datalen = buf[6];
            printf(" datalen=%d\n",msg->datalen);
            //帧序列号
            msg->frame_num[0] = buf[7 ];
            msg->frame_num[1] = buf[8 ];
            printf(" frame_num1=%d\n",msg->frame_num[0]);
             printf(" frame_num2=%d\n",msg->frame_num[1]);
}

void handle_msg(struct node_attribute *msg,int *buf,int len){
        int  check ;
       //判断校验和是否正确
          check =  sumCheck8(buf,len-3);
          printf("check = %d\n",check);
        if (check != buf[len-2])
        {
            printf("add8 erro\n");
            return -1;
        } 
        //地址、数据长度、帧序列号
        addr_datalen_fn(msg,buf);
        //数据
            for (size_t i = 0; i < len-13; i++)
            {
                   msg->data[i] = buf[i+9];
                    printf(" data %d= %d\n",i,msg->data[i]);
            }
            //信号强度RSSI
            msg->RSSI[0]  = buf[len-4];
            msg->RSSI[1]  = buf[len-3];
             printf(" rssi1=%d\n",msg->RSSI[0]);
             printf(" rssi2=%d\n",msg->RSSI[1]);
            printf("success\n");
}


struct node_attribute *msg_data_prase(int *buf, int len)
{
    struct node_attribute *msg = (struct node_attribute *)malloc(sizeof(struct node_attribute));
    /********************逻辑代码-start*********************/

int  check ;
    //判断接收的数据长度是否满足要求
    if (len <6)
    {
        printf("lenth err\n");
        return  -1;
    }
    //判断首位是否为68
    if (buf[0] == 0x68)
    {   
        //判断末尾是否为16
        if (buf[len-1] != 0x16)
        {
            return -1  ;
        }

        //功能码
            msg->funcode = buf[5];
        //有缘、户外、pt1000、水浸、烟感、门磁
    if (msg->funcode == 0x40 || msg->funcode == 0x43 || msg->funcode == 0x44 || msg->funcode == 0x48)
    {
             handle_msg(msg,buf,len);
    }
    //温湿度、倾角
    else if (msg->funcode == 0x42 || msg->funcode == 0x46)
    {
        //   //判断校验和是否正确
        //   check =  sumCheck8(buf,16);
        // if (check != buf[17])
        // {
        //     printf("add8 erro\n");
        //     return -1;
        // } 
        // //地址、功能码、数据长度、帧序列号
        // addr_fun_len_fn(msg,buf);
        //      //数据
        //     for (size_t i = 0; i < 7; i++)
        //     {
        //            msg->data[i] = buf[i+9];
        //     }
        //     //信号强度RSSI
        //     msg->RSSI[0]  = buf[15];
        //     msg->RSSI[1]  = buf[16];
        handle_msg(msg,buf,len);
    }
    //三路测温
    else if (msg->funcode == 0x45)
    {
        //  //判断校验和是否正确
        //   check =  sumCheck8(buf,18);
        // if (check != buf[19])
        // {
        //     printf("add8 erro\n");
        //     return -1;
        // } 
        // //地址、功能码、数据长度、帧序列号
        // addr_fun_len_fn(msg,buf);
        //      //数据
        //     for (size_t i = 0; i < 9; i++)
        //     {
        //            msg->data[i] = buf[i+9];
        //     }
        //     //信号强度RSSI
        //     msg->RSSI[0]  = buf[17];
        //     msg->RSSI[1]  = buf[18];
        handle_msg(msg,buf,len);
    }
    //六路测温
    else if (msg->funcode == 0x47)
    {
        //   //判断校验和是否正确
        //   check =  sumCheck8(buf,20);
        // if (check != buf[21])
        // {
        //     printf("add8 erro\n");
        //     return -1;
        // } 
        // //地址、功能码、数据长度、帧序列号
        // addr_fun_len_fn(msg,buf);
        //      //数据
        //     for (size_t i = 0; i < 11; i++)
        //     {
        //            msg->data[i] = buf[i+9];
        //     }
        //     //信号强度RSSI
        //     msg->RSSI[0]  = buf[19];
        //     msg->RSSI[1]  = buf[20];
        handle_msg(msg,buf,len);
    }
    }else
    {
        printf("error\n");
        return -1;
    }


    

    /********************逻辑代码-end*********************/
    //需要在函数外部free此结构体,否则会发生内存溢出
    return msg;
    
}