#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char* encode(unsigned char *data, size_t dataLen);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <your_string>\n", argv[0]);
        return 1;

    }

    printf("\n==============================\n=                            =\n= 6th Encoder & Decoder Test =\n=                            =\n==============================\n");
    
    char *myString = argv[1];
    printf("\nInitial string: %s\n", myString);
    
    unsigned char *myEncodedString = encode((unsigned char*)myString, (size_t)strlen(myString));
    printf("Encoded string: %s\n", myEncodedString);
    printf("Decoded string: %s\n", encode(myEncodedString, (size_t)strlen(myString)));

};

unsigned char* encode(unsigned char *data, size_t dataLen) {
    unsigned char *encodedStr = (unsigned char*)malloc(dataLen);
    if (encodedStr == NULL) {
        return NULL;
        
    }

    for (int i = 0; i < dataLen; i++) {
        char _key = i * (i + (i * (i * 2)));
        encodedStr[i] = data[i] ^ _key;

    }

    return encodedStr;
}
