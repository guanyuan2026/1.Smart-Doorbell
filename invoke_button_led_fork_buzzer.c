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

/*O_RDWRֻ����,O_NDELAY��������ʽ*/	
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
		if(buffer[0]){			//ȷ���а�������
			pid = fork();	//�������� 

			switch(pid){
			case -1:
				perror("\nfork\n");
				exit(1);
			case 0:   		              //�ӽ���
				ioctl(fd_buzzer,1,0);	//��������
				sleep(1);
				ioctl(fd_buzzer,0,0);
			default:					//������
				ioctl(fd_led,1,0);		//led��
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