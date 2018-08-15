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

//START����
//������������ȼ�
#define START_TASK_PRIO				3
//�����ջ��С 
#define START_STK_SIZE			  256
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//TOUCH����
//�����������ȼ�
#define TOUCH_TASK_PRIO				4
//�����ջ��С
#define TOUCH_STK_SIZE				128
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch����
void touch_task(void *p_arg);

//TEMP����
//�����������ȼ�
#define TEMP_TASK_PRIO 				5
//�����ջ��С
#define TEMP_STK_SIZE				64
//������ƿ�
OS_TCB TempTaskTCB;
//�����ջ
CPU_STK TEMP_TASK_STK[TEMP_STK_SIZE];
//led0����
void temp_watch_task(void *p_arg);

//EMWINDEMO����
//�����������ȼ�
#define EMWINDEMO_TASK_PRIO			6
//�����ջ��С
#define EMWINDEMO_STK_SIZE			2048
//������ƿ�
OS_TCB EmwindemoTaskTCB;
//�����ջ
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task����
void emwindemo_task(void *p_arg);

//Run����
//�����������ȼ�
#define RUN_TASK_PRIO 				7
//�����ջ��С
#define RUN_STK_SIZE				64
//������ƿ�
OS_TCB RUNTaskTCB;
//�����ջ
CPU_STK RUN_TASK_STK[RUN_STK_SIZE];
//led0����
void run_task(void *p_arg);


////////////////////////////////////////////////////////
OS_TMR 	tmr1;		//��ʱ��1
OS_TMR	tmr2;		//��ʱ��2
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void tmr2_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص�����

u32 Check_Sys=0;

// Target�¶�ֵ
extern const short baseTempMax[];
extern EVENT_VAL g_event_val_new;// ��ǰ״̬
extern EVENT_VAL g_event_val_last;// ��һ��״̬
extern TEMP_VAL g_temp_val_new;// ��ǰ�¶�
extern TEMP_VAL g_temp_val_last;// ��һ���¶�

//
// User Interface
//

// �Ƿ������ش���
// 0-OK 1-FLAME ERROR 2-SENSOR ERROR
extern int g_fatal_error;

// ����ģʽ�Ŀ�ʼ�ͽ�����־
// 0-end 1-start
extern int g_startup_mode;
extern int g_run_mode;
extern int g_feed_mode;
extern int g_shutdown_mode;
// ����ģʽ�ļ�ʱ��
extern u16 g_startup_mode_counter;
extern u16 g_run_mode_counter_mins;// minutes
extern u16 g_run_mode_counter_sec;// seconds
extern u16 g_feed_mode_counter;
extern u16 g_shutdown_mode_counter;


// RUNģʽ����ʱ��ʱ�������Լ�ʹ��
// ��λseconds(Ĭ��480s)
extern int g_run_timer_setting;

// 0-���϶�(Ĭ��) 1���϶�
// g_temp_val_new.temp_unit

// �Ҳ��¶���(��Ӧ�¶Ȱ�1234)
// g_temp_val_new.temp1
// g_temp_val_new.temp2
// g_temp_val_new.temp3
// g_temp_val_new.temp4

// RUN�������������¶�
// g_temp_val_new.temp5

// �¶Ȱ��Ƿ���쳣
// 0-OK(default) 1-�¶Ȱ��쳣
extern int g_temp1_error;
extern int g_temp2_error;
extern int g_temp3_error;
extern int g_temp4_error;
extern int g_temp5_error;

// ��ʾ�Ƿ��ʼ��
// 0-δ��ʼ��(default), 1-�ѳ�ʼ��
extern int g_factory_reseted;

// ����startup����run��־
// 0-run, 1-startup mode(default)
extern int g_startup_enable;

// smoke�������ٷֱȣ�Ĭ��ֵΪ50(%)
extern int g_smoke_val_percent;

// EMWIN�������
extern void GUIDEMO_AfterLogo(void);

#if 1
// LOGOͼƬת������
extern const unsigned char gImage_camp[1308];

// ��ȡÿ�����ص��2�ֽ���ɫ��Ϣ
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

// д��һ������յ����꣬Ȼ��д����������(��ɫ��Ϣ)
void image_show(u16 xsta,u16 ysta,u16 xend,u16 yend,u8 scan,u8 *p)
{  
	u32 i;
	u32 len=0;
	
	BlockWrite(xsta,xend,ysta,yend);
	
	len=(xend-xsta+1)*(yend-ysta+1);	//д������ݳ���
	for(i=0;i<len;i++)
	{
		*(__IO u16 *) (Bank1_LCD_D) = image_getcolor(scan&(1<<4),p);
		p+=2;
	}	    					  	    
} 

void image_display(u16 x,u16 y,u8 * imgx)
{
	HEADCOLOR *imginfo;
 	u8 ifosize=sizeof(HEADCOLOR);//�õ�HEADCOLOR�ṹ��Ĵ�С
	imginfo=(HEADCOLOR*)imgx;
 	image_show(x,y,x+imginfo->w-1,y+imginfo->h-1,imginfo->scan,imgx+ifosize);
}

