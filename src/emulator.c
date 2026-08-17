#include "emulator.h"
#include "alu.h"
#include "instruction_decode.h"
#include <string.h>


// Function that checks whether the register number is correct for a particular opcode

static bool valid_registers(const rv32e_decoded_instruction_t *i){
    switch(i->opcode){
        case 0x33: return i->rd < 16 && i->rs1 <16 && i->rs2 <16;
        case 0x03: case 0x13: case 0x67: return i->rd < 16 && i->rs1<16;
        case 0x23: case 0x63: return i->rs1 < 16 && i->rs2<16;
        case 0x17: case 0x37: case 0x6f: return i->rd<16;
        default: return true;
    }
}

static void trap(rv32e_cpu_t *cpu, rv32e_trap_t reason){
    cpu->trap = reason;
}

void rv32e_init(rv32e_cpu_t *cpu){
    
    memset(cpu, 0, sizeof(*cpu)); // Fills the cpu_t with zeros
    rv32e_registers_init(&cpu->registers);
    rv32e_memory_init(&cpu->memory);
}

void rv32e_load_program(rv32e_cpu_t *cpu, const uint32_t *program, size_t words){
    if(words > (RV32E_MEMORY_SIZE)/sizeof(uint32_t)) words = (RV32E_MEMORY_SIZE)/sizeof(uint32_t);

    for(size_t i=0;i<words;i++){
        rv32e_store32(&cpu->memory, (uint32_t)(i * 4u), program[i]);
    }
    cpu->pc = 0;
}

