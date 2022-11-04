#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C"
{
#endif

// 系统库
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>

#include <pthread.h>
#include <time.h>

#include <ctype.h>


// 用户库
#include <uart.h>
#include <app.h>



void *OriginMessage_thread(void *arg);

#ifdef __cplusplus
}
#endif

#endif