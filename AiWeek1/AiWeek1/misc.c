#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/* The function error() can be used just like printf.
 * Its arguments are the same. However, after printing the
 * output (error message) the program aborts.
 */
void error(char *msg, ...) {
  va_list argp;

  fprintf(stderr, "\n");
  fprintf(stderr, "ERROR: ");
  va_start(argp, msg);
  vfprintf(stderr, msg, argp);
  va_end(argp);
  fprintf(stderr, "\n");
  exit(-1);
}

/* prints a syntax error and aborts */
void syntaxError(char filename[]) {
  error("SYNTAX ERROR in file %s\n", filename);
}

/* read characters (skipping ' ' and '\t'). Returns first
 * encountered non-space character
 */
int skipSpaces(FILE *f) {
  int c = ' ';
  while ((c != EOF) && (c!= '\n') && ((c == ' ') || (c == '\t'))) {
    c = fgetc(f);
  }
  return c;
}
