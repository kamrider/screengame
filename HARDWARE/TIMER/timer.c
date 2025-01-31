#include "timer.h"
#include "lvgl.h"

void TIM6_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);  //使能TIM6时钟
    
    //定时器TIM6初始化
    TIM_TimeBaseStructure.TIM_Period = 999;  //设置自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 83;  //设置预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    
    //中断配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);  //使能更新中断
    TIM_Cmd(TIM6, ENABLE);  //使能定时器
}

void TIM6_DAC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
    {
        lv_tick_inc(1);  // 为LVGL提供1ms时基
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
} 