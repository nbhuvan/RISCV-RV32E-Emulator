#ifndef RV32E_REGISTER_H
#define RV32E_REGISTER_H

#include<stdint.h>

#define RV32E_REGISTER_COUNT 16

typedef struct{
    uint32_t x[RV32E_REGISTER_COUNT];
} rv32e_register_t;

void rv32e_registers_init(rv32e_register_t *registers);
uint32_t rv32e_register_read(const rv32e_register_t* registers, unsigned index);
void rv32e_register_write(rv32e_register_t* registers, unsigned index, uint32_t value);

#endif