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
#include "lv_100ask_2048.h"  // ���2048��Ϸͷ�ļ�

 
/************************************************************************/
static void game_2048_event_cb(lv_event_t * e);
	
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	
	LED_Init();					//��ʼ��LED 
	KEY_Init(); 				//������ʼ��  
	
	W25QXX_Init();			//W25QXX��ʼ��
	
	POINT_COLOR=RED; 
	
	while(W25QXX_ReadID()!=W25Q16)								//��ⲻ��W25QXX
	{
		delay_ms(500);
		LED1=!LED1;		//DS0��˸
	}

	
	GBK_Lib_Init();       //Ӳ��GBK�ֿ��ʼ��--(���ʹ�ò����ֿ��Һ�����汾���˴��������Σ������ֿ��ʼ���� 
	
	TIM6_Init();         // ��ʼ����ʱ��6��ΪLVGL��ʱ��
	lv_init();           // ��ʼ��LVGL
	lv_port_disp_init(); // ��ʾ�ӿڳ�ʼ��(����������LCD_Init)  
	lv_port_indev_init();// ����ӿڳ�ʼ��(����������tp_dev.init)
//	    // ��Ӳ��Դ���
//    LCD_Clear(RED);     // ����ʾ��ɫ
//    delay_ms(1000);
//    LCD_Clear(WHITE);   // ����ʾ��ɫ
//    delay_ms(1000);
//    // ����һ��checkbox�ؼ�
//    lv_obj_t * checkbox_obj = lv_checkbox_create(lv_scr_act());
//    lv_obj_set_size(checkbox_obj, 160, 30);
//    lv_obj_align(checkbox_obj, LV_ALIGN_CENTER, 0, 0);
	
	// ɾ�����Դ��룬����2048��Ϸ
	lv_obj_t * obj_2048 = lv_100ask_2048_create(lv_scr_act());
	lv_obj_set_size(obj_2048, 240, 240);  // ����ʵ����Ļ��С����
	lv_obj_center(obj_2048);
	
	// ���ù���
	lv_obj_clear_flag(obj_2048, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
	
	// ����������ǩ
	lv_obj_t * label = lv_label_create(lv_scr_act());
	lv_label_set_text_fmt(label, "SCORE:%d", lv_100ask_2048_get_score(obj_2048));
	lv_obj_align_to(label, obj_2048, LV_ALIGN_OUT_TOP_RIGHT, 0, -10);
	
	// �����Ϸ�¼��ص�
	lv_obj_add_event_cb(obj_2048, game_2048_event_cb, LV_EVENT_ALL, label);
	
	while(1)
	{
		lv_timer_handler();
		delay_ms(5);
	}
}

// �����Ϸ�¼��ص�����
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

//���败��������
 
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

/************************************************************************/

