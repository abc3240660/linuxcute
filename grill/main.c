#include "led.h"
#include "delay.h"
#include "sys.h"
#include "Tiky_LCD.h" 
#include "usart.h"
#include "sram.h"
#include "timer.h"
#include "malloc.h"
#include "GUI.h"
#include "GUIDEMO.h"
#include "includes.h"
#include "ec11key.h"
#include "adc.h"
#include "temp.h"
#include "control.h"

//START任务
//设置任务的优先级
#define START_TASK_PRIO				3
//任务堆栈大小 
#define START_STK_SIZE			  256
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				4
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//TEMP任务
//设置任务优先级
#define TEMP_TASK_PRIO 				5
//任务堆栈大小
#define TEMP_STK_SIZE				64
//任务控制块
OS_TCB TempTaskTCB;
//任务堆栈
CPU_STK TEMP_TASK_STK[TEMP_STK_SIZE];
//led0任务
void temp_watch_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			6
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			2048
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task任务
void emwindemo_task(void *p_arg);

//Run任务
//设置任务优先级
#define RUN_TASK_PRIO 				7
//任务堆栈大小
#define RUN_STK_SIZE				64
//任务控制块
OS_TCB RUNTaskTCB;
//任务堆栈
CPU_STK RUN_TASK_STK[RUN_STK_SIZE];
//led0任务
void run_task(void *p_arg);


////////////////////////////////////////////////////////
OS_TMR 	tmr1;		//定时器1
OS_TMR	tmr2;		//定时器2
void tmr1_callback(void *p_tmr, void *p_arg); 	//定时器1回调函数
void tmr2_callback(void *p_tmr, void *p_arg);	//定时器2回调函数

u32 Check_Sys=0;

// Target温度值
extern const short baseTempMax[];
extern EVENT_VAL g_event_val_new;// 当前状态
extern EVENT_VAL g_event_val_last;// 上一次状态
extern TEMP_VAL g_temp_val_new;// 当前温度
extern TEMP_VAL g_temp_val_last;// 上一次温度

//
// User Interface
//

// 是否发生严重错误
// 0-OK 1-FLAME ERROR 2-SENSOR ERROR
extern int g_fatal_error;

// 几种模式的开始和结束标志
// 0-end 1-start
extern int g_startup_mode;
extern int g_run_mode;
extern int g_feed_mode;
extern int g_shutdown_mode;
// 几种模式的计时器
extern u16 g_startup_mode_counter;
extern u16 g_run_mode_counter_mins;// minutes
extern u16 g_run_mode_counter_sec;// seconds
extern u16 g_feed_mode_counter;
extern u16 g_shutdown_mode_counter;


// RUN模式倒计时，时间设置以及使能
// 单位seconds(默认480s)
extern int g_run_timer_setting;

// 0-华氏度(默认) 1摄氏度
// g_temp_val_new.temp_unit

// 右侧温度区(对应温度棒1234)
// g_temp_val_new.temp1
// g_temp_val_new.temp2
// g_temp_val_new.temp3
// g_temp_val_new.temp4

// RUN界面最大字体的温度
// g_temp_val_new.temp5

// 温度棒是否读异常
// 0-OK(default) 1-温度棒异常
extern int g_temp1_error;
extern int g_temp2_error;
extern int g_temp3_error;
extern int g_temp4_error;
extern int g_temp5_error;

// 表示是否初始化
// 0-未初始化(default), 1-已初始化
extern int g_factory_reseted;

// 进入startup或者run标志
// 0-run, 1-startup mode(default)
extern int g_startup_enable;

// smoke进度条百分比，默认值为50(%)
extern int g_smoke_val_percent;

// EMWIN界面入口
extern void GUIDEMO_AfterLogo(void);

#if 1
// LOGO图片转大数组
extern const unsigned char gImage_camp[1308];

// 获取每个像素点的2字节颜色信息
u16 image_getcolor(u8 mode,u8 *str)
{
	u16 color;
	if(mode)
	{
		color=((u16)*str++)<<8;
		color|=*str;
	}else
	{
		color=*str++;
		color|=((u16)*str)<<8;
	}
	return color;	
}

// 写入一次起点终点坐标，然后写入所有数据(颜色信息)
void image_show(u16 xsta,u16 ysta,u16 xend,u16 yend,u8 scan,u8 *p)
{  
	u32 i;
	u32 len=0;
	
	BlockWrite(xsta,xend,ysta,yend);
	
	len=(xend-xsta+1)*(yend-ysta+1);	//写入的数据长度
	for(i=0;i<len;i++)
	{
		*(__IO u16 *) (Bank1_LCD_D) = image_getcolor(scan&(1<<4),p);
		p+=2;
	}	    					  	    
} 

