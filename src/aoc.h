/*
 * Copyright (C) 2026 Bryce Thuilot <bryce@thuilot.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the FSF, either version 3 of the License, or (at your option) any later
 * version. See the LICENSE file in the root of this repository for full license
 * text or visit: <https://www.gnu.org/licenses/gpl-3.0.html>.
 */

#ifndef AOC_H
#define AOC_H

/**
 * part1 - computes the answer to part 1 of a solution
 * @input: the input to the puzzle
 *
 * should compute the answer to part 1 of a day's solution.
 * the return value should be computed with answer().
 *
 * Return: the string of the answer to the solution, using
 *         the answer() function. The caller should free the
 *         result.
 */
char *part1(const char *input);

/**
 * part2 - computes the answer to part 2 of a solution
 * @input: the input to the puzzle
 *
 * should compute the answer to part 2 of a day's solution.
 * the return value should be computed with answer().
 *
 * Return: the string of the answer to the solution, using
 *         the answer() function. The caller should free the
 *         result.
 */
char *part2(const char *input);

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
char *answer(const char *fmt, ...);

#endif