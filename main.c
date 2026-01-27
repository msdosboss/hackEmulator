#include "cpu.h"

int main(){
    struct CPU cpu;
    initCPU(&cpu);
    printCPU(&cpu);

    uint16_t memory_out, address_out, PC;
    uint8_t write_out;

    uint16_t memory_in = 0;
    uint8_t reset = 0;
    uint16_t instruction = 0b0000000000010001;

    cpuTick(&cpu, instruction, memory_in, reset, &memory_out, &write_out, &address_out, &PC);
    printCPU(&cpu);
}
