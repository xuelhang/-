
#include <main.h>

 //系统层串口配置函数,串口配置成功返回0，失败返回-1;
int set_uart(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{

    struct termios opt;

    // 清空串口接收缓冲区
    tcflush(fd, TCIOFLUSH);

    // 获取串口配置参数
    //  tcgetattr(fd, &opt);
    ioctl(fd, TCGETS, &opt);

    opt.c_cflag &= (~CBAUD);  // 清除数据位设置
    opt.c_cflag &= (~PARENB); // 清除校验位设置

    //  opt.c_iflag |= IGNCR;       //忽略接收数据中的'\r'字符，在windows中换行为'\r\n'
    //  opt.c_iflag &= (~ICRNL);    //不将'\r'转换为'\n'
    //  opt.c_lflag &= (~ECHO);     //不使用回显

    // hex设置
    //  opt.c_iflag &= ~(INLCR | ICRNL); //不要回车和换行转换
    //  opt.c_iflag &= ~(IXON | IXOFF | IXANY); //不要软件流控制
    //  opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    //  opt.c_oflag &= ~ OPOST;
    opt.c_iflag = 0;
    opt.c_oflag = 0;
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // 原始模式

    // 设置波特率
    switch (nSpeed)
    {
    case 2400:
        cfsetspeed(&opt, B2400);
        break;

    case 4800:
        cfsetspeed(&opt, B4800);
        break;

    case 9600:
        cfsetspeed(&opt, B9600);
        break;

    case 38400:
        cfsetspeed(&opt, B38400);
        break;

    case 115200:
        cfsetspeed(&opt, B115200);
        break;

    default:
        return -1;
    }

    // 设置数据位
    switch (nBits)
    {
    case 7:
        opt.c_cflag |= CS7;
        break;

    case 8:
        opt.c_cflag |= CS8;
        break;

    default:
        return -1;
    }

    // 设置校验位
    switch (nEvent)
    {
    // 无奇偶校验
    case 'n':
    case 'N':
        opt.c_cflag &= (~PARENB);
        break;

    // 奇校验
    case 'o':
    case 'O':
        opt.c_cflag |= PARODD;
        break;

    // 偶校验
    case 'e':
    case 'E':
        opt.c_cflag |= PARENB;
        opt.c_cflag &= (~PARODD);
        break;

    default:
        return -1;
    }

    // 设置停止位
    switch (nStop)
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:
        return -1;
    }

    // 设置串口
    //  tcsetattr(fd,TCSANOW,&opt);
    ioctl(fd, TCSETS, &opt);

    return 0;
}

//串口初始化函数
int uart_init(const char *path)
{
    int fd;
    fd = open(path, O_RDWR);
    if (fd < 0)
    {
        perror(path);
        exit(-1);
    }
    if (set_uart(fd, 115200, 8, 'n', 1))
    {
        printf("set uart error\n");
    }

    return fd;

}


