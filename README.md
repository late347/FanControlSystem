# FanControlSystem

-this was a pair-programming project done with C++.

-ARM Cortex M3

-user was able to control the fanspeed manually with buttons

-user was alternatively able to set targetAirpressure, and then automatically system would sense the currentAirpressure from barometric sensor, and adjust the fanspeed accordingly.


NOTE!::
there was a small bug in the sense that the automatic adjustment mode works "too fastly" so that it doesn't allow enough time, 
for the newly acquired barometric measurements to "settle-down" and change the actualAirpressure (it means that typically when fan speed increases => the airpressure should have been allowed to settle down to the new value little bit more, because pressure changes aren't that fast typically...)
