/*
 * fsm.c
 *
 *  Created on: 20 thg 11, 2022
 *      Author: Administrator
 */


#include "fsm.h"
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "software_timer.h"

UART_HandleTypeDef huart2;

uint8_t command_flag;
uint8_t command_data[30];

int status=GET_START;
int communication_status=RECIVE_COMMAND;

void str_cpy(uint8_t* str1, uint8_t* str2) {
	for (int i=0;i<MAX_BUFFER_SIZE;i++) {
		str1[i]=str2[i];
	}
}

void command_parser_fsm(){
	switch (status) {
		case GET_START:
			if (buffer[index_buffer-1]=='!') {
				status=GET_COMMAND;
			}
			else {
				status=GET_START;
				index_buffer=0;
			}
			break;
		case GET_COMMAND:
			if (index_buffer>29) {
				status=GET_START;
				index_buffer=0;
			}
			if (buffer[index_buffer-1]=='#') {
				str_cpy(command_data, buffer);
				command_flag=1;
				status=GET_START;
				index_buffer=0;
			}
			break;
		default:
			break;
	}
}

uint8_t compare(uint8_t* str1, uint8_t* str2) {
	for (int i=0;i<MAX_BUFFER_SIZE;i++) {
		if (str1[i]!=str2[i]) return 0;
		if (str1[i]=='#') return 1;
	}
	return 1;
}

void uart_communication_fsm(){
	char str[50];
	switch (communication_status) {
		case RECIVE_COMMAND:
			ADC_Value=HAL_ADC_GetValue(&hadc1);
			if (command_flag==1) {
				uint8_t temp[MAX_BUFFER_SIZE]="!RST#";
				if (compare(command_data, temp)==1) {
					communication_status=SEND_ADC;
					setTimer1(1);
				}
				command_flag=0;
			}

			break;
		case SEND_ADC:
			if (command_flag==1) {
				uint8_t temp[MAX_BUFFER_SIZE]="!OK#";
				if (compare(command_data, temp)==1) {
					setTimer1(0);
					communication_status=RECIVE_COMMAND;
				}
				command_flag=0;
			}
			if (timer1_flag==1) {

				HAL_UART_Transmit (&huart2 , (void*)str , sprintf (str , "\n%ld\n", ADC_Value), 1000);
				setTimer1(300);
			}

			break;
		default:
			break;
	}
}
