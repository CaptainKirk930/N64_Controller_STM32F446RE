# N64_Controller_STM32F446RE
This code allows one to program an STM32F446RE to emulate the response of an N64 controller.

How the code works:

There is a global union of a struct/array that holds the current status of the N64 controller.
This struct/array can be edited using Update_Controller_Status(). You will need to pass in the N64_controller struct/array and the status of all 14 buttons, as well as a byte value for the x-axis and y-axis of the analog joystick.

This struct/array can also be edited by calling functions like: Update_Controller_A() or Update_Controller_B(), which will require the N64_controller struct/array and just a 1 or 0 for the status of the button whished to be changed. Again, the analog joystick will take a byte for the x-axis and a byte for the y-axis.

The main loop is polling constantly for the signal sent from the N64 and will respond with what the N64_controller struct/array is currently set to.

This is a work in progress.

Happy programming/gamming!

-John Kirkpatrick
