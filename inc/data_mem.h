#ifndef RV32E_DATA_MEM_H

#define RV32E_DATA_MEM_H

#include "instruction_mem.h"

#define RV32E_MEMORY_SIZE 4096u

typedef struct{
    uint8_t bytes[RV32E_MEMORY_SIZE];
} rv32e_memory_t;

void rv32e_memory_init(rv32e_memory_t *memory);

rv32e_mem_result_t rv32e_load32(const rv32e_memory_t* memory, uint32_t address, uint32_t *value);
rv32e_mem_result_t rv32e_load16(const rv32e_memory_t*memory, uint32_t address, uint32_t*value);
rv32e_mem_result_t rv32e_load8(const rv32e_memory_t*memory, uint32_t address, uint32_t*value);

rv32e_mem_result_t rv32e_store8(rv32e_memory_t*memory, uint32_t address, uint32_t value);
rv32e_mem_result_t rv32e_store16(rv32e_memory_t*memory, uint32_t address, uint32_t value);
rv32e_mem_result_t rv32e_store32(rv32e_memory_t* memory, uint32_t address, uint32_t value);

#endif