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
#include <lexer/lexer.h>
}

#include <doctest.hpp>

TEST_CASE("Token with only a digit is not an ID") {
  int prefix = GENERATE(0, 1, 2, 3, 4, 5, 6, 7, 8);
  char* text;
  CHECK(asprintf(&text, "%d", prefix));
  Token* token = (Token*)malloc(sizeof(Token));
  init_token(token, 0, 0, text);
  CHECK(!token_is_id(token));
  free(token);
}

TEST_CASE("Token prefixed by a digit and followed by letters is not an ID") {
  int prefix = GENERATE(0, 1, 2, 3, 4, 5, 6, 7, 8);
  char* text;
  CHECK(asprintf(&text, "%dID", prefix));
  Token* token = (Token*)malloc(sizeof(Token));
  init_token(token, 0, 0, text);
  CHECK(!token_is_id(token));
  free(token);
}

TEST_CASE("Token with only a letter is an ID") {
  Token* token = (Token*)malloc(sizeof(Token));
  init_token(token, 0, 0, "h");
  CHECK(token_is_id(token));
  free(token);
}

TEST_CASE("Token with only letters is an ID") {
  Token* token = (Token*)malloc(sizeof(Token));
  init_token(token, 0, 0, "helena");
  CHECK(token_is_id(token));
  free(token);
}

TEST_CASE("Token prefixed by a letter and followed by digits is an ID") {
  Token* token = (Token*)malloc(sizeof(Token));
  init_token(token, 0, 0, "h06");
  CHECK(token_is_id(token));
  free(token);
}
