# N64_Controller_STM32F446RE
This code allows one to program an STM32F446RE to emulate the response of an N64 controller.

Hardware:

Connect the ground pin from the N64 to the ground pin on the STM32F446RE.
Connect the data pin from the N64 to both pins D6 and D7 (PB10 and PA8) and the STM32F446RE. I recommend using a breadboard to "Y" the signal out for ease.

I also find it helpful to just cut the cord on an N64 controller and use those wires rather than inserting your own wires directly into the N64. 

Although it's not needed, keep in mind the N64 runs on 3.3V logic so don't go plugging in the power line to a 5V port.

How the code works:

There is a global union of a struct/array that holds the current status of the N64 controller.
This struct/array can be edited using Update_Controller_Status(). You will need to pass in the N64_controller struct/array and the status of all 14 buttons, as well as a byte value for the x-axis and y-axis of the analog joystick.

This struct/array can also be edited by calling functions like: Update_Controller_A() or Update_Controller_B(), which will require the N64_controller struct/array and just a 1 or 0 for the status of the button whished to be changed. Again, the analog joystick will take a byte for the x-axis and a byte for the y-axis.

The main loop is polling constantly for the signal sent from the N64 and will respond with what the N64_controller struct/array is currently set to.

I hope this is obvious, however, messing around with your N64 and controllers may result in damage to the system and/or controllers. I am not responsible for any damage to your system/controllers.

This is a work in progress.

Happy programming/gamming!

-John Kirkpatrick
