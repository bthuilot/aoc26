/*
 * Copyright (C) 2026 Bryce Thuilot <bryce@thuilot.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the FSF, either version 3 of the License, or (at your option) any later version.
 * See the LICENSE file in the root of this repository for full license text or
 * visit: <https://www.gnu.org/licenses/gpl-3.0.html>.
 */


#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

enum part {
  ONE,
  TWO,
};

enum result {
  FAILED,
  PASSED,
  NOT_RUN,
};

typedef struct {
  int day;
  enum part part;
  const char *input;
  const char *expected;
} TestCase;


TestCase testcases[] = {
    {0, ONE, "hello, world!", "hello, world!"},
    { 0, TWO, "hello, world!", "HELLO, WORLD!"},
};


enum result run_testcase(TestCase t) {
  int input_pipe[2], output_pipe[2];
  if (pipe(input_pipe) < 0)
    return NOT_RUN;

  if (pipe(output_pipe) < 0)
    return NOT_RUN;

  
  char *bin = (char *)malloc(6);
  snprintf(bin, 6, "day%02d", t.day);
  bin[5] = '\0';

  pid_t pid = fork();
  if (pid == -1) {
    return NOT_RUN;
  }

  fflush(stdout);
  if (pid == 0) {
    dup2(input_pipe[0], STDIN_FILENO);
    dup2(output_pipe[1], STDOUT_FILENO);

    close(input_pipe[0]);
    close(input_pipe[1]);
    close(output_pipe[0]);
    close(output_pipe[1]);

    char *part_flag = t.part == ONE ? "-p1" : "-p2";
    execlp(bin, bin, part_flag, "-q", "-", (char *)0);
    perror("failed to execute day program");
    exit(1);
  }

  printf("running test of day %d, part %d... ", t.day, t.part == ONE ? 1 : 2);
  close(input_pipe[0]);
  close(output_pipe[1]);
  write(input_pipe[1], t.input, strlen(t.input));
  close(input_pipe[1]);
  free(bin);

  char *output = malloc(256);
  read(output_pipe[0], output, 255);
  for (int i = 0; i < 256; i++) {
    if (output[i] == '\n')
      output[i] = '\0'; // stop parsing at first newline
  }

  if (strcmp(t.expected, output) != 0) {
    printf("FAILED\nexpected \"%s\" got \"%s\"\n", t.expected, output);
    return FAILED;
  }
  printf("PASSED\n");
  return PASSED;
}

int main(int argc, char **argv) {

  printf("===============================\n");
  printf("= advent of code 2026 - tests =\n");
  printf("===============================\n\n");

  int selected_day = -1;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {
      if (argc <= i + 1) {
        printf("please supply a date with '-d' flag\n");
        exit(1);
      }
      selected_day = (int)strtol(argv[i + 1], (char **)NULL, 10);
      if (errno != 0) {
        perror("'-d' is not a valid number\n");
        exit(1);
      }
    }
  }

  int total = 0, failed = 0, not_run = 0, passed =0;
  for (size_t tci = 0; tci < sizeof(testcases)/sizeof(TestCase); tci++) {
    TestCase tc = testcases[tci];
    if (selected_day >= 0 && tc.day != selected_day) {
      printf("skipping day %d, not selected\n", tc.day);
      continue;
    }

    enum result r = run_testcase(tc);
    switch (r) {
    case FAILED:
      failed++;
      break;
    case NOT_RUN:
      not_run++;
      break;
    case PASSED:
      passed++;
      break;
    }
    total++;
  }

  printf("\n%d tests ran - %d passed, %d failed, %d not run\n", total, passed, failed, not_run);


  return 0;
}
