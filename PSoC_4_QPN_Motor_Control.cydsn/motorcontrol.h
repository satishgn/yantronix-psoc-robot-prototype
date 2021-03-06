/*****************************************************************************
* Model: motorcontrol.qm
* File:  ./motorcontrol.h
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${.::motorcontrol.h} .....................................................*/
/*****************************************************************************
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#ifndef motorcontrol_h
#define motorcontrol_h

enum MotorControlSignals {
    RESERVE_SIG = Q_USER_SIG,
    LED_FLASH_SIG,
    LED_PULSE_SIG,
    SW_CHANGE_SIG,
    MOTOR_PWM_SIG,
    CAN_J1VRX_SIG,
    CAN_J1VRY_SIG,
    TERMINATE_SIG,
    MAX_SIG
};

extern struct MotorControl AO_MotorControl;

/*${AOs::MotorControl_ctor} ................................................*/
void MotorControl_ctor(void);


#endif /* motorcontrol_h */
