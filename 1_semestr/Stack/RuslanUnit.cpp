#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack.cpp"

int main() {

    Stack_t stk = {};
    int error = 0;

    StackInit(&stk);
    StackPush(&stk, 2.28);

    printf("Let's kill just a usual drugdealer: \n");

    *(double*)(stk.data + sizeof(int)) = 6.66;
    StackDump(&stk, "FUCK FBI", "MYFILE", 666, "COURT HACKING");
    printf("\n\n\n"
           "\"Guilty!! Of %lg article CCRF\"\n"
           "\"OMG! He's a satanist, not a drugdealer, BURN HIM\"\n\n", StackPop(&stk, &error));


    StackDisinit(&stk);

    return 0;
}
