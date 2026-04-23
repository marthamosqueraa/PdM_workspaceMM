
#include <APIlcd.h>
#include <stdio.h>
extern I2C_HandleTypeDef hi2c1;  // Para Manejo de I2C

#define SLAVE_ADDRESS_LCD (0x27 << 1) //LCD Address

void APILCD_sendcmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);

	if (cmd == 0x01 || cmd == 0x02)
	        HAL_Delay(2);

}

void APILCD_sendData (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=1
	data_t[1] = data_u|0x09;  //en=0, rs=1
	data_t[2] = data_l|0x0D;  //en=1, rs=1
	data_t[3] = data_l|0x09;  //en=0, rs=1
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void APILCD_init (void)
{
	HAL_Delay(50);
	APILCD_sendcmd (0x33);
	APILCD_sendcmd (0x32);
	APILCD_sendcmd (0x28);
	APILCD_sendcmd (0x0C);
	APILCD_sendcmd (0x06);
	APILCD_sendcmd (0x01);
	HAL_Delay(2);

}

void APILCD_sendstring (char *str)
{
	while (*str) APILCD_sendData (*str++);
}

void APILCD_sendfloat (float data, int numberofdigits)
{
	char xg_char[12];

	snprintf(xg_char, sizeof(xg_char),"%.2f", data);
	for (int i=0; i<numberofdigits &&   xg_char[i] != '\0'; i++)
	{
		APILCD_sendData (xg_char[i]);
	}
}

void APILCD_clear(void){
	APILCD_sendcmd (0x01);
	HAL_Delay(2);
}
