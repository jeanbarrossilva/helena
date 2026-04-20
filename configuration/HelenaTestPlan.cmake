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

include(HelenaLibrary)

# Performs configuration common to every testing library of Helena, including
# both internal and external dependencies for production code; and ones specific
# for testing, such as the doctest framework.
function(set_up_testing target_name)
  set_up_library(${target_name})
  target_include_directories(${target_name} PRIVATE
    ${CMAKE_SOURCE_DIR}/external/testing)
endfunction()
