/*********************************************************
author:wei-coder
date:2018-12-19
filename:main.h
description:主程序头文件
**********************************************************/

#define ORDER_LEN	16

typedef struct Order
{
	int		speed;
	int		pos_x;
	int		pos_y;
	char	order[4];
}order_t;
