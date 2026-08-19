#ifndef RV32E_EMULATOR_H
#define RV32E_EMULATOR_H

#include <stdbool.h>
#include <stdint.h>

#include "data_mem.h"
#include "register.h"

typedef enum{
    RV32E_TRAP_NONE = 0,
    RV32E_TRAP_ECALL,
    RV32E_TRAP_BREAKPOINT,
    RV32E_TRAP_ILLEGAL_INSTRUCTION,
    RV32E_TRAP_MEMORY
} rv32e_trap_t;

typedef struct{
    rv32e_register_t registers;
    rv32e_memory_t memory;
    uint32_t pc;
    uint64_t cycles;
    rv32e_trap_t trap;
} rv32e_cpu_t;

void rv32e_init(rv32e_cpu_t *cpu);
void rv32e_load_program(rv32e_cpu_t *cpu, const uint32_t *program, size_t words);
bool rv32e_step(rv32e_cpu_t *cpu);
bool rv32e_run(rv32e_cpu_t *cpu, uint64_t max_cycles);

#endif