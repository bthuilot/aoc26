/*
 * Copyright (C) 2026 Bryce Thuilot <bryce@thuilot.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the FSF, either version 3 of the License, or (at your option) any later
 * version. See the LICENSE file in the root of this repository for full license
 * text or visit: <https://www.gnu.org/licenses/gpl-3.0.html>.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../aoc.h"

/**
 * part1 - day 0 (dummy day) part1  solution
 * @input: the puzzle input
 *
 * day 0 is a dummy day to test the scaffolding for the repository.
 * Part 1 just returns the input as is.
 *
 * Return: the input as in, formatted using answer().
 */
char *part1(const char *input) { return answer("%s", input); }

/**
 * part1 - day 0 (dummy day) part 2 solution
 * @input: the puzzle input
 *
 * day 0 is a dummy day to test the scaffolding for the repository.
 * Part 2 returns the input, with all lower case letters converted
 * to upper case.
 *
 * Return: the input with all lower case characters converted to upper case,
 * formatted using answer().
 */
char *part2(const char *input) {
  size_t input_size = strlen(input);
  char *upper = malloc(input_size + 1);
  if (upper == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < input_size; i++) {
    upper[i] = toupper(input[i]);
  }
  upper[input_size] = '\0';

  char *a = answer("%s", upper);
  free(upper);
  return a;
}