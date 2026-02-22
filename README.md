# LED Matrix Drawer
## Overview
An interactive sketching interface utilising Arduino r4 wifi's built in 12x8 LED matrix, a joystick and 2 leds. Features real time toggling (drawing/erasing) aswell as quality of life features (responsive feedback, etc)
Note that this is the first phase refer to [Planned Features](#planned-features).

## Clip

![Slow Pan](slowpan.gif)

## Hardware
* Arduino uno R4 Wifi   &rarr; Controller
* The LED Matrix on the arduino &rarr; Main display
* Breadboard &rarr; Means of connection
* Male to male jumper wires 
* Female to male jumper wires 
* Analog Joystick  &rarr; Main input method
* Blue and Green LED (can be any just 2 dinstinct but Blue is prefered) &rarr; One for checking power and the other as a responsive feature
* 2 220 Ohm resistors 
### Pins
Green LED (responsive one) &rarr; Digital pin 4

Blue LED (power led) &rarr; 5V

Joystick X/Y &rarr; A1/A2 

Joystick Button &rarr; Digital pin 2

## Features
* Responsive feel
* State aware blinking; depending on if the current LED is on and off, ensuring always a "confirmation pulse"
* Precise movements with a deadzone/threshold of 100
* Only the art is saved in memory making this fairly efficient
* Instant lookup for when erasing and drawing utilising 'struct' and 'std::find' to check coords in O(*N*) time
* The drawing itself is more of a toggle system:
  1. Button press
  2. If exist &rarr; delete + unique led blink
  3. else &rarr; draw + unique led blink
* The green LED (or in general the responsive LED) serves multiple purposes:
  
  &rarr; Proximity check: When hovering over a pixel (LED), it will be on if its already a saved pixel (so a HIGH led)
  
  &rarr; Serves as visual feedback on when the process of drawing/erasing is done
  
  &rarr; Serves aswell as a buffer allowing the code to catch up to the user ensuring smooth and precise control
  
  &rarr; General purpose responsiveness (refer to state aware blinking)

* Blue LED (power LED) for a visual indicator of power and any flunctuation in voltage (knowing that blue LED are volt hungry so any dimming means a dip in voltage)
* The code clears and redraws the entire frame only when a change is detected '(Changed == true)', preventing the matrix from flickering or ghosting
* Input Sanitization: Uses the 'constrain()' function to prevent the cursor from "falling off" the edges of the matrix, which would otherwise crash the coordinate logic
* Cool startup sequence (cool asf)
* Debugging capabilities for checking Joystick values 'printPOS()'

## Schematic link
Small comment, on Wokwi, there is no uno r4 with the matrix so the logic is the same (aswell as the wiring), what changed is the extra matrix screen and the actual code (lowk generated it since i didnt wanna learn it for a simulation of my work), same logic applies tho

[Wokwi LED matrix drawer simulation](https://wokwi.com/projects/456675777169957889)

| Physical | Digital |
|----------| ------------|
| Code in git (actual project) | Ai generated code for simulation purpose |
| No extra matrix screen | Extra matrix screen |
| Matrix LED on Arduino | No matrix LED on Arduino |
| Arduino R4 wifi | Arduino R3 |
| Actual Project | Just a simulation to show what it looks like (features missing) |

## Planned features
* Shake to clear (instead of pressing the reset button)
* Cooler intro
* Save function to pc
* Make a digital case and pcb
