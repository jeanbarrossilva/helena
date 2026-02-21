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

#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdlib.h>

/**
 * Tokens are the smallest unit of semantic meaning in Helena, consisting of a
 * keyword or a sequence of symbols, by which all declarations and expressions
 * are composed. They are, essentially, the materialization of a given concept
 * of the language; the structures into which the source is, first, converted by
 * the lexer.
 */
struct Token {
  /**
   * Amount of newlines preceding the location of this token in the source. Can
   * be described, also, in terms of the index of the line in which this token
   * is. May be zero; in this case, this token is in the first line of the
   * source.
   */
  size_t column;

  /**
   * Amount of characters between the first character in the column in which
   * this token is and the first character of the text of this token. May be
   * zero; in this case, this token is the first occurrence in the column.
   */
  size_t row;

  /**
   * Textual representation of this token. Because Helena defines discrete
   * combinations of characters as actual, valid tokens, and this text may come
   * from external input, this text may be that of an invalid token. The
   * responsibility of checking such validity is imposed on the lexer.
   */
  const char *text;
} typedef Token;

/**
 * Initializes a region of memory to a token.
 *
 * @param token Pointer to the memory allocated for the token.
 * @param column Amount of newlines preceding the location of this token in the
 * source. Can be described, also, in terms of the index of the line in which
 * the token is.
 * @param row Amount of characters between the first character in the column in
 * which this token is and the first character of the text of this token.
 * @param text Textual representation of this token. Because Helena defines
 * discrete combinations of characters as actual, valid tokens, and this text
 * may come from external input, this text may be that of an invalid token. The
 * responsibility of checking such validity is imposed on the lexer.
 */
void init_token(Token *token, size_t column, size_t row, const char *text);

/* === PREDEFINED TOKENS === */

/**
 * Determines whether the given token is an identifier. In Helena, an
 * identifier is considered to be any sequence of non-whitespace characters in C
 * encoding containing at least one character, with a letter as the first
 * character and zero or more letters or numbers as subsequent characters.
 *
 * @param token The token to check whether it is an identifier.
 */
bool is_id(const Token *token);

#endif // LEXER_H
