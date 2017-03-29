/*****************************************************************************
* File    : cyapicallbacks.h
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#ifndef CYAPICALLBACKS_H
#define CYAPICALLBACKS_H
    
/*Define your macro callbacks here */
/*For more information, refer to the Macro Callbacks topic in the PSoC Creator Help.*/

#define CAN_RECEIVE_MSG_J1VRxData_CALLBACK
void CAN_ReceiveMsg_J1VRxData_Callback();

#define CAN_RECEIVE_MSG_J1VRyData_CALLBACK
void CAN_ReceiveMsg_J1VRyData_Callback();

#define CAN_RECEIVE_MSG_J1SWData_CALLBACK
void CAN_ReceiveMsg_J1SWData_Callback();

#endif /* CYAPICALLBACKS_H */   
/* [] */
