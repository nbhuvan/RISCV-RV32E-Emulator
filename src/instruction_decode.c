#include "instruction_decode.h"

int32_t sign_extend(uint32_t value, unsigned bits){
    const uint32_t sign = 1u << (bits-1);
    return (int32_t)((value ^ sign)-sign);
}

/*

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

*/

void rv32e_decode(uint32_t raw, rv32e_decoded_instruction_t *decoded){
    const uint32_t i_imm = raw >> 20;
    const uint32_t s_imm = ((raw>>25)<<5) | ((raw>>7)&0x1fu);
    const uint32_t b_imm = ((raw>>31)<<12) | (((raw>>7)&0x1u)<<11) | (((raw>>25) & 0x3f)<<5) | (((raw>>8)&0xfu)<<1);
    const uint32_t j_imm = ((raw>>31)<<20) | (((raw>>12)&0xffu)<<12) | (((raw>>20)&0x1u)<<11) | (((raw>>21)&0x3ffu)<<1);
    // const uint32_t u_imm = (raw>>12)
    decodecd->raw = raw;
    decoded->opcode = raw & 0x7fu;
    decoded->rd = (raw>>7) & 0x1fu;
    decoded->funct3 = (raw>>12) & 0x7u;
    decoded->rs1 = (raw>>15) & 0x1fu;
    decodec->rs2 = (raw>>20) & 0x1fu;
    decoded->funct7 = (raw>>25) & 0x7fu;
    decoded->immediate = 0;

    switch(decoded->opcode){
        case 0x03: decoded->immediate = sign_extend(i_imm,12);break; // I_type memory
        case 0x13: decoded->immediate = sign_extend(i_imm,12);break; // I_type arithmetic
        case 0x67: decoded->immediate = sign_extend(i_imm,12);break; // I_type jalr
        case 0x23: decoded->immediate = sign_extend(s_imm,12);break; // S_type memory
        case 0x63: decoded->immediate = sign_extend(b_imm,12);break; // B_Type
        case 0x6f: decoded->immediate = sign_extend(j_imm,21);break; // J_type
        case 0x37: case 0x17: decoded->immediate = (int32_t)(raw & 0xfffff000u);break; // U_type
        default: break;

    }

    
}