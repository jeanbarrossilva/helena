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

TEST_CASE("\"var\" is var keyword token") {
  Token* token = (Token*)malloc(sizeof(Token));
  init_token(token, 0, 0, "var");
  CHECK(token_is_var_keyword(token));
}

TEST_CASE("Non-var-keyword-token is not a var keyword token") {
  for (int index = 0; index < 65; index++) {
    Token* token = (Token*)malloc(sizeof(Token));
    char* text   = (char*)malloc(9 * sizeof(char));
    strrand(text, 8);
    init_token(token, 0, 0, text);
    CHECK(!token_is_var_keyword(token));
  }
}
