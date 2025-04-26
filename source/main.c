#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum tokens { NONE, PARENTHESES, OPERAND, OPERATION, EXPONENT, FUNCTION};

#define MAXTOKENLEN 50

char token[MAXTOKENLEN] = { '\0' };
enum tokens tokentype = 0;

void expression(void);
void term(void);
void factor(void);
void power(void);
void atom(void);

int gettoken(void);

int main(void) {
    while (gettoken() != EOF) {
        printf("%s\n", token);
        memset(token, '\0', MAXTOKENLEN);
    }

    return 0;
}

void expression(void) {

}

int getch(void);
void ungetch(int c);

// gettoken : Get the next token from the input
int gettoken(void) {
    char c = 0;
    char next = 0;
    char *tp = token;

    while (isspace(c = getch()) && c != '\n')
        ;
    
    *tp++ = c;
    *tp = '\0';

    if (c == '-') {    
        if (!isdigit(next = getch())) { // Check for arithmetic operation or negative number
            ungetch(next);
            *tp = '\0';
            return tokentype = OPERATION;
        }
        *tp++ = c = next;
    }
    else if (c == '+' || c == '*' || c == '/')  return tokentype = OPERATION;
    else if (c == ')' || c == '(')              return tokentype = PARENTHESES;
    else if (c == '^')                          return tokentype = EXPONENT;
    
    if (isdigit(c)) {
        for ( ; isdigit(c = getch()); *tp++ = c)
            ;

        if (c == '.') *tp++ = c;
        else {
            *tp = '\0';
            ungetch(c);
            return tokentype = OPERAND;
        }

        // collect decimal portion
        if (isdigit(c = getch())) {
            for (*tp++ = c; isdigit(c = getch()); *tp++ = c)
                ;
            *tp = '\0';
        }
        else {
            *tp++ = '0';
            *tp = '\0';
        }
        ungetch(c);
        return tokentype = OPERAND;
    }
    else if (isalpha(c)) {
        for ( ; isalnum(c = getch()); *tp++ = c)
            ;
        *tp = '\0';
        ungetch(c);
        return tokentype = FUNCTION;
    }

    tokentype = NONE;
    return c;
}


#define MAXBUFFER  100

static char cbuffer[MAXBUFFER] = { '\0' };
static char *cbp = &cbuffer[0];

int getch(void) {
    return (cbp > cbuffer) ? *--cbp : getchar();
}

void ungetch(int c) {
    if (cbuffer + MAXBUFFER - cbp <= 0) printf("cbuffer: Stack is full");
    else *cbp++ = c;
}