# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-src")
  file(MAKE_DIRECTORY "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-src")
endif()
file(MAKE_DIRECTORY
  "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-build"
  "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-subbuild/safe_math-populate-prefix"
  "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-subbuild/safe_math-populate-prefix/tmp"
  "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-subbuild/safe_math-populate-prefix/src/safe_math-populate-stamp"
  "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-subbuild/safe_math-populate-prefix/src"
  "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-subbuild/safe_math-populate-prefix/src/safe_math-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-subbuild/safe_math-populate-prefix/src/safe_math-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-subbuild/safe_math-populate-prefix/src/safe_math-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
