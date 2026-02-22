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

#ifndef ARGPARSER_H
#define ARGPARSER_H

/**
 * Configuration which modifies the behavior of the caller program. They are
 * given as immediate arguments to the caller program, and may be specified with
 * either their long name (prefixed by two dashes, "--") or their short name
 * (prefixed by a single dash, "-"); they may, also, be followed by an argument.
 */
struct Option {
  /**
   * Descriptive name of this option to be preceded by two dashes ("--") when
   * given in the call to the caller program. By definition, this name should
   * contain more characters than the short one.
   */
  const char *long_name;

  /**
   * Abbreviation of the long name of this option to be preceded by a single
   * dash ("-") in the call to the caller program. By definition, should be
   * shorter than the long name.
   */
  const char short_name;

  /**
   * Detailed explanation of all or most relevant consequences of specifying
   * this option in the call to the caller program.
   */
  const char *documentation;
} typedef Option;

/**
 * Operation pertaining to the caller program which may act as another program.
 * Different from options, subcommands have a single, long name describing what
 * they do, and options specific to them may be acceptable by them.
 */
struct Subcommand {
  /**
   * Characters of the argument to be given after the options to the caller
   * program (if any); usually, describes what this subcommand does as a verb
   * (e.g., "build").
   */
  const char *name;

  /** Detailed explanation of what this subcommand does. */
  const char *documentation;
} typedef Subcommand;

/** Characteristics of a program using argparser. */
struct Description {
  /** Name of the caller program. */
  const char *name;

  /** Detailed explanation of the purpose of the caller program. */
  const char *overview;

  /** Amount of options. */
  int option_count;

  /** Options acceptable by the caller program. */
  const Option *options;

  /** Amount of subcommands. */
  int subcommand_count;

  /** Subcommands available from the caller program. */
  const Subcommand *subcommands;
} typedef Description;

/**
 * Result of having requested argparser to attempt a default execution of the
 * caller program.
 */
enum DefaultExecutionStatus {
  /**
   * Default execution was performed. In this case, it may be that the caller
   * program should exit with code zero immediately.
   */
  EXECUTED,

  /**
   * Default execution was not performed because the caller program was not
   * described to argparser. Call the description function before the default
   * execution one.
   */
  UNDESCRIBED,

  /**
   * Default execution was not performed: the call to the caller program did not
   * match a common Unix program command-line call. The call is either one
   * specific to the program or illegal.
   */
  NONE,
} typedef DefaultExecutionStatus;

/**
 * Provides a description of the caller program to argparser, which will be used
 * to display information on the usage of the program in case the program is
 * executed with a help option.
 *
 *
 * If this function gets called by the caller program, it should be the first of
 * the argparser functions to be called by the caller program.
 *
 * @param name Name of the caller program.
 * @param overview Detailed explanation of the purpose of the caller program.
 * @param option_count Amount of options.
 * @param options Options acceptable by the caller program.
 * @param subcommand_count Amount of subcommands.
 * @param subcommands Subcommands available from the caller program.
 */
void describe(const char *name,
              const char *overview,
              int option_count,
              const Option *options,
              int subcommand_count,
              const Subcommand *subcommands);

/**
 * Retrieves the immediate subcommand specified in the call to the caller
 * program.
 *
 * A subcommand is an argument after the name of the program (i.e., after
 * argv[0]) which is neither an option nor an argument to an option. A
 * subcommand is only considered to be part of the caller program if it is
 * immediate; that is, it is the first argument after the name of the program or
 * after the last option passed into the program.
 *
 * @param argc Amount of arguments in argv.
 * @param argv Arguments passed into the caller program.
 * @return
 */
const char *subcommand(int argc, char **argv);

/**
 * Executes the given call to the caller program in case it is given a
 * predefined argument, common to most Unix command-line programs.
 *
 * @returns True in case a default argument was passed in; or, if none was,
 * false. Calling this function without having configured the caller program may
 * result in a failure.
 */
DefaultExecutionStatus execute_default(int argc, char **argv);

#endif
