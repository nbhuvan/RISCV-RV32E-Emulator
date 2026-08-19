CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -Iinc -g

CORE_SOURCES = src/alu.c src/emulator.c src/instruction_decode.c src/instruction_mem.c src/register.c src/data_mem.c

.PHONY: all clean test

all: rv32e-emulator

rv32e-emulator: $(CORE_SOURCES) src/main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f rv32e-emulator
