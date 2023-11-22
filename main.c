#include "stdio.h"
#include "protocol5.h"

int main() {

    char data[]="1 2 3 4 5 6 7 8";
    char result[MAX_PKT];
    protocol5(data,result);
    printf("Result: %s\n", result);
    return 0;

}
