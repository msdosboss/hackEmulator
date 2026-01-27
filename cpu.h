#include <stdint.h>
#include <stdio.h>

struct CPU {
    uint16_t PC;
    uint16_t a_register;
    uint16_t d_register;
};

void printCPU(struct CPU *cpu);
void initCPU(struct CPU *cpu);
void cpuTick(struct CPU *cpu, uint16_t instruction, uint16_t memory_input, uint8_t reset, 
        uint16_t *memory_out, uint8_t *write_memory, uint16_t *address_out, uint16_t *PC);
uint16_t alu(uint16_t input_x, uint16_t input_y, uint8_t zx, uint8_t nx, uint8_t zy, uint8_t ny, uint8_t f, uint8_t no,
             uint8_t *zr, uint8_t *ng);
void parseALUMicroCodes(uint16_t instruction, uint8_t *zx, uint8_t *nx, uint8_t *zy, uint8_t *ny, uint8_t *f, uint8_t *no);
