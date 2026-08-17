#include "data_mem.h"

#include <string.h>

void rv32e_memory_init(rv32e_memory_t *memory){
    memset(memory->bytes, 0, sizeof(memory->bytes));
}

rv32e_mem_result_t rv32e_load32(const rv32e_memory_t *memory, uint32_t address, uint32_t *value){
    return rv32e_fetch32(memory->bytes, sizeof(memory->bytes), address, value);
}

rv32e_mem_result_t rv32e_load16(const rv32e_memory_t*memory, uint32_t address, uint32_t*value){
    if((address & 1u)!=0){
        return RV32E_MEM_MISALIGNED;
    }
    if((uint64_t)address + 2u > size) return RV32E_MEM_OUT_OF_BOUNDS;

    *value = (uint32_t)memory[address] // Little Endian Memory order 
        | ((uint32_t)memory[address+1]<<8);

    return RV32E_MEM_OK;
}

rv32e_mem_result_t rv32e_load8(const rv32e_memory_t*memory, uint32_t address, uint32_t*value){

    if((uint64_t)address >= size) return RV32E_MEM_OUT_OF_BOUNDS;

    *value = (uint32_t)memory[address]; // Little Endian Memory order 

    return RV32E_MEM_OK;
}


rv32e_mem_result_t rv32e_store8(rv32e_memory_t *memory, uint32_t address, uint_32_t value){
    
    if((uint64_t)address >= sizeof(memory->bytes)) return RV32E_MEM_OUT_OF_BOUNDS;

    memory->bytes[address] = value & 0xffu;

    return RV32E_MEM_OK;

}
rv32e_mem_result_t rv32e_store16(rv32e_memory_t *memory, uint32_t address, uint_32_t value){
    if((address & 1u)!=0) return RV32E_MEM_MISALIGNED;
    if((uint64_t)address + 1u > sizeof(memory->bytes)) return RV32E_MEM_OUT_OF_BOUNDS;

    memory->bytes[address] = value & 0xffu;
    memory->bytes[address+1] = (value >> 8) & 0xffu;

    return RV32E_MEM_OK;

}
rv32e_mem_result_t rv32e_store32(rv32e_memory_t *memory, uint32_t address, uint_32_t value){
    if((address & 3u)!=0) return RV32E_MEM_MISALIGNED;
    if((uint64_t)address + 4u > sizeof(memory->bytes)) return RV32E_MEM_OUT_OF_BOUNDS;

    memory->bytes[address] = value & 0xffu;
    memory->bytes[address+1] = (value >> 8) & 0xffu;
    memory->bytes[address+2] = (value >> 16) & 0xffu;
    memory->bytes[address+3] = (value >> 24) & 0xffu;

    return RV32E_MEM_OK;

}