#include <stdio.h>
#include <math.h>

#include "../include/parser.h"
#include "../include/mylib.h"

enum types { UNKNOWN, PARENTHESES, OPERAND, OPERATION, EXPONENT, FUNCTION, END };

struct tokeninfo {
    char token[MAXTOKENLEN];
    enum types tokentype;
} current = {
    .token = {'\0'},
    .tokentype = UNKNOWN
};

void ungets(char *);
void recover(void);

double expr(void) {
    return exprtail(term());
}

double exprtail(double prev) {
    gettoken();
    if (current.token[0] == '+') {
        gettoken();
        return exprtail(prev + term());
    }
    else if (current.token[0] == '-') {
        gettoken();
        return exprtail(prev - term());
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
    if (current.token[0] == '*') {
        gettoken();
        return termtail(prev * factor());
    }
    else if (current.token[0] == '/') {
        gettoken();
        return termtail(prev / factor());
    }
    else if (current.token[0] == '(' || current.tokentype == OPERAND || current.tokentype == FUNCTION)
        return termtail(prev * factor());
    else {
        ungets(current.token);
        return prev;
    }
}

double factor(void) {
    if (current.tokentype == OPERAND || current.tokentype == FUNCTION || current.tokentype == PARENTHESES)
        return power();
    
    if (current.tokentype == OPERATION && current.token[0] == '-') {
        gettoken();
        return -1 * factor();
    }
}

double power(void) {
    return powertail(atom());
}

double powertail(double prev) {
    gettoken();
    if (current.token[0] == '^') {
        gettoken();
        return pow(prev, factor());
    }
    else {
        ungets(current.token);
        return prev;
    }
}

double atom(void) {
    switch (current.tokentype) {
        case OPERAND: return atof(current.token);
        case FUNCTION: return function();
        case PARENTHESES:
            if (current.token[0] != '(') {
                printf("Atom: Syntax Error: Unopened Parenthesis\n");
                recover();
                return -1;
            }

            gettoken();
            if (current.token[0] == ')') {
                printf("Atom: Syntax Error: Empty Parenthesis\n");
                recover();
                return -1;
            }

            double result = expr();
            
            gettoken();
            if (current.token[0] != ')') {
                printf("Atom: Syntax Error: Unclosed Parenthesis\n");
                ungets(current.token);
                recover();
                return -1;
            }

            return result;
        default:
            printf("Error: Atom: Unrecognized tokentype");
            return 0.0;
    }
}

enum funcs { SIN, COS, TAN, ASIN, ACOS, ATAN, SQRT, ABS, LN, LOG, EXP, FLOOR, CEIL, INVALID };
static const char *supported[] = { 
    "sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "abs", "ln", "log", "exp", "floor", "ceil"
};

double function(void) {
    // Check that input function is valid
    enum funcs i = 0;
    for (i; i < INVALID; i++)
        if (strcmp(current.token, supported[i]) == 0)
            break;
    if (i == INVALID) {
        printf("Syntax Error: Unrecognized Function\n");
        recover();
        return -1;
    }

    gettoken();
    if (current.token[0] != '(') {
        printf("Function: Syntax Error: Unopened Parenthesis\n");
        recover();
        return -1;
    }

    gettoken();
    double result = 0;
    switch (i) {
        case SIN:   result = sin(expr());   break;
        case COS:   result = cos(expr());   break;
        case TAN:   result = tan(expr());   break;
        case ASIN:  result = asin(expr());  break;
        case ACOS:  result = acos(expr());  break;
        case ATAN:  result = atan(expr());  break;
        case SQRT:  result = sqrtf(expr()); break;
        case ABS:   result = myabs(expr()); break;
        case LN:    result = log(expr());   break;
        case LOG:   result = log10(expr()); break;
        case EXP:   result = exp(expr());   break;
        case FLOOR: result = floor(expr()); break;
        case CEIL:  result = ceil(expr());  break;
    }
    
    gettoken();
    if (current.token[0] != ')') {
        printf("Function: Syntax Error: Unclosed Parenthesis\n");
        recover();
        return -1;
    }
    return result;
}

// Recover : Recover from a syntax error
void recover(void) {
    char token = '\0';
    while ((token = gettoken()) != '\n' && token != EOF) 
        ;
    if (token == '\n' || token == EOF) ungets(current.token);
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
    else if (c == '+') {
        if (!isdigit(next = getch())) { // Check for arithmetic operation or positive number
            ungetch(next);
            *tp = '\0';
            return current.tokentype = OPERATION;
        }
        --tp;
        *tp++ = c = next;
    } 
    else if (c == '*' || c == '/')  return current.tokentype = OPERATION;
    else if (c == ')' || c == '(')  return current.tokentype = PARENTHESES;
    else if (c == '^')              return current.tokentype = EXPONENT;

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

    if (c == '\n') {
        current.tokentype = END;
        return c;
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
    if (cbuffer + MAXBUFFER <= cbp) printf("ungetch: Overflow Error: Stack is full\n");
    else *cbp++ = c;
}

// ungets : push a string back on to the input
void ungets(char *s) {
    if (cbp + strlen(s) > cbuffer + MAXBUFFER) printf("ungets: Overflow Error: Stack is full\n");
    else {
        int length = strlen(s);
        s += length - 1;
        while (length-- > 0) {
            *cbp++ = *s--;
        }
    }
}