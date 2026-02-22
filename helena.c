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

#include <argparser.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <hx/limits.h>
#include <hx/string.h>

void helena_describe() {
  const Subcommand subcommands[] = {{"build", "Builds Helena from source."}};
  describe("helena",
           "Builds the Helena language from its source or executes one of the phases of the compilation process.",
           0,
           NULL,
           1,
           subcommands);
}

static void build() {
  char directory[MAX_PATH];
  getcwd(directory, sizeof(directory));
  hx_strcat(directory, sizeof(directory), "/build", FILL);
  execl("/opt/homebrew/bin/cmake", "cmake -B %s", directory, NULL);
  execl("/opt/homebrew/bin/cmake", "cmake --build %s", directory, NULL);
}

int main(const int argc, char **argv) {
  helena_describe();
  switch (execute_default(argc, argv)) {
    case EXECUTED: return 0;
    case UNDESCRIBED: exit(1);
    case NONE: ;
  }
  if (!strcmp(subcommand(argc, argv), "build")) return 0;
  build();
  return 0;
}
