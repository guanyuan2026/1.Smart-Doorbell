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
	unsigned char buffer[2] = {0};		//存放按键值
	char *read_key = "/dev/buttons_irq";
	char *leds = "/dev/leds";
	char *buzzer_ctl = "/dev/buzzer_ctl";
	unsigned int led_status[2] = {0};	//两个led状态
	int pipes[2],rc;		//管道
	pid_t pid;		//fork返回值	
	int key_value = 0;		//管道读取键值

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
	
	printf("APP open %s success!\n",read_key);
	
	ioctl(fd_led,1,1);
	
	rc = pipe(pipes);	//创建管道                 
	if(rc == -1){
		perror("\npipes\n");
		return -1;;
	}
	
	while(1){
		read(fd_button,buffer,sizeof(buffer));
		//printf("duqujianzhi");
		if(buffer[0]){			
			pid = fork();	//创建进程 

			switch(pid){
			case -1:
				perror("\nfork\n");
				exit(1);
			case 0:	 
				printf("jinru zijincheng");
				close(pipes[1]);	//由于此函数只负责读，因此将写描述关闭(资源宝贵)
				printf("jinru zijincheng");		//阻塞，等待从管道读取数据
				while( (rc = read(pipes[0],&key_value,1)) > 0 ){ 
					if(key_value)
					{
						printf("%d",(key_value + 1));
						ioctl(fd_buzzer,1,0);		//蜂鸣器响
						sleep(1);
						ioctl(fd_buzzer,0,0);
					}
				}
			default:
			printf("jinru fujincheng");
				close(pipes[0]);  	//关闭读描述字
printf("jinru fujincheng");				
				rc = write( pipes[1], &buffer[0], 1);	//写入管道
				if( rc == -1 ){
					perror("Parent: write");
					close(pipes[1]);
				}
				ioctl(fd_led,1,0);		//led亮
				sleep(1);
				ioctl(fd_led,0,0);
				wait(NULL);	
			}
		}
		if(buffer[1]){
			ioctl(fd_led,0,1);
			exit(0);
		}		
	}	
	close(fd_button);
	close(fd_led);
	close(fd_buzzer);
}