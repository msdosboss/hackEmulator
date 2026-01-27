#include "cpu.h"


void initCPU(struct CPU *cpu){
    cpu->PC = 0;
    cpu->a_register = 0;
    cpu->d_register = 0;
}


void printCPU(struct CPU *cpu){
    printf("cpu->PC = %u\ncpu->a_register = %u\ncpu->d_register = %u\n", cpu->PC, cpu->a_register, cpu->d_register);
}


void parseALUMicroCodes(uint16_t instruction, uint8_t *zx, uint8_t *nx, uint8_t *zy, uint8_t *ny, uint8_t *f, uint8_t *no){
    //zx = bit 11, nx = bit 10, zy = bit 9, ny = bit 8, f = bit 7, no = bit 6 (starting with bit 0)
    *zx = (instruction >> 11) & 1;
    *nx = (instruction >> 10) & 1;
    *zy = (instruction >> 9) & 1;
    *ny = (instruction >> 8) & 1;
    *f = (instruction >> 7) & 1;
    *no = (instruction >> 6) & 1;
}


void cpuTick(struct CPU *cpu, uint16_t instruction, uint16_t memory_input, uint8_t reset, 
        uint16_t *memory_out, uint8_t *write_memory, uint16_t *address_out, uint16_t *PC){
    uint8_t is_C_instruction = (uint8_t)((instruction & 0b1000000000000000) >> 15);

    if (is_C_instruction){
        //ALU Logic
        uint8_t zx, nx, zy, ny, f, no, zr, ng;
        parseALUMicroCodes(instruction, &zx, &nx, &zy, &nx, &f, &no);
        uint8_t is_M_register_input = (instruction >> 12) & 1;
        uint16_t alu_output;

        if(is_M_register_input){
            alu_output = alu(cpu->d_register, memory_input, zx, nx, zy, ny, f, no, &zr, &ng);
        }
        else{
            alu_output = alu(cpu->d_register, cpu->a_register, zx, nx, zy, ny, f, no, &zr, &ng);
        }
        *memory_out = alu_output;
        
        //JMP Logic
        uint8_t is_jmp_if_grt = instruction & 1;
        uint8_t is_jmp_if_eq = (instruction >> 1) & 1;
        uint8_t is_jmp_if_lt = (instruction >> 2) & 1;

        uint8_t is_alu_output_neg = (uint8_t)((alu_output & 0b1000000000000000) >> 15);

        //if no special case we just want to increase PC by one
        cpu->PC++;

        if(is_jmp_if_grt){
            if(!is_alu_output_neg){
                cpu->PC = cpu->a_register;
            }
        }
        if(is_jmp_if_eq){
            if(alu_output == 0){
                cpu->PC = cpu->a_register;
            }
        }
        if(is_jmp_if_lt){
            if(is_alu_output_neg){
                cpu->PC = cpu->a_register;
            }
        }
        if(reset){
            cpu->PC = 0;
        }

        *PC = cpu->PC;

        //Dest Logic
        uint8_t is_dest_M_reg = (instruction >> 3) & 1;
        uint8_t is_dest_D_reg = (instruction >> 4) & 1;
        uint8_t is_dest_A_reg = (instruction >> 5) & 1;

        if(is_dest_M_reg){
            *write_memory = 1; 
        }
        else{
            *write_memory = 0;
        }

        //write to address_out before writing cpu->a_register 
        *address_out = cpu->a_register;
        if(is_dest_A_reg){
            cpu->a_register = alu_output;
        }
        if(is_dest_D_reg){
            cpu->d_register = alu_output;
        }


    }

    else{
        cpu->a_register = instruction;
        
        //setting output values
        *write_memory = 0;
        *memory_out = 0;
        *address_out = cpu->a_register;
        *PC = cpu->PC;
    }

}

uint16_t alu(uint16_t input_x, uint16_t input_y, uint8_t zx, uint8_t nx, uint8_t zy, uint8_t ny, uint8_t f, uint8_t no,
             uint8_t *zr, uint8_t *ng){
    if(zx){
        input_x = 0;
    }

    if(nx){
        input_x = ~input_x;
    }

    if(zy){
        input_y = 0;
    }

    if(ny){
        input_y = ~input_y;
    }

    uint16_t out;
    if(f){
        out = input_x + input_y;
    }
    else{
        out = input_x & input_y;
    }

    if(no){
        out = ~out;
    }

    if (out == 0){
        *zr = 1;
    }
    else{
        *zr = 0;
    }
    //checks if out is negative
    if ((out & 0b10000000000000000)){
        *ng = 1;
    }
    else{
        *ng = 0;
    }

    return out;
}
