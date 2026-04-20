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
  #include <argparser/owned_array.h>
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cstdlib>
#include <doctest.hpp>

using namespace std;

TEST_CASE("Initializes") {
  OwnedArray *array = (OwnedArray *) malloc(sizeof(OwnedArray));
  owned_array_init(array, sizeof(int));
  CHECK(array->head == NULL);
  CHECK(array->capacity == 0);
  CHECK(array->element_size == sizeof(int));
  CHECK(array->count == 0);
}

TEST_CASE("Appends") {
  OwnedArray *array = (OwnedArray *) malloc(sizeof(OwnedArray));
  int element = 2;
  owned_array_init(array, sizeof(int));
  owned_array_append(array, &element);
  CHECK(((int *) array->head)[0] == 2);
}

TEST_CASE("Copies") {
  OwnedArray *array = (OwnedArray *) malloc(sizeof(OwnedArray));
  int element = 2;
  owned_array_init(array, sizeof(int));
  owned_array_append(array, &element);
  owned_array_copy(array, 0, element);
  CHECK(element == 2);
  CHECK(((int *) array->head)[0] == 2);
}
