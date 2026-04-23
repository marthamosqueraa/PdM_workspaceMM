/*
 * APIadxl.c
 *
 *  Created on: Apr 19, 2026
 *      Author: lilo
 */


#include <APIadxl.h>
#include <math.h>

//Para Dependencias externas
extern SPI_HandleTypeDef hspi1;

//Definiciones para ADXL345

#define ADXL_REG_DEV      0x00 //Para conocer DeviceID
#define ADXL_REG_POWERCTL 0x2D //Para Power Control
#define ADXL_REG_DATAFMT  0x31 //Es para el registro Data Format
#define ADXL_REG_DATAX0   0x32 // DataX0

#define ADXL_CS_PORT GPIOB       //Pin asignado para CS
#define ADXL_CS_PIN GPIO_PIN_6

#define ADXL_SCALE 0.0039f  //Porque trabaja en Full Resolution
#define RAD2DEG 57.2957795f //Para conversion radianes a grados

//Calibración por offset
//Al medir los valores con el sensor quieto:
// x=0.0 y=-0.1 z=0.9
//Offsets Esperados:
static float OFFSETX = 0.0f;	// = x - 0.0
static float OFFSETY = 0.1f;  // = y - 0.1
static float OFFSETZ = -0.1f; // = z - 1.0

//Funciones privadas

//static void ADXL_Write(uint8_t reg, uint8_t data);
//static void ADXL_Read(uint8_t reg, uint8_t *buffer, uint8_t lenght);


static void ADXL_Write(uint8_t reg, uint8_t data){

	uint8_t writeBuf[2]; //Creo buffer de 2 Bytes, Reg y data

	writeBuf[0]=reg;
	writeBuf[1]=data;

	//CS en low, para acivar el Slave, luego definir los datos a enviar
	// y finalmente terminar la comunicación con el ADXL345
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, writeBuf, sizeof(writeBuf), 100);
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

}


static void ADXL_Read(uint8_t reg, uint8_t *buffer, uint8_t lenght){

	reg |= 0x80; //Para activar bit de lectura
	if (lenght>1) reg |= 0x40; //para activar bit de lectura multiple

	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, &reg, 1, 100); //Para enviar dirección del registro
	HAL_SPI_Receive(&hspi1, buffer, lenght, 100); //Para recibir lenght bytes
	HAL_GPIO_WritePin(ADXL_CS_PORT, ADXL_CS_PIN, GPIO_PIN_SET);

}

//Públicas

void ADXL345_Init(void){
	uint8_t chipID=0;

	ADXL_Read(0x00, &chipID, 1);

	if(chipID==0xE5){

		//Cuando el Sensor responde correctamente, se consiguran sus registros
		ADXL_Write(ADXL_REG_POWERCTL, 0x00); //Sensor en standby
		ADXL_Write(ADXL_REG_DATAFMT, 0x08); // Para definir el g-Range
		ADXL_Write(ADXL_REG_POWERCTL, 0x08); // Para el Measurement mode

	}else {
		//Error en el chipID
		while(1);
	}

}

void ADXL345_ReadRaw(int16_t *x, int16_t *y, int16_t *z){

	uint8_t buf[6];//buffer para los 6 Bytes de Datos de ADXL345

	//La lectura ocurre desde el Registro 0X32 (ADXL_REG_DATAX0)
	ADXL_Read(ADXL_REG_DATAX0, buf, 6);

	//Reconstruccion de x, y, z (MSB<<8|LSB)
	*x = (int16_t)((buf[1]<<8)|buf[0]);
	*y = (int16_t)((buf[3]<<8)|buf[2]);
	*z = (int16_t)((buf[5]<<8)|buf[4]);
}

//Calibración
void ADXL345_ReadGCalibrate(ADXL345_Data_t *data){

	int16_t xg, yg, zg;

	ADXL345_ReadRaw(&xg, &yg, &zg);

	//Para convertir a g (Full resolution equivale a 3.9mg/LSB

	data->x = (xg*ADXL_SCALE) - OFFSETX;
	data->y = (yg*ADXL_SCALE) - OFFSETY;
	data->z = (zg*ADXL_SCALE) - OFFSETZ;
}


void ADXL345_ReadAngles(ADXL345_Angles_t *angles) {

	ADXL345_Data_t aceler;

	//Para leer las aceleraciones en g
	ADXL345_ReadGCalibrate(&aceler);

	//Para el calculo de angulos [Rad]
	float rollRad = atan2f(aceler.y, aceler.z);

	float pitchRad = atan2f(-aceler.x, sqrtf((aceler.y * aceler.y)+(aceler.z * aceler.z)));

	//Conversion Rad a Grados
	angles->xroll = rollRad * RAD2DEG;
	angles->ypitch = pitchRad * RAD2DEG;

}

//Para filtrar los angulos y no verlos cambiantes en el LCD
void ADXL345_AnglesFiltered(ADXL345_Angles_t *angles){

	static float xrollf;
	static float ypitchf;
	static uint8_t initialized =0;
	const float alpha = 0.1f;

	ADXL345_Angles_t anglesraw;
	ADXL345_ReadAngles(&anglesraw);

	//Para los valores de angulos en bruto.
	if(!initialized){
		xrollf= anglesraw.xroll;
		ypitchf=anglesraw.ypitch;
		initialized=1;
	} else{
		xrollf = (alpha * anglesraw.xroll) + (1.0f - alpha) * xrollf;
		ypitchf = (alpha * anglesraw.ypitch) + (1.0f - alpha) * ypitchf;

	}

	angles -> xroll = xrollf;
	angles -> ypitch = ypitchf;
}
