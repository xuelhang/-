#定义变量
#ARCH默认为x86，使用gcc编译器，
#否则使用arm编译器
ARCH ?= arm
TARGET = DG_Host


#根据输入的ARCH变量来选择编译器
#ARCH=x86，使用gcc
#ARCH=arm，使用arm-gcc
ifeq ($(ARCH),x86)
CC = gcc
else
CC = arm-linux-gnueabihf-gcc
endif


#存放中间文件的路径
BUILD_DIR = build_$(ARCH)
#存放源文件的文件夹
SRC_DIR = Src
#存放头文件的文件夹
#includes . :代表了两个文件夹,includes 和.   ("."为Makefile所在目录的意思)
INC_DIR = Inc .


#调用的外部库
LINK = -lpthread

#wildcard函数:获取文件列表,并使用空格分隔开。它的格式如下：
# $(wildcard 匹配规则)
#例如函数调用“$(wildcard *.c)”，函数执行后会把当前目录的所有c文件列出


#patsubst函数:功能为模式字符串替换。它的格式如下：
#$(patsubst 匹配规则, 替换规则, 输入的字符串)

#notdir函数:notdir函数用于去除文件路径中的目录部分。它的格式如下：
#$(notdir 文件名)

#源文件
#使用wildcard列出目录下所有.c
SRCS = $(wildcard $(SRC_DIR)/*.c)

#目标文件（*.o）
#使用patsubst将SRCS变量下的所有.c首先用notdir取出路径,然后转化到到BUILD_DIR变量目录下为.o(仅转化字符串名称用作编译规则)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

#头文件
#使用wildcard列出INCDIR/目录下所有.h
DEPS = $(wildcard $(INC_DIR)/*.h)

#指定头文件的路径
#参数 -I:指定头文件的路径
#本处若写为CFLAGS = -I $(INC_DIR) 是否也能正常编译?
CFLAGS = $(patsubst %, -I%, $(INC_DIR))

#目标文件
#BUILD_DIR目录下的TARGET文件:依赖SRCS目录下的所有.c文件
#自动化变量 $@:规则中的目标的集合,即执行符合条件的所有目标,在本命令中为*.o
#			$^:所有依赖文件的集合,在本命令中为*.c和*.o
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LINK)

#*.o文件的生成规则
#.o的生成依赖于SRC_DIR目录下的.c和DEPS变量中列出的所有.h
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS) 
#创建一个编译目录，用于存放过程文件
#命令前带“@”,表示不在终端上输出
#$<:依赖中的第一个文件,本处为*.c(一个.o的生成依赖于一个.c)
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(CFLAGS) 

#伪目标
.PHONY: clean cleanall
#按架构删除
clean:
	rm -rf $(BUILD_DIR)

#全部删除
cleanall:
	rm -rf build_x86 build_arm
