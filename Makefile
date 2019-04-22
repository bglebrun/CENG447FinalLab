# Makefile does:
# all (default):		builds program
# flash:						flashes program to board
# clean: 						cleans object/hex files
# test-connection:	calls avrdude to test connection

# Name for compilation
PROJECT		?= ultrasonic

# Objects to build
OBJECTS		?= timers.o robotIo.o motor_driver.o servo.o ultrasonic.o pcint.o main.o 

# Microprocessor
AVRMCU		?= atmega328p
AVRCLOCK	?= 16000000

# Avr connection deets
PORT		?= com6
PROGRAMMER	?= arduino

# Avr toolchain
AVRCPP		?= avr-gcc.exe
AVROBJCPY 	?= avr-objcopy.exe
AVRSIZE		?= avr-size.exe
AVRDUDE		?= avrdude.exe

# build flags
AVRCPPFLAGS ?= -mmcu=$(AVRMCU) -Wall -DF_CPU=$(AVRCLOCK) -I. -Os
AVRLDFLAGS	?= -mmcu=$(AVRMCU)
AVRDUDEOPT	?= -c $(PROGRAMMER) -p $(AVRMCU) -P $(PORT)

all: $(PROJECT).hex

%.hex: %.elf
		$(AVROBJCPY) -j .text -j .data -O ihex $< $@

$(PROJECT).elf: $(OBJECTS)
	$(AVRCPP) $(AVRLDFLAGS) -o $@ $^
	$(AVRSIZE) -C --mcu=$(AVRMCU) $@

%.o: %.c
	$(AVRCPP) $(AVRCPPFLAGS) -c $< -o $@

%.o: %.S
	$(AVRCPP) -x assembler-with-cpp $(AVRCPPFLAGS) -c $< -o $@


test-connection:
	-@echo $(AVRDUDE) $(AVRDUDEOPT)
	-@ 		 $(AVRDUDE) $(AVRDUDEOPT)

flash:
	-@		 $(AVRDUDE) $(AVRDUDEOPT) -b 115200 -U flash:w:$(PROJECT).hex:i

clean:
	rm *.o *.hex *.elf
