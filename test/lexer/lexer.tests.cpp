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

TEST_CASE("Tokenizes") {
  Token* tokens = (Token*)malloc(3 * sizeof(Token));
  lexer_tokenize("Hello,\nworld!", tokens);
  CHECK(tokens[0].row == 0);
  CHECK(tokens[0].column == 0);
  CHECK(strncmp(tokens[0].text, "Hello,", 6) == 0);
  CHECK(tokens[0].length == 6);
  CHECK(tokens[0].span == 6);
  CHECK(tokens[1].row == 0);
  CHECK(tokens[1].column == tokens[0].length);
  CHECK(tokens[1].length == 1);
  CHECK(tokens[1].span == 1);
  CHECK(tokens[2].row == 1);
  CHECK(tokens[2].column == 0);
  CHECK(strncmp(tokens[2].text, "world!", 6) == 0);
  CHECK(tokens[2].length == 6);
  CHECK(tokens[2].span == 6);
  free(tokens);
}
