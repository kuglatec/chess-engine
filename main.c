#include "movegen.c"
#include <stdio.h>

int main() {
    
    setbuf(stdout, NULL); /*disable buffering for stdout*/
    setbuf(stdin, NULL); /*same for stdin*/
    cli();
    return 0;
}
