#ifndef COMPUTER_H
#define COMPUTER_H

#include "cpu.h"
#include <stdlib.h>

struct HackComputer {
    struct CPU cpu;
    uint16_t ram[0xFFFF];
    uint16_t rom[0x7FFF];  
    uint16_t memory_out;
    uint16_t address_out; 
    uint16_t pc;
    uint8_t write_out;

    uint16_t memory_in;
    uint8_t reset;
    uint16_t instruction;
};

void computerLoop(struct HackComputer *computer, int n);
void initComputer(struct HackComputer *computer, char *file_name);
void freeInstructionsStr(char **instructions_str);
void parseInstructions(struct HackComputer *computer, char **instructions_str);
char **parseFile(char *file_name);

#endif
