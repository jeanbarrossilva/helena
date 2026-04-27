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
#include <stdio.h>

/**
 * Tokens are the smallest unit of semantic meaning in Helena, consisting of a
 * keyword or a sequence of symbols, by which all declarations and expressions
 * are composed. They are, essentially, the materialization of a given concept
 * of the language; the structures into which the source is, first, converted by
 * the lexer.
 */
struct Token {
  /**
   * Amount of characters between the first character in the column in which
   * this token is and the first character of the text of this token. May be
   * zero; in this case, this token is the first occurrence in the column.
   */
  size_t column;

  /**
   * Amount of newlines preceding the location of this token in the source. Can
   * be described, also, in terms of the index of the line in which this token
   * is. May be zero; in this case, this token is in the first line of the
   * source.
   */
  size_t row;

  /**
   * Textual representation of this token. Because Helena defines discrete
   * combinations of characters as actual, valid tokens, and this text may come
   * from external input, this text may be that of an invalid token. The
   * responsibility of checking such validity is imposed on the lexer.
   */
  char* text;

  /** Amount of characters in the text of a single occurrence of this token. */
  size_t length;

  /**
   * Sum of the amount of characters of the first and each consecutive
   * occurrence of this token; by definition, span ≥ count. E.g., from some
   * source containing "n  = 2", with two adjacent whitespaces on the left-hand
   * side, only one whitespace token would be extracted by the lexer, whose
   * count is 1 and whose span is 2.
   */
  size_t span;
} typedef Token;

/**
 * Extracts tokens from the given Helene source code.
 *
 * This is the first step of the compilation phase, in which the characters
 * (from, e.g., a .helena file) are read and grouped into tokens, which may or
 * may not compose valid Helena syntax. Usually, the tokens extracted by this
 * function are passed into the parser, which performs syntax validation and,
 * in case the syntax is valid, generates an abstract syntax tree (AST).
 *
 * @param source Helena source code.
 * @param tokens Array to which extracted tokens will be appended.
 */
void lexer_tokenize(const char* source, Token* tokens);

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
 * @param length Amount of characters in the text of a single occurrence of this
 * token.
 * @param span Sum of the amount of characters of the first and each consecutive
 * occurrence of this token; by definition, span ≥ count. E.g., from some source
 * containing "n  = 2", with two adjacent whitespaces on the left-hand side,
 * only one whitespace token would be extracted by the lexer, whose count is 1
 * and whose span is 2.
 */
void init_token(Token* token,
                const size_t column,
                const size_t row,
                const char* text,
                const size_t length,
                const size_t span);

/**
 * Determines whether the given token is an = (equals sign), denoting an
 * attribution of the result of evaluating an expression to some variable.
 * Attributors may be separated from the name of the variable and the expression
 * by whitespaces.
 *
 * @param token The token to check whether it is an attributor.
 */
bool token_is_attributor(const Token* token);

/**
 * Determines whether the given token is an identifier. In Helena, an
 * identifier is considered to be any sequence of non-whitespace characters in C
 * encoding containing at least one character, with a letter as the first
 * character and zero or more letters or numbers as subsequent characters.
 *
 * @param token The token to check whether it is an identifier.
 */
bool token_is_id(const Token* token);

/**
 * Determines whether the given token is a newline (i.e., "\n" in a C string
 * format).
 *
 *  @param token The token to check whether it is a newline.
 */
bool token_is_newline(const Token* token);

/**
 * Determines whether the given token is the keyword for beginning the
 * declaration of a variable which can be reassigned, i.e., is mutable.
 *
 * @param token The token to check whether it is a "var" keyword.
 */
bool token_is_var_keyword(const Token* token);

/**
 * Determines whether the given token is a whitespace (" "). Whitespaces may be
 * inserted into the source for separating (separable) tokens from each other,
 * and may be so consecutively.
 *
 * @param token The token to check whether it is a whitespace.
 */
bool token_is_whitespace(const Token* token);

/**
 * Writes the given token to the specified stream.
 *
 * @param token Token to write to the stream.
 * @param stream Pointer to the stream to which the representation of the token
 * will be written.
 * @return Amount of characters written to the stream. In case there was an
 * error, it will be a negative integer.
 */
int token_fprintf(const Token* token, FILE* stream);

/**
 * Deinitializes a given token, freeing its allocated memory.
 *
 * @param token Token to be deinitialized.
 */
void token_deinit(Token* token);

#endif  // !LEXER_H
