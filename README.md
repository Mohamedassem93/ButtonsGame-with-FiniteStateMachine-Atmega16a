# ButtonsGame-with-FiniteStateMachine-Atmega16a
### Buttons Game using Moore finite state machine Designed &amp; Implemented on Atmega16a microcontroller.

![img_20180220_115751](https://user-images.githubusercontent.com/29497414/38028384-1152b7c6-3293-11e8-84e3-aebfda812c8f.jpg)
## Design & Development process:
By using Top-Down Design methodology, this project moves from #Requirements to #Design and #Implementation through designing and implementing #Moore finite state machine.
# NOTE:
- In #Requirement stage, I used some expected requirements and specs to be as my imaginary agreement with my client that asked me for this #Game system .. thinking about what are the expected client requirements is a pretty good way to design and implement the project with well-defined guidelines for a more efficient final project in both software & hardware.
- In #Design stage, I used neither #Flowchart nor #Pseudo code in #Algorithm designing since Moore FSM(finite state machine) describes the system as a machine that produces outputs automatically according to current system state.
- Finally, #Testing is done using #Debug on simulation with Atmel Studio #Simulator (this is already verified and you can run the project on #simulator to verify it by yourself).


# 1) Requirements:
What will the system do? Specs and Constraints?

### System Overview:
Buttons game using 2 buttons and 10 LEDs (1 button + 5 LEDs for each player) since there are 2 players only to play the game. Once started, the fattest player turns on all his all 5 LEDs before the other's LEDs are all turned on, he wins and the game restarts automatically so the 2 players can play again.

### Functions:
The game system does the following:
1- Starts with an initial state where all 10 LEDs are off.
2- when pressing push switch many times the LEDs are turned on sequentially till any player completes his 5 LEDs on.
3- when any player completes his 5 LEDs on, the whole 5 LEDs of the winner must blink as 200ms on >> 200 off >> 200ms on >> all off(the initial state) and at the same time whatever the player record of the on LEDs, his LEDs must be turned off immediately.

### Constraints:
Develop the system prototype using AVR 8 bits microcontroller only.

### Deliverable:
The system may be delivered on a breadboard since it's considered as a prototype, not a final product, but you must decrease the number of wires as possible as you can. Note that installing the controller chip on a breadboard is forbidden by any mean so please use a kit for this.

### Power:
The game must use small power consumption (no need for an external battery as USB programmer would be more than enough) and you may use whatever current limiters to reduce power.

### Time of delivery:
Since this is a prototype, you take your time for design, development, and testing, no deadline is specified.

### Prototype Size:
As small as the breadboard used, but with enough space between the 2 playing switches for comfort playing experience.

### Safety & Security:
Nothing specific, just make sure you implement a stable system with well-connected wires.


# 2) Design:
- By using #Moore FSM as the output is a function of the current state (in other words, output pattern defines what the current state means).
- I used State Transition Table instead of STG(state transition graph) since the table is easier to me.
### check this image for the FSM design:
![img_20180220_120246](https://user-images.githubusercontent.com/29497414/38043998-d4b7c3e4-32b8-11e8-85b2-146c63207be0.jpg)


# 3) Implementation:
- By using a struct as obtained in the main.c file
- FSM is implemented by looping in while(1) for these steps:
1) OUTPUT
2) WAIT
3) INPUT
4) CHANGE CURRENT STATE


# 4) Testing & Verification:
- Here comes the magic of FSM as it simply gives a one-to-one mapping between the State Transition Table and the software implementation, so when it comes to #Testing, in addition to using #Debug on #Simulator mode, you can also verify that the #Designed FSM is working after #Implemented by comparing all states from this one-to-one mapping relationship. 

# Project Demo:
https://youtu.be/WfG8HQ42PJI


# NOTE:
- Files: debounce.c, debounce.h, inlinedebounce.h are related to the Push Buttons debouncing library.
- Files: timers.c, timers.h are related to the Timer0 library.
- Files: delay.c, delay.h are related to the Delay library.
### All these 3 libraries I made them for my own use on AVR microcontrollers.


# COPYRIGHTS:
### Feel free to use all files in this project repository for purpose of #EDUCATION only.
### For any #COMMERCIAL or #NON_EDUCATIONAL use for any code in these files, please contact me at amr.mostaafaa@gmail.com
