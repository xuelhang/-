#include <main.h>

struct uart_attribute uart2 = 
{   
    .name = "uart2",
    .path = "/dev/ttymxc1"
};
struct uart_attribute uart3 = 
{
    .name = "uart3",
    .path = "/dev/ttymxc2"
};
struct uart_attribute uart4 = 
{
    .name = "uart4",
    .path = "/dev/ttymxc3"
};


int main(int argc, char *argv[])
{
    int res;

    pthread_t thread;
    void *thread_return;




    /*创建线程，线程为OriginMessage_thread函数*/
    res = pthread_create(&thread, NULL, OriginMessage_thread, NULL);
    if (res != 0)
    {
        printf("create thread fail\n");
        exit(res);
    }

    while (1)
    {       
    }

    return 0;
}
//记录、处理、应答
void *OriginMessage_thread(void *arg)
{
    int res;


    //初始化串口，并获取文件描述符fd
    uart2.fd = uart_init(uart2.path);
    uart3.fd = uart_init(uart3.path);
    uart4.fd = uart_init(uart4.path);

    while (1)
    {
       msg_recv_handle(&uart2);
       msg_recv_handle(&uart3);
       msg_recv_handle(&uart4); 
    }

    /*退出线程*/
    pthread_exit(NULL);
}


//