void image_display(u16 x,u16 y,u8 * imgx)
{
	HEADCOLOR *imginfo;
 	u8 ifosize=sizeof(HEADCOLOR);//得到HEADCOLOR结构体的大小
	imginfo=(HEADCOLOR*)imgx;
 	image_show(x,y,x+imginfo->w-1,y+imginfo->h-1,imginfo->scan,imgx+ifosize);
}

// 在指定位置开始显示一张图片
void disp_img(u16 x, u16 y)
{
	image_display(x,y,(u8*)gImage_camp);//在指定地址显示图片
}
#endif

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();	
	
	delay_init();	    	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
 	LED_Init();			    //LED端口初始化
	Lcd_Initialize();
	
	// 横屏和扫描方式设置
	LCD_Display_Dir(HORIZON_DISPLAY);

	EC11_EXTI_Init();//EC11编码器初始化
	my_mem_init(SRAMIN); 	//初始化内部内存池
	
	Adc_Init();
	Control_Init();

	PID_Init();
	
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
							 
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟
#if 1
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();  			//STemWin初始化
#if 0
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&tmr1,		//定时器1
                (CPU_CHAR	*)"tmr1",		//定时器名字
                (OS_TICK	 )0,			//0
                (OS_TICK	 )10,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码
				
				
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//单次定时器
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);
	OSTmrStart(&tmr1,&err);	//开启定时器1
	OSTmrStart(&tmr2,&err);	//开启定时器2
