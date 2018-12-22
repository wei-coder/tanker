#!Makefile
#author: wei-coder
#date:	2018-12
#description: makefile
#
#
## patsubst 函数的目的是根据某种模式替换字符串
#下面代码的目的是定义C++源码文件，object文件，其中C++文件是通过查找*.cpp获取，而object文件则是文件名与源文件相同，后缀改为.o
SRCS=$(shell find . -name "*.cpp")
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

CC=g++
LD=ld
RM=rm

FLAGS = -c -Wall -ggdb -gstabs+ -fno-stack-protector -I ./
LD_FLAGS = -lwiringPi

TARGET=tanker

start:$(OBJS)
	@echo 生成可执行文件 $< ...
	$(CC) -o $(TARGET) $(OBJS)

.cpp.o:
	@echo 编译代码文件 $< ...
	$(CC) $(FLAGS) $< -o $@

.PHONY:clean
clean:
	$(RM) -rf $(OBJS) tanker


