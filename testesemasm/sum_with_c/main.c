#include <stdio.h>

long sum(long, long);

int main() {
    int n1 = 20;
    int n2 = 7;
    int sumNumbers = sum(n1, n2);
    
    printf("Sum %d, %d = %d\n", n1, n2, sumNumbers);
    return 0;
    
}