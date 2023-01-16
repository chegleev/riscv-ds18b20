#include "ds18b20.h"

//GPIO_Pin_10 is connected to the DQ pin of DS18B20
void GPIOA10_Zero() {

	GPIO_InitTypeDef GPIO_InitStructure = { 0 };

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void GPIOA10_One() {

	GPIO_InitTypeDef GPIO_InitStructure = { 0 };

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned char w1_find() {
	unsigned char device;
	GPIOA10_Zero();
	Delay_Us(480);
	GPIOA10_One();
	Delay_Us(65);
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0x00)
		device = 1;
	else
		device = 0;
	Delay_Us(420);
	return device;
}

void w1_sendcmd(unsigned char cmd) {
	for (unsigned char i = 0; i < 8; i++)
	{
		if ((cmd & (1 << i)) == 1 << i)
				{
			GPIOA10_Zero();
			Delay_Us(2);
			GPIOA10_One();
			Delay_Us(65);
		} else
		{
			GPIOA10_Zero();
			Delay_Us(65);
			GPIOA10_One();
			Delay_Us(5);
		}
	}
}

void w1_send_byte(unsigned char byte) {
	for (unsigned char i = 0; i < 8; i++)
	{
		if ((byte & (1 << i)) == 1 << i)
		{
			GPIOA10_Zero();
			Delay_Us(2);
			GPIOA10_One();
			Delay_Us(65);
		} else
		{
			GPIOA10_Zero();
			Delay_Us(65);
			GPIOA10_One();
			Delay_Us(5);
		}
	}
}

unsigned char w1_receive_byte() {
	unsigned char data = 0;
	for (unsigned char i = 0; i < 8; i++)
	{
		GPIOA10_Zero();
		Delay_Us(2);
		GPIOA10_One();
		Delay_Us(7);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == 0x00)
			data &= ~(1 << i);
		else
			data |= 1 << i;
		Delay_Us(50);
	}
	return data;
}

int get_temp_18b20() {
	int temp = 0;
	if (w1_find() == 1)
	{
		w1_sendcmd(0xcc);
		w1_sendcmd(0x44);
		Delay_Ms(750);

		unsigned char data[2];
		w1_find();
		w1_sendcmd(0xcc);
		w1_sendcmd(0xbe);
		data[0] = w1_receive_byte();
		data[1] = w1_receive_byte();

		temp = (data[1] << 8) + data[0];
		temp = temp >> 4;
	}
	return temp;
}
