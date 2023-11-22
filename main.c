#include "stdio.h"
#include "protocol5.h"

int main() {

    char data[]="A B C D E F G H I J K L M N O P Q";
    char result[MAX_PKT];
    protocol5(data,result);
    printf("Result: %s\n", result);
    return 0;

}
