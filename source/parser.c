#include <stdio.h>

#include "../include/parser.h"
// #include "../include/stack.h"

enum types { UNKNOWN, PARENTHESES, OPERAND, OPERATION, EXPONENT, FUNCTION, END};

struct tokeninfo {
    char token[MAXTOKENLEN];
    enum types tokentype;
} current = {
    .token = {'\0'},
    .tokentype = UNKNOWN
};

void ungets(char *);
void printstack(void);

double expr(void) {
    return exprtail(atom());
}

double exprtail(double prev) {
    gettoken();
    if (current.token[0] == '+') {
        gettoken();    
        return exprtail(prev + atom()); // + term()
    }
    else if (current.token[0] == '-') {
        gettoken();   
        return exprtail(prev - atom()); // - term()
    }
    else {
        ungets(current.token);
        return prev;
    }
}

double term(void) {
    return termtail(factor());
}

double termtail(double prev) {
    gettoken();
    if (current.token[0] == '*')       
        return termtail(prev * factor());
    else if (current.token[0] == '/')   
        return termtail(prev / factor());
    else if (current.tokentype == PARENTHESES)
        return factor() * factor();  
    else {
        ungets(current.token);
        gettoken();
        return prev;
    }
}

double factor(void) {
    gettoken();
    if (current.token[0] == '-')
        return -1 * factor();
    else {
        ungets(current.token);
        gettoken();
        return power();
    }
}

double power(void) {
    return powertail(atom());
}

double powertail(double prev) {
    gettoken();
    if (current.tokentype == EXPONENT)
        return pow(prev, factor());
    else {
        ungets(current.token);
        gettoken();
        return prev;
    }
}

double atom(void) {
    switch (current.tokentype) {
        case OPERAND: return atof(current.token);
        case FUNCTION: return function();
        case PARENTHESES:
            if (current.token[0] == '(') {
                gettoken();
                double result = expr();
                
                gettoken();
                if (current.token[0] != ')') {
                    return -1;
                }

                return result;
            }
    }
}

enum funcs { SIN, COS, TAN, INVALID };
static const char *supported[] = { "sin", "cos", "tan" };

double function(void) {
    // Check that input function is valid
    enum funcs i = 0;
    for (i; i < INVALID; i++)
        if (strcmp(current.token, supported[i]) == 0)
            break;
    if (i == INVALID) return -1;

    gettoken();
    if (current.token[0] != '(') return -1;

    gettoken();
    double result = 0;
    switch (i) {
        case SIN: result = sin(expr());
        case COS: result = cos(expr());
        case TAN: result = tan(expr());
    }

    gettoken();
    if (current.token[0] != ')') return -1;

    return result;
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
    if (cbuffer + MAXBUFFER <= cbp) printf("ungetch: Overflow Error: Stack is full");
    else *cbp++ = c;
}

// ungets : push a string back on to the input
void ungets(char *s) {
    if (cbp + strlen(s) > cbuffer + MAXBUFFER) printf("ungets: Overflow Error: Stack is full");
    else {
        int length = strlen(s);
        s += length - 1;
        while (length-- > 0) {
            *cbp++ = *s--;
        }
    }
}

void printstack(void) {
    char *sp = cbuffer;
    printf("<");
    while (*sp != '\0')
        printf("%c, ", *sp++);
    printf("\b\b>\n");
}