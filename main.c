#include "main.h"

int main(){

    struct HackComputer computer;

    initComputer(&computer, "b.out");

    computerLoop(&computer, 10);

}

