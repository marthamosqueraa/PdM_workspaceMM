/*
 * APIADXL.h
 *
 *  Created on: Apr 19, 2026
 *      Author: lilo
 */

#ifndef API_INC_APIADXL_H_
#define API_INC_APIADXL_H_

#include "stm32f4xx_hal.h"

//Estructura para la data en los 3 ejes
typedef struct
{
	float x;
	float y;
	float z;
}ADXL345_Data_t;

//Estructura para los valores en grados
typedef struct{
	float xroll; 	//Inclinacion eje X
	float ypitch; 	//Inclinacion eje Y
}ADXL345_Angles_t;

//Prototipos
void ADXL345_Init(void);
void ADXL345_ReadRaw(int16_t *x, int16_t *y, int16_t *z);
void ADXL345_ReadGCalibrate(ADXL345_Data_t *data);
void ADXL345_ReadAngles(ADXL345_Angles_t *angles);
void ADXL345_AnglesFiltered(ADXL345_Angles_t *angles);

#endif /* API_INC_APIADXL_H_ */
