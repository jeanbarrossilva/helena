/*
 * Copyright © 2026 Jean Silva
 *
 * This file is part of the Helena programming language open-source project.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *                  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

extern "C" {
#include <hx/string.h>
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <doctest.hpp>

using namespace std;

TEST_CASE(
    "Exits when concatenating non-sequentially and string ends up unfilled") {
  ConcatenationStrategy strategy = GENERATE(FILL, TRUNCATE);
  pid_t pid                      = fork();
  if (pid == 0) {
    char actual[64] = "Hello, ";
    hx_strcat(actual, sizeof(actual), "world!", strategy);
    return;
  }
  int status = 0;
  waitpid(pid, &status, 0);
  CHECK(WIFEXITED(status));
  CHECK(WEXITSTATUS(status) == EINVAL);
}

TEST_CASE(
    "Exits when concatenating non-truncatingly and string is overflowed") {
  ConcatenationStrategy strategy = GENERATE(FILL, SEQUENTIAL);
  pid_t pid                      = fork();
  if (pid == 0) {
    char actual[12] = "Hello, ";
    hx_strcat(actual, sizeof(actual), "world!", strategy);
    return;
  }
  int status = 0;
  waitpid(pid, &status, 0);
  CHECK(WIFEXITED(status));
  CHECK(WEXITSTATUS(status) == ENAMETOOLONG);
}

TEST_CASE("Concatenates with truncation") {
  char actual[12] = "Hello, ";
  string expected = "Hello, worl";
  size_t concatenation_length =
      hx_strcat(actual, sizeof(actual), "world!", TRUNCATE);
  CHECK(concatenation_length == 11);
  CHECK(actual == expected);
}

TEST_CASE("Concatenates sequentially without filling") {
  char actual[64] = "Hello, ";
  string expected = "Hello, world!";
  size_t concatenation_length =
      hx_strcat(actual, sizeof(actual), "world!", SEQUENTIAL);
  CHECK(concatenation_length == expected.length());
  CHECK(actual == expected);
}

TEST_CASE("Concatenates with when string gets filled") {
  ConcatenationStrategy strategy = GENERATE(FILL, TRUNCATE, SEQUENTIAL);
  char actual[14]                = "Hello, ";
  string expected                = "Hello, world!";
  size_t concatenation_length =
      hx_strcat(actual, sizeof(actual), "world!", strategy);
  CHECK(concatenation_length == expected.length());
  CHECK(actual == expected);
}
