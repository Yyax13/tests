#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char* encode(unsigned char *data, size_t dataLen);

int main() {
    printf("\n====================\n=                  =\n= 1st Encoder Test =\n=                  =\n====================\n");
    
    char *myString = "Who is \"EDR\"?";
    printf("\nInitial string: %s\n", myString);
    
    unsigned char *myEncodedString = encode((unsigned char*)myString, (size_t)strlen(myString));
    printf("Encoded string: %s\n", myEncodedString);

};

unsigned char* encode(unsigned char *data, size_t dataLen) {
    unsigned char *encodedStr = (unsigned char*)malloc(dataLen);
    if (encodedStr == NULL) {
        return NULL;
        
    }

    for (int i = 0; i < dataLen; i++) {
        encodedStr[i] = data[i] ^ i;

    }

    return encodedStr;
}
