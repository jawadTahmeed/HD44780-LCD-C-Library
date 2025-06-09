# Target MCU
MCU = atmega8

#this example uses internal RC oscillator at 1MHz
# CPU Frequency (1 MHz)
F_CPU = 1000000UL

# Compiler and flags
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Os




# Source files
SRCS = weather_monitor_demo.c lcd_16x2_lib.c

# Output files
TARGET = weather_monitor_demo
ELF = $(TARGET).elf
HEX = $(TARGET).hex

# Default rule
all: $(HEX)

# Compile to ELF
$(ELF): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

# Convert to HEX
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Clean build files
clean:
	rm -f *.o *.elf *.hex
