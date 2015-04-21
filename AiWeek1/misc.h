#ifndef MISC_H
#define MISC_H

/* Some miscellaneous helper functions */
#include <stdio.h>

/* The function error() can be used just like printf.
 * Its arguments are the same. However, after printing the
 * output (error message) the program aborts.
 */
void error(char *msg, ...);

/* prints a syntax error and aborts */
void syntaxError(char filename[]);

/* read characters (skipping ' ' and '\t'). Returns first
 * encountered non-space character
 */
int skipSpaces(FILE *f);

#endif
