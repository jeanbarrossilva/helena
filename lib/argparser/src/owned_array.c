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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/argparser/owned_array.h"

static void grow_on_overflow(OwnedArray *array, const int new_count) {
  if (new_count <= array->capacity) return;
  array->capacity = (array->capacity == 0) ? 4 : array->capacity * 2;
}

void owned_array_init(OwnedArray *array, const int element_size) {
  if (array == NULL) return;
  array->head = NULL;
  array->capacity = 0;
  array->element_size = element_size;
  array->count = 0;
}

void owned_array_append(OwnedArray *array, void *element) {
  const int new_count = array->count + 1;
  grow_on_overflow(array, new_count);
  array->head = realloc(array->head, new_count);
  char *offset = (char *) array->head + new_count * array->element_size;
  memmove(offset, element, array->element_size);
  free(element);
  array->count = new_count;
}

void owned_array_copy(OwnedArray *array, void *destination, const int index) {
  const bool is_out_of_range = array->count == 0 || index < 0 || index > array->
                               count - 1;
  if (is_out_of_range) return;
  memcpy(destination, &array->head[index], array->element_size);
}
