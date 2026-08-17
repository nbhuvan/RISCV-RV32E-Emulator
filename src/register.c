#include "register.h"
#include <assert.h>
#include <string.h>


void rv32e_registers_init(rv32e_register_t *registers){
    memset(registers, 0, sizeof(*registers));
}

uint32_t rv32e_register_read(const rv32e_register_t* registers, unsigned index){
    assert(index < RV32E_REGISTER_COUNT);
    return index == 0 ? 0u : registers->x[index];

}
void rv32e_register_write(rv32e_register_t* registers, unsigned index, uint32_t value){
    assert(index < RV32E_REGISTER_COUNT);
    if(index != 0){
        registers->x[index] = value;
    }
}