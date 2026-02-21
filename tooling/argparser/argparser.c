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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"
#include "owned_array.h"

static OwnedArray *descriptions = NULL;
static const int default_option_count = 1;
static const Option default_options[] = {
  {"help", 'h', "Provide assistance on how to use the program."}
};

/**
 * Initializes a region of memory to a description.
 *
 * @param description Pointer to the memory allocated for the description.
 * @param name Name of the caller program.
 * @param overview Detailed explanation of the purpose of the caller program.
 * @param option_count Amount of options.
 * @param options Options acceptable by the caller program.
 * @param subcommand_count Amount of subcommands.
 * @param subcommands Subcommands available from the caller program.
 */
static void description_init(Description *description,
                             const char *name,
                             const char *overview,
                             const int option_count,
                             const Option *options,
                             const int subcommand_count,
                             const Subcommand *subcommands) {
  if (description == NULL) return;
  description->name = name;
  description->overview = overview;
  description->option_count = option_count;
  description->options = options;
  description->subcommand_count = subcommand_count;
  description->subcommands = subcommands;
}

/**
 * Converts an option from argparser into an option of the GNU extension for
 * getopt.
 *
 * @param option argparser option to be converted into a GNU option.
 */
static struct option option_to_gnu(const Option option) {
  const struct option gnu_option = {
    option.long_name,
    no_argument,
    NULL,
    option.short_name
  };
  return gnu_option;
}

static void help(const Description description) {
  printf("OVERVIEW: %s\nUSAGE: %s", description.overview, description.name);
}

static void run_default_for_option_at_index(const Description description,
                                            const int option_index) {
  switch (option_index) {
    case 0: help(description);
  }
}

static int argparser_getopt(const int argc,
                            char * const *argv,
                            const int option_count,
                            const Option *options) {
  struct option *gnu_options = malloc(sizeof(struct option) * option_count);
  int *flag = NULL;
  char *optstring = NULL;
  for (int index = 0; index < option_count; index++) {
    Option argparser_option = options[index];
    gnu_options[index] = option_to_gnu(argparser_option);
    if (optstring == NULL) {
      optstring = strcpy(optstring, &argparser_option.short_name);
    } else {
      strcat(optstring, &argparser_option.short_name);
    }
  }
  free(gnu_options);
  return getopt_long(argc, argv, optstring, gnu_options, flag);
}

/**
 * Removes leading whitespaces from the given string.
 *
 * @param string String whose start will be trimmed.
 */
static void trim_start(char *string) {
  char *buffer = NULL;
  int buffer_length = 0;
  int buffer_capacity = 0;
  const int string_length = strlen(string);
  for (int index = 0; index < string_length; index++) {
    const char character = string[index];
    if (buffer == NULL && character == ' ') continue;
    if (buffer == NULL) {
      buffer_capacity = sizeof(char) * (string_length - index + 1);
      buffer = malloc(buffer_capacity);
    }
    buffer[buffer_length++] = character;
  }
  if (buffer == NULL) return;
  buffer[buffer_length++] = 0;
  memmove(string, buffer, buffer_length);
  free(buffer);
}

/**
 * Finds the first occurrence of the given character in the string and returns
 * the index of such character.
 *
 * @param string String in which the character whose index will be returned is.
 * @param character Character whose index in the string should be returned.
 * @return The index of the character in the string; or a negative integer in
 * case it is not in the string.
 */
static int first_index(const char *string, const char character) {
  const char *character_in_string = strchr(string, character);
  if (character_in_string == NULL) return -1;
  return character_in_string - string;
}

void describe(const char *name,
              const char *overview,
              const int option_count,
              const Option *options,
              const int subcommand_count,
              const Subcommand *subcommands) {
  Description *description = malloc(sizeof(Description));
  description_init(description,
                   name,
                   overview,
                   option_count,
                   options,
                   subcommand_count,
                   subcommands);
  if (descriptions == NULL) {
    descriptions = malloc(sizeof(OwnedArray));
    owned_array_init(descriptions, sizeof(Description));
  }
  owned_array_append(descriptions, description);
}

DefaultExecutionStatus execute_default(const int argc, char **argv) {
  int flag;
  while ((flag = argparser_getopt(argc,
                                  argv,
                                  default_option_count,
                                  default_options)) != -1) {
    for (int option_index = 0; option_index < default_option_count; option_index
         ++) {
      const Option argparser_option = default_options[option_index];
      if (flag != argparser_option.short_name) continue;
      if (descriptions == NULL) return UNDESCRIBED;
      const char *program_name = argv[0];
      const Description *the_description = NULL;
      for (int description_index = 0; description_index < descriptions->count;
           description_index++) {
        Description *a_description = malloc(sizeof(Description));
        owned_array_copy(descriptions, a_description, description_index);
        if (program_name != a_description->name) continue;
        the_description = a_description;
      }
      if (the_description == NULL) return UNDESCRIBED;
      run_default_for_option_at_index(*the_description, option_index);
      return EXECUTED;
    }
  }
  return NONE;
}

const char *subcommand(const int argc, char **argv) {
  if (argc == 1) return NULL;
  int last_option_index = -1;
  for (int index = 1; index < argc; index++) {
    char *argument = argv[index];
    if (argument == NULL) continue;
    trim_start(argument);
    if (strlen(argument) == 0) continue;
    const bool is_option = first_index(argument, '-') == 0;
    if (is_option) {
      last_option_index = index;
      continue;
    }
    if (last_option_index >= 0 && index == last_option_index + 1) {
      last_option_index = -1;
      continue;
    }
    return argument;
  }
  return NULL;
}
