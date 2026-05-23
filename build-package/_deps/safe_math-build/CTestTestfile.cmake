# CMake generated Testfile for 
# Source directory: /home/nikita/repos/calc_cli/build-package/_deps/safe_math-src
# Build directory: /home/nikita/repos/calc_cli/build-package/_deps/safe_math-build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[SafeMathTests]=] "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-build/SafeMathTests")
set_tests_properties([=[SafeMathTests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/nikita/repos/calc_cli/build-package/_deps/safe_math-src/CMakeLists.txt;45;add_test;/home/nikita/repos/calc_cli/build-package/_deps/safe_math-src/CMakeLists.txt;0;")
subdirs("../googletest-build")
