/*****************************************************************************
* Model: motorcontrol.qm
* File:  ./motorcontrol.c
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
/*${.::motorcontrol.c} .....................................................*/
/*****************************************************************************
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#include "qpn.h"
#include "bsp.h"
#include "motorcontrol.h"
#include <stdio.h>

Q_DEFINE_THIS_MODULE("motorcontrol")

#define APP_100MS_TIMEOUT        (QTimeEvtCtr)(BSP_TICKS_PER_SEC / 10U)


#if ((QP_VERSION < 580) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8)))
#error qpn version 5.8.0 or higher required
#endif

/*${AOs::MotorControl} .....................................................*/
typedef struct MotorControl {
/* protected: */
    QActive super;

/* private: */
    int16_t J1_VRx_Out;
    int16_t J1_VRy_Out;
    uint16_t motorPWMCompare;
    uint8_t motorDirection;
    uint8_t switchState;
    char_t txData[TX_DATA_SIZE];
} MotorControl;

/* protected: */
static QState MotorControl_initial(MotorControl * const me);
static QState MotorControl_operational(MotorControl * const me);
static QState MotorControl_swChanged(MotorControl * const me);
static QState MotorControl_ledFlashing(MotorControl * const me);


/*${AOs::AO_MotorControl} ..................................................*/
struct MotorControl AO_MotorControl;

/*${AOs::MotorControl_ctor} ................................................*/
void MotorControl_ctor(void) {
    MotorControl *me = &AO_MotorControl;
    QActive_ctor(&me->super, Q_STATE_CAST(&MotorControl_initial));
    me->J1_VRx_Out = 0;
    me->J1_VRy_Out = 0;
    me->motorPWMCompare = 0;
    me->motorDirection = MOTOR_DIR_CW;
    me->switchState = SWITCH_RELEASED;
}
/*${AOs::MotorControl} .....................................................*/
/*${AOs::MotorControl::SM} .................................................*/
static QState MotorControl_initial(MotorControl * const me) {
    /* ${AOs::MotorControl::SM::initial} */
    return Q_TRAN(&MotorControl_operational);
}
/*${AOs::MotorControl::SM::operational} ....................................*/
static QState MotorControl_operational(MotorControl * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::MotorControl::SM::operational} */
        case Q_ENTRY_SIG: {
            //QActive_armX((QActive *)me, 0U, APP_100MS_TIMEOUT, APP_100MS_TIMEOUT);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::MotorControl::SM::operational} */
        case Q_EXIT_SIG: {
            //QActive_disarmX(&me->super, 0U);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::MotorControl::SM::operational::initial} */
        case Q_INIT_SIG: {
            status_ = Q_TRAN(&MotorControl_ledFlashing);
            break;
        }
        /* ${AOs::MotorControl::SM::operational::SW_CHANGE} */
        case SW_CHANGE_SIG: {
            status_ = Q_TRAN(&MotorControl_swChanged);
            break;
        }
        /* ${AOs::MotorControl::SM::operational::LED_FLASH} */
        case LED_FLASH_SIG: {
            status_ = Q_TRAN(&MotorControl_ledFlashing);
            break;
        }
        /* ${AOs::MotorControl::SM::operational::CAN_J1VRX} */
        case CAN_J1VRX_SIG: {
            me->J1_VRx_Out = (int16_t)Q_PAR(me);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::MotorControl::SM::operational::CAN_J1VRY} */
        case CAN_J1VRY_SIG: {
            me->J1_VRy_Out = (int16_t)Q_PAR(me);
            sprintf(me->txData, "VRy: %u \r\n", me->J1_VRy_Out);
            BSP_printString(me->txData);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::MotorControl::SM::operational::MOTOR_PWM} */
        case MOTOR_PWM_SIG: {
            me->motorDirection = (uint8_t)Q_PAR(me);
            me->motorPWMCompare = (uint16_t)(Q_PAR(me) >> 16);

            BSP_setMotorSpeedDirection(me->motorDirection, me->motorPWMCompare);

            sprintf(me->txData, "VRx: %u => %hu \r\n", me->J1_VRx_Out, me->motorPWMCompare);
            BSP_printString(me->txData);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::MotorControl::SM::operational::LED_PULSE} */
        case LED_PULSE_SIG: {
            BSP_setLedPWM(me->motorPWMCompare);
            BSP_sendCANMsgLED1Data(me->motorPWMCompare);
            status_ = Q_HANDLED();
            break;
        }
        /* ${AOs::MotorControl::SM::operational::Q_TIMEOUT} */
        case Q_TIMEOUT_SIG: {
            /* Do nothing for now */
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*${AOs::MotorControl::SM::operational::swChanged} .........................*/
static QState MotorControl_swChanged(MotorControl * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::MotorControl::SM::operational::swChanged} */
        case Q_ENTRY_SIG: {
            /* Store the current switch state */
            me->switchState = (uint8_t)Q_PAR(me);

            if (me->switchState == SWITCH_PRESSED)
            {
                BSP_printString("switch pressed.\r\n");
            }
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&MotorControl_operational);
            break;
        }
    }
    return status_;
}
/*${AOs::MotorControl::SM::operational::ledFlashing} .......................*/
static QState MotorControl_ledFlashing(MotorControl * const me) {
    QState status_;
    switch (Q_SIG(me)) {
        /* ${AOs::MotorControl::SM::operational::ledFlashing} */
        case Q_ENTRY_SIG: {
            BSP_ledFlash();
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&MotorControl_operational);
            break;
        }
    }
    return status_;
}


