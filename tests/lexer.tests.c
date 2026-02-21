/* 
 * Copyright © 2026 Jean Silva
 *
 * This file is part of the Helena programming language open-source project.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License. You may obtain a copy of the License at
 *
 *                            http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 */

#include <check.h>
#include <lexer.h>
#include <stdio.h>
#include <stdlib.h>

START_TEST(token_with_only_a_digit_is_not_an_id) {
  for (int prefix = 0; prefix <= 8; prefix++) {
    char* text;
    ck_assert(asprintf(&text, "%d", prefix));
    Token *token = malloc(sizeof(Token));
    init_token(token, 0, 0, text);
    ck_assert(!is_id(token));
    free(token);
  }
}
END_TEST

START_TEST(token_prefixed_by_a_digit_and_followed_by_letters_is_not_an_id) {
  for (int prefix = 0; prefix <= 8; prefix++) {
    char* text;
    ck_assert(asprintf(&text, "%dID", prefix));
    Token *token = malloc(sizeof(Token));
    init_token(token, 0, 0, text);
    ck_assert(!is_id(token));
    free(token);
  }
}
END_TEST

START_TEST(token_with_only_a_letter_is_an_id) {
  Token *token = malloc(sizeof(Token));
  init_token(token, 0, 0, "g");
  ck_assert(is_id(token));
  free(token);
}
END_TEST

START_TEST(token_with_only_letters_is_an_id) {
  Token *token = malloc(sizeof(Token));
  init_token(token, 0, 0, "Helena");
  ck_assert(is_id(token));
  free(token);
}
END_TEST

START_TEST(token_prefixed_by_a_letter_and_followed_by_digits_is_an_id) {
  Token *token = malloc(sizeof(Token));
  init_token(token, 0, 0, "h06");
  ck_assert(is_id(token));
  free(token);
}
END_TEST
