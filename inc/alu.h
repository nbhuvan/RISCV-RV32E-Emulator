#ifndef RV32E_ALU_H
#define RV32E_ALU_H

#include <stdint.h>

uint32_t rv32e_add(uint32_t left, uint32_t right);
uint32_t rv32e_sub(uint32_t left, uint32_t right);
uint32_t rv32e_sll(uint32_t left, uint32_t right);
uint32_t rv32e_slt(uint32_t left, uint32_t right);
uint32_t rv32e_sltu(uint32_t left, uint32_t right);
uint32_t rv32e_srl(uint32_t left, uint32_t right);
uint32_t rv32e_sra(uint32_t left, uint32_t right);
#endif
