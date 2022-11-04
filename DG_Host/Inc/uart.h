#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" 
{
#endif


#define MAX_MSG_LENGTH      1024

int set_uart(int fd, int nSpeed, int nBits, char nEvent, int nStop);
int uart_init(const char *path);













#ifdef __cplusplus
}
#endif

#endif
