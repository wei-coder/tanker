#include <csocket>
#include "motor_driver.h"
#include "tanker.h"

#define CTLBD_ADDR	0X40
#define SERVER_PORT	8090


int main(int argc, char * argv[])
{
	MotorHAT ctlbd(CTLBD_ADDR);
	Tanker	tanker(ctlbd.getMotor(0),ctlbd.getMotor(1));

	struct sockaddr_in hostip;
	struct sockaddr_in clientip;
	int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(serv_fd < 0)
	{
		perror("socket");
		return;
	}
	
	hostip.sin_addr.s_addr = inet_addr(argv[0]);
	hostip.sin_family = AF_INET;
	hostip.sin_port = htons(SERVER_PORT);

    int on = 1;
	if((setsockopt(serv_fd, SOL_SOCKET,SO_REUSEADDR,&on, sizeof(on)))<0)  
	{  
		perror("setsockopt failed");  
		return;
	}

	int ret = set_socket_non_blocking(serv_fd);
	if(-1 == ret)
	{
		printf("set socket non_blocking failed!\n");
	}

    ret = bind(serv_fd, (struct sockaddr*)&hostip, sizeof(struct sockaddr));
	if(-1 == ret)
	{
	    perror("bind");
	    return;
	}
	ret = listen(serv_fd, 10);
	if(-1 == ret)
	{
		perror("listen");
		return;
	}

	int addrlen;
	int controller = 0;
	while(1)
	{
		controller = accept(serv_fd, (struct sockaddr*)&clientip, &addrlen);
		if(-1 == controller)
		{
			break;
		}
		sleep(1);
	}
	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(controller, &fdset);
	char order[10];
	int buflen = 0;
	while(1)
	{
		int result = select(1, NULL, &fdset, &fdset, (struct timeval *) 10); 
		if(result < 1) 
		{
			cout << "there is a fatal error！ EXIT" << endl;
			exit(1); 
		}
		else
		{
			if(FD_ISSET(controller,&fdset))
			{ 
				buflen = recvfrom(controller,order,256,0);
				switch(order[0])
				{
					case 'l':
						break;
					case 'r':
						break;
					case 'f':
						break;
					case 'b':
						break;
				}
			}
		}
	}
}
