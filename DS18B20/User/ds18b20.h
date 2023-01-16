#ifndef USER_DS18B20_H_
#define USER_DS18B20_H_

#include "ch32v30x.h"

void GPIOA10_Zero();
void GPIOA10_One();
unsigned char w1_find();
void w1_sendcmd(unsigned char cmd);
void w1_send_byte(unsigned char byte);
unsigned char w1_receive_byte();
int get_temp_18b20();

#endif /* USER_DS18B20_H_ */
