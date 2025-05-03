#include "../include/mylib.h"

// atof : convert string parameter to double
double atof(char *s) {
    double n = 0.0;
    int power = 1;
    short sign = 0;

    while (isspace(*s++))
        ;
    
    sign = (*s == '-') ? -1 : 1;
    if (*s == '-' || *s == '+') s++;

    while (isdigit(*s)) n = 10.0 * n + (*s++ - '0');

    if (*s != '.') return sign * n;
    s++;

    while (isdigit(*s)) { 
        n = 10.0 * n + (*s++ - '0'); 
        power *= 10;
    }

    return sign * n / power;
}

// strcmp : return 0 if s1 = s2 and return non-zero if s1 != s2
int strcmp(char *s1, char *s2) {
    for ( ; *s1 == *s2; s1++, s2++)
        if (*s1 == '\0')
            return 0;
    return *s1 - *s2;
}

// strlen : return the length of the string argument passed
int strlen(char *s) {
    int len = 0;
    while (*s++) ++len;
    return len;
}

// isspace : returns 1 if character parameter is a space, tab, 
int isspace(char c) { return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r') ? 1 : 0; }

// isdigit : return 1 if character parameter is a number, otherwise return 0
int isdigit(char c) { return (c >= '0' && c <= '9') ? 1 : 0; }

// isalpha : return 1 if character parameter is alphabetic, otherwise return 0
int isalpha(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? 1 : 0; }

// isalnum : return 1 if character parameter is alphanumeric, otherwise return 0
int isalnum(char c) { return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) ? 1 : 0; }