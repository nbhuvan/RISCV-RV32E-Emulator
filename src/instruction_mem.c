#include "instruction_mem.h"

rv32e_mem_result_t rv32e_fetch32(const uint8_t *memory, size_t size, uint32_t address, uint32_t *instruction){

    if((address & 3u)!=0){
        return RV32E_MEM_MISALIGNED;
    }
    if((uint64_t)address + 4u > size) return RV32E_MEM_OUT_OF_BOUNDS;

    *instruction = (uint32_t)memory[address] // Little Endian Memory order
        | ((uint32_t)memory[address + 1] << 8)
        | ((uint32_t)memory[address + 2] << 16)
        | ((uint32_t)memory[address + 3] << 24);

    return RV32E_MEM_OK;
}