#endif
	
	OS_CRITICAL_ENTER();	//进入临界区
	// STemWin UI任务	
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
#endif
#if 1
	// EC11按键任务
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);		
#endif
#if 1
	// 温度读取任务
	OSTaskCreate((OS_TCB*     )&TempTaskTCB,		
				 (CPU_CHAR*   )"Temp task", 		
                 (OS_TASK_PTR )temp_watch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TEMP_TASK_PRIO,     
                 (CPU_STK*    )&TEMP_TASK_STK[0],	
                 (CPU_STK_SIZE)TEMP_STK_SIZE/10,	
                 (CPU_STK_SIZE)TEMP_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
#endif	
#if 1
	// RUN任务
	OSTaskCreate((OS_TCB*     )&RUNTaskTCB,		
				 (CPU_CHAR*   )"RUN task", 		
                 (OS_TASK_PTR )run_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )RUN_TASK_PRIO,     
                 (CPU_STK*    )&RUN_TASK_STK[0],	
                 (CPU_STK_SIZE)RUN_STK_SIZE/10,	
                 (CPU_STK_SIZE)RUN_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
#endif								 
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//定时器1的回调函数 100ms
void tmr1_callback(void *p_tmr, void *p_arg)
{
	static u8 tmr1_num_s=0;//1S
	tmr1_num_s++;
	if(tmr1_num_s>=10){
//		if(g_startup_mode) g_startup_mode_counter++;
//		else g_startup_mode_counter=0;
		
//		if(g_feed_mode) g_feed_mode_counter++;
//		else g_feed_mode_counter=0;
		
//		if(g_shutdown_mode) g_shutdown_mode_counter++;
//		else g_shutdown_mode_counter=0;
		
		tmr1_num_s=0;
	}

/*	static u32 tmr1_num=0;
//	LCD_ShowxNum(62,111,tmr1_num,3,16,0x80); //显示定时器1的执行次数
//	LCD_Fill(6,131,114,313,lcd_discolor[tmr1_num%14]);//填充区域
	tmr1_num++;		//定时器1执行次数加1*/
//	Check_Sys++;
}

//定时器2的回调函数
void tmr2_callback(void *p_tmr,void *p_arg)
{
/*	static u8 tmr2_num = 0;
	tmr2_num++;		//定时器2执行次数加1
	LCD_ShowxNum(182,111,tmr2_num,3,16,0x80);  //显示定时器1执行次数
	LCD_Fill(126,131,233,313,lcd_discolor[tmr2_num%14]); //填充区域
	LED1 = ~LED1;
	printf("定时器2运行结束\r\n");*/
}

// EMWIN界面入口
void emwindemo_task(void *p_arg)
{
	EVENT_VAL evt_val = g_event_val_last;
	TEMP_VAL temp_val = g_temp_val_last;
	
	// 该处温度应改为由温度棒读出
	// 如果温度棒没接或者读不出，请设置为0，温度会显示为"-"
	// 该处为第一次显示，后续在temp_watch_task中更新温度信息即可
	temp_val.temp1 = 0;
	temp_val.temp2 = 0;
	temp_val.temp3 = 0;
	temp_val.temp4 = 0;
	temp_val.temp5 = 0;
	
	// 默认值，可自行设置
	temp_val.target_val = 250;
		
	g_temp_val_new = temp_val;
	g_event_val_new = evt_val;
	
	LCD_Clear(WHITE);

#if 1
	disp_img(0, 70);
	delay_ms(5000);
#endif

	while(1)
	{
		GUIDEMO_AfterLogo();
	}
}

// 每100ms，扫描一次EC11编码器状态
void touch_task(void *p_arg)
{
	OS_ERR err;
	
	while(1)
	{
		EC11_BUT_Scan();
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
	}
}

// 每隔2秒更新一次温度信息
// 该函数中的温度信息应全部改为从温度棒读取
void temp_watch_task(void *p_arg)
{
	OS_ERR err;
	TEMP_VAL temp_val = g_temp_val_last;
	
	// 该处温度应改为由温度棒读出
	// 如果温度棒没接或者读不出，请设置为0，温度会显示为"-"
	temp_val.temp1 = 0;
	temp_val.temp2 = 0;
	temp_val.temp3 = 0;
	temp_val.temp4 = 0;
	temp_val.temp5 = 0;
	
	// 默认值，可自行设置
	temp_val.target_val = 450;
	
	while(1)
	{
		LED0 = !LED0;
		
		// 该处温度应改为由温度棒读出
		// 如果温度棒没接或者读不出，请设置为0，温度会显示为"-"
/*		temp_val.temp1 += 10;
		temp_val.temp2 += 15;
		temp_val.temp3 += 20;
		temp_val.temp4 += 25;
		temp_val.temp5 += 30;
		
		if (temp_val.temp1 > 500) {
			temp_val.temp1 = 0;
		}
		if (temp_val.temp2 > 500) {
			temp_val.temp2 = 0;
		}
		if (temp_val.temp3 > 500) {
			temp_val.temp3 = 0;
		}
		if (temp_val.temp4 > 500) {
			temp_val.temp4 = 0;
		}
		if (temp_val.temp5 > 500) {
			temp_val.temp5 = 0;
		}
		
		temp_val.temp1 %= baseTempMax[1] + 1;
		temp_val.temp2 %= baseTempMax[1] + 1;
		temp_val.temp3 %= baseTempMax[1] + 1;
		temp_val.temp4 %= baseTempMax[1] + 1;
		temp_val.temp5 %= baseTempMax[1] + 1;
		
		// 保存至全局变量
		g_temp_val_new.temp1 = temp_val.temp1;
		g_temp_val_new.temp2 = temp_val.temp2;
		g_temp_val_new.temp3 = temp_val.temp3;
		g_temp_val_new.temp4 = temp_val.temp4;
		g_temp_val_new.temp5 = temp_val.temp5;*/
		
		// 保存至全局变量
		g_temp_val_new.temp1 = (int)Get_Temperature(ADC_Channel_6);
		g_temp_val_new.temp2 = (int)Get_Temperature(ADC_Channel_5);
		g_temp_val_new.temp3 = (int)Get_Temperature(ADC_Channel_4);
		g_temp_val_new.temp4 = (int)Get_Temperature(ADC_Channel_3);
		g_temp_val_new.temp5 = (int)Get_Temperature(ADC_Channel_2);
		
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}

// 运行模式
void run_task(void *p_arg)
{
	OS_ERR err;
	
	while(1)
	{
		if (1 == g_run_mode) {
			g_run_mode_counter_sec++;
			
			if (g_run_mode_counter_mins != g_run_mode_counter_sec / 60) {
				GetPeriod(g_temp_val_new.target_val, g_temp_val_new.temp5);
			}
			
			g_run_mode_counter_mins = g_run_mode_counter_sec / 60;
		} else {
			g_run_mode_counter_mins = 0;
			g_run_mode_counter_sec = 0;
		}
		
		if(g_startup_mode) Startup_Mode();
		else if(g_feed_mode) Feed_Mode();
		else if(g_shutdown_mode) Shutdown_Mode();
		else if(g_run_mode) Run_Mode();
		else{//什么模式都不在
				MOT = Control_OFF;		
				HOT = Control_OFF;
				FAN = Control_OFF;
		}
		
		if(g_startup_mode) g_startup_mode_counter++;
		else g_startup_mode_counter=0;
			
		if(g_feed_mode) g_feed_mode_counter++;
		else g_feed_mode_counter=0;
			
		if(g_shutdown_mode) g_shutdown_mode_counter++;
		else g_shutdown_mode_counter=0;
		
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);
	}
}

