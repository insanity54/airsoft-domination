airsoft-domination
==================

![Prototype of airsoft-domination box](http://i.imgur.com/dNrytCP)


In combat pursuit sports, there is a game called Domination a.k.a. Flag Control, which is where two teams battle over control of an area.

This Arduino sketch is the brain for a little box with some neopixels and a buzzer that sits in the center of the field. Once the game starts, both teams try to get to the box and click the button to toggle between team colors. The box shows red if red team controls the area, or blue if blue team controls the area.

Operation
=========

Since this design only uses one pushbutton, you have two choices of input- *long press* and *click*. A long press is pressing the button without releasing for two seconds. A click is a quick press and release in under two seconds.

### Power on


### Programming mode (beep followed by all neopixels off)

* *click* the button to select the time to win. Time to win is a per-team timer. It is the amount of time a team must have control of the sector to win. Each click adds 1 minute to the counter.
* *long press* the button to save time to win and enter the next mode


### Preparation mode (all neopixels flashing purple)

* In this mode, the unit is in standby and will wait for input until starting the game. This is the time all players except for the player starting the device go to their starting positions.
* *long press* to start the game countdown


### Countdown mode (beeping and all neopixels flashing purple)

* unit beeps and flashes purple for 15 seconds. In this time, the player who started their device has 15 seconds to run and join their team at their starting position.
* Players on either side of field watch the flashing purple and wait for green.


### Game mode (beep and all neopixels solid green)
* No team is in control at start, so neopixels show green.
* Green means game on, and all players try to get to the device and push the button.
* When button is pressed, device beeps and switches to red, and red's timer counts up every 1 second.
* When button is pressed again, device beeps and toggles to blue, and blue's timer counts up every 1 second.
* When button is pressed, device toggles to opposite team's color and increases their timer as long as they have control
* First team's timer to become greater than programmed time to win wins the game.


###End mode (beeps on and off and winning team's color flashes)
* A team has won! GG.

