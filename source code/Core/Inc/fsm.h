/*
 * fsm.h
 *
 *  Created on: 20 thg 11, 2022
 *      Author: Administrator
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"

//flags
extern uint8_t command_flag;
extern uint8_t command_data[30];

//status
extern int status;
extern int communication_status;


//define states
#define GET_START		0
#define GET_COMMAND		1
#define	GET_END			2

#define RECEIVE_COMMAND	10
#define SEND_ADC		11

//prototypes
void command_parser_fsm() ;
void uart_communication_fsm();

#endif /* INC_FSM_H_ */
