#include <stdio.h>

#include "../include/parser.h"

int main(void) {
    while (gettoken() != EOF) {
        double result = expr();
        if (gettoken() == '\n')
            printf("\t=%.4lf\n", result);
    }
    return 0;
}