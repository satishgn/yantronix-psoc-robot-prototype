/*****************************************************************************
* File    : bsp.h
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/
    
#ifndef bsp_h
#define bsp_h

#define BSP_TICKS_PER_SEC       (100u)

#define TX_DATA_SIZE            (50u)

#define SWITCH_RELEASED         (0u)
#define SWITCH_PRESSED          (1u)

#define MOTOR_DIR_CW            (0u)
#define MOTOR_DIR_CCW           (1u)

void BSP_init(void);
void BSP_printString(const char_t *string);
void BSP_ledOff(void);
void BSP_ledOn(void);
void BSP_ledFlash(void);
void BSP_setLedPWM(uint16_t ledPWMCompare);
void BSP_setMotorSpeedDirection(uint8_t motorDirection, uint16_t motorPWMCompare);
void BSP_sendCANMsgLED1Data(uint16_t ledPWMCompare);

#endif /* bsp_h */
