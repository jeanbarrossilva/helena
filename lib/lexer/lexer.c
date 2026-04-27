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

#include <assert.h>
#include <ctype.h>
#include <lexer/lexer.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/**
 * Amount of bytes allocated by default for arrays when extracting tokens from
 * Helena source.
 */
static const size_t INITIAL_ARRAY_CAPACITY = 8;

static void lexer_append_token(Token** tokens,
                               const size_t column,
                               const size_t row,
                               const char* text,
                               size_t length) {
  if (tokens == NULL)
    return;
  init_token(*tokens, column, row, text, length, length);
  token_fprintf(*tokens, stdout);
  printf("\n");
  (*tokens)++;
}

void lexer_tokenize(const char* source, Token* tokens) {
  if (source == NULL || source[0] == '\0' || tokens == NULL)
    return;
  size_t row              = 0;
  size_t column           = 0;
  const char* text        = NULL;
  bool has_previous_token = false;
  while (true) {
    if (isspace(*source) || *source == 0) {
      if (text != NULL) {
        const ptrdiff_t length = &*source - &*text;
        lexer_append_token(&tokens, column - length, row, text, length);
      }
      if (*source != 0) {
        if (has_previous_token) {
          Token* previous_token = tokens - 1;
          if (token_is_newline(previous_token) ||
              token_is_whitespace(previous_token))
            previous_token->span++;
        } else
          lexer_append_token(&tokens, column, row, source, sizeof(char));
      }
      text               = NULL;
      has_previous_token = true;
    } else if (text == NULL)
      text = source;
    if (*source == '\n') {
      row++;
      column = 0;
    } else if (*source == 0)
      break;
    else
      column++;
    source++;
  }
}

void init_token(Token* token,
                const size_t column,
                const size_t row,
                const char* text,
                const size_t length,
                const size_t span) {
  if (token == NULL || text == NULL)
    return;
  assert(column >= 0);
  assert(row >= 0);
  assert(length >= 0);
  assert(span >= length);
  token->column = column;
  token->row    = row;
  token->text   = text;
  token->length = length;
  token->span   = span;
}

bool token_is_attributor(const Token* token) {
  if (token == NULL || token->text == NULL || token->length != 1)
    return false;
  return token->text[0] == '=';
}

bool token_is_id(const Token* token) {
  if (token == NULL || token->text == NULL)
    return false;
  for (int index = 0; index < token->length; index++) {
    const char character = token->text[index];
    if (index == 0 && isnumber(character) || !isalnum(character))
      return false;
  }
  return true;
}

bool token_is_newline(const Token* token) {
  if (token == NULL || token->text == NULL || token->length != 1)
    return false;
  return token->text[0] == '\n';
}

bool token_is_var_keyword(const Token* token) {
  if (token == NULL || token->text == NULL || token->length != 3)
    return false;
  return strcmp(token->text, "var") == 0;
}

bool token_is_whitespace(const Token* token) {
  if (token == NULL || token->text == NULL || token->length != 1)
    return false;
  return token->text[0] == ' ';
}

int token_fprintf(const Token* token, FILE* stream) {
  if (token == NULL || stream == NULL || token->text == NULL)
    return -1;
  return fprintf(stream,
                 "Token { row: %zu, column: %zu, text: “%.*s”, length: %zu, "
                 "span: %zu }",
                 token->row, token->column, (int)token->length, token->text,
                 token->length, token->span);
}

void token_deinit(Token* token) {
  free(token->text);
  free(token);
}
