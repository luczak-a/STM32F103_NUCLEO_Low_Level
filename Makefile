# Author: Adam Luczak
# Description: Makefile for "Low_Level_UART" STM32 project
# Host OS: Windows

PROJECT_NAME = low_level_uart
LINKER_SCRIPT = STM32F103XB_FLASH.ld
STARTUP_FILE = startup_stm32f103xb.s

#### Toolchain ####
CC 		= arm-none-eabi-gcc
LD 	  	= arm-none-eabi-ld
AS 		= arm-none-eabi-as
GDB 	= arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy

#### ST-Link Utility ####
ST_LINK = ST-LINK_CLI -c SWD UR

#### Compiler options ####
CFLAGS = -std=gnu99 -g -Wall -T$(LINKER_SCRIPT)
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m3
CFLAGS += -mfloat-abi=soft --specs=nosys.specs

#### Project specific options ####
SRC_FILES = $(STARTUP_FILE) $(wildcard Src/*.c)
CFLAGS += -IInc -ICMSIS/Core -ICMSIS/Device


#### Make commands ####
all: $(PROJECT_NAME).hex

clean:
	del $(PROJECT_NAME).elf $(PROJECT_NAME).hex

$(PROJECT_NAME).elf: $(SRC_FILES)
		$(CC) $(CFLAGS) -o $@ $^

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	$(OBJCOPY) -O ihex $^ $@

flash:
	$(ST_LINK) -ME -P $(PROJECT_NAME).hex -V after_programming -Rst