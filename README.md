# multiTimerSTM

Picture: 


Video Demo: 


Project description: 
A multi-purpose timer built using stm32f401re used for timing physio excercises and generating random starts for track and field sprints. 


features:
- stopwatch mode, after pressing ready, will tell the user to start by choosing a random time interval
- timer mode, counts down from 10 seconds to track physio exercises
- customizable outputs: buzzer (sound), haptic (vibrate)
- end goal is to make this a small, portable, battery powered device.


Order of events:
- setup IDE and buttons
- get software debouncing and button state tracking working
- be able to track how long a button is released and pressed for
- implement the state machine logic
- put the button logic for switching between states
- make the timer and stopwatch modes work
- add the random start functionality for the stopwatch
- add the OLED display, buzzer, and haptic motor
- customize the OLED display menu

Next steps
- wait for blue pill (to replace nucleo) to arrive and test with this development board
- wait for LiPo batteries to arrive and make project portable. Add battery life checking features.
- put blue pill and other hardware onto soldered breadboard or PCB
- design and 3D print case to allow for handheld portability 
- eventually move away from using a dev board, build a custom PCB 


Back-story: 
At the track with friends, wanted to race 100 meters, but realized that in order to make the force, the start needed to be random. We needed another person to say "3...2...1...GO". A device can be built to do this by randomly indicating the start time, making it fair for both people.

Recovering from a knee injury, must do excercises, 10 seconds each, 10 reps, and there are about 6-7 different exercises like this. Instead of counting down from 10 each time, or using a stopwatch or my phone, an easy and mindless method can be used to indicate when to switch exercises and count reps. 

