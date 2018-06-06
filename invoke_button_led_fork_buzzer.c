#include <stdio.h>
#include <stdlib.h>	//exit

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
	int fd_button,fd_led,fd_buzzer;
	unsigned char buffer[2] = {0};
	char *read_key = "/dev/buttons_irq";
	char *leds = "/dev/leds";
	char *buzzer_ctl = "/dev/buzzer_ctl";
	unsigned int led_status[2] = {0};
	int pipes[2],rc;
	pid_t pid;

/*O_RDWR只读打开,O_NDELAY非阻塞方式*/	
	if((fd_button = open(read_key,O_RDWR|O_NDELAY))<0){
		printf("open %s failed\n",read_key);
		return -1;
	}
	
	if((fd_led = open(leds, O_RDWR|O_NOCTTY|O_NDELAY))<0){
		printf("open %s failed\n",leds);
		return -1;
	}
	
	if((fd_buzzer = open(buzzer_ctl,O_RDWR|O_NOCTTY|O_NDELAY))<0){
		printf("open %s failed\n",buzzer_ctl);
		return -1;
	}
	
	while(1){
		read(fd_button,buffer,sizeof(buffer));
		if(buffer[0]){			//确认有按键按下
			pid = fork();	//创建进程 

			switch(pid){
			case -1:
				perror("\nfork\n");
				exit(1);
			case 0:   		              //子进程
				ioctl(fd_buzzer,1,0);	//蜂鸣器响
				sleep(1);
				ioctl(fd_buzzer,0,0);
			default:					//父进程
				ioctl(fd_led,1,0);		//led亮
				sleep(1);
				ioctl(fd_led,0,0);
				wait(NULL);
			}	
			
		}
		if(buffer[1]){
			exit(0);
		}
	}
	
	close(fd_button);
	close(fd_led);
	close(fd_buzzer);
}