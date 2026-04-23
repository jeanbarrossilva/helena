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

include(HelenaTestPlan)

add_executable(helenaLexerTests
  configuration.cpp
  strrand.c
  strrand.tests.cpp
  token_general_checkers.tests.cpp
  token_is_id.tests.cpp)
set_up_testing(helenaLexerTests)
target_include_directories(helenaLexerTests PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(helenaLexerTests PRIVATE helenaLexer)
add_test(NAME helenaLexerTests COMMAND helenaLexerTests)
