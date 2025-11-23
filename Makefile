TARGET = firmware

SRC = src/startup.c src/main.c src/scheduler.c
OBJ = $(SRC:.c=.o)

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -O2 -ffreestanding -nostdlib -Iinc
LDFLAGS = -T linker.ld -nostdlib -ffreestanding

all: build $(TARGET).elf $(TARGET).bin
	@rm -rf src/*.o

build:
	mkdir -p build

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o build/$(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary build/$(TARGET).elf build/$(TARGET).bin

clean:
	rm -f src/*.o build/*.elf build/*.bin
