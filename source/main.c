#include <stdio.h>

#include "../include/parser.h"

int main(void) {
    char t;
    while ((t = gettoken()) != EOF) {
        if (t == '\n') continue;
        double result = expr();

        if ((t = gettoken()) == '\n' || t == EOF)
            printf("\t= %.6g\n", result);
    }
    return 0;
}