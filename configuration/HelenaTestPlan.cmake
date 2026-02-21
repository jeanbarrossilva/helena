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

function(link_testing_library)
  find_package(PkgConfig)
  if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR
            "pkg-config is required for retrieving test dependencies but is not
            installed."
    )
    return()
  endif()
  pkg_check_modules(CHECK REQUIRED check)
  include_directories(${CHECK_INCLUDE_DIRS})
  link_directories(${CHECK_LIBRARY_DIRS})
  add_definitions(${CHECK_CFLAGS_OTHER})
endfunction()

link_testing_library()
