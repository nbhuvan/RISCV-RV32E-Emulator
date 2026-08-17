#ifndef RV32E_INSTRUCTION_DECODE_H
#define RV32E_INSTRUCTION_DECODE_H

#include <stdint.h>

typedef struct{
    uint32_t raw;
    unsigned opcode;
    unsigned rd;
    unsigned funct3;
    unsigned rs1;
    unsigned rs2;
    unsigned funct7;
    int32_t immediate;
} rv32e_decoded_instruction_t;

void rv32e_decode(uint32_t raw, rv32e_decoded_instruction_t *decoded);
int32_t sign_extend(uint32_t value, unsigned bits);
#endif
