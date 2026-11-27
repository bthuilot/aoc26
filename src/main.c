/*
 * Copyright (C) 2026 Bryce Thuilot <bryce@thuilot.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the FSF, either version 3 of the License, or (at your option) any later
 * version. See the LICENSE file in the root of this repository for full license
 * text or visit: <https://www.gnu.org/licenses/gpl-3.0.html>.
 */

#include "aoc.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_(x) #x
#define STR(x) STR_(x)

#ifndef DAY
#error "a day must be specified with -DDAY=NN"
#endif

/**
 * answer - allocates a new answer string
 * @fmt: printf-style format string
 * @...: arguments consumed by @fmt
 *
 * allocates a new answer string using printf formatting.
 * It should be used for returning from part1/part2 functions.
 * should be freed by the main function.
 *
 * Return: A NUL-terminated string on success, or %NULL if the format could
 *         not be processed or memory could not be allocated. The caller
 *         owns the result and must release it with free().
 */
char *answer(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(NULL, 0, fmt, ap);
  va_end(ap);

  char *s = malloc(n + 1);
  va_start(ap, fmt);
  vsnprintf(s, n + 1, fmt, ap);
  va_end(ap);

  return s;
}

static char *read_file(const char *path) {
  FILE *f = strcmp(path, "-") ? fopen(path, "rb") : stdin;
  if (!f) {
    perror(path);
    exit(1);
  }

  size_t cap = 4096, len = 0, n;
  char *buf = malloc(cap);
  while ((n = fread(buf + len, 1, cap - len - 1, f)) > 0) {
    len += n;
    if (len + 1 == cap)
      buf = realloc(buf, cap *= 2);
  }

  buf[len] = '\0';
  fclose(f);
  return buf;
}

int main(int argc, char **argv) {
  enum parts { ONE, TWO, BOTH };

  enum parts selected = BOTH;
  int quiet = 0;
  char *path = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      printf("advent of code 2026 - day " STR(DAY) "\n");
      printf("github.com/bthuilot/aoc26\n\n");
      printf("(C) bryce thuilot <bryce@thuilot.io>\n");
      exit(0);
    } else if (strcmp(argv[i], "-p1") == 0) {
      selected = ONE;
    } else if (strcmp(argv[i], "-p2") == 0) {
      selected = TWO;
    } else if (strcmp(argv[i], "-q") == 0) {
      quiet = 1;
    } else if (path != NULL) {
      printf("please only specify one path");
      exit(1);
    } else {
      path = argv[i];
    }
  }

  if (!path) {
    path = "inputs/" STR(DAY) ".txt";
  }

  char *input = read_file(path);

  if (!quiet) {
    printf("=======================\n");
    printf("= advent of code 2026 =\n");
    printf("=        day" STR(DAY) "        =\n");
    printf("=======================\n");
  }
  if (selected == ONE || selected == BOTH) {
    char *a1 = part1(input);
    printf(quiet ? "%s\n" : "part 1: %s\n", a1 ? a1 : "(not solved)");
    free(a1);
  }
  if (selected == TWO || selected == BOTH) {
    char *a2 = part2(input);
    printf(quiet ? "%s\n" : "part 2: %s\n", a2 ? a2 : "(not solved)");
    free(a2);
  }

  free(input);

  return 0;
}
