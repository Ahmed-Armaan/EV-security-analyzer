TARGET = firmware
OBJS = startup.o main.o scheduler.o

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -O2 -ffreestanding -nostdlib
LDFLAGS = -T linker.ld

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(TARGET).elf
	@rm -rf *.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

startup.o: startup.c
	$(CC) -c startup.c -mcpu=cortex-m3 -mthumb -o startup.o

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f *.o *.elf *.bin
