/*********************************************************************
*          Portions COPYRIGHT 2013 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2013  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.22 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO.c
Purpose     : Several GUIDEMO routines
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO.c
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    15-November-2013
  * @brief   Several GUIDEMO routines
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "GUIDEMO.h"
#include "common.h"
#include "ec11key.h"
#include "os.h"
#include "control.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontTradeGothicCn19;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontTradeGothicCn38;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontTradeGothicCn42;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontTradeGothicCn48;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontTradeGothicCn58;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontTradeGothicCn77;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontTradeGothicCn144;

/*******************************************************************
*
*       Static variables
*
********************************************************************
*/
// 多边形绘制向上箭头
static const GUI_POINT _aPointArrowUp[] = {
  {  0,  0 },
  { 8, 10 },
  { 2, 10 },
  { 2, 20 },
  {-2, 20 },
  {-2, 10 },
  {-8, 10 },
};
// 多边形绘制向下箭头
static const GUI_POINT _aPointArrowDown[] = {
  {  0,  0 },
  { -8, -10 },
  { -2, -10 },
  { -2, -20 },
  { 2, -20 },
  { 2, -10 },
  { 8, -10 },
};

// 多边形绘制向右箭头
static const GUI_POINT _aPointArrowRight[] = {
  {  0,  0 },
  { -20, -8 },
  { -20, -2 },
  { -40, -2 },
  { -40, 2 },
  { -20, 2 },
  { -20, 8 },
};

static const GUI_POINT _aPointArrowProgressLeft[] = {
  { 0,   0},// X:0 Y:0
  { 10, -5},// X:right Y:up 
  { 10,  5},// X:right Y:down
};

static const GUI_POINT _aPointArrowProgressRight[] = {
  { 0,   0},// X:0 Y:0
  { -10, 5},// X:right Y:up 
  { -10, -5},// X:right Y:down
};

// 多边形绘制圆圈四周的右上角位置的箭头
static const GUI_POINT _aPointArrowRightUp[] = {
  {  0,  0  },
  {  -10, 10},
  {  -15, -5  },
};

// 多边形绘制圆圈四周的右下角位置的箭头
static const GUI_POINT _aPointArrowRightDown[] = {
  {  0,  0  },
  {  -15, 5},
  {  -10, -10  },
};

