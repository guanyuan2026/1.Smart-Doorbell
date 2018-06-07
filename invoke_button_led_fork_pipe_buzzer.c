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
	unsigned char buffer[2] = {0};		//��Ű���ֵ
	char *read_key = "/dev/buttons_irq";
	char *leds = "/dev/leds";
	char *buzzer_ctl = "/dev/buzzer_ctl";
	unsigned int led_status[2] = {0};	//����led״̬
	int pipes[2],rc;		//�ܵ�
	pid_t pid;		//fork����ֵ	
	unsigned char key_value[2] = {0};		//�ܵ���ȡ��ֵ

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
	
	printf("APP open %s success!\n",read_key);
	
	ioctl(fd_led,1,1);		
	
	rc = pipe(pipes);	//�����ܵ�                 
	if(rc == -1){
		perror("\npipes\n");
		return -1;;
	}
	
	pid = fork();	//��������
	
	if(pid < 0)
	{
		perror("\nfork\n");
		exit(1);
	}
	if(pid == 0)	//�ӽ���
	{
		close(pipes[1]);	//���ڴ˺���ֻ���������˽�д�����ر�(��Դ����)
		while(1)
		{	
			while( (rc = read(pipes[0],key_value,2)) > 0 ){ //�������ȴ��ӹܵ���ȡ����
				if(key_value[0])
				{
					ioctl(fd_buzzer,1,0);		//��������
					sleep(1);
					ioctl(fd_buzzer,0,0);
				}
				if(key_value[1]){
					printf("child over receive\n");
					close(pipes[0]);
					printf("child over\n");
					exit(0);
				}
			}
		}	
	}
	
	close(pipes[0]);  	//�رն�������
	while(1){
		read(fd_button,buffer,sizeof(buffer));
		rc = write( pipes[1], buffer, 2);	//д��ܵ�
		if( rc == -1 ){
			perror("Parent: write");
			close(pipes[1]);
		}
		if(buffer[0]){			
			ioctl(fd_led,1,0);		//led��
			sleep(1);
			ioctl(fd_led,0,0);
		}
		if(buffer[1]){
			printf("over key\n");
			wait(NULL);
			printf("over\n");
			ioctl(fd_led,0,1);
			exit(0);
		}		
	}	
	close(fd_button);
	close(fd_led);
	close(fd_buzzer);
}