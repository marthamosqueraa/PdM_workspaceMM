/*
 * MEFtilt.c.c
 *
 *  Created on: Apr 19, 2026
 *      Author: lilo
 */

#include <math.h>
#include <MEFtilt.h>

//Vars privada de la maquina
static mefTilt_t smartTilt;
static delay_t alarmDelay;

// Delta Tilt Limite aceptado
#define TILT_LIMIT 5.0f

void mefTiltRun(void) {
    // Para actualizar el estado de button en cada ciclo
    debounceFSM_update();
    pressType_t btnEvent = readKey();

    switch (smartTilt.currentState) {

        case STATE_INIT:
        	APILCD_init();
            ADXL345_Init();
            debounceFSM_init();

            // Inicialización de variables
            //Valores diseño de la Red son conocidos por Equipo técnico
            smartTilt.AzDisenio = 0.0f;
            smartTilt.TiltDisenio = 0.0f;
            delayInit(&alarmDelay, 100);

            APILCD_clear();
            APILCD_sendstring("Bienvenido");
            HAL_Delay(1500); // Pausa breve para lectura

            smartTilt.currentState = STATE_IDLE;
            APILCD_clear();
            APILCD_sendstring("Estado: IDLE");
            break;

        case STATE_IDLE:
            if (btnEvent == SHORT_PRESSED) {
                smartTilt.currentState = STATE_READANDCOMPARE;
            } else if (btnEvent == LONG3_PRESSED) {
                smartTilt.currentState = STATE_CALIBRATION;
            }
            break;

        case STATE_CALIBRATION:
        	APILCD_clear();
        	APILCD_sendstring("Calibrando...");
            ADXL345_Data_t calData;
            ADXL345_ReadGCalibrate(&calData); // Llamado a la funcion de calibracion de APIadxl.c

            HAL_Delay(2000); // Para simular tiempo del proceso
            smartTilt.currentState = STATE_IDLE;
            APILCD_clear();
            APILCD_sendstring("IDLE - Listo");
            break;

        case STATE_READANDCOMPARE:
            // Para obtener los valores filtrados de los ángulos
            ADXL345_AnglesFiltered(&smartTilt.currentAngles);

            // Realizamos el cálculo de las desviaciones = ValoresMedidos - ValoresDiseño
            smartTilt.DeltAz = fabsf(smartTilt.currentAngles.xroll - smartTilt.AzDisenio);
            smartTilt.DeltT = fabsf(smartTilt.currentAngles.ypitch - smartTilt.TiltDisenio);

            if (smartTilt.DeltT < TILT_LIMIT) {
                smartTilt.currentState = STATE_REPORT;
            } else {
                smartTilt.currentState = STATE_ALARM;
                APILCD_clear(); // Limpiar una vez al entrar a alarma
            }
            break;

        case STATE_REPORT:
        	//APILCD_clear();
        	APILCD_sendcmd(0x80);
        	APILCD_sendstring("Az:");
        	APILCD_sendfloat(smartTilt.currentAngles.xroll, 5);
        	APILCD_sendstring("     ");
        	APILCD_sendcmd(0xC0); // Segunda línea
            APILCD_sendstring("T:");
            APILCD_sendfloat(smartTilt.currentAngles.ypitch, 5);
            APILCD_sendstring(" OK");

            // Presionar Button 1, determina el volver a IDLE
            if (btnEvent == SHORT_PRESSED || btnEvent == LONG3_PRESSED) {
                smartTilt.currentState = STATE_IDLE;
                APILCD_clear();
                APILCD_sendstring("Estado: IDLE");
            }
        break;

        case STATE_ALARM:
            // ALARMA equivale a 3312 y a la variacion de Tilt DeltT
            if (delayRead(&alarmDelay)) {
                HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

                APILCD_sendcmd(0x80);
                APILCD_sendstring("3312WARNING");
                APILCD_sendcmd(0xC0);
                APILCD_sendstring("DT:");
                APILCD_sendfloat(smartTilt.DeltT, 5);

            }

            if (btnEvent == SHORT_PRESSED || btnEvent == LONG3_PRESSED) {
            	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);//Para al salir de alarma, se apague el LED
                smartTilt.currentState = STATE_IDLE;
                APILCD_clear();
                APILCD_sendstring("Estado: IDLE");
            }
            break;

        default:
            smartTilt.currentState = STATE_INIT;
        break;
    }
}
