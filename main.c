#include "stdio.h"
#include "protocol5.h"

int main() {
    // See you again Song lyrics :)
    // link:https://www.youtube.com/watch?v=RgKAFK5djSk

    char data[]="It's been a long day without you, my friend."
                " And I'll tell you all about it when I see you again."
                " We've come a long way from where we began."
                " Oh, I'll tell you all about it when I see you again."
                " When I see you again.";
    printf("this is Data that will be sent using Protocol 5 :\n"
           "\t| | | | | | | | | | | |\n"
           "\tv v v v v v v v v v v v\n"
           "(\"%s\")\n\n", data);
    char result[MAX_PKT];
    protocol5(data,result);
    printf("Result: %s\n", result);
    return 0;

}
