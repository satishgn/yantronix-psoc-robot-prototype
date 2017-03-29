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

#define ADC_CHANNEL_NUMBER_0    (0u)
#define ADC_CHANNEL_NUMBER_1    (1u)

#define SWITCH_RELEASED         (0u)
#define SWITCH_PRESSED          (1u)

void BSP_init(void);
void BSP_printString(const char_t *string);
void BSP_ledOff(void);
void BSP_ledOn(void);
void BSP_ledFlash(void);
void BSP_setLedPWM(uint16_t PWM_Compare);
uint32_t BSP_isADCEOC(void);
int16_t BSP_getADCResult(uint16_t adcChannel);
void BSP_sendCANMsgJ1VRxData(int16_t J1_VRx_Mid, int16_t J1_VRx_Max, int16_t J1_VRx_Out);
void BSP_sendCANMsgJ1VRyData(int16_t J1_VRy_Mid, int16_t J1_VRy_Max, int16_t J1_VRy_Out);
void BSP_sendCANMsgJ1SWData(uint8_t switchState);

#endif /* bsp_h */
