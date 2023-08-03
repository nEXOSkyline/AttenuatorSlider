# AttenuatorSlider
The Laser Attenuator Control uses two Autodesk Fusion 360 CAD models.

The dimensions/measurements for the two models can be found from the model themselves. 
I suggest creating a sketch on the face that has the edge that you are trying to get the dimensions from and then using the dimensions feature to acquire the measurement of whatever edge, face, etc.

The first model contains the primary bracket that is attached on top of the laser as well as the support L-Bracket that connects the primary bracket to the Fuyu Linear Sliding Stage

Here is the link to the Fusion 360 file, in case the DWG file that I attached does not work. 
https://a360.co/3OoeHLH



The second model contains the limit swtich fixture that holds the two limit switches in place. 

Here is the link to the Fusion 360 file, in case the DWG file that I attached does not work. 
https://a360.co/3QpZX1w

----------------------------------------------------------------------------------------------------------------------------------------------------------------

What has been done:
	Most of the hardware compononts has been designed, modeled, and printed via Fusion 360
	Most of the electronics/circuity has been wired and connected into a closed circuit

What still needs to be done: 
	The whole hardware setup needs to be tested at SLAC to see if the dimensions and design has been improved from the last time that I tested it.
	The Arduino/Python Gooey still needs to be developed.
	The Limit Switches needs to be tested to see if they work.
	The wire connecting the stepper driver to the stepper motor has been extended, but needs to test again to see if it was correctly made.
 
----------------------------------------------------------------------------------------------------------------------------------------------------------------

Notes about the Limit Switches:
The Upper Limit Switch color code is:
The red is paired with white. 
The black is paired with black. 
(This switch is referred to as UI in the Arduino code and it is the limit switch that is away from the stepper motor)

The Lower Limit Switch color code is:
The red is paired with red.
The black is paired with green.
(This switch is referred to as LI in the Arduino code and it is the limit switch that is towards/next to the stepper motor)

The two black wires should be wired to digital pins 8 and 10, while the two red wires are wired to the 5 volts on the Arduino.

----------------------------------------------------------------------------------------------------------------------------------------------------------------

Lastly, if there is anything that is unclear, ask the Professor, who can contact me. 
