#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef int bool;
#define FALSE 0
#define TRUE 1;

enum tokens { UNKNOWN, PARENTHESES, OPERAND, OPERATION, EXPONENT, FUNCTION};

#define MAXTOKENLEN 50

char token[MAXTOKENLEN] = { '\0' };
enum tokens tokentype = 0;

double expression(void);
double term(void);
double factor(void);
double power(void);
double atom(void);
double function(void);

int gettoken(void);

int main(void) {
    while (gettoken() != EOF) {
        // long int result = expression();
        printf("Token: %s\n", token);

        memset(token, '\0', MAXTOKENLEN);
    }

    return 0;
}

double expression(void) {}
double term(void) {}
double factor(void) {}

// power : evaluate exponents and return their value
double power(void) {
    
}

// atom : Return the basic building block of an expression
//        numbers, function calls, and other expressions in parentheses
double atom(void) {
    switch (tokentype) {
        case OPERAND:   return atof(token);
        case FUNCTION:  return function();
        case PARENTHESES:
            enum tokens type = UNKNOWN;

            // syntax checks
            if (token[0] == '(') {
                double quantity = expression();
                if ((type = gettoken()) != PARENTHESES && token[0] != ')') {
                    printf("Atom: Syntax Error: Unclosed Parenthesis \'(\'\n");
                    return -1;
                }
                return quantity;
            }
            else {
                printf("Atom: Syntax Error: Missing Initialization \'(\'\n");
                return -1;
            }
        default:
            printf("Atom: Token Error: Unknown Token \"%s\"\n", token);
            return -1;
    }
}

enum funcs { SIN, COS, TAN, INVALID };
static const char *supported[] = { "sin", "cos", "tan" };

// function : Return the evaluated function call
double function(void) {
    // Check that input function is valid
    enum funcs i = 0;
    for (i; i < INVALID; i++)
        if (strcmp(token, supported[i]) == 0)
            break;
    if (i == INVALID) {
        printf("Function: Token Error: Unknown Token \"%s\"\n", token);
        return -1;
    }
    
    // Syntax Checks
    enum tokens type = UNKNOWN;
    if ((type = gettoken()) != PARENTHESES && token[0] != '(') {
        printf("Function: Syntax Error: Missing Initialization \'(\'\n");
        return -1;
    }

    double quantity = expression();

    if ((type = gettoken()) != PARENTHESES && token[0] != ')') {
        printf("Function: Syntax Error: Unclosed Parenthesis \'(\'\n");
        return -1;
    }

    // Evaluate the function
    switch (i) {
        case SIN:
            return sin(quantity);
        case COS:
            return cos(quantity);
        case TAN:
            return tan(quantity);
        default:
            return -1;
    }
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

    tokentype = UNKNOWN;
    return c;
}


#define MAXBUFFER  100

static char cbuffer[MAXBUFFER] = { '\0' };
static char *cbp = &cbuffer[0];

// getch : get next character from input
int getch(void) {
    return (cbp > cbuffer) ? *--cbp : getchar();
}

// ungetch : push a character back on to the input
void ungetch(int c) {
    if (cbuffer + MAXBUFFER - cbp <= 0) printf("cbuffer: Stack is full");
    else *cbp++ = c;
}

// ungets : push a string back on to the input
void ungets(char *s) {
    // TODO
}