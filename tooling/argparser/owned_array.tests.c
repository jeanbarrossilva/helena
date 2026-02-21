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

#include <check.h>
#include <stdlib.h>

#include "owned_array.h"

START_TEST(initializes) {
  OwnedArray *array = malloc(sizeof(OwnedArray));
  owned_array_init(array, sizeof(int));
  ck_assert(array->head == NULL);
  ck_assert(array->capacity == 0);
  ck_assert(array->element_size == sizeof(int));
  ck_assert(array->count == 0);
}
END_TEST

START_TEST(appends) {
  OwnedArray *array = malloc(sizeof(OwnedArray));
  int element = 2;
  owned_array_init(array, sizeof(int));
  owned_array_append(array, &element);
  ck_assert(((int *) array->head)[0] == 2);
}
END_TEST

START_TEST(copies) {
  OwnedArray *array = malloc(sizeof(OwnedArray));
  int element = 2;
  owned_array_init(array, sizeof(int));
  owned_array_append(array, &element);
  owned_array_copy(array, 0, element);
  ck_assert(element == 2);
  ck_assert(((int *) array->head)[0] == 2);
}
END_TEST