// ��ָ��λ�ÿ�ʼ��ʾһ��ͼƬ
void disp_img(u16 x, u16 y)
{
	image_display(x,y,(u8*)gImage_camp);//��ָ����ַ��ʾͼƬ
}
#endif

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();	
	
	delay_init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
 	LED_Init();			    //LED�˿ڳ�ʼ��
	Lcd_Initialize();
	
	// ������ɨ�跽ʽ����
	LCD_Display_Dir(HORIZON_DISPLAY);

	EC11_EXTI_Init();//EC11��������ʼ��
	my_mem_init(SRAMIN); 	//��ʼ���ڲ��ڴ��
	
	Adc_Init();
	Control_Init();

	PID_Init();
	
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
							 
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��
#if 1
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();  			//STemWin��ʼ��
#if 0
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )0,			//0
                (OS_TICK	 )10,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����
				
				
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);
	OSTmrStart(&tmr1,&err);	//������ʱ��1
	OSTmrStart(&tmr2,&err);	//������ʱ��2
#endif
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	// STemWin UI����	
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
	// EC11��������
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
	// �¶ȶ�ȡ����
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
	// RUN����
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
								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//��ʱ��1�Ļص����� 100ms
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
//	LCD_ShowxNum(62,111,tmr1_num,3,16,0x80); //��ʾ��ʱ��1��ִ�д���
//	LCD_Fill(6,131,114,313,lcd_discolor[tmr1_num%14]);//�������
	tmr1_num++;		//��ʱ��1ִ�д�����1*/
//	Check_Sys++;
}

//��ʱ��2�Ļص�����
void tmr2_callback(void *p_tmr,void *p_arg)
{
/*	static u8 tmr2_num = 0;
	tmr2_num++;		//��ʱ��2ִ�д�����1
	LCD_ShowxNum(182,111,tmr2_num,3,16,0x80);  //��ʾ��ʱ��1ִ�д���
	LCD_Fill(126,131,233,313,lcd_discolor[tmr2_num%14]); //�������
	LED1 = ~LED1;
	printf("��ʱ��2���н���\r\n");*/
}

// EMWIN�������
void emwindemo_task(void *p_arg)
{
	EVENT_VAL evt_val = g_event_val_last;
	TEMP_VAL temp_val = g_temp_val_last;
	
	// �ô��¶�Ӧ��Ϊ���¶Ȱ�����
	// ����¶Ȱ�û�ӻ��߶�������������Ϊ0���¶Ȼ���ʾΪ"-"
	// �ô�Ϊ��һ����ʾ��������temp_watch_task�и����¶���Ϣ����
	temp_val.temp1 = 0;
	temp_val.temp2 = 0;
	temp_val.temp3 = 0;
	temp_val.temp4 = 0;
	temp_val.temp5 = 0;
	
	// Ĭ��ֵ������������
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

// ÿ100ms��ɨ��һ��EC11������״̬
void touch_task(void *p_arg)
{
	OS_ERR err;
	
	while(1)
	{
		EC11_BUT_Scan();
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
	}
}

// ÿ��2�����һ���¶���Ϣ
// �ú����е��¶���ϢӦȫ����Ϊ���¶Ȱ���ȡ
void temp_watch_task(void *p_arg)
{
	OS_ERR err;
	TEMP_VAL temp_val = g_temp_val_last;
	
	// �ô��¶�Ӧ��Ϊ���¶Ȱ�����
	// ����¶Ȱ�û�ӻ��߶�������������Ϊ0���¶Ȼ���ʾΪ"-"
	temp_val.temp1 = 0;
	temp_val.temp2 = 0;
	temp_val.temp3 = 0;
	temp_val.temp4 = 0;
	temp_val.temp5 = 0;
	
	// Ĭ��ֵ������������
	temp_val.target_val = 450;
	
	while(1)
	{
		LED0 = !LED0;
		
		// �ô��¶�Ӧ��Ϊ���¶Ȱ�����
		// ����¶Ȱ�û�ӻ��߶�������������Ϊ0���¶Ȼ���ʾΪ"-"
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
		
		// ������ȫ�ֱ���
		g_temp_val_new.temp1 = temp_val.temp1;
		g_temp_val_new.temp2 = temp_val.temp2;
		g_temp_val_new.temp3 = temp_val.temp3;
		g_temp_val_new.temp4 = temp_val.temp4;
		g_temp_val_new.temp5 = temp_val.temp5;*/
		
		// ������ȫ�ֱ���
		g_temp_val_new.temp1 = (int)Get_Temperature(ADC_Channel_6);
		g_temp_val_new.temp2 = (int)Get_Temperature(ADC_Channel_5);
		g_temp_val_new.temp3 = (int)Get_Temperature(ADC_Channel_4);
		g_temp_val_new.temp4 = (int)Get_Temperature(ADC_Channel_3);
		g_temp_val_new.temp5 = (int)Get_Temperature(ADC_Channel_2);
		
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}

// ����ģʽ
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
		else{//ʲôģʽ������
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