// <屏幕操作说明温度.docx>中所述的1.1界面的所有项目
static char * SubMenuTextAll[] =
{
	"HIGH",// 500°
	"495°F",
	"490°F",

	"185°F",
	
	"180°F",
	"175°F",
	"HIGH SMOKE",// 160°
	"LOW SMOKE",// 160°
	"FEED",
	"SHUTDOWN",
	"- - -",
	"SETUP",
	"EXIT",
};
/*
HIGH HIGH
475  495
450  490
425  485// center
400  480
375  475

175  195
200  190
225  185 // center
200  180
175  175
*/
static char * SubMenuTextAllC[] =
{
	"HIGH",// 500°
	"495°C",
	"490°C",
	"185°C",// N times
	"180°C",
	"175°C",
	"HIGH SMOKE",// 160°
	"LOW SMOKE",// 160°
	"FEED",
	"SHUTDOWN",
	"- - -",
	"SETUP",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的2.2界面的所有项目
static char * SetupText[] =
{
	"FACTORY RESET",
	"TEMPERATURE UNIT:F",
	"INVERT SCREEN",
	"COLOR",
	"CUSTOMER SERVICE",
	"EXIT",
};

static char * SetupTextC[] =
{
	"FACTORY RESET",
	"TEMPERATURE UNIT:C",
	"INVERT SCREEN",
	"COLOR",
	"CUSTOMER SERVICE",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的2.4界面的所有项目
static const char * SetupColorText[] =
{
	"BLUE",
	"GREEN",
	"RED",
	"BLACK",
	"WHITE",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的2.3界面的所有项目
static const char * SetupServiceText[] =
{
	"Camp Chef",
	"1-800-650-2433",
	"M-Th 7 am to 5 pm MST",
	"support@CampChef.com",
	"www.CampChef.com",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的1.2界面的所有项目
static const char * HelpText[] =
{
	"Turn knob left or right to",
	"highlight menu.",
	"Press knob to select.",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的3.3界面的所有项目
static const char * ShutdownModeText[] =
{
	"SHUTDOWN MODE",
	"15:00 REMAINING",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的4.3界面的所有项目
static const char * FeedModeText[] =
{
	"FEED MODE",
	"08:00 REMAINING",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的5.3界面的所有项目
static const char * StartupModeText[] =
{
	"STARTUP MODE",
	"10:00 REMAINING",
	"EXIT",
};

static const char * FactoryResetText[] =
{
	"FACTORY RESET",
	"YES",
	"NO",
};

// <屏幕操作说明温度.docx>中所述的4.2界面的所有项目
static const char * FeedHelpText[] =
{
	"Only use \"FEED\" if hopper",
	"and auger are empty.",
	"(also for first startup cycle)",
	"Push and hold knob for three",
	"seconds to begin.",
	"EXIT",
};

// <屏幕操作说明温度.docx>中所述的1.1界面中所有温度选项
// 选择成功后，该值会成为RUN界面的Target值
const short baseTempMax[] =
{
	500, // HIGH
	400,
	375,
	350,
	325,
	300,
	275,
	250,
	225,
	200,
	175,
	500, // HIGH SMOKE
	160, // LOW SMOKE
};

// <屏幕操作说明温度.docx>中所述的1.1界面的X坐标信息
// 一共有5个显示位置，通过旋钮切换共有18个选择
const short SubMenuPosArray[13][5] = 
{
	{0,  0,  140,145,145},
	{0,  150,130,145,145},
	{150,145,130,145,145},
	{145,145,130,145,145},
	{145,145,130,145,103},
	{145,145,130,103,107},
	{145,145,78, 107,155},
	{145,103,82, 155,115},
	{103,107,140,115,159},
	{107,155,85, 159,147},
	{155,115,153,147,158},
	{115,159,130,158,0  },
	{159,147,150,0,  0  },
};

// <屏幕操作说明温度.docx>中所述的3.3界面的X坐标信息
// 一共有5个显示位置，通过旋钮切换共有3个选择
const short ShutdownModePosArray[3][5] = 
{
	{0,  0,  90, 135,225},
	{0,  130,90, 220,0  },
	{130,130,210,0,  0  },
};

// <屏幕操作说明温度.docx>中所述的4.3界面的X坐标信息
// 一共有5个显示位置，通过旋钮切换共有3个选择
const short FeedModePosArray[3][5] = 
{
	{0,  0,  150, 135,225},
	{0,  170,90, 220,0  },
	{170,130,210,0,  0  },
};

// <屏幕操作说明温度.docx>中所述的5.3界面的X坐标信息
// 一共有5个显示位置，通过旋钮切换共有3个选择
const short StartupModePosArray[3][5] = 
{
	{0,  0,  150, 135,225},
	{0,  145,90, 215,0  },
	{145,130,210, 0,  0  },
};

const short FactoryResetPosArray[3][5] = 
{
	{0,  0,  150, 135,225},
	{0,  140,210, 225,0  },
	{140,220,220, 0,  0  },
};

// <屏幕操作说明温度.docx>中所述的2.2界面的X坐标信息
// 一共有7个显示位置，通过旋钮切换共有4个选择
const short SetupPosArray[6][7] = 
{
	{0,  0,  0,  105, 100,140,200},
	{0,  0,  140,58, 140,200,115},
	{0,  140,100,105, 200,115,215},
	{140,100,140,190,115,215,0  },
	{100,140,200,65, 215,0,  0  },
	{140,200,115,205,0,  0,  0  },
};

// <屏幕操作说明温度.docx>中所述的2.4界面的X坐标信息
// 一共有6个显示位置，通过旋钮切换共有6个选择
const short SetupColorPosArray[6][6] = 
{
	{0,  0,  202,205,222,205},
	{0,  220,190,222,205,205},
	{213,205,213,205,205,220},
	{205,222,190,205,215,0  },
	{222,205,185,218,0,  0  },
	{205,205,206,0,  0,  0  },
};

// 进度条橙色
#define USER_GRILL_COLOR   0x00285AEF

#define USER_COLOR1_RED   	0x002A2AA8
#define USER_COLOR2_BLACK  	0x00262A2D
#define USER_COLOR3_GRAY   	0x00454343
#define USER_COLOR4_GRAY   	0x0073716F
#define USER_COLOR5_GRAY   	0x00818181
#define USER_COLOR6_GRAY   	0x00C6C4C2
#define USER_COLOR7_GRAY   	0x00E8E7E6

EVENT_VAL g_event_val_new;
EVENT_VAL g_event_val_last;

TEMP_VAL g_temp_val_new;
TEMP_VAL g_temp_val_last;

// 0-ok 1-FLAME ERROR 2-SENSOR ERROR
int g_fatal_error = 0;

// 0-end 1-start
int g_startup_mode = 0;
int g_run_mode = 0;
int g_feed_mode = 0;
int g_shutdown_mode = 0;

// RUN模式倒计时，时间设置以及使能
int g_run_timer_setting = 480;// 480 seconds

// 0-OK 1-温度棒异常
int g_temp1_error = 0;
int g_temp2_error = 0;
int g_temp3_error = 0;
int g_temp4_error = 0;
int g_temp5_error = 0;

// default 50%
int g_smoke_val_percent = 50;

// 表示是否初始化
// 0-未初始化, 1-已初始化
int g_factory_reseted = 0;

// 进入startup或者run标志
// 0-run, 1-startup mode
int g_startup_enable = 1;

// 蓝色字体的颜色是可以切换的
GUI_COLOR font_color = GUI_BLACK;

// 背景色可在黑白之间切换，默认为黑色
GUI_COLOR bk_color = GUI_WHITE;

// 被选择的项目要高亮显示的颜色
GUI_COLOR highlight_color = GUI_WHITE;

int g_temp_center = 0;

void GUIDEMO_SetupUI(void);
void GUIDEMO_SetupColorUI(void);
void GUIDEMO_SubMenu(void);
void GUIDEMO_MainUI(void);
void GUIDEMO_SetupServiceUI(void);
void GUIDEMO_HelpUI(void);
void GUIDEMO_ShutdownHelpUI(void);
void GUIDEMO_ShutdownModeUI(void);
void GUIDEMO_FeedHelpUI(void);
void GUIDEMO_FeedModeUI(void);
void GUIDEMO_LowSmokeUI(void);
void GUIDEMO_StartupModeUI(void);
void GUIDEMO_OtherTempModeUI(void);
void GUIDEMO_AfterLogo(void);

extern const unsigned char gImage_p1_leftdown[1388];
extern const unsigned char gImage_p1_rightdown[1388];

extern u16 g_run_mode_counter_mins;// minutes
extern u16 g_run_mode_counter_sec;// seconds

extern void image_display(u16 x,u16 y,u8 * imgx);
extern void GUIDEMO_TempMenu(void);
extern void GUIDEMO_SmokeMenu(void);
extern void GUIDEMO_ModeMenu(void);

void GUIDEMO_FactoryResetUI(void)
{
	OS_ERR err;
	int time_remain = 100;
	int menu_index = 2;
	int menu_index_last = 0;
	EC11_STA ec_sta = EC11_IDLE;
	
	int menu_index_pos1 = 0;
	int menu_index_pos2 = 0;
	int menu_index_pos3 = 0;
	int menu_index_pos4 = 0;
	int menu_index_pos5 = 0;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	do
	{	
		menu_index_pos1 = 0;
		menu_index_pos2 = 0;
		menu_index_pos3 = 0;
		menu_index_pos4 = 0;
		menu_index_pos5 = 0;
		
		// menu switch
		if (menu_index != menu_index_last) {
			if (menu_index >= 3) {
				menu_index_pos1 = menu_index - 2;
			}
			
			if (menu_index >= 2) {
				menu_index_pos2 = menu_index - 1;
			}
			
			menu_index_pos3 = menu_index;
			
			if (menu_index <= 2) {
				menu_index_pos4 = menu_index + 1;
			}

			if (menu_index <= 1) {
				menu_index_pos5 = menu_index + 2;
			}
			
			GUI_SetColor(bk_color);
			GUI_SetBkColor(bk_color);
	
			// display text
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			GUI_DispStringAt("                                                    ", 90, 40);
			if (menu_index_pos1 > 0) {
				GUI_DispStringAt(FactoryResetText[menu_index_pos1-1], FactoryResetPosArray[menu_index-1][0], 40);
			}
			GUI_DispStringAt("                                                    ", 90, 90);
			if (menu_index_pos2 > 0) {
				GUI_DispStringAt(FactoryResetText[menu_index_pos2-1], FactoryResetPosArray[menu_index-1][1], 90);
			}

			GUI_SetColor(USER_COLOR1_RED);
			GUI_FillRoundedRect(125, 135, 355, 200, 10);
	
			GUI_SetColor(highlight_color);
			GUI_SetBkColor(USER_COLOR1_RED);
			GUI_SetFont(&GUI_FontTradeGothicCn58);
			GUI_DispStringAt("              ", 140, 140);
			if (menu_index_pos3 > 0) {
				GUI_DispStringAt(FactoryResetText[menu_index_pos3-1], FactoryResetPosArray[menu_index-1][2], 140);
			}
			
			GUI_SetColor(font_color);
			GUI_SetBkColor(bk_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			GUI_DispStringAt("                                    ", 90, 200);
			if (menu_index_pos4 > 0) {
				GUI_DispStringAt(FactoryResetText[menu_index_pos4-1], FactoryResetPosArray[menu_index-1][3], 200);
			}
			GUI_DispStringAt("                                ", 90, 240);
			if (menu_index_pos5 > 0) {
				GUI_DispStringAt(FactoryResetText[menu_index_pos5-1], FactoryResetPosArray[menu_index-1][4], 240);
			}
		}
		
		menu_index_last = menu_index;
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			if (menu_index > 2) {
				menu_index--;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			if (menu_index < 3) {
				menu_index++;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT == ec_sta) {
			if (2 == menu_index) {
				ec_sta = EC11_IDLE;
			}
		} else if (EC11_BUT_LONG == ec_sta) {
			if (3 == menu_index) {
				ec_sta = EC11_IDLE;
			}
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			time_remain = 100;
		}
		
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
	} while(EC11_IDLE == ec_sta);
	
//	if (EC11_BUT == ec_sta) {
		if (2 == menu_index) {
			bk_color = GUI_WHITE;
			font_color = GUI_BLACK;
			g_factory_reseted = 1;
			
			g_event_val_new.force_flush = 1;
			g_event_val_new.menu_index = 11;
			
			GUIDEMO_SubMenu();
		} else if (3 == menu_index) {
			GUIDEMO_SetupUI();
		}
//	}
}

void GUIDEMO_FeedCmpUI(void)
{
	EC11_STA ec_sta = EC11_IDLE;

	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	GUI_SetBkColor(bk_color);
	
	GUI_SetColor(font_color);
	GUI_SetBkColor(bk_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt("FEED COMPLETE", 130, 130);
	
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(320, 235, 440, 312, 10);
	
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_SetColor(highlight_color);
	GUI_SetFont(&GUI_FontTradeGothicCn77);
	GUI_DispStringAt("EXIT", 333, 235);
	
	do
	{
		MOT = Control_OFF;		//防止FEED MODE结束没有关闭控制器
		HOT = Control_OFF;
		FAN = Control_OFF;
		
		ec_sta = EC11_KEY_Scan(0);
		
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if (EC11_BUT == ec_sta) {
		g_event_val_new.force_flush = 1;
		g_event_val_new.menu_index = 11;
		
		GUIDEMO_SubMenu();
	}
}

void GUIDEMO_ShutdownCmpUI(void)
{
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	GUI_SetColor(font_color);
	GUI_SetBkColor(bk_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt("SHUTDOWN COMPLETE", 78, 130);
	GUI_DispStringAt("CYCLE POWER TO RESET", 70, 170);

	do
	{
		MOT = Control_OFF;		//防止关机模式结束后没有关闭3个控制器
		HOT = Control_OFF;
		FAN = Control_OFF;

		delay_ms(100);

	} while(1);
}

// <屏幕操作说明温度.docx>中所述的7.3/8.2界面的所有项目
void GUIDEMO_OtherTempModeUI(void)
{
	int progress_offset = 0;
	int progress_offset_last = 0;
	int smoke_setting = 50;// 50%
	int smoke_setting_last = 0;// 50%
	int time_remain = 100;
	char dispStr[64] = "";	
	EC11_STA ec_sta = EC11_IDLE;
	TEMP_VAL temp_val = g_temp_val_new;

	if (g_smoke_val_percent != 0) {
		smoke_setting = g_smoke_val_percent;
	}
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	GUI_SetBkColor(bk_color);
	
	// display text
	GUI_SetColor(font_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	if (0 == g_temp_val_new.temp_unit) {
		sprintf(dispStr, "Target %.3d°F", temp_val.target_val);
	} else {
		sprintf(dispStr, "Target %.3d°C", temp_val.target_val);
	}
	GUI_DispStringAt(dispStr, 30, 10);
	GUI_DispStringAt("Smoke Select:", 30, 60);

	GUI_DrawCircle(310,50,35);
	GUI_DrawArc(315,50,50,50,-30,30);
	GUI_FillPolygon (&_aPointArrowRightUp[0], 3, 363, 20);
	GUI_FillPolygon (&_aPointArrowRightDown[0], 3, 365, 75);
	
	GUI_SetColor(USER_COLOR4_GRAY);
	GUI_FillRect(100, 110, 247, 140);
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRect(247, 110, 400, 140);
	
	image_display(100, 110, (u8*)gImage_p1_leftdown);
	image_display(380, 110, (u8*)gImage_p1_rightdown);
	
	GUI_SetColor(font_color);
	GUI_DispStringAt("Start:", 30, 180);
	
	GUI_DrawEllipse(210,200,12,30);
	GUI_DrawArc(200,200,50,50,-35,35);
	GUI_DrawLine(210,170,240,170);
	GUI_DrawLine(210,230,240,230);
	GUI_FillPolygon (&_aPointArrowRight[0], 7, 180, 200);
	
	GUI_SetFont(&GUI_FontTradeGothicCn38);
	GUI_DispStringAt("Push", 130, 150);
	
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt("3 Sec", 270, 180);
	
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(320, 235, 440, 312, 10);
	
	GUI_SetColor(highlight_color);
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_SetFont(&GUI_FontTradeGothicCn77);
	GUI_DispStringAt("EXIT", 333, 235);
	
	do
	{		
		if (smoke_setting_last != smoke_setting) {
			progress_offset = 225 - (225 * smoke_setting / 100);
			GUI_SetColor(USER_COLOR4_GRAY);
			
			if (progress_offset > 215) {
				progress_offset = 215;
			}
				
			if (progress_offset_last != progress_offset) {
				GUI_SetColor(USER_COLOR4_GRAY);
				GUI_FillRect(125, 110, 135+progress_offset, 140);
				GUI_SetColor(USER_COLOR1_RED);
				GUI_FillRect(135+progress_offset, 110, 370, 140);
				
				GUI_SetColor(GUI_WHITE);
				
				GUI_DrawLine(135+progress_offset, 110, 135+progress_offset, 140);
				GUI_FillPolygon (&_aPointArrowProgressLeft[0], 3, 135+progress_offset-20, 125);
				GUI_FillPolygon (&_aPointArrowProgressRight[0], 3, 135+progress_offset+20, 125);
			}
		}
		
		smoke_setting_last = smoke_setting;
		
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_RIGHT == ec_sta) {
			if (smoke_setting < 100) {
				smoke_setting +=5;
			}
			time_remain = 100;
			ec_sta = EC11_IDLE;
		} else if (EC11_LEFT == ec_sta) {
			if (smoke_setting > 0) {
				smoke_setting -=5;
			}
			time_remain = 100;
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	g_smoke_val_percent = smoke_setting;
	
	if ((EC11_BUT == ec_sta) || (0 == time_remain)) {
		g_event_val_new.force_flush = 1;
		g_event_val_new.menu_index = 11;
		
		GUIDEMO_SubMenu();
	} else if (EC11_BUT_LONG == ec_sta) {
		if (1 == g_startup_enable) {
			GUIDEMO_StartupModeUI();
		} else {
			GUIDEMO_MainUI();
		}
	}
}

// <屏幕操作说明温度.docx>中所述的5.3界面的所有项目
void GUIDEMO_StartupModeUI(void)
{
	OS_ERR err;
	char dispStr[64] = "";
	int time_remain = 1 * 60 * 10;// 10 minutes
	int menu_index = 2;
	int menu_index_last = 0;
	EC11_STA ec_sta = EC11_IDLE;
	
	int menu_index_pos1 = 0;
	int menu_index_pos2 = 0;
	int menu_index_pos3 = 0;
	int menu_index_pos4 = 0;
	int menu_index_pos5 = 0;
	
	#if FOR_DEBUG_USE
		time_remain /= 60;
	#endif
	
	g_startup_mode = 1;
	g_run_mode = 0;
	g_run_mode_counter_mins = 0;
	
	// can enter startup mode only once
	g_startup_enable = 0;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	do
	{
		sprintf(dispStr,"%.2d:%.2d REMAINING", time_remain/600, (time_remain%600)/10);
		
		menu_index_pos1 = 0;
		menu_index_pos2 = 0;
		menu_index_pos3 = 0;
		menu_index_pos4 = 0;
		menu_index_pos5 = 0;
		
		// menu switch
		if (menu_index != menu_index_last) {
			if (menu_index >= 3) {
				menu_index_pos1 = menu_index - 2;
			}
			
			if (menu_index >= 2) {
				menu_index_pos2 = menu_index - 1;
			}
			
			menu_index_pos3 = menu_index;
			
			if (menu_index <= 2) {
				menu_index_pos4 = menu_index + 1;
			}

			if (menu_index <= 1) {
				menu_index_pos5 = menu_index + 2;
			}
			
			GUI_SetColor(bk_color);
			GUI_SetBkColor(bk_color);
	
			// display text
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			GUI_DispStringAt("                                                    ", 90, 40);
			if (menu_index_pos1 > 0) {
				if (menu_index_pos1 == 2) {
					GUI_DispStringAt(dispStr, StartupModePosArray[menu_index-1][0], 40);
				} else {
					GUI_DispStringAt(StartupModeText[menu_index_pos1-1], StartupModePosArray[menu_index-1][0], 40);
				}
			}
			GUI_DispStringAt("                                                    ", 90, 90);
			if (menu_index_pos2 > 0) {
				if (menu_index_pos2 == 2) {
					GUI_DispStringAt(dispStr, StartupModePosArray[menu_index-1][1], 90);
				} else {
					GUI_DispStringAt(StartupModeText[menu_index_pos2-1], StartupModePosArray[menu_index-1][1], 90);
				}
			}

			GUI_SetColor(USER_COLOR1_RED);
			GUI_FillRoundedRect(25, 135, 455, 200, 10);
	
			GUI_SetColor(highlight_color);
			GUI_SetBkColor(USER_COLOR1_RED);
			GUI_SetFont(&GUI_FontTradeGothicCn58);
			GUI_DispStringAt("                                ", 70, 140);
			if (menu_index_pos3 > 0) {
				if (menu_index_pos3 == 2) {
					GUI_DispStringAt(dispStr, StartupModePosArray[menu_index-1][2], 140);
				} else {
					GUI_DispStringAt(StartupModeText[menu_index_pos3-1], StartupModePosArray[menu_index-1][2], 140);
				}
			}
			
			GUI_SetColor(font_color);
			GUI_SetBkColor(bk_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			GUI_DispStringAt("                                    ", 90, 200+5);
			if (menu_index_pos4 > 0) {
				if (menu_index_pos4 == 2) {
					GUI_DispStringAt(dispStr, StartupModePosArray[menu_index-1][3], 200+5);
				} else {
					GUI_DispStringAt(StartupModeText[menu_index_pos4-1], StartupModePosArray[menu_index-1][3], 200+5);
				}
			}
			GUI_DispStringAt("                                ", 90, 240);
			if (menu_index_pos5 > 0) {
				if (menu_index_pos5 == 2) {
					GUI_DispStringAt(dispStr, StartupModePosArray[menu_index-1][4], 240);
				} else {
					GUI_DispStringAt(StartupModeText[menu_index_pos5-1], StartupModePosArray[menu_index-1][4], 240);
				}
			}
			
			// for update time remaining only
			if (2 == menu_index) {
				GUI_SetColor(highlight_color);
				GUI_SetFont(&GUI_FontTradeGothicCn58);
			} else if (3 == menu_index) {
				GUI_SetColor(font_color);
				GUI_SetFont(&GUI_FontTradeGothicCn42);
			}
		} else {
			if (0 == time_remain%10) {
				if (2 == menu_index) {
					GUI_SetBkColor(USER_COLOR1_RED);
					GUI_DispStringAt(dispStr, StartupModePosArray[menu_index-1][2], 140);
				} else if (3 == menu_index) {
					GUI_DispStringAt(dispStr, StartupModePosArray[menu_index-1][1], 90);
				}
			}
		}
		
		menu_index_last = menu_index;
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			if (menu_index > 2) {
				menu_index--;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			if (menu_index < 3) {
				menu_index++;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT == ec_sta) {
			if (2 == menu_index) {
				ec_sta = EC11_IDLE;
			}
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			g_run_mode=1;//执行完点火模式才能进入运行模式
			break;
		}
		
		if (2 == menu_index) {// draw big font(GUI_FontTradeGothicCn58) need more time, so delay less time
			OSTimeDlyHMSM(0,0,0,90,OS_OPT_TIME_PERIODIC,&err);
		} else if (3 == menu_index) {
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
		}
	} while(EC11_IDLE == ec_sta);
	
	g_startup_mode = 0;
	
	if (EC11_BUT == ec_sta) {
		g_event_val_new.force_flush = 1;
		
		GUIDEMO_SubMenu();
	}
	if (0 == time_remain) {
		GUIDEMO_MainUI();
	}
}

// <屏幕操作说明温度.docx>中所述的5.2界面的所有项目
void GUIDEMO_LowSmokeUI(void)
{
	int time_remain = 100;
	EC11_STA ec_sta = EC11_IDLE;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	GUI_SetBkColor(bk_color);
	
	// display text
	GUI_SetColor(font_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	if (0 == g_temp_val_new.temp_unit) {
		GUI_DispStringAt("Target: 160°F", 30, 10);
	} else {
		GUI_DispStringAt("Target: 160°C", 30, 10);
	}
	
	GUI_DispStringAt("Smoke: 50%", 30, 60);

	GUI_SetColor(USER_COLOR4_GRAY);
	GUI_FillRect(100, 110, 247, 140);
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRect(247, 110, 400, 140);
	
	image_display(100, 110, (u8*)gImage_p1_leftdown);
	image_display(380, 110, (u8*)gImage_p1_rightdown);
	
	GUI_SetColor(font_color);
	GUI_DispStringAt("Start:", 30, 180);
	
	GUI_DrawEllipse(210,200,12,30);
	GUI_DrawArc(200,200,50,50,-35,35);
	GUI_DrawLine(210,170,240,170);
	GUI_DrawLine(210,230,240,230);
	GUI_FillPolygon (&_aPointArrowRight[0], 7, 180, 200);
	
	GUI_SetFont(&GUI_FontTradeGothicCn38);
	GUI_DispStringAt("Push", 130, 150);
	
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt("3 Sec", 270, 180);
	
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(320, 235, 440, 312, 10);
	
	GUI_SetColor(highlight_color);
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_SetFont(&GUI_FontTradeGothicCn77);
	GUI_DispStringAt("EXIT", 333, 235);
	
	do
	{
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if ((EC11_BUT == ec_sta) || (0 == time_remain)) {
		g_event_val_new.force_flush = 1;
		g_event_val_new.menu_index = 11;
		
		GUIDEMO_SubMenu();
	} else if (EC11_BUT_LONG == ec_sta) {
		if (1 == g_startup_enable) {
			GUIDEMO_StartupModeUI();
		} else {
			GUIDEMO_MainUI();
		}
	}
}

// <屏幕操作说明温度.docx>中所述的4.3界面的所有项目
void GUIDEMO_FeedModeUI(void)
{
	OS_ERR err;
	char dispStr[64] = "";
	int time_remain = 1 * 60 * 10;// 8 minutes
	int menu_index = 2;
	int menu_index_last = 0;
	EC11_STA ec_sta = EC11_IDLE;
	
	int menu_index_pos1 = 0;
	int menu_index_pos2 = 0;
	int menu_index_pos3 = 0;
	int menu_index_pos4 = 0;
	int menu_index_pos5 = 0;
	
	#if FOR_DEBUG_USE
		time_remain /= 60;
	#endif
	
	g_feed_mode = 1;
	g_run_mode = 0;
	g_run_mode_counter_mins = 0;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	do
	{
		sprintf(dispStr,"%.2d:%.2d REMAINING", time_remain/600, (time_remain%600)/10);

		menu_index_pos1 = 0;
		menu_index_pos2 = 0;
		menu_index_pos3 = 0;
		menu_index_pos4 = 0;
		menu_index_pos5 = 0;
		
		// menu switch
		if (menu_index != menu_index_last) {
			if (menu_index >= 3) {
				menu_index_pos1 = menu_index - 2;
			}
			
			if (menu_index >= 2) {
				menu_index_pos2 = menu_index - 1;
			}
			
			menu_index_pos3 = menu_index;
			
			if (menu_index <= 2) {
				menu_index_pos4 = menu_index + 1;
			}

			if (menu_index <= 1) {
				menu_index_pos5 = menu_index + 2;
			}
			
			GUI_SetColor(bk_color);
			GUI_SetBkColor(bk_color);
	
			// display text
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			GUI_DispStringAt("                                                    ", 90, 40);
			if (menu_index_pos1 > 0) {
				if (menu_index_pos1 == 2) {
					GUI_DispStringAt(dispStr, FeedModePosArray[menu_index-1][0], 40);
				} else {
					GUI_DispStringAt(FeedModeText[menu_index_pos1-1], FeedModePosArray[menu_index-1][0], 40);
				}
			}
			GUI_DispStringAt("                                                    ", 90, 90);
			if (menu_index_pos2 > 0) {
				if (menu_index_pos2 == 2) {
					GUI_DispStringAt(dispStr, FeedModePosArray[menu_index-1][1], 90);
				} else {
					GUI_DispStringAt(FeedModeText[menu_index_pos2-1], FeedModePosArray[menu_index-1][1], 90);
				}
			}

			GUI_SetColor(USER_COLOR1_RED);
			GUI_FillRoundedRect(25, 135, 455, 200, 10);
			GUI_SetBkColor(USER_COLOR1_RED);
			GUI_SetColor(highlight_color);
			GUI_SetFont(&GUI_FontTradeGothicCn58);
			GUI_DispStringAt("                                ", 70, 140);
			if (menu_index_pos3 > 0) {
				if (menu_index_pos3 == 2) {
					GUI_DispStringAt(dispStr, FeedModePosArray[menu_index-1][2], 140);
				} else {
					GUI_DispStringAt(FeedModeText[menu_index_pos3-1], FeedModePosArray[menu_index-1][2], 140);
				}
			}
			
			GUI_SetColor(font_color);
			GUI_SetBkColor(bk_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			GUI_DispStringAt("                                    ", 90, 200+5);
			if (menu_index_pos4 > 0) {
				if (menu_index_pos4 == 2) {
					GUI_DispStringAt(dispStr, FeedModePosArray[menu_index-1][3], 200+5);
				} else {
					GUI_DispStringAt(FeedModeText[menu_index_pos4-1], FeedModePosArray[menu_index-1][3], 200+5);
				}
			}
			GUI_DispStringAt("                                ", 90, 240);
			if (menu_index_pos5 > 0) {
				if (menu_index_pos5 == 2) {
					GUI_DispStringAt(dispStr, FeedModePosArray[menu_index-1][4], 240);
				} else {
					GUI_DispStringAt(FeedModeText[menu_index_pos5-1], FeedModePosArray[menu_index-1][4], 240);
				}
			}
			
			// for update time remaining only
			if (2 == menu_index) {
				GUI_SetColor(highlight_color);
				GUI_SetFont(&GUI_FontTradeGothicCn58);
			} else if (3 == menu_index) {
				GUI_SetColor(font_color);
				GUI_SetFont(&GUI_FontTradeGothicCn42);
			}
		} else {
			if (0 == time_remain%10) {
				if (2 == menu_index) {
					GUI_SetBkColor(USER_COLOR1_RED);
					GUI_DispStringAt(dispStr, FeedModePosArray[menu_index-1][2], 140);
				} else if (3 == menu_index) {
					GUI_DispStringAt(dispStr, FeedModePosArray[menu_index-1][1], 90);
				}
			}
		}
		
		menu_index_last = menu_index;
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHTj
		if (EC11_LEFT == ec_sta) {
			if (menu_index > 2) {
				menu_index--;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			if (menu_index < 3) {
				menu_index++;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT == ec_sta) {
			if (2 == menu_index) {
				ec_sta = EC11_IDLE;
			}
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		if (2 == menu_index) {// draw big font(GUI_FontTradeGothicCn58) need more time, so delay less time
			OSTimeDlyHMSM(0,0,0,90,OS_OPT_TIME_PERIODIC,&err);
		} else if (3 == menu_index) {
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
		}
	} while(EC11_IDLE == ec_sta);
	
	g_feed_mode = 0;
	
	if (EC11_BUT == ec_sta) {
		g_event_val_new.force_flush = 1;
		g_event_val_new.menu_index = 11;
		
		GUIDEMO_SubMenu();
	}
	
	if (0 == time_remain) {
		GUIDEMO_FeedCmpUI();
	}
}

// <屏幕操作说明温度.docx>中所述的4.2界面的所有项目
void GUIDEMO_FeedHelpUI(void)
{
	int time_remain = 100;
	EC11_STA ec_sta = EC11_IDLE;

	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	GUI_SetBkColor(bk_color);
	
	// display text
	GUI_SetColor(font_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt(FeedHelpText[0], 30, 10);
	GUI_DispStringAt(FeedHelpText[1], 30, 55);
	GUI_DispStringAt(FeedHelpText[2], 30, 100);
	GUI_DispStringAt(FeedHelpText[3], 30, 145);
	GUI_DispStringAt(FeedHelpText[4], 30, 190);
	
	GUI_DrawEllipse(100,270,12,30);
	GUI_DrawArc(90,270,50,50,-35,35);
	GUI_DrawLine(100,240,130,240);
	GUI_DrawLine(100,300,130,300);
	GUI_FillPolygon (&_aPointArrowRight[0], 7, 80, 270);
	
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(320, 235, 440, 312, 10);
	
	GUI_SetColor(highlight_color);
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_SetFont(&GUI_FontTradeGothicCn77);
	GUI_DispStringAt(HelpText[3], 333, 235);
	
	do
	{
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if ((EC11_BUT == ec_sta) || (0 == time_remain)) {
		g_event_val_new.force_flush = 1;
		
		GUIDEMO_SubMenu();
	} else if (EC11_BUT_LONG == ec_sta) {
		GUIDEMO_FeedModeUI();
	}
}

// <屏幕操作说明温度.docx>中所述的3.3界面的所有项目
void GUIDEMO_ShutdownModeUI(void)
{
	OS_ERR err;
	int time_remain = 1 * 60 * 10;// 15 minutes
	int menu_index = 2;
	int menu_index_last = 0;
	char dispStr[64] = "";
	EC11_STA ec_sta = EC11_IDLE;
	
	int menu_index_pos1 = 0;
	int menu_index_pos2 = 0;
	int menu_index_pos3 = 0;
	int menu_index_pos4 = 0;
	int menu_index_pos5 = 0;	
	
	#if FOR_DEBUG_USE
		time_remain /= 60;
	#endif
	
	g_shutdown_mode = 1;
	g_run_mode = 0;
	g_run_mode_counter_mins = 0;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	do
	{
		sprintf(dispStr,"%.2d:%.2d REMAINING", time_remain/600, (time_remain%600)/10);
		
		menu_index_pos1 = 0;
		menu_index_pos2 = 0;
		menu_index_pos3 = 0;
		menu_index_pos4 = 0;
		menu_index_pos5 = 0;
		
		// menu switch
		if (menu_index != menu_index_last) {
			if (menu_index >= 3) {
				menu_index_pos1 = menu_index - 2;
			}
			
			if (menu_index >= 2) {
				menu_index_pos2 = menu_index - 1;
			}
			
			menu_index_pos3 = menu_index;
			
			if (menu_index <= 2) {
				menu_index_pos4 = menu_index + 1;
			}

			if (menu_index <= 1) {
				menu_index_pos5 = menu_index + 2;
			}
			
			GUI_SetColor(bk_color);
			GUI_SetBkColor(bk_color);
			
			// display text
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			GUI_DispStringAt("                                                    ", 90, 40);
			if (menu_index_pos1 > 0) {
				if (menu_index_pos1 == 2) {
					GUI_DispStringAt(dispStr, ShutdownModePosArray[menu_index-1][0], 40);
				} else {
					GUI_DispStringAt(ShutdownModeText[menu_index_pos1-1], ShutdownModePosArray[menu_index-1][0], 40);
				}
			}
			GUI_DispStringAt("                                                    ", 90, 90);
			if (menu_index_pos2 > 0) {
				if (menu_index_pos2 == 2) {
					GUI_DispStringAt(dispStr, ShutdownModePosArray[menu_index-1][1], 90);
				} else {
					GUI_DispStringAt(ShutdownModeText[menu_index_pos2-1], ShutdownModePosArray[menu_index-1][1], 90);
				}
			}

			GUI_SetColor(USER_COLOR1_RED);
			GUI_FillRoundedRect(25, 135, 455, 200, 10);
			GUI_SetBkColor(USER_COLOR1_RED);
			GUI_SetColor(highlight_color);
			GUI_SetFont(&GUI_FontTradeGothicCn58);
			GUI_DispStringAt("                                ", 70, 140);
			if (menu_index_pos3 > 0) {
				if (menu_index_pos3 == 2) {
					GUI_DispStringAt(dispStr, ShutdownModePosArray[menu_index-1][2], 140);
				} else {
					GUI_DispStringAt(ShutdownModeText[menu_index_pos3-1], ShutdownModePosArray[menu_index-1][2], 140);
				}
			}
			
			GUI_SetColor(font_color);
			GUI_SetBkColor(bk_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			GUI_DispStringAt("                                    ", 90, 200+5);
			if (menu_index_pos4 > 0) {
				if (menu_index_pos4 == 2) {
					GUI_DispStringAt(dispStr, ShutdownModePosArray[menu_index-1][3], 200+5);
				} else {
					GUI_DispStringAt(ShutdownModeText[menu_index_pos4-1], ShutdownModePosArray[menu_index-1][3], 200+5);
				}
			}
			GUI_DispStringAt("                                ", 90, 240);
			if (menu_index_pos5 > 0) {
				if (menu_index_pos5 == 2) {
					GUI_DispStringAt(dispStr, ShutdownModePosArray[menu_index-1][4], 240);
				} else {
					GUI_DispStringAt(ShutdownModeText[menu_index_pos5-1], ShutdownModePosArray[menu_index-1][4], 240);
				}
			}
			
			// for update time remaining only
			if (2 == menu_index) {
				GUI_SetColor(highlight_color);
				GUI_SetFont(&GUI_FontTradeGothicCn58);
			} else if (3 == menu_index) {
				GUI_SetColor(font_color);
				GUI_SetFont(&GUI_FontTradeGothicCn42);
			}
		} else {
			if (0 == time_remain%10) {
				if (2 == menu_index) {
					GUI_SetBkColor(USER_COLOR1_RED);
					GUI_DispStringAt(dispStr, ShutdownModePosArray[menu_index-1][2], 140);
				} else if (3 == menu_index) {
					GUI_DispStringAt(dispStr, ShutdownModePosArray[menu_index-1][1], 90);
				}
			}
		}
		
		menu_index_last = menu_index;
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			if (menu_index > 2) {
				menu_index--;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			if (menu_index < 3) {
				menu_index++;
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT == ec_sta) {
			if (2 == menu_index) {
				ec_sta = EC11_IDLE;
			}
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		if (2 == menu_index) {// draw big font(GUI_FontTradeGothicCn58) need more time, so delay less time
			OSTimeDlyHMSM(0,0,0,90,OS_OPT_TIME_PERIODIC,&err);
		} else if (3 == menu_index) {
			OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_PERIODIC,&err);
		}
	} while(EC11_IDLE == ec_sta);
	
	g_shutdown_mode = 0;
	
	if (EC11_BUT == ec_sta) {
		g_event_val_new.force_flush = 1;
		g_event_val_new.menu_index = 11;
		
		GUIDEMO_SubMenu();
	}
	
	if (0 == time_remain) {
		GUIDEMO_ShutdownCmpUI();
	}
}

// <屏幕操作说明温度.docx>中所述的3.2界面的所有项目
void GUIDEMO_ShutdownHelpUI(void)
{
	int time_remain = 100;
	EC11_STA ec_sta = EC11_IDLE;

	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);	
	GUI_SetBkColor(bk_color);
	
	// display text
	GUI_SetColor(font_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt("Push and hold knob for three", 30, 25);
	GUI_DispStringAt("seconds to begin shutdown.", 30, 70);
	
	GUI_DrawEllipse(100,270,12,30);
	GUI_DrawArc(90,270,50,50,-35,35);
	GUI_DrawLine(100,240,130,240);
	GUI_DrawLine(100,300,130,300);
	GUI_FillPolygon (&_aPointArrowRight[0], 7, 80, 270);
	
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(320, 235, 440, 312, 10);
	
	GUI_SetColor(highlight_color);
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_SetFont(&GUI_FontTradeGothicCn77);
	GUI_DispStringAt(HelpText[3], 333, 235);

	do
	{
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			time_remain = 100;
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if ((EC11_BUT == ec_sta) || (0 == time_remain)) {
		g_event_val_new.force_flush = 1;
		
		GUIDEMO_SubMenu();
	} else if (EC11_BUT_LONG == ec_sta) {
		GUIDEMO_ShutdownModeUI();
	}
}

// <屏幕操作说明温度.docx>中所述的1.2界面的所有项目
void GUIDEMO_HelpUI(void)
{
	int time_remain = 100;
	EC11_STA ec_sta = EC11_IDLE;

	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	GUI_SetBkColor(bk_color);
	
	// display text
	GUI_SetColor(font_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt(HelpText[0], 20, 10);
	GUI_DispStringAt(HelpText[1], 20, 50);
	GUI_DrawCircle(90,150,35);
	GUI_DrawArc(95,150,50,50,-30,30);
	GUI_FillPolygon (&_aPointArrowRightUp[0], 3, 143, 120);
	GUI_FillPolygon (&_aPointArrowRightDown[0], 3, 145, 175);
	
	GUI_DrawEllipse(100,270,12,30);
	GUI_DrawArc(90,270,50,50,-35,35);
	GUI_DrawLine(100,240,130,240);
	GUI_DrawLine(100,300,130,300);
	GUI_FillPolygon (&_aPointArrowRight[0], 7, 80, 270);
	
	GUI_DispStringAt(HelpText[2], 30, 190);
	
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(320, 235, 440, 312, 10);
	
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_SetColor(highlight_color);
	GUI_SetFont(&GUI_FontTradeGothicCn77);
	GUI_DispStringAt(HelpText[3], 333, 235);
	
	do
	{
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if ((EC11_BUT == ec_sta) || (0 == time_remain)) {
		g_event_val_new.force_flush = 1;
		
		GUIDEMO_SubMenu();
	}
}

// <屏幕操作说明温度.docx>中所述的2.3界面的所有项目
void GUIDEMO_SetupServiceUI(void)
{
	int time_remain = 300;
	EC11_STA ec_sta = EC11_IDLE;

	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	GUI_SetBkColor(bk_color);
	
	// display text
	GUI_SetColor(font_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	GUI_DispStringAt(SetupServiceText[0], 160+15, 20-10);
	GUI_DispStringAt(SetupServiceText[1], 115+15, 65-10);
	GUI_DispStringAt(SetupServiceText[2], 60+15, 110-10);
	GUI_DispStringAt(SetupServiceText[3], 45+15, 160-10);
	GUI_DispStringAt(SetupServiceText[4], 78+15, 205-10);
	
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(180, 250-10, 310, 315, 10);
	
	GUI_SetColor(highlight_color);
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_SetFont(&GUI_FontTradeGothicCn77);
	GUI_DispStringAt(SetupServiceText[5], 197, 255-15);
	
	do
	{
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if ((EC11_BUT == ec_sta) || (0 == time_remain)) {
		GUIDEMO_SubMenu();
	}
}

// <屏幕操作说明温度.docx>中所述的2.4界面的所有项目
void GUIDEMO_SetupColorUI(void)
{
	int time_remain = 100;
	int menu_index = 3;
	int menu_index_last = 0;	
	EC11_STA ec_sta = EC11_IDLE;
	
	int menu_index_pos1 = 0;
	int menu_index_pos2 = 0;
	int menu_index_pos3 = 0;
	int menu_index_pos4 = 0;
	int menu_index_pos5 = 0;
	int menu_index_pos6 = 0;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	do
	{
		menu_index_pos1 = 0;
		menu_index_pos2 = 0;
		menu_index_pos3 = 0;
		menu_index_pos4 = 0;
		menu_index_pos5 = 0;
		menu_index_pos6 = 0;
		
		
		// menu switch
		if (menu_index != menu_index_last) {
			if (menu_index >= 3) {
				menu_index_pos1 = menu_index - 2;
			}
			
			if (menu_index >= 2) {
				menu_index_pos2 = menu_index - 1;
			}
			
			menu_index_pos3 = menu_index;
			
			if (menu_index <= 5) {
				menu_index_pos4 = menu_index + 1;
			}

			if (menu_index <= 4) {
				menu_index_pos5 = menu_index + 2;
			}
			
			if (menu_index <= 3) {
				menu_index_pos6 = menu_index + 3;
			}
			
			GUI_SetColor(bk_color);
			GUI_FillRect(0,0,480,320);
			GUI_SetBkColor(bk_color);
			
			GUI_SetColor(USER_COLOR1_RED);
			GUI_FillRoundedRect(30, 105, 450, 170, 10);
	
			// display text
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			if (menu_index_pos1 > 0) {
				GUI_DispStringAt(SetupColorText[menu_index_pos1-1], SetupColorPosArray[menu_index-1][0]-15, 20);
			}
			if (menu_index_pos2 > 0) {
				GUI_DispStringAt(SetupColorText[menu_index_pos2-1], SetupColorPosArray[menu_index-1][1]-15, 60);
			}
			GUI_SetColor(highlight_color);
			GUI_SetBkColor(USER_COLOR1_RED);
			GUI_SetFont(&GUI_FontTradeGothicCn58);
			if (menu_index_pos3 > 0) {
					GUI_DispStringAt(SetupColorText[menu_index_pos3-1], SetupColorPosArray[menu_index-1][2]-15, 110);
			}
			GUI_SetColor(font_color);
			GUI_SetBkColor(bk_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			if (menu_index_pos4 > 0) {
					GUI_DispStringAt(SetupColorText[menu_index_pos4-1], SetupColorPosArray[menu_index-1][3]-15, 170);
			}
			if (menu_index_pos5 > 0) {
					GUI_DispStringAt(SetupColorText[menu_index_pos5-1], SetupColorPosArray[menu_index-1][4]-15, 210);
			}
			if (menu_index_pos6 > 0) {
					GUI_DispStringAt(SetupColorText[menu_index_pos6-1], SetupColorPosArray[menu_index-1][5]-15, 250);
			}
		}
		
		menu_index_last = menu_index;
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			if (menu_index > 1) {
				menu_index--;
			}
			time_remain = 100;
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			if (menu_index < 6) {
				menu_index++;
			}
			time_remain = 100;
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT == ec_sta) {	
			if (menu_index != 6) {
				ec_sta = EC11_IDLE;
				if (1 == menu_index) {
					font_color = GUI_CYAN;
				} else if (2 == menu_index) {
					font_color = GUI_GREEN;
				} else if (3 == menu_index) {
					font_color = GUI_RED;
				} else if (4 == menu_index) {
					if (bk_color != GUI_BLACK) {
						font_color = GUI_BLACK;
					}
				} else if (5 == menu_index) {
					if (bk_color != GUI_WHITE) {
						font_color = GUI_WHITE;
					}
				}
				// force to flush
				menu_index_last = 0;
			}
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}

		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if ((EC11_BUT == ec_sta) || (0 == time_remain)) {
		GUIDEMO_SubMenu();
	}
}

// <屏幕操作说明温度.docx>中所述的2.2界面的所有项目
void GUIDEMO_SetupUI(void)
{
	int time_remain = 100;
	int menu_index = 3;
	int menu_index_last = 0;
	char ** pSetupText = NULL;
	EC11_STA ec_sta = EC11_IDLE;
	
	int menu_index_pos1 = 0;
	int menu_index_pos2 = 0;
	int menu_index_pos3 = 0;
	int menu_index_pos4 = 0;
	int menu_index_pos5 = 0;
	int menu_index_pos6 = 0;
	int menu_index_pos7 = 0;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	do
	{
		menu_index_pos1 = 0;
		menu_index_pos2 = 0;
		menu_index_pos3 = 0;
		menu_index_pos4 = 0;
		menu_index_pos5 = 0;
		menu_index_pos6 = 0;
		menu_index_pos7 = 0;
		
		if (0 == g_temp_val_new.temp_unit) {
			pSetupText = SetupText;
		} else {
			pSetupText = SetupTextC;
		}
		
		// menu switch
		if (menu_index != menu_index_last) {
			if (menu_index >= 4) {
				menu_index_pos1 = menu_index - 3;
			}
			
			if (menu_index >= 3) {
				menu_index_pos2 = menu_index - 2;
			}
			
			if (menu_index >= 2) {
				menu_index_pos3 = menu_index - 1;
			}
			
			menu_index_pos4 = menu_index;

			if (menu_index <= 5) {
				menu_index_pos5 = menu_index + 1;
			}
			
			if (menu_index <= 4) {
				menu_index_pos6 = menu_index + 2;
			}

			if (menu_index <= 3) {
				menu_index_pos7 = menu_index + 3;
			}
			
			GUI_SetColor(bk_color);
			GUI_FillRect(0,0,480,320);
			GUI_SetBkColor(bk_color);
			
			GUI_SetColor(USER_COLOR1_RED);
			GUI_FillRoundedRect(10, 132, 470, 195, 10);
	
			// display text
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			if (menu_index_pos1 > 0) {
				GUI_DispStringAt(pSetupText[menu_index_pos1-1], SetupPosArray[menu_index-1][0], 20);
			}
			if (menu_index_pos2 > 0) {
				GUI_DispStringAt(pSetupText[menu_index_pos2-1], SetupPosArray[menu_index-1][1], 55);
			}
			if (menu_index_pos3 > 0) {
					GUI_DispStringAt(pSetupText[menu_index_pos3-1], SetupPosArray[menu_index-1][2], 90);
			}
			GUI_SetColor(highlight_color);
			GUI_SetBkColor(USER_COLOR1_RED);
			GUI_SetFont(&GUI_FontTradeGothicCn58);
			if (menu_index_pos4 > 0) {
					GUI_DispStringAt(pSetupText[menu_index_pos4-1], SetupPosArray[menu_index-1][3], 135);
			}
			
			GUI_SetColor(font_color);
			GUI_SetBkColor(bk_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			if (menu_index_pos5 > 0) {
					GUI_DispStringAt(pSetupText[menu_index_pos5-1], SetupPosArray[menu_index-1][4], 196);
			}
			if (menu_index_pos6 > 0) {
					GUI_DispStringAt(pSetupText[menu_index_pos6-1], SetupPosArray[menu_index-1][5], 235);
			}
			if (menu_index_pos7 > 0) {
					GUI_DispStringAt(pSetupText[menu_index_pos7-1], SetupPosArray[menu_index-1][6], 270);
			}
			g_event_val_last.menu_index = menu_index;
		}
		
		menu_index_last = menu_index;
		ec_sta = EC11_KEY_Scan(0);
		
		// skip EC11_LEFT and EC11_RIGHT
		if (EC11_LEFT == ec_sta) {
			if (menu_index > 1) {
				menu_index--;
			}
			time_remain = 100;
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT == ec_sta) {
			if (menu_index < 6) {
				menu_index++;
			}
			time_remain = 100;
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT == ec_sta) {
			if (1 == menu_index) {
				//bk_color = GUI_WHITE;
				//font_color = GUI_BLACK;
			} else if (2 == menu_index) {
				time_remain = 100;
				ec_sta = EC11_IDLE;
				if (0 == g_temp_val_new.temp_unit) {
					g_temp_val_new.temp_unit = 1;// 摄氏度					
				} else {
					g_temp_val_new.temp_unit = 0;// 华氏度
				}
				// force to flush
				menu_index_last = 0;
			} else if (3 == menu_index) {
				time_remain = 100;
				ec_sta = EC11_IDLE;
				if (GUI_BLACK == bk_color) {
					bk_color = GUI_WHITE;
					font_color = GUI_BLACK;
					highlight_color = GUI_WHITE;
				} else {
					bk_color = GUI_BLACK;
					font_color = GUI_WHITE;
					highlight_color = GUI_BLACK;
				}
				// force to flush
				menu_index_last = 0;
			}
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		if (0 == time_remain) {
			break;
		}
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	if (EC11_BUT == ec_sta) {
		if (1 == menu_index) {
			GUIDEMO_FactoryResetUI();
		} else if (4 == menu_index) {
			GUIDEMO_SetupColorUI();
		} else if (5 == menu_index) {
			GUIDEMO_SetupServiceUI();
		} else if (6 == menu_index) {
			g_event_val_new.force_flush = 1;
			
			GUIDEMO_SubMenu();
		}
	} else {
		if (0 == time_remain) {
			g_event_val_new.force_flush = 1;
			
			GUIDEMO_SubMenu();
		}
	}
}

void CalcTempValBigGap(int center_val, int value[])
{		
	if (500 == center_val) {
		value[0] = 0;
		value[1] = 0;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val >= 475) && (center_val < 500)) {
		value[0] = 0;
		value[1] = 500;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val > 450) && (center_val < 475)) {
		value[0] = 500;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val >= 225) && (center_val <= 450)) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = center_val - 25*2;
	} else if ((center_val > 200) && (center_val < 225)) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = center_val - 25;
		value[4] = 175;
	} else if ((center_val > 175) && (center_val <= 200)) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = 175;
		value[4] = 0;
	} else if (175 == center_val) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = center_val;
		value[3] = 0; 
		value[4] = 0;
	} else if (170 == center_val) {
		value[0] = center_val + 25*2;
		value[1] = center_val + 25;
		value[2] = 0;
		value[3] = 0; 
		value[4] = 0;
	} else if (165 == center_val) {
		value[0] = center_val + 25*2;
		value[1] = 0;
		value[2] = 0;
		value[3] = 0; 
		value[4] = 0;
	} else if (center_val <= 160) {
		value[0] = 0;
		value[1] = 0;
		value[2] = 0;
		value[3] = 0; 
		value[4] = 0;
	}
}

void CalcTempValSmallGap(int center_val, int value[])
{		
	if (500 == center_val) {
		value[0] = 0;
		value[1] = 0;
		value[2] = center_val;
		value[3] = center_val - 5;
		value[4] = center_val - 5*2;
	} else if ((center_val >= 495) && (center_val < 500)) {
		value[0] = 0;
		value[1] = 500;
		value[2] = center_val;
		value[3] = center_val - 5;
		value[4] = center_val - 5*2;
	} else if ((center_val > 490) && (center_val < 495)) {
		value[0] = 500;
		value[1] = center_val + 5;
		value[2] = center_val;
		value[3] = center_val - 5;
		value[4] = center_val - 5*2;
	} else if ((center_val >= 185) && (center_val <= 490)) {
		value[0] = center_val + 5*2;
		value[1] = center_val + 5;
		value[2] = center_val;
		value[3] = center_val - 5;
		value[4] = center_val - 5*2;
	} else if ((center_val > 180) && (center_val < 185)) {
		value[0] = center_val + 2*2;
		value[1] = center_val + 2;
		value[2] = center_val;
		value[3] = center_val - 2;
		value[4] = 175;
	} else if ((center_val > 175) && (center_val <= 180)) {
		value[0] = center_val + 5*2;
		value[1] = center_val + 5;
		value[2] = center_val;
		value[3] = 175;
		value[4] = 0;
	} else if (175 == center_val) {
		value[0] = center_val + 5*2;
		value[1] = center_val + 5;
		value[2] = center_val;
		value[3] = 0; 
		value[4] = 0;
	} else if (170 == center_val) {
		value[0] = center_val + 5*2;
		value[1] = center_val + 5;
		value[2] = 0;
		value[3] = 0; 
		value[4] = 0;
	} else if (165 == center_val) {
		value[0] = center_val + 5*2;
		value[1] = 0;
		value[2] = 0;
		value[3] = 0; 
		value[4] = 0;
	} else if (center_val <= 160) {
		value[0] = 0;
		value[1] = 0;
		value[2] = 0;
		value[3] = 0; 
		value[4] = 0;
	}
}

// 对应<屏幕操作说明温度.docx>中所述的1.1界面
void GUIDEMO_SubMenu(void)
{
	int simple_mode = 0;
	int pen_size = 0;
	int time_remain = 100;
	int progress_offset = 0;
	int tmep_val_gap = 5;
	int temp_val_center = 355;
	int temp_val_center_last = 0;
	char dispStr[64] = "";	
	char ** pSubMenuTextAll = NULL;
	EC11_STA ec_sta = EC11_IDLE;
	EVENT_VAL evt_val = g_event_val_new;
	TEMP_VAL temp_val = g_temp_val_new;
	
	// 5个显示位置所要显示内容在SubMenuTextAll
	// 字符串数组中的序号，从1开始
	int menu_index_pos1 = 0;
	int menu_index_pos2 = 0;
	int menu_index_pos3 = 0;
	int menu_index_pos4 = 0;
	int menu_index_pos5 = 0;
	
	int temp1_error_last = 0xFF;
	int temp2_error_last = 0xFF;
	int temp3_error_last = 0xFF;
	int temp4_error_last = 0xFF;
	int temp5_error_last = 0xFF;
	int temp_val_array[5] = {0};
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	GUI_SetBkColor(bk_color);
	GUI_SetColor(font_color);
	// 多边形绘制
	GUI_FillPolygon (&_aPointArrowUp[0], 7, 185-40, 10);
	GUI_FillPolygon (&_aPointArrowDown[0], 7, 185-40, 268);
	
	GUI_SetColor(USER_COLOR5_GRAY);
	GUI_SetFont(&GUI_FontTradeGothicCn19);
	GUI_DispStringAt("ONE", 305+10+3, 175-92);
	GUI_DispStringAt("TWO", 305+90+10+3, 175-92);
	GUI_DispStringAt("THREE", 302-10+12+5, 175+92-92+10);
	GUI_DispStringAt("FOUR", 302+90+10+3, 175+92-92+10);
	
	pen_size = GUI_GetPenSize();
	GUI_SetPenSize(2);
	
	GUI_DrawLine(280, 20, 465, 20);
	GUI_DrawLine(280, 225,465, 225);
	GUI_DrawLine(280, 20, 280, 225);
	GUI_DrawLine(465, 20, 465, 225);
	
	GUI_DrawLine(373, 20, 373, 225);
	GUI_DrawLine(280, 123, 465, 123);
	GUI_SetPenSize(pen_size);
	
	GUI_SetColor(USER_COLOR4_GRAY);
	GUI_FillRect(15, 270, 58, 312);
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRect(422, 270, 465, 312);
	image_display(25, 275, (u8*)gImage_p1_leftdown);
	image_display(435, 275, (u8*)gImage_p1_rightdown);
	progress_offset = 364 - (364 * g_smoke_val_percent) / 100;
	
	if (progress_offset <= 20) {
		progress_offset = 20;
	}
	if (progress_offset >= 344) {
		progress_offset = 344;
	}
	
	GUI_SetColor(USER_COLOR4_GRAY);
	GUI_FillRect(58, 270, 58+progress_offset, 312);
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRect(58+progress_offset, 270, 422, 312);
	
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(58+progress_offset, 275, 58+progress_offset, 307);
	GUI_FillPolygon (&_aPointArrowProgressLeft[0], 3, 58+progress_offset-20, 291);
	GUI_FillPolygon (&_aPointArrowProgressRight[0], 3, 58+progress_offset+20, 291);
	
	// 被选中的项目，外围画框
	GUI_SetColor(USER_COLOR1_RED);
	GUI_SetBkColor(USER_COLOR1_RED);
	GUI_FillRoundedRect(10, 115, 275, 175, 10);	

	EC11_Clear();

#if 0
	if (g_temp_center != 0) {
		simple_mode = 1;
		temp_val_center = g_temp_center;
	} else {
		simple_mode = 0;
	}
#endif

	do
	{
		menu_index_pos1 = 0;
		menu_index_pos2 = 0;
		menu_index_pos3 = 0;
		menu_index_pos4 = 0;
		menu_index_pos5 = 0;
		
		temp_val_array[0] = 0;
		temp_val_array[1] = 0;
		temp_val_array[2] = 0;
		temp_val_array[3] = 0;
		temp_val_array[4] = 0;
		
		ec_sta = EC11_IDLE;
		evt_val = g_event_val_new;
		temp_val = g_temp_val_new;
		

		#if FOR_DEBUG_USE
		if (g_fatal_error != 0) {
		#else
		if ((g_fatal_error != 0) || (g_temp5_error != 0)) {
		#endif
			GUI_SetBkColor(bk_color);
			GUI_SetColor(bk_color);
			GUI_FillRect(15, 270, 465, 312);
			delay_ms(1000);

			GUI_SetColor(USER_COLOR1_RED);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			if (1 == g_fatal_error) {
				GUI_DispStringAt("FLAME ERROR", 50, 275);
			} else {
				GUI_DispStringAt("SENSOR ERROR", 50, 275);
			}
			
			delay_ms(1000);
			continue;
		}
		
		if (0 == temp_val.temp_unit) {
			pSubMenuTextAll = SubMenuTextAll;
		} else {
			pSubMenuTextAll = SubMenuTextAllC;
		}

		if (temp_val_center > 500) {
			temp_val_center = 500;
		}
		if (temp_val_center < 165) {
			temp_val_center = 165;
		}

		if (25 == tmep_val_gap) {
			CalcTempValBigGap(temp_val_center, temp_val_array);
		} else {
			CalcTempValSmallGap(temp_val_center, temp_val_array);
		}
			
		if (1 == simple_mode) {
			if (0 == (temp_val_array[0]||temp_val_array[1])) {
				evt_val.menu_index = 1;
			}
			if (0 == (temp_val_array[0])) {
				evt_val.menu_index = 2;
			}
			
			if (0 == (temp_val_array[4])) {
				evt_val.menu_index = 5;
			}
			if (0 == (temp_val_array[3]||temp_val_array[4])) {
				evt_val.menu_index = 6;
			}
		} else if (simple_mode > 1){
			evt_val.menu_index = simple_mode + 5;
		} else if (0 == simple_mode){
			simple_mode = evt_val.menu_index - 5;
		}
		
		// 根据旋钮左右旋转，动态计算5个显示位置，所要显示的字符串内容
		// 旋钮旋转时才更新UI
		if ((evt_val.menu_index != g_event_val_last.menu_index) || evt_val.force_flush) {
			g_event_val_new.force_flush = 0;
			if (evt_val.menu_index >= 3) {
				menu_index_pos1 = evt_val.menu_index - 2;
			}
			
			if (evt_val.menu_index >= 2) {
				menu_index_pos2 = evt_val.menu_index - 1;
			}
			
			menu_index_pos3 = evt_val.menu_index;
			
			if (evt_val.menu_index <= 12) {
				menu_index_pos4 = evt_val.menu_index + 1;
			}

			if (evt_val.menu_index <= 11) {
				menu_index_pos5 = evt_val.menu_index + 2;
			}
			
			GUI_SetBkColor(bk_color);

			// display text
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			GUI_DispStringAt("                                ", 10, 38);
			if (menu_index_pos1 > 0) {
				if ((evt_val.menu_index > 3) && (evt_val.menu_index < 9)) {
					if (0 == g_temp_val_new.temp_unit) {
						sprintf(dispStr, "%.3d°F", temp_val_array[0]);
					} else {
						sprintf(dispStr, "%.3d°C", temp_val_array[0]);
					}
					if (500 == (temp_val_center + (tmep_val_gap * 2))) {
						sprintf(dispStr, "%s", pSubMenuTextAll[0]);
					}
				} else {
					sprintf(dispStr, "%s", pSubMenuTextAll[menu_index_pos1-1]);
				}
				GUI_DispStringAt(dispStr, SubMenuPosArray[evt_val.menu_index-1][0]-40, 38);
			}
			GUI_DispStringAt("                                ", 10, 73);
			if (menu_index_pos2 > 0) {
				if ((evt_val.menu_index > 2) && (evt_val.menu_index < 8)) {
					if (0 == g_temp_val_new.temp_unit) {
						sprintf(dispStr, "%.3d°F", temp_val_array[1]);
					} else {
						sprintf(dispStr, "%.3d°C", temp_val_array[1]);
					}
					if (500 == (temp_val_center + (tmep_val_gap * 1))) {
						sprintf(dispStr, "%s", pSubMenuTextAll[0]);
					}
				} else {
					sprintf(dispStr, "%s", pSubMenuTextAll[menu_index_pos2-1]);
				}
				GUI_DispStringAt(dispStr, SubMenuPosArray[evt_val.menu_index-1][1]-40, 73);
			}

			GUI_SetBkColor(USER_COLOR1_RED);
			GUI_SetColor(highlight_color);
			GUI_SetFont(&GUI_FontTradeGothicCn58);
			
			GUI_DispStringAt("                      ", 16, 115);
			if (menu_index_pos3 > 0) {
				if ((evt_val.menu_index > 1) && (evt_val.menu_index < 7)) {
					if (0 == g_temp_val_new.temp_unit) {
						sprintf(dispStr, "%.3d°F", temp_val_array[2]);
					} else {
						sprintf(dispStr, "%.3d°C", temp_val_array[2]);
					}
					if (500 == temp_val_center) {
						sprintf(dispStr, "%s", pSubMenuTextAll[0]);
					}
				} else {
					sprintf(dispStr, "%s", pSubMenuTextAll[menu_index_pos3-1]);
				}
				GUI_DispStringAt(dispStr, SubMenuPosArray[evt_val.menu_index-1][2]-40, 116);
			}
			
			GUI_SetBkColor(bk_color);
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			GUI_DispStringAt("                                ", 10, 176);
			if (menu_index_pos4 > 0) {
				if ((evt_val.menu_index > 0) && (evt_val.menu_index < 6)) {
					if (0 == g_temp_val_new.temp_unit) {
						sprintf(dispStr, "%.3d°F", temp_val_array[3]);
					} else {
						sprintf(dispStr, "%.3d°C", temp_val_array[3]);
					}
				} else {
					sprintf(dispStr, "%s", pSubMenuTextAll[menu_index_pos4-1]);
				}
				GUI_DispStringAt(dispStr, SubMenuPosArray[evt_val.menu_index-1][3]-40, 176);
			}
			GUI_DispStringAt("                                ", 10, 210);
			if (menu_index_pos5 > 0) {
				if ((evt_val.menu_index > 0) && (evt_val.menu_index < 5)) {
					if (0 == g_temp_val_new.temp_unit) {
						sprintf(dispStr, "%.3d°F", temp_val_array[4]);
					} else {
						sprintf(dispStr, "%.3d°C", temp_val_array[4]);
					}
				} else {
					sprintf(dispStr, "%s", pSubMenuTextAll[menu_index_pos5-1]);
				}
				GUI_DispStringAt(dispStr, SubMenuPosArray[evt_val.menu_index-1][4]-40, 210);
			}
			
			g_event_val_last.menu_index = evt_val.menu_index;
		}
		
		temp_val_center_last = temp_val_center;
		g_temp_center = temp_val_center;
		
		if (0 == time_remain%10) {
			GUI_SetBkColor(bk_color);		
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn38);
			if (0 == g_temp1_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp1);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp1);
				}
				GUI_DispStringAt(dispStr, 290+5, 125-92);
			} else {// 如果温度棒故障，则显示”-“
				if (temp1_error_last != g_temp1_error) {
					GUI_DispStringAt("          ", 290+5, 125-92-14+10);
					GUI_DispStringAt("_ _ _", 290+10, 125-92-14+10);
				}
			}
			temp1_error_last = g_temp1_error;
			if (0 == g_temp2_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp2);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp2);
				}
				GUI_DispStringAt(dispStr, 290+92+5, 125-92);
			} else {
				if (temp2_error_last != g_temp2_error) {
					GUI_DispStringAt("          ", 290+92+5, 125-92-14+10);
					GUI_DispStringAt("_ _ _", 290+92+10, 125-92-14+10);
				}
			}
			temp2_error_last = g_temp2_error;
			if (0 == g_temp3_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp3);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp3);
				}
				GUI_DispStringAt(dispStr, 290+5, 125+92-92+10);
			} else {
				if (temp3_error_last != g_temp3_error) {
					GUI_DispStringAt("          ", 290+5, 125+92-92-14+10+10);
					GUI_DispStringAt("_ _ _", 290+10, 125+92-92-14+10+10);
				}
			}
			temp3_error_last = g_temp3_error;
			if (0 == g_temp4_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp4);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp4);
				}
				GUI_DispStringAt(dispStr, 290+92+5, 125+92-92+10);
			} else {
				if (temp4_error_last != g_temp4_error) {
					GUI_DispStringAt("          ", 290+92+5, 125+92-92-14+10+10);
					GUI_DispStringAt("_ _ _", 290+92+10, 125+92-92-14+10+10);
				}
			}
			temp4_error_last = g_temp4_error;
		}
		
		// 扫面EC11编码器有无动作
		ec_sta = EC11_KEY_Scan(1);
		if (EC11_LEFT == ec_sta) {
			if (1 == simple_mode) {
				if (temp_val_center < 500) {
					tmep_val_gap = 5;
					temp_val_center += 5;
					g_event_val_new.force_flush = 1;
				}
			} else {
				if (simple_mode > 1) {
					simple_mode--;
					if (3 == simple_mode) {
						temp_val_center = 165;
					}
					if (2 == simple_mode) {
						temp_val_center = 170;
					}
					if (1 == simple_mode) {
						temp_val_center = 175;
					}
				}
			}
			ec_sta = EC11_IDLE;// 更新部分UI，但无需退出该界面
		} else if (EC11_RIGHT == ec_sta) {// --
			if (1 == simple_mode) {
				if (temp_val_center >= 180) {// 175+5
					tmep_val_gap = 5;
					temp_val_center -= 5;
					g_event_val_new.force_flush = 1;
				} else {
					simple_mode++;
				}
			} else {
				if (simple_mode < 8) {
					simple_mode++;
				}
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_RIGHT_FAST == ec_sta) {
			if (1 == simple_mode) {
				if (temp_val_center >= 200) {// 175+25
					tmep_val_gap = 25;
					temp_val_center -= 25;
					g_event_val_new.force_flush = 1;
				} else {
					simple_mode++;
				}
			} else {
				if (simple_mode < 8) {
					simple_mode++;
				}
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_LEFT_FAST == ec_sta) {// ++
			if (1 == simple_mode) {
				if (temp_val_center < 480) {
					tmep_val_gap = 25;
					temp_val_center += 25;
					g_event_val_new.force_flush = 1;
				}
			} else {
				if (simple_mode > 1) {
					simple_mode--;
				}
			}
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;// 忽略该消息
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;// 忽略该消息
		}
		
		time_remain--;
		if (0 == time_remain) {
			time_remain = 100;
		}
		
		g_event_val_new = evt_val;
		
		delay_ms(100);
	} while(EC11_IDLE == ec_sta);
	
	// 退出该界面，跳转到下一个界面
	if (EC11_BUT == ec_sta) {
		g_event_val_new.force_flush = 1;
		if (12 == evt_val.menu_index) {
			GUIDEMO_SetupUI();
		} else if (11 == evt_val.menu_index) {
			GUIDEMO_HelpUI();
		} else if (10 == evt_val.menu_index) {
			GUIDEMO_ShutdownHelpUI();
		} else if (9 == evt_val.menu_index) {
			GUIDEMO_FeedHelpUI();
		} else if (8 == evt_val.menu_index) {
			GUIDEMO_LowSmokeUI();
		} else if (7 == evt_val.menu_index) {
			GUIDEMO_LowSmokeUI();
		} else if ((evt_val.menu_index >= 1)&&(evt_val.menu_index <= 6)) {
			g_temp_val_new.target_val = temp_val_center;
			GUIDEMO_OtherTempModeUI();
		} else {// 13 EXIT
			GUIDEMO_MainUI();
		}
	}
}

void GUIDEMO_AfterLogo(void)
{
	// 默认子项目ID，对应”---“项
	g_event_val_new.menu_index = 11;
	g_temp_val_new.temp_unit = 0;// 华氏度
	
	g_temp_val_last = g_temp_val_new;
	
	GUIDEMO_SubMenu();
}

// RUN界面
void GUIDEMO_MainUI(void)
{
	int pen_size = 0;
	char dispStr[64] = "";
	int progress_offset = 0;
	int temp1_error_last = 0xFF;
	int temp2_error_last = 0xFF;
	int temp3_error_last = 0xFF;
	int temp4_error_last = 0xFF;
	int temp5_error_last = 0xFF;
	int time_remain = 10;
	int disp_switch = 0;
	
	EC11_STA ec_sta = EC11_IDLE;
	TEMP_VAL temp_val = g_temp_val_new;
	
	GUI_SetColor(bk_color);
	GUI_FillRect(0,0,480,320);
	
	GUI_SetColor(font_color);
	GUI_SetBkColor(bk_color);
	GUI_SetFont(&GUI_FontTradeGothicCn48);
	if (0 == g_temp_val_new.temp_unit) {
		sprintf(dispStr, "Target %.3d°F", temp_val.target_val);
	} else {
		sprintf(dispStr, "Target %.3d°C", temp_val.target_val);
	}
	GUI_DispStringAt(dispStr, 15, 135);
	
	sprintf(dispStr, "Time %.2d:%.2d", (g_run_mode_counter_mins/60)%24, g_run_mode_counter_mins%60);
	GUI_DispStringAt(dispStr, 15, 185);
	
	GUI_SetColor(USER_COLOR5_GRAY);
	GUI_SetFont(&GUI_FontTradeGothicCn19);
	GUI_DispStringAt("ONE", 305+10+3, 175-92);
	GUI_DispStringAt("TWO", 305+90+10+3, 175-92);
	GUI_DispStringAt("THREE", 302-10+12+5, 175+92-92+10);
	GUI_DispStringAt("FOUR", 302+90+10+3, 175+92-92+10);
	
	pen_size = GUI_GetPenSize();
	GUI_SetPenSize(2);
	
	GUI_DrawLine(280, 20, 465, 20);
	GUI_DrawLine(280, 225,465, 225);
	GUI_DrawLine(280, 20, 280, 225);
	GUI_DrawLine(465, 20, 465, 225);
	
	GUI_DrawLine(373, 20, 373, 225);
	GUI_DrawLine(280, 123, 465, 123);
	GUI_SetPenSize(pen_size);
			
	GUI_SetColor(USER_COLOR4_GRAY);
	GUI_FillRect(15, 270, 58, 312);
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRect(422, 270, 465, 312);
	image_display(25, 275, (u8*)gImage_p1_leftdown);
	image_display(435, 275, (u8*)gImage_p1_rightdown);
	progress_offset = 364 - (364 * g_smoke_val_percent) / 100;
	
	if (progress_offset <= 20) {
		progress_offset = 20;
	}
	if (progress_offset >= 344) {
		progress_offset = 344;
	}
	
	GUI_SetColor(USER_COLOR4_GRAY);
	GUI_FillRect(58, 270, 58+progress_offset, 312);
	GUI_SetColor(USER_COLOR1_RED);
	GUI_FillRect(58+progress_offset, 270, 422, 312);
	
	GUI_SetColor(GUI_WHITE);
	GUI_DrawLine(58+progress_offset, 275, 58+progress_offset, 307);
	GUI_FillPolygon (&_aPointArrowProgressLeft[0], 3, 58+progress_offset-20, 291);
	GUI_FillPolygon (&_aPointArrowProgressRight[0], 3, 58+progress_offset+20, 291);
	
	do 
	{
		ec_sta = EC11_IDLE;
		temp_val = g_temp_val_new;
		
		if (g_fatal_error != 0) {
			GUI_SetBkColor(bk_color);
			GUI_SetColor(bk_color);
			GUI_FillRect(15, 270, 465, 312);
			delay_ms(2000);
			GUI_SetColor(USER_COLOR1_RED);
			GUI_SetFont(&GUI_FontTradeGothicCn42);
			
			if (1 == g_fatal_error) {
				GUI_DispStringAt("FLAME ERROR", 50, 275);
			} else {
				GUI_DispStringAt("SENSOR ERROR", 50, 275);
			}
			
			delay_ms(2000);
			continue;
		}
		
		if ((temp_val.temp5 < 0) || (temp_val.temp5 > 500)) {
			temp_val.temp5 = 0;
		}
		
		temp_val.temp5 = temp_val.temp5 % (temp_val.target_val+1);
		
		g_temp5_error = 0;
	//	temp_val.temp5 = 200;
		 
		if (0 == time_remain%10) {
			if (1 == g_run_mode) {
				GUI_SetColor(font_color);
				GUI_SetFont(&GUI_FontTradeGothicCn48);
				if (0 == disp_switch) {
					disp_switch = 1;
					sprintf(dispStr, "Time %.2d:%.2d", (g_run_mode_counter_mins/60)%24, g_run_mode_counter_mins%60);
				} else {
					disp_switch = 0;
					sprintf(dispStr, "Time %.2d %.2d", (g_run_mode_counter_mins/60)%24, g_run_mode_counter_mins%60);
				}
				
				GUI_DispStringAt(dispStr, 15, 185);
			}
		
			GUI_SetColor(USER_COLOR1_RED);
			GUI_SetFont(&GUI_FontTradeGothicCn144);
					
			if (0 == g_temp5_error) {
				g_event_val_new.force_flush = 0;
				sprintf(dispStr, "%.3d", temp_val.temp5);
				GUI_DispStringAt(dispStr, 15, 0);
				GUI_SetFont(&GUI_FontTradeGothicCn58);
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "°F");
				} else {
					sprintf(dispStr, "°C");
				}
				GUI_DispStringAt(dispStr, 175, 20);
				g_temp_val_last.temp5 = temp_val.temp5;
			} else {
				if (temp5_error_last != g_temp5_error) {
					GUI_SetColor(bk_color);
					GUI_FillRect(0,0,270,130);
					
					GUI_SetColor(USER_COLOR1_RED);
			
					pen_size = GUI_GetPenSize();
					GUI_SetPenSize(3);
					GUI_DrawLine(70, 70, 90, 70);
					GUI_DrawLine(100, 70, 120, 70);
					GUI_DrawLine(130, 70, 150, 70);
					GUI_SetPenSize(pen_size);
				}
			}
			temp5_error_last = g_temp5_error;
			GUI_SetBkColor(bk_color);		
			GUI_SetColor(font_color);
			GUI_SetFont(&GUI_FontTradeGothicCn38);
			if (0 == g_temp1_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp1);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp1);
				}
				GUI_DispStringAt(dispStr, 290+5, 125-92);
			} else {// 如果温度棒故障，则显示”-“
				if (temp1_error_last != g_temp1_error) {
					GUI_DispStringAt("          ", 290+5, 125-92-14+10);
					GUI_DispStringAt("_ _ _", 290+10, 125-92-14+10);
				}
			}
			temp1_error_last = g_temp1_error;
			if (0 == g_temp2_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp2);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp2);
				}
				GUI_DispStringAt(dispStr, 290+92+5, 125-92);
			} else {
				if (temp2_error_last != g_temp2_error) {
					GUI_DispStringAt("          ", 290+92+5, 125-92-14+10);
					GUI_DispStringAt("_ _ _", 290+92+10, 125-92-14+10);
				}
			}
			temp2_error_last = g_temp2_error;
			if (0 == g_temp3_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp3);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp3);
				}
				GUI_DispStringAt(dispStr, 290+5, 125+92-92+10);
			} else {
				if (temp3_error_last != g_temp3_error) {
					GUI_DispStringAt("          ", 290+5, 125+92-92-14+10+10);
					GUI_DispStringAt("_ _ _", 290+10, 125+92-92-14+10+10);
				}
			}
			temp3_error_last = g_temp3_error;
			if (0 == g_temp4_error) {
				if (0 == g_temp_val_new.temp_unit) {
					sprintf(dispStr, "%.3d°F", temp_val.temp4);
				} else {
					sprintf(dispStr, "%.3d°C", temp_val.temp4);
				}
				GUI_DispStringAt(dispStr, 290+92+5, 125+92-92+10);
			} else {
				if (temp4_error_last != g_temp4_error) {
					GUI_DispStringAt("          ", 290+92+5, 125+92-92-14+10+10);
					GUI_DispStringAt("_ _ _", 290+92+10, 125+92-92-14+10+10);
				}
			}
			temp4_error_last = g_temp4_error;
		}

		// 最好该函数内部能识别出是快转还是慢转
		ec_sta = EC11_KEY_Scan(0);
		
		// no support in this ui
		if (EC11_BUT_LONG == ec_sta) {
			ec_sta = EC11_IDLE;
		} else if (EC11_BUT_NO_FREE <= ec_sta) {
			ec_sta = EC11_IDLE;
		}
		
		time_remain--;
		
		if (0 == time_remain) {
			time_remain = 10;
		}
		
		delay_ms(80);
	} while(EC11_IDLE == ec_sta);
	
	if (180 == temp_val.target_val) {
		g_event_val_new.menu_index = 5;
	} else if (175 == temp_val.target_val) {
		g_event_val_new.menu_index = 6;
	} else if (170 == temp_val.target_val) {
		g_event_val_new.menu_index = 7;
	} else if (165 == temp_val.target_val) {
		g_event_val_new.menu_index = 8;
	} else if (490 == temp_val.target_val) {
		g_event_val_new.menu_index = 3;
	} else if (495 == temp_val.target_val) {
		g_event_val_new.menu_index = 2;
	} else if (500 == temp_val.target_val) {
		g_event_val_new.menu_index = 1;
	} else {
		g_event_val_new.menu_index = 4;
	}
	
	g_event_val_new.force_flush = 1;
	GUIDEMO_SubMenu();		
}
/*************************** End of file ****************************/
