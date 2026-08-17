#ifndef RV32E_INSTRUCTION_MEM_H
#define RV32E_INSTRUCTION_MEM_H

#include <stdint.h>
#include <stddef.h>

typedef enum{
    RV32E_MEM_OK = 0,
    RV32E_MEM_MISALIGNED,
    RV32E_MEM_OUT_OF_BOUNDS
} rv32e_mem_result_t;

rv32e_mem_result_t rv32e_fetch32(const uint8_t *memory, size_t size, uint32_t address, uint32_t *instruction);

#endif