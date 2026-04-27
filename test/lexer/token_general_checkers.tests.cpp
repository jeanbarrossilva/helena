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
#include <strrand.h>
}

#include <doctest.hpp>

#define TOKEN_CHECKER_TESTS(name, text, length)            \
  TEST_CASE("Checks true for " #name " token") {           \
    Token* token = (Token*)malloc(sizeof(Token));          \
    init_token(token, 0, 0, text, length, length);         \
    CHECK(token_is_##name(token));                         \
    free(token);                                           \
  }                                                        \
                                                           \
  TEST_CASE("Check false for non-" #name " token") {       \
    for (int index = 0; index < 65; index++) {             \
      Token* token      = (Token*)malloc(sizeof(Token));   \
      char* random_text = (char*)malloc(9 * sizeof(char)); \
      strrand(random_text, 8);                             \
      init_token(token, 0, 0, random_text, 8, 8);          \
      CHECK(!token_is_##name(token));                      \
      token_deinit(token);                                 \
    }                                                      \
  }

TOKEN_CHECKER_TESTS(attributor, "=", 1)
TOKEN_CHECKER_TESTS(var_keyword, "var", 3)
TOKEN_CHECKER_TESTS(whitespace, " ", 1)
