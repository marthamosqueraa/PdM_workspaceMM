/*
 * APImeftilt.h
 *
 *  Created on: Apr 21, 2026
 *      Author: lilo
 */

#ifndef INC_MEFTILT_H_
#define INC_MEFTILT_H_

#include <APIadxl.h>
#include <APIlcd.h>
#include "API_debounce.h"
#include "API_delay.h"

// Enumeración de estados según requerimiento
typedef enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_CALIBRATION,
    STATE_READANDCOMPARE,
    STATE_REPORT,
    STATE_ALARM
} mefTiltState_t;

// Estructura para encapsular la MEF
typedef struct {
    mefTiltState_t currentState;
    float AzDisenio;
    float TiltDisenio;
    float DeltAz;
    float DeltT;
    ADXL345_Angles_t currentAngles;
} mefTilt_t;

// Prototipo principal
void mefTiltRun(void);

#endif /* INC_MEFTILT_H_ */
