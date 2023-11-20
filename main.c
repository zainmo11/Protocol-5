#include "stdio.h"
#include "protocol5/protocol5.h"
int main() {

    char data[]="Hello, this is protocol 5";
    char result[]=" ";
    protocol5(data,result);
    return 0;

}
