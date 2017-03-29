/*****************************************************************************
* File    : bsp.c
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#include "qpn.h"
#include "bsp.h"
#include "project.h"
#include "core_cm0_psoc4.h"  /* CMSIS-compliant header file for the MCU used */
#include "motorcontrol.h"
#include <stdio.h>

//Q_DEFINE_THIS_FILE

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
* DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
*/
//enum KernelUnawareISRs { /* see NOTE00 */
//    /* ... */
//    MAX_KERNEL_UNAWARE_CMSIS_PRI  /* keep always last */
//};
///* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
//Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

enum KernelAwareISRs {
    SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI, /* see NOTE00 */
    /* ... */
    MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >>(8-__NVIC_PRIO_BITS)));

//#define BTN_SW                 (1U << SW_SHIFT)
#define PWM_LED_ACK             PWM_M1_PWM_PERIOD_VALUE / 10

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

CY_ISR(SysTick_Handler)
{
//    /* state of the button debouncing, see below */
//    static struct ButtonsDebouncing {
//        uint32_t depressed;
//        uint32_t previous;
//    } buttons = { ~0U, ~0U };
//    uint32_t current;
//    uint32_t tmp;
    
    QF_tickXISR(0U); /* process time events for rate 0 */
    
//    /* Perform the debouncing of buttons. The algorithm for debouncing
//    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
//    * and Michael Barr, page 71.
//    */
//    current = ~SW_PS; /* read Port with the state of Button */
//    tmp = buttons.depressed; /* save the debounced depressed buttons */
//    buttons.depressed |= (buttons.previous & current); /* set depressed */
//    buttons.depressed &= (buttons.previous | current); /* clear released */
//    buttons.previous   = current; /* update the history */
//    tmp ^= buttons.depressed;     /* changed debounced depressed */
//    if ((tmp & BTN_SW) != 0U) {  /* debounced B1 state changed? */
//        if ((buttons.depressed & BTN_SW) != 0U) { /* is BTN_SW1 depressed? */
//            QACTIVE_POST_ISR(&AO_MotorControl, SW_CHANGE_SIG, SWITCH_PRESSED);
//        }
//    }    
}

/* BSP functions ===========================================================*/
void BSP_init(void)
{
    UART_Start();

    PWM_M1_Start();
    PWM_M2_Start();
    PWM_M3_Start();

    PWM_L1_Start();
    PWM_L2_Start();

    CAN_Start();
    
    /* Point the Systick vector to the ISR */
    CyIntSetSysVector(CY_INT_SYSTICK_IRQN, SysTick_Handler);
}

void BSP_printString(const char_t *string)
{
    UART_UartPutString(string);
}

void BSP_ledOff(void)
{
    //LED1_Write(0);
}

void BSP_ledOn(void)
{
    //LED1_Write(1);
}

void BSP_ledFlash(void)
{
    Control_LED1_Write(0);
}

void BSP_setLedPWM(uint16_t ledPWMCompare)
{
    /* Set PWM pulse width */
    PWM_L2_WriteCompare(ledPWMCompare);
    Control_LED1_Write(1);
}

void BSP_setMotorSpeedDirection(uint8_t motorDirection, uint16_t motorPWMCompare)
{
    if (motorDirection == MOTOR_DIR_CW)
    {
        M1_IN1_Write(0);
        M1_IN2_Write(1);

        M2_IN3_Write(0);
        M2_IN4_Write(1);

        M3_IN1_Write(0);
        M3_IN2_Write(1);                        
    } 
    else
    {
        M1_IN1_Write(1);
        M1_IN2_Write(0);
        
        M2_IN3_Write(1);
        M2_IN4_Write(0);

        M3_IN1_Write(1);
        M3_IN2_Write(0);
    }
    
    PWM_M1_WriteCompare(motorPWMCompare);
    PWM_M2_WriteCompare(motorPWMCompare);
    PWM_M3_WriteCompare(motorPWMCompare);
}

void BSP_sendCANMsgLED1Data(uint16_t ledPWMCompare)
{
    CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_LED1Data, HI8(ledPWMCompare));
    CAN_TX_DATA_BYTE2(CAN_TX_MAILBOX_LED1Data, LO8(ledPWMCompare));
    
    CAN_SendMsgLED1Data();
}

