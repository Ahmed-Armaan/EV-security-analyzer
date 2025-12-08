TARGET = firmware

# automatically include all C files in src/
SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -O2 -ffreestanding -nostdlib -Iinc
LDFLAGS = -T linker.ld -nostdlib -ffreestanding

all: build build/$(TARGET).elf build/$(TARGET).bin

build:
	mkdir -p build

# Compile object files
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link
build/$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

# Convert ELF → BIN
build/$(TARGET).bin: build/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f src/*.o build/*.elf build/*.bin
