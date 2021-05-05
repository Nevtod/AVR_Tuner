MCU = atmega8535
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -std=c99 -Wall -g -Os -mmcu=${MCU} -I.
TARGET = main
SRCS = main.c
FUSE = -U lfuse:w:0xD4:m -U hfuse:w:0xD9:m
all:
	@${CC} ${CFLAGS} -o ${TARGET}.bin ${SRCS}
	@${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
	@avrdude -p ${MCU} ${FUSE} -c usbasp -U flash:w:${TARGET}.hex:i -F -P usb -B12 -v

clean:
	@rm -f *.bin *.hex *.in *.com

	
def:
	make clean
	make all
	sudo make flash
	minicom -D /dev/ttyUSB0 -b 9600 -C data.in
	
