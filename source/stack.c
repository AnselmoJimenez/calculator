#include <stdio.h>

#define MAXSTACK 100

static char *cstack[MAXSTACK] = {"\0"};
static char *cp = cstack;

// push : Push a character onto the stack
void push(int c) {
    if (cstack + MAXSTACK <= cp) printf("Push: Overflow Error: Stack is full");
    else *cp++ = c; 
}

// pop : return the character at the top of the stack
int pop(void) {
    char tmp = *--cp;
    *cp = '\0';
    return tmp;
}

// printstack : print the current contents of the stack
void printstack(void) {
    printf("<");
    for (int i = 0; i < MAXSTACK && cstack[i] != '\0'; i++)
        printf("%c, ", cstack[i]);
    printf("\b\b>\n");
}