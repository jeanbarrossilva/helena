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

#ifndef HX_LIMITS_H
#define HX_LIMITS_H

#ifndef MAX_PATH
#if __linux__
#include <linux/limits.h>
#elif _WIN32
#include <windows.h>
#elif __APPLE__
#include <sys/syslimits.h>
#define MAX_PATH PATH_MAX
#endif
#endif // MAX_PATH

#endif // HX_LIMITS_H
