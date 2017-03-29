/*****************************************************************************
* Product: PSoC_4_QPN_Motor_Control
* Last Updated for Version: 5.8.2
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
* http://www.state-machine.com
* mailto:info@state-machine.com
*****************************************************************************/
/*****************************************************************************
* File    : main.c
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#include "qpn.h"            /* QP-nano */
#include "bsp.h"            /* Board Support Package */
#include "motorcontrol.h"   /* Application interface */

/*..........................................................................*/
static QEvt l_motorControlQueue[10];

/* QF_active[] array defines all active object control blocks --------------*/
QActiveCB const Q_ROM QF_active[] = {
    { (QActive *)0,                  (QEvt *)0,             0U                           },
    { (QActive *)&AO_MotorControl,   l_motorControlQueue,   Q_DIM(l_motorControlQueue)   }
};

/*..........................................................................*/
int main() {
    MotorControl_ctor();        /* instantiate all AOs */

    QF_init(Q_DIM(QF_active));  /* initialize the QF-nano framework */
    BSP_init();                 /* initialize the Board Support Package */

    return QF_run();            /* transfer control to QF-nano */
}
