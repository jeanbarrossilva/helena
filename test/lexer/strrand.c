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

#include <stdlib.h>
#include <strrand.h>

/** Inclusive range of printable ASCII characters. */
static const char RANGE[2] = {32, 126};

/**
 * Generates a null-terminated random string composed by printable ASCII
 * characters.
 *
 * @param string Pointer to the string.
 * @param length Amount of characters which can be put into the string,
 * disregarding the null character '\0' at length + 1.
 */
void strrand(char* string, size_t length) {
  for (int index = 0; index < length; index++)
    string[index] = (char)(rand() % (RANGE[1] - RANGE[0])) + RANGE[0];
  string[length] = '\0';
}
