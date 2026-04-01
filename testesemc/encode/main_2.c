#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char* encode(unsigned char *data, size_t dataLen);

int main() {
    printf("\n====================\n=                  =\n= 2nd Encoder Test =\n=                  =\n====================\n");
    
    char *myString = "Who is \"EDR\"?";
    printf("\nInitial string: %s\n", myString);
    
    unsigned char *myEncodedString = encode((unsigned char*)myString, (size_t)strlen(myString));
    printf("Encoded string: %s\n", myEncodedString);

};

unsigned char* encode(unsigned char *data, size_t dataLen) {
    unsigned char *encodedStr = (unsigned char*)malloc(dataLen + 1);
    if (encodedStr == NULL) {
        return NULL;
        
    }

    for (int i = 0; i < dataLen; i++) {
        char rKey = data[(i - 2) % dataLen] ^ (i * i);
        encodedStr[i] = data[i] ^ rKey;

    }

    encodedStr[dataLen] = '\0';
    return encodedStr;
}
