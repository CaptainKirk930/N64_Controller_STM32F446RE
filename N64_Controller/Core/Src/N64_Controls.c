/*
 * N64_Controls.c
 *
 *  Created on: Dec 24, 2020
 *      Author: John Kirkpatrick
 */
#include "N64_Controls.h"

void delay (uint16_t delay) {
	__HAL_TIM_SET_COUNTER (&htim1, 0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
}

// Zero is sent with 3 microseconds low and 1 microsecond high
void Send_Zero() {
	// Set pin low
    HAL_GPIO_WritePin(DATA_OUT_GPIO_Port, DATA_OUT_Pin, GPIO_PIN_RESET);
    delay(THREE_MICROSECOND_DELAY);

    // Set pin high
    HAL_GPIO_WritePin(DATA_OUT_GPIO_Port, DATA_OUT_Pin, GPIO_PIN_SET);
    delay(MICROSECOND_DELAY);
}

// One is sent with 1 microsecond low and 3 microseconds high
void Send_One() {
	// Set pin low
	HAL_GPIO_WritePin(DATA_OUT_GPIO_Port, DATA_OUT_Pin, GPIO_PIN_RESET);
	delay(MICROSECOND_DELAY);

	// Set pin high
	HAL_GPIO_WritePin(DATA_OUT_GPIO_Port, DATA_OUT_Pin, GPIO_PIN_SET);
	delay(THREE_MICROSECOND_DELAY);
}

void Update_Controller_Status(N64_controller_u *N64_controller, uint8_t A, uint8_t B, uint8_t Z,
		uint8_t start, uint8_t up, uint8_t down, uint8_t left, uint8_t right, uint8_t left_bumper, uint8_t right_bumper,
		uint8_t C_up, uint8_t C_down, uint8_t C_left, uint8_t C_right, uint8_t X_axis, uint8_t Y_axis) {

	N64_controller->N64_controller_s.A_Button = A;
	N64_controller->N64_controller_s.B_Button = B;
	N64_controller->N64_controller_s.Z_Button = Z;
	N64_controller->N64_controller_s.Start_Button = start;
	N64_controller->N64_controller_s.Up_Button = up;
	N64_controller->N64_controller_s.Down_Button = down;
	N64_controller->N64_controller_s.Left_Button = left;
	N64_controller->N64_controller_s.Right_Button = right;
	N64_controller->N64_controller_s.Left_Bumper = left_bumper;
	N64_controller->N64_controller_s.Right_Bumper = right_bumper;
	N64_controller->N64_controller_s.C_Up = C_up;
	N64_controller->N64_controller_s.C_Down = C_down;
	N64_controller->N64_controller_s.C_Left = C_left;
	N64_controller->N64_controller_s.C_Right = C_right;
	N64_controller->N64_controller_s.X_Axis = X_axis;
	N64_controller->N64_controller_s.Y_Axis = Y_axis;
}

void Update_Controller_A(N64_controller_u *N64_controller, uint8_t A) {

	N64_controller->N64_controller_s.A_Button = A;
}

void Update_Controller_B(N64_controller_u *N64_controller, uint8_t B) {

	N64_controller->N64_controller_s.B_Button = B;
}

void Update_Controller_Z(N64_controller_u *N64_controller, uint8_t Z) {

	N64_controller->N64_controller_s.Z_Button = Z;
}

void Update_Controller_Start(N64_controller_u *N64_controller, uint8_t start) {

	N64_controller->N64_controller_s.Start_Button = start;
}

void Update_Controller_Up(N64_controller_u *N64_controller, uint8_t up) {

	N64_controller->N64_controller_s.Up_Button = up;
}

void Update_Controller_Down(N64_controller_u *N64_controller, uint8_t down) {

	N64_controller->N64_controller_s.Down_Button = down;
}

void Update_Controller_Left(N64_controller_u *N64_controller, uint8_t left) {

	N64_controller->N64_controller_s.Left_Button = left;
}

void Update_Controller_Right(N64_controller_u *N64_controller, uint8_t right) {

	N64_controller->N64_controller_s.Right_Button = right;
}

void Update_Controller_Left_Bumper(N64_controller_u *N64_controller, uint8_t left_bumper) {

	N64_controller->N64_controller_s.Left_Bumper = left_bumper;
}

void Update_Controller_Right_Bumper(N64_controller_u *N64_controller, uint8_t right_bumper) {

	N64_controller->N64_controller_s.Right_Bumper = right_bumper;
}

void Update_Controller_C_Up(N64_controller_u *N64_controller, uint8_t C_up) {

	N64_controller->N64_controller_s.C_Up = C_up;
}

void Update_Controller_C_Down(N64_controller_u *N64_controller, uint8_t C_down) {

	N64_controller->N64_controller_s.C_Down = C_down;
}

void Update_Controller_C_Left(N64_controller_u *N64_controller, uint8_t C_left) {

	N64_controller->N64_controller_s.C_Left = C_left;
}

void Update_Controller_C_Right(N64_controller_u *N64_controller, uint8_t C_right) {

	N64_controller->N64_controller_s.C_Right = C_right;
}

void Update_Controller_X_Axis(N64_controller_u *N64_controller, uint8_t X_axis) {

	N64_controller->N64_controller_s.X_Axis = X_axis;
}

void Update_Controller_Y_Axis(N64_controller_u *N64_controller, uint8_t Y_axis) {

	N64_controller->N64_controller_s.Y_Axis = Y_axis;
}

// This function will send a signal back to the N64 of the current status of the N64_controller union
void Press_Buttons(N64_controller_u *N64_controller) {

	// Loop through all the buttons
	for (uint8_t i = 0; i < MAX_BUTTONS_TO_PRESS; i++) {
		// Loop through each bit
		for (uint8_t j = 0; j < BYTE; j++) {
			// If bit is set to 1, send a one to N64, else send a zero
			if ( (N64_controller->N64_controller_a[i] & (0x01 << j)) == (1 << j)) {
				Send_One();
			} else {
				Send_Zero();
			}
		}
	}

	// Stop bit
	Send_One();
}
