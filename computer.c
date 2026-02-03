#include "computer.h"

// The pointers returned need to be freed
char **parseFile(char *file_name){
    // counting amount of lines in the file
    FILE *file = fopen(file_name, "r");
    //16 bits + 1 for \n + 1 for null terminator
    int MAX_LINE_LEN = 18;
    char buffer[MAX_LINE_LEN];
    int i = 0;
    while(fgets(buffer, MAX_LINE_LEN, file) != NULL){
        i++;
    }
    fclose(file);
    int len = i;

    char **instructions_str = malloc(sizeof(char *) * (len + 1));
    for(i = 0; i < len; i++){
        instructions_str[i] = malloc(sizeof(char) * MAX_LINE_LEN);
    }
    
    instructions_str[len] = NULL;

    file = fopen(file_name, "r");
    i = 0;
    while(fgets(instructions_str[i], MAX_LINE_LEN, file) != NULL){
        i++;
    }
    fclose(file);

    return instructions_str;
}


void freeInstructionsStr(char **instructions_str){
    int i = 0;
    while(instructions_str[i] != NULL){
        free(instructions_str[i]);
        i++;
    }
    free(instructions_str);
}


// This function assumes clean input
void parseInstructions(struct HackComputer *computer, char **instructions_str){
    int i = 0;
    while(instructions_str[i] != NULL){
        computer->rom[i] = strtol(instructions_str[i], NULL, 2);
        i++;
    }
}


void initComputer(struct HackComputer *computer, char *file_name){
    initCPU(&(computer->cpu));
    computer->memory_out = 0;
    computer->address_out = 0;
    computer->pc = 0;
    computer->write_out = 0;

    computer->memory_in = 0;
    computer->reset = 0;
    for(int i = 0; i < 0xFFFF; i++){
        computer->ram[i] = 0;
    }

    // loads the program into rom    
    char **instructions_str = parseFile(file_name);
    parseInstructions(computer, instructions_str);
    freeInstructionsStr(instructions_str);

    computer->instruction = computer->rom[0];
}


void computerLoop(struct HackComputer *computer, int n){
    int i = 0;
    while(i < n){
        computer->instruction = computer->rom[computer->pc];
        computer->memory_in = computer->ram[computer->address_out]; 
        cpuTick(&(computer->cpu), computer->instruction, computer->memory_in, computer->reset, &(computer->memory_out), &(computer->write_out), &(computer->address_out), &(computer->pc));
        //printCPU(&(computer->cpu));
        if(computer->write_out){
            computer->ram[computer->address_out] = computer->memory_out;
        }
        i++;
    }
}
