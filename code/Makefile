PROGRAM ?= a

RISCV_PREFIX ?= riscv32-unknown-elf-
RISCV_CC ?= $(RISCV_PREFIX)gcc
RISCV_OBJCOPY ?= $(RISCV_PREFIX)objcopy

ROOT_DIR := ..
SOURCE := $(PROGRAM).c
STARTUP := start.S
LINKER_SCRIPT := linker.ld
OBJECTS := $(PROGRAM).o start.o
ELF := $(PROGRAM).elf
BIN := $(PROGRAM).bin
ARRAY := $(PROGRAM)_program.c
CONVERTER := $(ROOT_DIR)/bin-to-array

CFLAGS ?= -march=rv32e -mabi=ilp32e -ffreestanding -fno-builtin -O0 -g
LDFLAGS ?= -nostdlib -nostartfiles -Wl,-T,$(LINKER_SCRIPT) -Wl,--no-relax
LDLIBS ?= -lgcc

.PHONY: all array clean

all: array

array: $(ARRAY)

$(ARRAY): $(BIN) $(CONVERTER)
	$(CONVERTER) $(BIN) $@ $(PROGRAM)

$(BIN): $(ELF)
	$(RISCV_OBJCOPY) -O binary $< $@

$(ELF): $(OBJECTS) $(LINKER_SCRIPT)
	$(RISCV_CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $@

$(PROGRAM).o: $(SOURCE)
	$(RISCV_CC) $(CFLAGS) -c $< -o $@

start.o: $(STARTUP)
	$(RISCV_CC) $(CFLAGS) -c $< -o $@

$(CONVERTER): $(ROOT_DIR)/tools/bin_to_array.c
	$(CC) -std=c11 -Wall -Wextra -Wpedantic -Werror $< -o $@

clean:
	rm -f $(PROGRAM).o start.o $(ELF) $(BIN) $(ARRAY) $(CONVERTER)
