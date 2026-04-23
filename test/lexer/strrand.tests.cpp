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
#include <strrand.h>
}

#include <doctest.hpp>

TEST_CASE("Generates random string") {
  char* str1 = (char*)malloc(65 * sizeof(char));
  strrand(str1, 64);
  char* str2 = (char*)malloc(65 * sizeof(char));
  strrand(str2, 64);
  CHECK(strcmp(str1, str2));
}
