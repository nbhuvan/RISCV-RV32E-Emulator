#include "emulator.h"

#include <stddef.h>
#include<stdio.h>

#include "../code/a_program.c"

int main(){
    rv32e_cpu_t cpu;

    rv32e_init(&cpu);
    rv32e_load_program(&cpu, a, a_words);
    rv32e_run(&cpu, 1000);

    // printf("x1=%u x2=%u x3=%u cycles=%llu trap=%d\n",
    //        rv32e_register_read(&cpu.registers, 1),
    //        rv32e_register_read(&cpu.registers, 2),
    //        rv32e_register_read(&cpu.registers, 3),
    //        (unsigned long long)cpu.cycles, cpu.trap);
    for(int i=0;i<16;i++){
        printf("x%d=%u\n",i,rv32e_register_read(&cpu.registers,i));
    }
    printf("return value (x10)=%u pc=%u cycles=%llu trap=%d\n",
           rv32e_register_read(&cpu.registers, 10),
           cpu.pc,
           (unsigned long long)cpu.cycles,
           cpu.trap);

    return cpu.trap == RV32E_TRAP_BREAKPOINT ? 0 : 1;


}
