#include "emulator.h"

#include<stdio.h>

int main(){
    /* addi x1, x0, 5; addi x2, x0, 7; add x3, x1, x2; ebreak */
    const uint32_t program[] = { 0x00500093, 0x00700113, 0x002081b3, 0x00100073 };
    rv32e_cpu_t cpu;

    rv32e_init(&cpu);
    rv32e_load_program(&cpu, program, sizeof(program) / sizeof(program[0]));
    rv32e_run(&cpu, 100);

    printf("x1=%u x2=%u x3=%u cycles=%llu trap=%d\n",
           rv32e_register_read(&cpu.registers, 1),
           rv32e_register_read(&cpu.registers, 2),
           rv32e_register_read(&cpu.registers, 3),
           (unsigned long long)cpu.cycles, cpu.trap);
    return cpu.trap == RV32E_TRAP_BREAKPOINT ? 0 : 1;


}