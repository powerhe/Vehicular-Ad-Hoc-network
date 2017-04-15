#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
struct termios opt;
int led_num;
int fd;
int SerialInit();
int main()         //主函数
{
		SerialInit();
        int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	server_sockfd = socket(AF_INET,SOCK_STREAM,0);
        
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        server_address.sin_port = 9734;
        server_len = sizeof(server_address);
	bind(server_sockfd,(struct sockaddr *)&server_address, server_len);

        listen(server_sockfd,5);
	while(1) {
		SerialInit();
		char ch;           
		printf("server waiting\n");
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,&client_len);
		read(client_sockfd,&ch,1);
		write(fd,&ch,1);
		close(fd);
		printf("get the data is:%c\n",ch);
        close(client_sockfd);
               
}
return 0;
}
int SerialInit()     //串口初始化函数
{
	fd=open("/dev/ttyUSB0",O_RDWR);	
	tcgetattr(fd,&opt);
	cfsetispeed(&opt,B9600);
	cfsetospeed(&opt,B9600);
	opt.c_cflag |= (CLOCAL|CREAD);
	opt.c_cflag &= ~CSIZE;
	opt.c_cflag |= CS8;
	opt.c_cflag &= ~PARENB;
	opt.c_cflag &= ~CSTOPB;
	opt.c_iflag &= ~INPCK;
	opt.c_iflag |= IGNPAR|ICRNL;
	opt.c_iflag &= ~(IXON|IXOFF|IXANY); 
	opt.c_oflag |= OPOST;   
 	opt.c_cc[VMIN]=0;
	opt.c_cc[VTIME]=0;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &opt);
}

