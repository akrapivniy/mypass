PATH	:= $(PATH):/home/akrapivniy/ml3020/tools/bin

PREFIX = arm-none-eabi

CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
CP      = arm-none-eabi-objcopy
OD      = arm-none-eabi-objdump

TOOLCHAIN_DIR := ../tools/$(PREFIX)

SOURCES=ssd1306-i2c.c usb-msc-lib.c usb-keyboard-msc.c main.c
OBJECTS=$(SOURCES:.c=.o)


CFLAGS		+= -Os -g -Wall -Wextra -I$(TOOLCHAIN_DIR)/include \
		   -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -MD -DSTM32F1
LDFLAGS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group \
		   -nostartfiles -Wl,--gc-sections \
			-L$(TOOLCHAIN_DIR)/lib -L$(TOOLCHAIN_DIR)/lib/stm32/f1 \
		   -mthumb -march=armv7 -mfix-cortex-m3-ldrd -msoft-float
all: test

clean:
	rm -f *.lst *.o *.elf *.bin *.d

test:main.bin

main.bin: main.elf
	@ echo "[Copying]"
	$(CP) -Obinary  main.elf main.bin
	$(OD) -S main.elf > main.lst

main.elf: $(OBJECTS)
	@ echo "[Linking]"
	$(LD) -T stm32.ld $(LDFLAGS) $(OBJECTS) -lopencm3_stm32f1 -o main.elf

.o:  $(SOURCES)
	@ echo "[Compiling main code]"
	$(CC) -g -Wall -I./ -c $(CFLAGS) -Os $< -o $@

prog: test
	sudo st-flash write v1 main.bin 0x08000000


