
#include "../include/parser.h"

enum types { UNKNOWN, PARENTHESES, OPERAND, OPERATION, EXPONENT, FUNCTION};

struct tokeninfo {
    char token[MAXTOKENLEN];
    enum types tokentype;
} current = {
    .token = {'\0'},
    .tokentype = UNKNOWN
};

double expr(void) {

}

double exprtail(void) {

}

double term(void) {

}

double termtail(void) {

}

double factor(void) {

}

double negation(void) {

}

double power(void) {

}

double powertail(void) {

}

double atom(void) {

}

double function(void) {

}

int getch(void);
void ungetch(int c);

// gettoken : Get next token from input
int gettoken(void) {
    char c = 0;
    char next = 0;
    char *tp = current.token;

    while (isspace(c = getch()) && c != '\n')
        ;
    
    *tp++ = c;
    *tp = '\0';

    if (c == '-') {    
        if (!isdigit(next = getch())) { // Check for arithmetic operation or negative number
            ungetch(next);
            *tp = '\0';
            return current.tokentype = OPERATION;
        }
        *tp++ = c = next;
    }
    else if (c == '+' || c == '*' || c == '/')  return current.tokentype = OPERATION;
    else if (c == ')' || c == '(')              return current.tokentype = PARENTHESES;
    else if (c == '^')                          return current.tokentype = EXPONENT;

    if (isdigit(c)) {
        for ( ; isdigit(c = getch()); *tp++ = c)
            ;

        if (c == '.') *tp++ = c;
        else {
            *tp = '\0';
            ungetch(c);
            return current.tokentype = OPERAND;
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
        return current.tokentype = OPERAND;
    }
    else if (isalpha(c)) {
        for ( ; isalnum(c = getch()); *tp++ = c)
            ;
        *tp = '\0';
        ungetch(c);
        return current.tokentype = FUNCTION;
    }

    current.tokentype = UNKNOWN;
    return c;
}

#define MAXBUFFER 100

static char cbuffer[MAXBUFFER] = {'\0'};
static char *cbp = &cbuffer[0];

// getch : get next character from input
int getch(void) {
    return (cbp > cbuffer) ? *--cbp : getchar();
}

// ungetch : push a character back on to the input
void ungetch(int c) {
    if (cbuffer + MAXBUFFER - cbp <= 0) printf("ungetch: Overflow Error: Stack is full");
    else *cbp++ = c;
}

// ungets : push a string back on to the input
void ungets(char *s) {
    if (cbuffer + MAXBUFFER < cbp - strlen(s)) printf("ungets: Overflow Error: Stack is full");
    else {
        int length = strlen(s);
        while (length-- > 0) *cbp++ = *s++;
    }
}