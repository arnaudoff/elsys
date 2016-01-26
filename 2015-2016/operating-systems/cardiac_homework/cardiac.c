#include <stdio.h>
#include <math.h>

int main()
{
    int program_counter = 0;
    int accumulator = 0;
    int instruction = 0;

    int memory[100];
    memory[0] = 1;

    while (1) {
        instruction = memory[program_counter];

        program_counter++;

        int opcode = instruction / 100;
        int operand = instruction % 100;

        switch (opcode) {
            case 0: // INP
                scanf("%d", &memory[operand]);
                break;
            case 1: // CLA
                accumulator = memory[operand];
                break;
            case 2: // ADD
                accumulator += memory[operand];
                break;
            case 3: // TAC
                if (accumulator < 0) {
                    program_counter = operand;
                }

                break;
            case 4: // SFT
            {
                int left_shift = operand / 10;
                int right_shift = operand % 10;

                accumulator = (accumulator * pow(10, left_shift)) /
                                 accumulator * pow(10, right_shift);
                break;
            }
            case 5: // OUT
                printf("%d\n", memory[operand]);
                break;
            case 6: // STO
                memory[operand] = accumulator;
                break;
            case 7: // SUB
                accumulator -= memory[operand];
                break;
            case 8: // JMP
                memory[99] = 800 + program_counter;
                program_counter = operand;
                break;
            case 9: // HRS
                return 0;
                break;
            default:
                fprintf(stderr,
                        "Invalid opcode '%d'. Expected range is [0;9].\n",
                        opcode);
                return 1;
        }
    }

    return 0;
}

