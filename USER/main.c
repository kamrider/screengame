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
 
 
/************************************************************************/

	
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
	    // ��Ӳ��Դ���
    LCD_Clear(RED);     // ����ʾ��ɫ
    delay_ms(1000);
    LCD_Clear(WHITE);   // ����ʾ��ɫ
    delay_ms(1000);
    // ����һ��checkbox�ؼ�
    lv_obj_t * checkbox_obj = lv_checkbox_create(lv_scr_act());
    lv_obj_set_size(checkbox_obj, 160, 30);
    lv_obj_align(checkbox_obj, LV_ALIGN_CENTER, 0, 0);
	
	while(1)
	{
		lv_timer_handler();
		delay_ms(5);
	}
}




















/************************************************************************/

//���败��������
 
//DevEBox  ��Խ����
//�Ա����̣�mcudev.taobao.com
//�Ա����̣�shop389957290.taobao.com	

/************************************************************************/

