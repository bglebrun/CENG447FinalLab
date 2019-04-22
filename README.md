# CENG447Lab5

Servo and Timer 2

## Getting Started

Pull down, edit the ```PORT``` variable in the [Makefile](https://github.com/bglebrun/CENG447Lab0/blob/master/Makefile#L18), you can check this the lazy way with the Arduino IDE or look for the port in your devices in windows. I'd know how to do this elsewhere if linux/osx was my daily driver but meh

### Prerequisistes
* avr-dude
* make
* avr-cpp
* Arduino Uno

[Good instructions on how to get these for windows](http://fab.cba.mit.edu/classes/863.16/doc/projects/ftsmin/windows_avr.html)

### Make
* ``` make ``` will make the necessary *.o, *.hex and *.elf files
* ``` flash ``` will grab these files and flash them to the board'
* ``` test-connection ``` will invoke avrdude's simple connection test
* ``` clean ``` Cleans up project files

### License
WTFPL, just like, do w/e man
