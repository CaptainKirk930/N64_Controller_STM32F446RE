/*
 * N64_Controls.h
 *
 *  Created on: Dec 24, 2020
 *      Author: John Kirkpatrick
 */

#ifndef INC_N64_CONTROLS_H_
#define INC_N64_CONTROLS_H_

#include "tim.h"

#define MAX_BUTTONS_TO_PRESS      32

#define MICROSECOND_DELAY         80
#define THREE_MICROSECOND_DELAY   455

#define BYTE                      8

// N64 Controller Struct

typedef struct __attribute__ ((packed)){
	// Byte 0
	uint8_t A_Button:1;
	uint8_t B_Button:1;
	uint8_t Z_Button:1;
	uint8_t Start_Button:1;
	uint8_t Up_Button:1;
	uint8_t Down_Button:1;
	uint8_t Left_Button:1;
	uint8_t Right_Button:1;
	// Byte 1
	uint8_t Not_used:1;
	uint8_t Not_used2:1;
	uint8_t Left_Bumper:1;
	uint8_t Right_Bumper:1;
	uint8_t C_Up:1;
	uint8_t C_Down:1;
	uint8_t C_Left:1;
	uint8_t C_Right:1;
	// Byte 2
	uint8_t X_Axis:8;
	// Byte 3
	uint8_t Y_Axis:8;
}N64_controller_t;

typedef union {
	N64_controller_t N64_controller_s;
	uint8_t N64_controller_a[4];
}N64_controller_u;

void delay (uint16_t delay);

void Send_Zero();
void Send_One();

void Update_Controller_Status(N64_controller_u *N64_controller, uint8_t A, uint8_t B, uint8_t Z,
		uint8_t start, uint8_t up, uint8_t down, uint8_t left, uint8_t right, uint8_t left_bumper, uint8_t right_bumper,
		uint8_t C_up, uint8_t C_down, uint8_t C_left, uint8_t C_right, uint8_t X_axis, uint8_t Y_axis);

void Update_Controller_A(N64_controller_u *N64_controller, uint8_t A);
void Update_Controller_B(N64_controller_u *N64_controller, uint8_t B);
void Update_Controller_Z(N64_controller_u *N64_controller, uint8_t Z);
void Update_Controller_Start(N64_controller_u *N64_controller, uint8_t start);
void Update_Controller_Up(N64_controller_u *N64_controller, uint8_t up);
void Update_Controller_Down(N64_controller_u *N64_controller, uint8_t down);
void Update_Controller_Left(N64_controller_u *N64_controller, uint8_t left);
void Update_Controller_Right(N64_controller_u *N64_controller, uint8_t right);
void Update_Controller_Left_Bumper(N64_controller_u *N64_controller, uint8_t left_bumper);
void Update_Controller_Right_Bumper(N64_controller_u *N64_controller, uint8_t right_bumper);
void Update_Controller_C_Up(N64_controller_u *N64_controller, uint8_t C_up);
void Update_Controller_C_Down(N64_controller_u *N64_controller, uint8_t C_down);
void Update_Controller_C_Left(N64_controller_u *N64_controller, uint8_t C_left);
void Update_Controller_C_Right(N64_controller_u *N64_controller, uint8_t C_right);
void Update_Controller_X_Axis(N64_controller_u *N64_controller, uint8_t X_axis);
void Update_Controller_Y_Axis(N64_controller_u *N64_controller, uint8_t Y_axis);

void Press_Buttons(N64_controller_u *N64_controller);

#endif /* INC_N64_CONTROLS_H_ */
