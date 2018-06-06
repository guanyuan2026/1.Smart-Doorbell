#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
	int fd_button,fd_led;
	unsigned char buffer[2];
	char *read_key = "/dev/buttons_irq";
	char *leds = "/dev/leds";
	unsigned int led_status[2] = {0};

/*O_RDWR只读打开,O_NDELAY非阻塞方式*/	
	if((fd_button = open(read_key,O_RDWR|O_NDELAY))<0){
		printf("APP open %s failed\n",read_key);
		return -1;
	}
	
	if((fd_led = open(leds, O_RDWR|O_NOCTTY|O_NDELAY))<0){
		printf("open %s failed\n",leds);
		return -1;
	}
		 
	
	printf("APP open %s success!\n",read_key);
	while(1){
		read(fd_button,buffer,sizeof(buffer));
		if(buffer[0]){
			if(led_status[0])
			{
				led_status[0] = 0;
			}
			else
			{
				led_status[0] = 1;
			}
			ioctl(fd_led,led_status[0],0);
			
		}
		if(buffer[1]){
			if(led_status[1])
			{
				led_status[1] = 0;
			}
			else
			{
				led_status[1] = 1;
			}
			ioctl(fd_led,led_status[1],1);
		}
	}
	
	close(fd_button);
	close(fd_led);
}