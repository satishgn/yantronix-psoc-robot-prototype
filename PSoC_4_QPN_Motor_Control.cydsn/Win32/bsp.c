/*****************************************************************************
* Product: BSP for Remote Control, Win32
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
/*****************************************************************************
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#include "qpn.h"
#include "bsp.h"
#include "motorcontrol.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

Q_DEFINE_THIS_FILE

#define PWM_M1_PWM_PERIOD_VALUE		24000lu
#define PWM_LED_ACK             	PWM_M1_PWM_PERIOD_VALUE / 10

int16_t J1_VRx_Mid = 2047, J1_VRy_Mid = 2047;
int16_t J1_VRx_Max = 4095, J1_VRy_Max = 4095;
int16_t J1_VRx_Out = 2047, J1_VRx_Out_Prev = 2047;
int16_t J1_VRy_Out = 2047, J1_VRy_Out_Prev = 2047;

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*..........................................................................*/
void BSP_init() {
    printf("Win32 Test\nQP-nano version: %s\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);
}

void BSP_printString(const char_t *string)
{
    printf(string);
}

void BSP_ledOff() {
    //printf("OFF\n");
}

void BSP_ledOn() {
    //printf("ON\n");
}

void BSP_ledFlash(void) {
    printf("LED Flashing...\n");
}

void BSP_setLedPWM(uint16_t ledPWMCompare) {
    printf("LED Pulsing [%d]...\n", ledPWMCompare);
}

void BSP_setMotorSpeedDirection(uint8_t motorDirection, uint16_t motorPWMCompare) {
	printf("Motor Speed: %d and Direction: %d\n", motorPWMCompare, motorDirection);
}

void BSP_sendCANMsgLED1Data(uint16_t ledPWMCompare) {
	(void)ledPWMCompare;
}

void Q_onAssert(char_t const Q_ROM * const file, int_t line) {
    fprintf(stderr, "\nAssertion failed in %s, line %d\n", file, line);
    exit(-1);
}

void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC);
}

void QF_onCleanup(void) {
    printf("\nBye! Bye!\n");
}

void QF_onClockTickISR(void) {
    QF_tickXISR(0U); /* perform the QF-nano clock tick processing */

    if (_kbhit()) { /* any key pressed? */
        int ch = _getch();
        switch (ch)
		{
			case 27 : { /* ESC key pressed */
            QF_stop();
			break;
			}
			
			case 32 : { /* SHIFT key pressed */
			QACTIVE_POST_ISR(&AO_MotorControl, SW_CHANGE_SIG, SWITCH_PRESSED);
			break;
			}

			case 72 : { /* UP key pressed */
            ++J1_VRy_Out;
			break;
			}

			case 80 : { /* DOWN key pressed */
            --J1_VRy_Out;
			break;
			}

			case 77 : { /* RIGHT key pressed */
            ++J1_VRx_Out;
			break;
			}

			case 75 : { /* LEFT key pressed */
            --J1_VRx_Out;
			break;
			}	
		}
    }
	
	if (J1_VRx_Out_Prev != J1_VRx_Out)
	{
		uint16_t motorPWMCompare = 0;
		uint8_t motorDirection = MOTOR_DIR_CW;

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
		
		J1_VRx_Out_Prev = J1_VRx_Out;
	}
	
	if (J1_VRy_Out_Prev != J1_VRy_Out)
	{	
		/* Saturates ADC result to positive numbers */
		if (J1_VRy_Out < 0)
		{
			J1_VRy_Out = 0;
		}
		
		(void)J1_VRy_Mid;
		(void)J1_VRy_Max;
		
		QACTIVE_POST_ISR(&AO_MotorControl, CAN_J1VRY_SIG, J1_VRy_Out);   
		
		J1_VRy_Out_Prev = J1_VRy_Out;
	}
}




