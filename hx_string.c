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

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "hx_string.h"

size_t hx_strcat(char *destination,
                 const size_t destination_size,
                 const char *source,
                 const ConcatenationStrategy strategy) {
  const size_t source_length = strlen(source);
  if (source_length == 0) return 0;
  size_t destination_length;
  const size_t source_size = source_length + sizeof(char);
  if (strategy != TRUNCATE && destination_size < (
        destination_length = strlen(destination)) + source_size) {
    fprintf(stderr,
            "hx_strcat(): \"%s\" does not fit into the %lu byte(s) allocated for \"%s\" (off by %lu byte(s)).",
            source,
            source_size,
            destination,
            destination_size - destination_length + source_length);
    exit(ENAMETOOLONG);
  }
  if (strategy == SEQUENTIAL) {
    return strncat(destination, source, destination_size) - destination;
  }
#if defined(__OpenBSD__) || defined(__APPLE__)
  return strlcat(destination, source, destination_size);
#else
  destination_length = strlen(destination);
  int concatenation_index = destination_length + sizeof(char);
  strcpy(&destination[concatenation_index], source);
  return destination_length + source_length + sizeof(char);
#endif
}
