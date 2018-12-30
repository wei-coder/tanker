/*********************************************************
author:wei-coder
date:2018-12-19
filename:main.cpp
description:Ö÷³ÌÐòº¯Êý
 **********************************************************/

#include <sys/socket.h>
#include <sys/select.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "motor_driver.h"
#include "tanker.h"
#include "main.h"

#define CTLBD_ADDR	0X40
#define SERVER_PORT	8090

using std::cout;
using std::endl;


int swapInt32(int value)
{
	return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

static int  set_socket_non_blocking (int sfd)  
{  
	int flags, s;  
	flags = fcntl (sfd, F_GETFL, 0);  
	if (flags == -1)  
	{  
		return -1;  
	}  
	flags |= O_NONBLOCK;  
	s = fcntl (sfd, F_SETFL, flags);  
	if (s == -1)  
	{  
		return -1;  
	}
	return 0;  
}  

int main(int argc, char * argv[])
{
	MotorHAT ctlbd(CTLBD_ADDR);

#if 0
	DCMotor & motor1 = ctlbd.getMotor(1);
	DCMotor & motor2 = ctlbd.getMotor(2);
	DCMotor & motor3 = ctlbd.getMotor(3);
	DCMotor & motor4 = ctlbd.getMotor(4);

	motor1.setSpeed(100);
	motor1.run(FORWARD);
	sleep(2);
	motor1.run(RELEASE);
	sleep(2);
	cout << "Motor 1 test end" << endl;

	motor2.setSpeed(100);
	motor2.run(FORWARD);
	sleep(2);
	motor2.run(RELEASE);
	cout << "Motor 2 test end" << endl;

	motor3.setSpeed(100);
	motor3.run(FORWARD);
	sleep(2);
	motor3.run(RELEASE);
	cout << "Motor 3 test end" << endl;

	motor4.setSpeed(200);
	motor4.run(FORWARD);
	sleep(10);
	motor4.run(RELEASE);
	cout << "Motor 4 test end" << endl;


	return 0;
#endif
	Tanker	tanker(ctlbd.getMotor(1),ctlbd.getMotor(2));

#if 0
	tanker.forward(100);
	sleep(10);
	tanker.backward(100);
	sleep(10);
	tanker.left(100);
	sleep(1);
	tanker.right(100);
	sleep(1);
	tanker.stop();
	return 0;
#endif
	struct sockaddr_in hostip;
	struct sockaddr_in clientip;
	int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_fd < 0)
	{
		cout << "create socket failed!" << endl;
		return -1;
	}

	hostip.sin_addr.s_addr = inet_addr("192.168.0.114");
	hostip.sin_family = AF_INET;
	hostip.sin_port = htons(8090);

	int on = 1;
	if((setsockopt(serv_fd, SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on)))<0)  
	{
		cout << "set socket option SO_REUSEADDR failed!" << endl;	
		return -1;
	}

	int ret = bind(serv_fd, (struct sockaddr*)&hostip, sizeof(struct sockaddr));
	if(-1 == ret)
	{
		cout << "bind failed!" << endl;
		return -1;
	}
	cout << "bind sucess!" << endl;
	ret = listen(serv_fd, 10);
	if(-1 == ret)
	{
		cout << "listen failed!" << endl;
		return -1;
	}

	socklen_t addrlen = sizeof(struct sockaddr);
	int controller = 0;
	cout << "wait connect!" << endl;
	controller = accept(serv_fd, (struct sockaddr*)&clientip, &addrlen);
	if(0 < controller)
	{
		cout << "Get connect from " << inet_ntoa(clientip.sin_addr) << endl;
	}
	else
	{
		cout << "[ERROR " << errno << "]: " << strerror(errno) << endl;
	}
	order_t order;
	ssize_t buflen = 0;
	while(1)
	{
			buflen = recv(controller,(void *)(&order), 16,0);
			if(buflen > 0)
			{
				cout << "speed: " << order.speed << endl;
				cout << "pos_x: " << order.pos_x << endl;
				cout << "pos_y: " << order.pos_y << endl;
				cout << "order: " << order.order[0] << endl;
			}
			else
			{
				if(0 == buflen)
				{
					cout << "connect break, reconnect!" << endl;
				}
				else
				{
					cout << "Something wrong!" << endl;
					close(controller);
				}
				controller = accept(serv_fd, (struct sockaddr*)&clientip, &addrlen);
				if(0 < controller)
				{
					struct in_addr ip = {0};
					ip.s_addr = ntohl(clientip.sin_addr.s_addr);
					cout << "Get connect from " << inet_ntoa(ip) << endl;
					cout << "Get connect from " << inet_ntoa(clientip.sin_addr) << endl;
				}
				else
				{
					cout << "[ERROR " << errno << "]: " << strerror(errno) << endl;
				}
				continue;
			}

			switch(order.order[0])
			{
				case 'l':
					tanker.left(order.speed);
					cout << "turn left" << endl;
					break;
				case 'r':
					tanker.right(order.speed);
					cout << "turn right" << endl;
					break;
				case 'f':
					tanker.forward(order.speed);
					cout << "run forward" << endl;
					break;
				case 'b':
					tanker.backward(order.speed);
					cout << "run backward" << endl;
					break;
				case 's':
					tanker.stop();
					cout << "Tanker Stop!" << endl;
					break;
				case 'g':
					tanker.moveto(order.pos_x,order.pos_y,order.speed);
					cout << "Go to x:" << order.pos_x << ", y:" << order.pos_y << endl;
					break;
				default:
					break;
			}
	}
}
