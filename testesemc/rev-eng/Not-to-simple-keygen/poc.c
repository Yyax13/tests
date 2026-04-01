#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "utils/main.h"

bytes_t hashUserData(UserData_t userData);
string_t genSerial(bytes_t baseHash);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <name> <mail>\n", argv[0]);
        return 1;

    }

    UserData_t userData = {
        .Email = {
            .s = argv[2],
            .len = strlen(argv[2])

        },
        .FullName = {
            .s = argv[1],
            .len = strlen(argv[1])

        }
    };

    bytes_t hash = hashUserData(userData);
    string_t serial = genSerial(hash);

    fprintf(stdout, "%s\n%s\n%s\n", userData.FullName.s, userData.Email.s, serial.s);
    return 0;

}

bytes_t hashUserData(UserData_t userData) {
    bytes_t fullNameBytes = {
        .b = (uchar_t*)(userData.FullName.s),
        .len = userData.FullName.len

    };

    bytes_t emailBytes = {
        .b = (uchar_t*)(userData.Email.s),
        .len = userData.Email.len

    };

    bytes_t fullNameHash = hashUltraPro(fullNameBytes);
    bytes_t emailHash = hashUltraPro(emailBytes);
    bytes_t baseForBaseHash = {
        .len = fullNameHash.len + emailHash.len

    };

    baseForBaseHash.b = malloc(baseForBaseHash.len);
    memcpy(baseForBaseHash.b, fullNameHash.b, fullNameHash.len);
    memcpy(baseForBaseHash.b + fullNameHash.len, emailHash.b, emailHash.len);

    free(fullNameHash.b);
    free(emailHash.b);

    bytes_t baseHash = hashUltraPro(baseForBaseHash);
    free(baseForBaseHash.b);

    return baseHash;

};

string_t genSerial(bytes_t baseHash) {
    char serial[17] = "HOWOABUSANOVINHA"; // 16 + \0
    serial[16] = '\0';

    for (int i = 0; i < 16; i += 3) {
        if (serial[(i + 1) % 16] + serial[i] != serial[(i + 2) % 16] << 3) {
            if (serial[(i + 3) % 16] != (char)baseHash.b[(i)] << 2) {
                serial[(i + 3) % 16] = (char)baseHash.b[(i)] << 2;

            }
        } else {
            serial[(i + 1) % 16] = (serial[(i + 2) % 16] << 3) - serial[i];

        }
    }

    string_t out = {
        .s = serial,
        .len = strlen(serial) + 1

    };

    return out;

};