#define MAXTOKENLEN 50

double expr(void);
double exprtail(double);
double term(void);
double termtail(double);
double factor(void);
double power(void);
double powertail(double);
double atom(void);
double function(void);

void recover(void);
int gettoken(void);