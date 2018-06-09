目标：
HOME：门铃按键
BACK：关闭系统

LED2：状态指示灯
LED3：系统指示灯
文件说明：
Makefile:
build.sh:
buttons_irq.c：按键驱动程序
invoke_button_irq.c：按键应用程序
invoke_button_led.c：按键翻转LED状态
invoke_button_led_fork_buzzer.c：按键按下LED亮1S，创建子进程，蜂鸣器响1S，回收僵尸进程。增加退出按键。
invoke_button_led_fork_pipe_buzzer.c：进程通信实现，增加系统指示灯

