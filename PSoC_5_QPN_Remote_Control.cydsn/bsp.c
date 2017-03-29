/*****************************************************************************
* File    : bsp.c
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#include "qpn.h"
#include "bsp.h"
#include "project.h"
#include "core_cm3_psoc5.h"
#include "remotecontrol.h"
#include <stdio.h>

//Q_DEFINE_THIS_FILE

/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
* DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
*/
enum KernelUnawareISRs { /* see NOTE00 */
    /* ... */
    MAX_KERNEL_UNAWARE_CMSIS_PRI  /* keep always last */
};
/* "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts */
Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

enum KernelAwareISRs {
    SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI, /* see NOTE00 */
    /* ... */
    MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};
/* "kernel-aware" interrupts should not overlap the PendSV priority */
Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >>(8-__NVIC_PRIO_BITS)));

#define BTN_SW                 (1U << SW_SHIFT)

int32 map(int32 x, int32 in_min, int32 in_max, int32 out_min, int32 out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

CY_ISR(SysTick_Handler) {
    /* state of the button debouncing, see below */
    static struct ButtonsDebouncing {
        uint32_t depressed;
        uint32_t previous;
    } buttons = { ~0U, ~0U };
    uint32_t current;
    uint32_t tmp;
    
    QF_tickXISR(0U); /* process time events for rate 0 */
    
    /* Perform the debouncing of buttons. The algorithm for debouncing
    * adapted from the book "Embedded Systems Dictionary" by Jack Ganssle
    * and Michael Barr, page 71.
    */
    current = ~SW_PS; /* read Port with the state of Button */
    tmp = buttons.depressed; /* save the debounced depressed buttons */
    buttons.depressed |= (buttons.previous & current); /* set depressed */
    buttons.depressed &= (buttons.previous | current); /* clear released */
    buttons.previous   = current; /* update the history */
    tmp ^= buttons.depressed;     /* changed debounced depressed */
    if ((tmp & BTN_SW) != 0U) {  /* debounced B1 state changed? */
        if ((buttons.depressed & BTN_SW) != 0U) { /* is BTN_SW1 depressed? */
            QACTIVE_POST_ISR(&AO_RemoteControl, SW_CHANGE_SIG, SWITCH_PRESSED);
        }
    }    
}

/* BSP functions ===========================================================*/
void BSP_init(void) {
    UART_Start();
    ADC_Start();
    PWM_L1_Start();
    PWM_L2_Start();

    /* Start ADC conversion */
    ADC_StartConvert();

    CAN_Start();
    
    /* Point the Systick vector to the ISR */
    CyIntSetSysVector(CY_INT_SYSTICK_IRQN, SysTick_Handler);
}

void BSP_printString(const char_t *string)
{
    UART_PutString(string);
}

void BSP_ledOff(void) {
    //LED1_Write(0);
}

void BSP_ledOn(void) {
    //LED1_Write(1);
}

void BSP_ledFlash(void) {
    Control_LED1_Write(0);
}

void BSP_setLedPWM(uint16_t PWM_Compare) {
    /* Set PWM pulse width */
    PWM_L2_WriteCompare(PWM_Compare);
    Control_LED1_Write(1);
}

uint32_t BSP_isADCEOC(void) {
    return ADC_IsEndConversion(ADC_RETURN_STATUS);
}

int16_t BSP_getADCResult(uint16_t adcChannel) {
    return ADC_GetResult16(adcChannel);
}

void BSP_sendCANMsgJ1VRxData(int16_t J1_VRx_Mid, int16_t J1_VRx_Max, int16_t J1_VRx_Out)
{
    CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_J1VRxData) = HI8(J1_VRx_Mid);
    CAN_TX_DATA_BYTE2(CAN_TX_MAILBOX_J1VRxData) = LO8(J1_VRx_Mid);
    CAN_TX_DATA_BYTE3(CAN_TX_MAILBOX_J1VRxData) = HI8(J1_VRx_Max);
    CAN_TX_DATA_BYTE4(CAN_TX_MAILBOX_J1VRxData) = LO8(J1_VRx_Max);
    CAN_TX_DATA_BYTE5(CAN_TX_MAILBOX_J1VRxData) = HI8(J1_VRx_Out);
    CAN_TX_DATA_BYTE6(CAN_TX_MAILBOX_J1VRxData) = LO8(J1_VRx_Out);
    
    CAN_SendMsgJ1VRxData();
}

void BSP_sendCANMsgJ1VRyData(int16_t J1_VRy_Mid, int16_t J1_VRy_Max, int16_t J1_VRy_Out)
{
    CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_J1VRyData) = HI8(J1_VRy_Mid);
    CAN_TX_DATA_BYTE2(CAN_TX_MAILBOX_J1VRyData) = LO8(J1_VRy_Mid);
    CAN_TX_DATA_BYTE3(CAN_TX_MAILBOX_J1VRyData) = HI8(J1_VRy_Max);
    CAN_TX_DATA_BYTE4(CAN_TX_MAILBOX_J1VRyData) = LO8(J1_VRy_Max);
    CAN_TX_DATA_BYTE5(CAN_TX_MAILBOX_J1VRyData) = HI8(J1_VRy_Out);
    CAN_TX_DATA_BYTE6(CAN_TX_MAILBOX_J1VRyData) = LO8(J1_VRy_Out);
    
    CAN_SendMsgJ1VRyData();
}

void BSP_sendCANMsgJ1SWData(uint8_t switchState)
{
    CAN_TX_DATA_BYTE1(CAN_TX_MAILBOX_J1SWData) = switchState;
    
    CAN_SendMsgJ1SWData();
}

void CAN_ReceiveMsg_LED1Data_Callback()
{        
    uint16 PWM_Compare = ((int16) ((int16) CAN_RX_DATA_BYTE1(CAN_RX_MAILBOX_LED1Data) << 8)) | CAN_RX_DATA_BYTE2(CAN_RX_MAILBOX_LED1Data);
    
    QACTIVE_POST_ISR(&AO_RemoteControl, CAN_RECEIVE_SIG, PWM_Compare);
}

/* QF callbacks ============================================================*/
void QF_onStartup(void) {    
    /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(BCLK__BUS_CLK__HZ / BSP_TICKS_PER_SEC);

    /* assing all priority bits for preemption-prio. and none to sub-prio. */
    NVIC_SetPriorityGrouping(0U);

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
