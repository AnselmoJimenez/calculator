#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAXTOKENLEN 50

double expr(void);
double exprtail(void);
double term(void);
double termtail(void);
double factor(void);
double negation(void);
double power(void);
double powertail(void);
double atom(void);
double function(void);

int gettoken(void);