#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>

struct termios opt;
int fd;
int SerialInit();

int main ()
{
	char *data = 0;
	int led_num;

	SerialInit();
	
	printf ( "%s\r\n\r\n", "Content-Type:text/html;charset=utf8" );/*plain*/
	printf ( "<h1>Hello World!</h1>" );
	
 	data = getenv ( "QUERY_STRING" );
	if ( data == 0 )
	{
		printf ( "<h2>Not Get Input!</h2>" );
	}
	else
	{
		sscanf ( data, "control=%d", &led_num);
		printf ( "<h2>control=%d</h2>", led_num);
	} 
	led_num+='0';
	write(fd,&led_num,1);
	close(fd);
    printf("<script>window.location=\"http://192.168.1.102:8080/index.html\";</script>");
	return 0;
}

int SerialInit()
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


