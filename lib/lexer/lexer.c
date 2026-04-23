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

#include <ctype.h>
#include <lexer/lexer.h>
#include <strings.h>

void init_token(Token* token,
                const size_t column,
                const size_t row,
                const char* text) {
  if (token == NULL)
    return;
  token->column = column;
  token->row    = row;
  token->text   = text;
}

bool token_is_attributor(const Token* token) {
  return strlen(token->text) == 1 && token->text[0] == '=';
}

bool token_is_id(const Token* token) {
  const char* text = token->text;
  for (int index = 0; index < strlen(text); index++) {
    const char character = text[index];
    if (index == 0 && isnumber(character) || !isalnum(character))
      return false;
  }
  return true;
}

bool token_is_var_keyword(const Token* token) {
  return strcmp(token->text, "var") == 0;
}

bool token_is_whitespace(const Token* token) {
  return strcmp(token->text, " ") == 0;
}
