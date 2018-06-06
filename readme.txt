
按键按下后创建子进程，蜂鸣器响，回收僵尸进程。增加退出按键。
invoke_button_led_fork_pipe_buzzer.c：
键值传递：
1.管道通信：进程同步，管道先输入，再读取
2.共享内存：
增加系统指示灯

build.sh:

Makefile:


HOME：门铃按键
BACK:关闭系统

LED2：状态指示灯
LED3：系统指示灯