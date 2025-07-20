Description: 
- I was doing a 100 meter race with my friend and we realized we need a third person to time us and tell us when to start, I wanted to try making a device that could do this for us 
- I also thought I could add some of my own features, such as a stopwatch and display, and a timer mode that could be used for interval runs around the track (ie 10 seconds sprint, 5 seconds rest)
- This concept could be taken a step further and made into a "Smart Baton" (like they use in Relay Races): make a custom PCB, 3D printing a compact case, using a LiPo battery, and use some higher quality buttons

Features:
- in stopwatch mode, when press start, a random interval from 2-5 seconds will pass, then the device will beep/vibrate (depending on output mode) telling the user to GO, this is to test the reaction/time in a race (simulates a third party telling the racers when to go)
- back and ok buttons, button state tracking, and button debouncing using timers and interrupts (used logic from morse-code tracker project to aid with this)
- state machine to transition between different modes
- can customize the output mode (haptic/vibrate, buzzer, silent) from the menu
- can navigate between modes and can pause, play, and reset timers and stopwatch 

Hardware:
- Nucle-F446RE development board
- haptic/vibrational motor
- buzzer
- transistors (limit current supplied from dev board)
- push buttons
- 0.98" OLED Display

Improvements:
- incorporating RTC might make it simpler
- add ability to customize the two timer periods (right now they are just 10 seconds and 5 seconds)
- take it a step further and actually build the Smart Baton - PCB design, printed case, etc. 

Video Demonstration:
https://drive.google.com/file/d/16z4KxHUYy8rXcmrOj_2vOF_finLPhyB3/view?usp=sharing