void CAN_ReceiveMsg_J1VRxData_Callback()
{
    uint16_t motorPWMCompare = 0;
    uint8_t motorDirection = MOTOR_DIR_CW;
    
    int16_t J1_VRx_Mid = ((int16) ((int16) CAN_RX_DATA_BYTE1(CAN_RX_MAILBOX_J1VRxData) << 8)) | CAN_RX_DATA_BYTE2(CAN_RX_MAILBOX_J1VRxData);
    int16_t J1_VRx_Max = ((int16) ((int16) CAN_RX_DATA_BYTE3(CAN_RX_MAILBOX_J1VRxData) << 8)) | CAN_RX_DATA_BYTE4(CAN_RX_MAILBOX_J1VRxData);
    int16_t J1_VRx_Out = ((int16) ((int16) CAN_RX_DATA_BYTE5(CAN_RX_MAILBOX_J1VRxData) << 8)) | CAN_RX_DATA_BYTE6(CAN_RX_MAILBOX_J1VRxData);

    /* Saturates ADC result to positive numbers */
    if (J1_VRx_Out < 0)
    {
        J1_VRx_Out = 0;
    }
                
    if (J1_VRx_Out < J1_VRx_Mid)
    {
        motorPWMCompare = map(J1_VRx_Out, J1_VRx_Mid, 0, 0, PWM_M1_PWM_PERIOD_VALUE);
        motorDirection = MOTOR_DIR_CCW;
    }
    else
    {
        motorPWMCompare = map(J1_VRx_Out, J1_VRx_Mid, J1_VRx_Max, 0, PWM_M1_PWM_PERIOD_VALUE);
        motorDirection = MOTOR_DIR_CW;
    }

    QACTIVE_POST_ISR(&AO_MotorControl, CAN_J1VRX_SIG, J1_VRx_Out);
    QACTIVE_POST_ISR(&AO_MotorControl, MOTOR_PWM_SIG, (motorPWMCompare << 16) | motorDirection);
    if (motorPWMCompare > PWM_LED_ACK)
    {
        QACTIVE_POST_ISR(&AO_MotorControl, LED_PULSE_SIG, 0);      
    }
    else
    {
        QACTIVE_POST_ISR(&AO_MotorControl, LED_FLASH_SIG, 0);              
    }
}

void CAN_ReceiveMsg_J1VRyData_Callback()
{    
    int16_t J1_VRy_Mid = ((int16) ((int16) CAN_RX_DATA_BYTE1(CAN_RX_MAILBOX_J1VRyData) << 8)) | CAN_RX_DATA_BYTE2(CAN_RX_MAILBOX_J1VRyData);
    int16_t J1_VRy_Max = ((int16) ((int16) CAN_RX_DATA_BYTE3(CAN_RX_MAILBOX_J1VRyData) << 8)) | CAN_RX_DATA_BYTE4(CAN_RX_MAILBOX_J1VRyData);
    int16_t J1_VRy_Out = ((int16) ((int16) CAN_RX_DATA_BYTE5(CAN_RX_MAILBOX_J1VRyData) << 8)) | CAN_RX_DATA_BYTE6(CAN_RX_MAILBOX_J1VRyData);    

    /* Saturates ADC result to positive numbers */
    if (J1_VRy_Out < 0)
    {
        J1_VRy_Out = 0;
    }
    
    (void)J1_VRy_Mid;
    (void)J1_VRy_Max;
    
    QACTIVE_POST_ISR(&AO_MotorControl, CAN_J1VRY_SIG, J1_VRy_Out);   
}

void CAN_ReceiveMsg_J1SWData_Callback()
{    
    uint8_t switchState = CAN_RX_DATA_BYTE1(CAN_RX_MAILBOX_J1SWData);
    
    QACTIVE_POST_ISR(&AO_MotorControl, SW_CHANGE_SIG, switchState);
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {    
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(CYDEV_BCLK__SYSCLK__HZ / BSP_TICKS_PER_SEC);

    /* set priorities of ALL ISRs used in the system, see NOTE00
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    * Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
    * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
    */
    NVIC_SetPriority(SysTick_IRQn,   SYSTICK_PRIO);
    /* ... */

    /* enable IRQs... */

    CyGlobalIntEnable; /* Enable global interrupts. */
}

/*..........................................................................*/
void QV_onIdle(void) { /* CATION: called with interrupts DISABLED, NOTE01 */
#ifdef NDEBUG
    /* Put the CPU and peripherals to the low-power mode.
    * you might need to customize the clock management for your application,
    * see the datasheet for your particular Cortex-M MCU.
    */
    QV_CPU_SLEEP();  /* atomically go to sleep and enable interrupts */
#else
    QF_INT_ENABLE(); /* just enable interrupts */
#endif
}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR module, int loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;

    NVIC_SystemReset();
}

/*****************************************************************************
* NOTE00:
* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
* ISR priority that is disabled by the QF framework. The value is suitable
* for the NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
* with the numerical values of priorities equal or higher than
* QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
* are "QF-aware".
*
* Conversely, any ISRs prioritized above the QF_AWARE_ISR_CMSIS_PRI priority
* level (i.e., with the numerical values of priorities less than
* QF_AWARE_ISR_CMSIS_PRI) are never disabled and are not aware of the kernel.
* Such "QF-unaware" ISRs cannot call any QF services. The only mechanism
* by which a "QF-unaware" ISR can communicate with the QF framework is by
* triggering a "QF-aware" ISR, which can post/publish events.
*
* NOTE01:
* The QV_onIdle() callback is called with interrupts disabled, because the
* determination of the idle condition might change by any interrupt posting
* an event. QV_onIdle() must internally enable interrupts, ideally
* atomically with putting the CPU to the power-saving mode.
*
* NOTE02:
* One of the LEDs is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts locked, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
