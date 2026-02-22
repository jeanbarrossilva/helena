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

#ifndef HX_STRINGS_H
#define HX_STRINGS_H

#include <stdio.h>

/**
 * Allows for explicitly specifying whether truncation (i.e., copying less
 * characters than those contained in a source string due to lack of space onto
 * a destination string) is allowed, mitigating cases of unintended truncation
 * often caused by calls to strcat() or concatenations with an insufficient size
 * to fill the destination by calls to strncat().
 */
enum ConcatenationStrategy {
  /**
   * Concatenating the destination and the source must result in the destination
   * being completely filled, with one and only null byte, by which it is
   * terminated; otherwise, the program gets aborted.
   */
  FILL,

  /**
   * The source may be appended to the destination partially, in case the size
   * of the destination cannot fit the source completely. Upon truncation, the
   * destination always ends up with a null terminator.
   */
  TRUNCATE,

  /**
   * Concatenating the source to the destination may result in the destination
   * storing fewer characters than it can.
   *
   * While this strategy prohibits truncation, it is similar to calling strcat()
   * in that the destination not being completely filled may make it prone to a
   * buffer overflow attack; see the security implications described in the
   * documentation of hx_strcat().
   *
   * When employing this strategy, ensure that the destination is later filled,
   * be it by posterior concatenations or other methods. In case no other
   * operations that fill it are intended to take place, prefer using FILL
   * instead.
   */
  SEQUENTIAL
} typedef ConcatenationStrategy;

/**
 * Appends one string to another.
 *
 * The functions provided by C in string.h for concatenating strings are not
 * inherently safe, as they are susceptible to buffer overflow attacks (Miller,
 * T. C., & de Raadt, T., 1999). These functions, strcat() and its length-based
 * variant, strncat(), are unsafe in their own right.
 *
 * strcat() removes the terminator from the destination (if it is
 * null-terminated) and appends the source onto such destination, which may end
 * up being null-terminated. This is the simplest approach to append one string
 * to another, but two issues arise:
 *
 * 1. Memory allocated for the destination may be greater than the sum of its
 * length, that of the source being appended and that of the terminator. In case
 * this is not taken care of later, an attacker may write to the remaining
 * bytes, injecting code and/or causing an overflow.
 * 2. The source may not fit into the destination. In this scenario, the
 * resulting string will be truncated, without the API giving any indication
 * that truncation occurred; furthermore, the string may not be null-terminated,
 * as there may not be enough memory in the destination for the terminator.
 *
 * strncat(), then, was later introduced as an alternative which tackles some of
 * these issues: the size of the portion of the source to be appended to the
 * destination is required to be specified by the caller. Upon a call to
 * strncat(), the source is truncated in order for it and the terminator to fit
 * into the destination in case it cannot contain the source completely.
 * The destination, however, will still end up with unused bytes if appending
 * the source and the terminator to it results in its length being lesser than
 * that of the destination + that of the source + that of the terminator.
 *
 * This Helena extension function works around these vulnerabilities by
 * requiring that a concatenation strategy be specified by the caller. Whether
 * truncation is allowed must be made explicit and, if it were to occur while
 * having been disallowed, the call to this function results in the program
 * being aborted.
 *
 * References:
 *
 * - Miller, T. C., & de Raadt, T. (1999). strlcpy and strlcat: Consistent,
 * Safe, String Copy and Concatenation. Proceedings of the FREENIX Track: 1999
 * USENIX Annual Technical Conference.
 * https://www.usenix.org/conference/1999-usenix-annual-technical-conference/strlcpy-and-strlcat-consistent-safe-string-copy
 *
 * @param destination String to which the source may be appended.
 * @param destination_size Total amount of bytes allocated for the destination.
 * Note that this is not necessarily equal to its length, as the destination may
 * contain fewer characters than it can store — specially when concatenating.
 * This may be equal to sizeof(destination).
 * @param source String to be appended to the destination.
 * @param strategy Determines whether the concatenation must result in the
 * destination being exactly filled; or if truncation is allowed; etc.
 */
size_t hx_strcat(char *destination,
                 size_t destination_size,
                 const char *source,
                 ConcatenationStrategy strategy);

#endif
