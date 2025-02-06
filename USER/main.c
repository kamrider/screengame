#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  

#include "w25qxx.h"
#include "touch.h"
#include "GBK_LibDrive.h"	
#include "timer.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_widgets.h"
#include "lv_100ask_2048.h"  // 添加2048游戏头文件

 
/************************************************************************/
static void game_2048_event_cb(lv_event_t * e);
	
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
	KEY_Init(); 				//按键初始化  
	
	W25QXX_Init();			//W25QXX初始化
	
	POINT_COLOR=RED; 
	
	while(W25QXX_ReadID()!=W25Q16)								//检测不到W25QXX
	{
		delay_ms(500);
		LED1=!LED1;		//DS0闪烁
	}

	
	GBK_Lib_Init();       //硬件GBK字库初始化--(如果使用不带字库的液晶屏版本，此处可以屏蔽，不做字库初始化） 
	
	TIM6_Init();         // 初始化定时器6作为LVGL的时基
	lv_init();           // 初始化LVGL
	lv_port_disp_init(); // 显示接口初始化(这里面会调用LCD_Init)  
	lv_port_indev_init();// 输入接口初始化(这里面会调用tp_dev.init)
//	    // 添加测试代码
//    LCD_Clear(RED);     // 先显示红色
//    delay_ms(1000);
//    LCD_Clear(WHITE);   // 再显示白色
//    delay_ms(1000);
//    // 创建一个checkbox控件
//    lv_obj_t * checkbox_obj = lv_checkbox_create(lv_scr_act());
//    lv_obj_set_size(checkbox_obj, 160, 30);
//    lv_obj_align(checkbox_obj, LV_ALIGN_CENTER, 0, 0);
	
	// 删除测试代码，创建2048游戏
	lv_obj_t * obj_2048 = lv_100ask_2048_create(lv_scr_act());
	lv_obj_set_size(obj_2048, 240, 240);  // 根据实际屏幕大小调整
	lv_obj_center(obj_2048);
	
	// 禁用滚动
	lv_obj_clear_flag(obj_2048, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
	
	// 创建分数标签
	lv_obj_t * label = lv_label_create(lv_scr_act());
	lv_label_set_text_fmt(label, "SCORE:%d", lv_100ask_2048_get_score(obj_2048));
	lv_obj_align_to(label, obj_2048, LV_ALIGN_OUT_TOP_RIGHT, 0, -10);
	
	// 添加游戏事件回调
	lv_obj_add_event_cb(obj_2048, game_2048_event_cb, LV_EVENT_ALL, label);
	
	while(1)
	{
		lv_timer_handler();
		delay_ms(5);
	}
}

// 添加游戏事件回调函数
static void game_2048_event_cb(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * obj_2048 = lv_event_get_target(e);
	lv_obj_t * label = lv_event_get_user_data(e);

	if(code == LV_EVENT_VALUE_CHANGED) {
		if (lv_100ask_2048_get_best_tile(obj_2048) >= 2048)
			lv_label_set_text(label, "YOU WIN!");
		else if(lv_100ask_2048_get_status(obj_2048))
			lv_label_set_text(label, "GAME OVER!");
		else
			lv_label_set_text_fmt(label, "SCORE: %d", lv_100ask_2048_get_score(obj_2048));
	}
}






















/************************************************************************/

//电阻触摸屏测试
 
//DevEBox  大越创新
//淘宝店铺：mcudev.taobao.com
//淘宝店铺：shop389957290.taobao.com	

/************************************************************************/

