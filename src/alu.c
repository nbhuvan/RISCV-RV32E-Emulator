#include "alu.h"


uint32_t rv32e_add(uint32_t left, uint32_t right) {return left+right;}
uint32_t rv32e_sub(uint32_t left, uint32_t right) {return left-right;}
uint32_t rv32e_sll(uint32_t left, uint32_t right) {return left << (right & 31u);}
uint32_t rv32e_slt(uint32_t left, uint32_t right) {return (int32_t)left < (int32_t)right;}
uint32_t rv32e_sltu(uint32_t left, uint32_t right) {return left < right;}
uint32_t rv32e_srl(uint32_t left, uint32_t right) {return left >> (right&31);}
uint32_t rv32e_sra(uint32_t left, uint32_t right) {return (uint32_t)((int32_t)left >> (right&31));}
