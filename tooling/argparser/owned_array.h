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

#ifndef OWNED_ARRAY_H
#define OWNED_ARRAY_H

/**
 * An owned array is a dynamic (i.e., growable) collection that owns the
 * elements appended to it. Such ownership is given by appended elements being
 * moved to the array instead of copied, having the memory originally allocated
 * for them freed; therefore, posterior attempts of reading it would result in
 * undefined behavior.
 *
 * This data structure is specific to the argparser utility, and is employed
 * only to store the descriptions of caller programs. The array of descriptions
 * is indirectly written to by calls to the description function of the utility,
 * but should never be read by any other code external to the implementation of
 * argparser.
 */
struct OwnedArray {
  void *head;
  int capacity;
  int element_size;
  int count;
} typedef OwnedArray;

/**
 * Initializes a region of memory to an empty owned array.
 *
 * @param array Pointer to the memory allocated for the array.
 * @param element_size Size of an element of the array in bytes.
 */
void owned_array_init(OwnedArray *array, int element_size);

/**
 * Moves an element from its current address to that at the end of the array.
 *
 * A move denotes a transfer the ownership of the element being appended from
 * the caller of this function to the array itself; therefore, the region of
 * memory in which the element was prior to the call to this function will be
 * freed, and reading it will result in undefined behavior.
 *
 * @param array Pointer to the memory allocated to the initialized array.
 * @param element Element to be appended.
 */
void owned_array_append(OwnedArray *array, void *element);

/**
 * Copies the element at the given index in the array.
 *
 * @param array Pointer to the array whose element at the index will be copied.
 * @param destination Pointer to the variable into which the element at the
 * given index will be copied. In case the index is out of range, the variable
 * will have its value unchanged.
 * @param index Index of the element to be copied in the array.
 */
void owned_array_copy(OwnedArray *array, void *destination, int index);

#endif // OWNED_ARRAY_H
