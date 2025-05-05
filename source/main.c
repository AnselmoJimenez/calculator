#include <stdio.h>

#include "../include/parser.h"

int main(void) {
    while (gettoken() != EOF) {
        double result = expr();

        char t;
        if ((t = gettoken()) == '\n' || t == EOF)
            printf("\t= %.4lf\n", result);
    }
    return 0;
}