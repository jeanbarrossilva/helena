#[[
Copyright © 2026 Jean Silva

This file is part of the Helena programming language open-source project.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

                   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
]]

cmake_minimum_required(VERSION 4.0.2)
project(Helena LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/configuration)
add_subdirectory(lib/argparser)
add_subdirectory(lib/hx)
add_subdirectory(lib/lexer)
add_executable(helena helena.c)
target_link_libraries(helena PRIVATE argparser hx)

option(IS_TESTING_ENABLED "Enable testing?" ON)
if(IS_TESTING_ENABLED)
  enable_testing()
endif()
