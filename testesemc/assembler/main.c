#include <errno.h>
#include <stdio.h>
#include <string.h>

enum InstrFmt {
    REGREG,
    REGIMM
};

struct Instr {
    char *mnemonic;
    unsigned char opcode;
    enum InstrFmt fmt;
};

struct Reg {
    char *mnemonic;
};

const struct Instr instructions[5] = {
    {"mov", 0x01, REGIMM},
    {"add", 0x02, REGREG},
    {"sub", 0x03, REGREG},
    {"mov", 0x04, REGREG},
    {"xor", 0x05, REGREG},
};

const struct Reg registers[4] = {
    {"r0"},
    {"r1"},
    {"r2"},
    {"r3"}};

#define LINE_MAX_LEN 4096

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Usage: %s <source>\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(argv[1], "r");
    if (source == NULL) {
        printf("Can't open %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    char line[LINE_MAX_LEN];
    while (fgets(line, LINE_MAX_LEN, source)) {
        char instruction[4];
        char *paramA;
        char *paramB;

        if (sscanf("%3s %s, %s", instruction, paramA, paramB) == 3) {
            for (int i = 0; i < 5; i++) {
                if (strcmp(instructions[i].mnemonic, instruction) == 0) {
                    //TODO:BASIC ASM
                }
            }

            fprintf(stderr, "Invalid instruction: %s\n", instruction);
            return 1;
        }
    }

    return 0;
}
