#include "stdio.h"
#include "protocol5.h"

int main() {

    char data[]="Hello, this is protocol 5";
    char result[MAX_PKT];
    protocol5(data,result);
    printf("Result: %s\n", result);
    return 0;

}
