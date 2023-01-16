#include "debug.h"
#include "ds18b20.h"

void GPIO_LED_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;   //LED
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    Delay_Init();

    GPIO_LED_INIT();

    while(1)
    {
		int t = get_temp_18b20();

		//if the temperature is equal to or greater than 27°C, the LED is on
		if (t >= 27) {
			GPIO_SetBits(GPIOB, GPIO_Pin_4); //LED on
		} else {
			GPIO_ResetBits(GPIOB, GPIO_Pin_4); //LED off
		}

		Delay_Ms(2000);
    }
}
