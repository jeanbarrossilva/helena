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
#include <hx/string.h>
#include <stdlib.h>
#include <string.h>

size_t hx_strcat(char* destination,
                 const size_t destination_size,
                 const char* source,
                 const ConcatenationStrategy strategy) {
  const size_t source_length = strlen(source);
  if (source_length == 0)
    return 0;
  const size_t destination_length = strlen(destination);
  size_t concatenation_length     = destination_length + source_length;
  size_t concatenation_size       = concatenation_length + sizeof(char);
  if (destination_size < concatenation_size) {
    if (strategy == TRUNCATE) {
      concatenation_length = destination_size - sizeof(char);
      concatenation_size   = destination_size;
    } else {
      fprintf(stderr,
              "hx_strcat(): last %zu character(s) of \"%s\" do not fit into "
              "the %zu byte(s) allocated for \"%s\" (off by %zu byte(s)).\n",
              source_length, source, destination_size, destination,
              concatenation_size - destination_size);
      exit(ENAMETOOLONG);
      return 0;
    }
  }
  if (strategy != SEQUENTIAL && destination_size > concatenation_size) {
    fprintf(stderr,
            "hx_strcat(): the %zu characters of \"%s\" do not fill the %zu "
            "byte(s) allocated for \"%s\" (remaining %zu byte(s)).\n",
            source_length, source, destination_size, destination,
            destination_size - concatenation_size);
    exit(EINVAL);
    return 0;
  }
#if defined(__APPLE__) || defined(__MACH__) || defined(__OpenBSD__)
  strlcat(destination, source, destination_size);
#else
  strncat(destination, source, destination_size);
#endif
  return concatenation_length;
}
