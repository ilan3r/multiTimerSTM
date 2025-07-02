Description: 
- I was doing a 100 meter race with my friend and we realized we need a third person to time us and tell us when to start, I wanted to try making a device that could do this for us 
- I also thought I could add some of my own features, such as a stopwatch and display, and a timer mode that could be used for interval runs around the track (ie 10 seconds sprint, 5 seconds rest)
- This concept could be taken a step further and made into a "Smart Baton": make a custom PCB, 3D printing a compact case, using a LiPo battery, and use some higher quality buttons

Features
- buttons, button state tracking, and button debouncing using timers and interrupts
- state machine to transition between different modes
- home screen menu to 
- can customize the output mode (haptic/vibrate, buzzer, silent) through the menu
- 0.98" inch oled display with crisp responsiveness to buttons 

Hardware
- Nucle-F446RE development board
- haptic/vibrational motor
- buzzer
- transistors (limit current supplied from dev board)
- push buttons
