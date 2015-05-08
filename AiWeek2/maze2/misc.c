#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

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

void syntaxError(char filename[]) {
  error("SYNTAX ERROR in file %s\n", filename);
}

int skipSpaces(FILE *f) {
  int c = ' ';
  while ((c != EOF) && (c!= '\n') && ((c == ' ') || (c == '\t'))) {
    c = fgetc(f);
  }
  return c;
}