bool rv32e_step(rv32e_cpu_t *cpu){
    uint32_t raw;
    rv32e_decoded_instruction_t i;
    uint32_t left, right, result, address;
    uint32_t next_pc = cpu->pc + 4u;
    bool write_result = false;

    if(cpu->trap!=RV32E_TRAP_NONE || rv32e_fetch32(cpu->memory.bytes, sizeof(cpu->memory.bytes), cpu->pc, &raw)!=RV32E_MEM_OK){
        if(cpu->trap == RV32E_TRAP_NONE) trap(cpu,RV32E_TRAP_MEMORY);
        return false;
    }
    rv32e_decode(raw,&i);

    if(!valid_registers(&i)){
        trap(cpu,RV32E_TRAP_ILLEGAL_INSTRUCTION);
        return false;
    }
    left = rv32e_register_read(&cpu->registers,i.rs1);
    right = rv32e_register_read(&cpu->registers,i.rs2);
    result = 0;

    switch (i.opcode){
        case 0x33: // R-type
            if(i.funct3 == 0 && i.funct7 == 0x00) result = rv32e_add(left,right);
            else if(i.funct3 == 0 && i.funct7 == 0x20) result = rv32e_sub(left,right);
            else if(i.funct3 == 1 && i.funct7 == 0x00) result = rv32e_sll(left,right);
            else if(i.funct3 == 2 && i.funct7 == 0x00) result = rv32e_slt(left,right);
            else if(i.funct3 == 3 && i.funct7 == 0x00) result = rv32e_sltu(left,right);
            else if(i.funct3 == 4 && i.funct7 == 0x00) result = left ^ right;
            else if(i.funct3 == 5 && i.funct7 == 0x00) result = rv32e_srl(left,right);
            else if(i.funct3 == 5 && i.funct7 == 0x20) result = rv32e_sra(left,right);
            else if(i.funct3 == 6 && i.funct7 == 0x00) result = left | right;
            else if(i.funct3 == 7 && i.funct7 == 0x00) result = left & right;
            else {
                trap(cpu,RV32E_TRAP_ILLEGAL_INSTRUCTION);
                return false;                
            }
            write_result = true;
            break;
        
        case 0x13: // Immediate - Type
            if(i.funct3 == 0) result = rv32e_add(left,(uint32_t)i.immediate);
            else if(i.funct3 == 0x01 && i.funct7 == 0x00) result = left << (i.immediate & 31);
            else if(i.funct3 == 0x02) result = (int32_t)left < i.immediate;
            else if(i.funct3 == 0x03) result = left < (uint32_t)i.immediate;
            else if(i.funct3 == 0x04) result = left ^ (uint32_t)i.immediate;
            else if(i.funct3 == 0x05 && i.funct7 == 0x00) result = left >> (i.immediate & 31);
            else if(i.funct3 == 0x05 && i.funct7 == 0x20) result = (uint32_t)((int32_t)left >> (i.immediate & 31));
            else if(i.funct3 == 0x06) result = left | (uint32_t)i.immediate;
            else if(i.funct3 == 0x07) result = left & (uint32_t)i.immediate;
            else{
                trap(cpu,RV32E_TRAP_ILLEGAL_INSTRUCTION);
                return false;  
            }
            write_result = true;
            break;

        case 0x03: // Load
            address = left + (uint32_t) i.immediate;
            if(i.funct3==0){
                
                if(rv32e_load8(&cpu->memory,address,&right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
                right = (uint32_t)sign_extend(right,8);
            }
            else if(i.funct3==1){
                
                if(rv32e_load16(&cpu->memory,address,&right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
                right = (uint32_t)sign_extend(right,16);
            }
            else if(i.funct3==2){
                
                if(rv32e_load32(&cpu->memory,address,&right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
            }
            else if(i.funct3==4){
                
                if(rv32e_load8(&cpu->memory,address,&right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
            }
            else if(i.funct3==5){
                
                if(rv32e_load16(&cpu->memory,address,&right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
            }
            else{
                trap(cpu,RV32E_TRAP_ILLEGAL_INSTRUCTION);
                return false;
            }
            write_result = true;
            break;
        case 0x23: // Store
            address = left + (uint32_t)i.immediate;
            if(i.funct3==0){
                if(rv32e_store8(&cpu->memory,address,right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
            }
            else if(i.funct3==1){
                if(rv32e_store16(&cpu->memory,address,right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
            }
            else if(i.funct3==2){
                if(rv32e_store32(&cpu->memory,address,right)!=RV32E_MEM_OK){
                    trap(cpu,RV32E_TRAP_MEMORY);
                    return false;
                }
            }
            else{
                trap(cpu,RV32E_TRAP_ILLEGAL_INSTRUCTION);
                return false;
            }
            break;

        case 0x67: //jalr
            if(i.funct3!=0){
                trap(cpu,RV32E_TRAP_ILLEGAL_INSTRUCTION);
                return false;
            }
            result = next_pc;
            next_pc = left + (uint32_t)i.immediate;
            write_result = true;
            break;
        
        case 0x63:  //B type
            if(i.funct3==0){
                if(left==right){
                    next_pc = cpu->pc + (uint32_t)i.immediate;
                }
            }
            else if(i.funct3==1){
                if(left!=right){
                    next_pc = cpu->pc + (uint32_t)i.immediate;
                }
            }
            else if(i.funct3==4){
                if((int32_t)left<(int32_t)right){
                    next_pc = cpu->pc + (uint32_t)i.immediate;
                }
            }
            else if(i.funct3==5){
                if((int32_t)left>=(int32_t)right){
                    next_pc = cpu->pc + (uint32_t)i.immediate;
                }
            }
            else if(i.funct3==6){
                if(left<right){
                    next_pc = cpu->pc + (uint32_t)i.immediate;
                }
            }
            else if(i.funct3==7){
                if(left>=right){
                    next_pc = cpu->pc + (uint32_t)i.immediate;
                }
            }
            else{
                trap(cpu,RV32E_TRAP_ILLEGAL_INSTRUCTION);
                return false;
            }
            break;

        case 0x6f: //JAL
            result = next_pc;
            next_pc = cpu->pc + (uint32_t)i.immediate;
            write_result = true;
            break;

        case 0x37: 
            result = (uint32_t)i.immediate;
            write_result = true;
            break;
        
        case 0x17:

            result = cpu->pc + (uint32_t)i.immediate;
            write_result = true;
            break;
        default:
            trap(cpu, RV32E_TRAP_ILLEGAL_INSTRUCTION); return false;
    }
    if(write_result) rv32e_register_write(&cpu->registers, i.rd, result);
    cpu->pc = next_pc;
    cpu->cycles++;
    return true;
    
    
}

bool rv32e_run(rv32e_cpu_t *cpu, uint64_t max_cycles){
    while(cpu->trap==RV32E_TRAP_NONE && cpu->cycles < max_cycles){
        if(!rv32e_step(cpu)) break;
    }
    return cpu->trap == RV32E_TRAP_NONE;
}
