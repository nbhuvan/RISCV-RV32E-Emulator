CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -Iinc -g

CORE_SOURCES = src/alu.c src/emulator.c src/instruction_decode.c src/instruction_mem.c src/register.c src/data_mem.c

.PHONY: all clean test program-array

all: program-array rv32e-emulator

program-array:
	$(MAKE) -C code array

rv32e-emulator: $(CORE_SOURCES) src/main.c code/a_program.c | program-array
	$(CC) $(CFLAGS) $(CORE_SOURCES) src/main.c -o $@

clean:
	rm -f rv32e-emulator
