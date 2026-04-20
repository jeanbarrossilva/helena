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

# Performs configuration common to all libraries of Helena. One of the steps
# involved is defining the directories included into the specified target; these
# directories contain dependencies belonging to Deus and external ones.
function(set_up_library target_name)
  target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/include)
endfunction()
