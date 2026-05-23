# CMake generated Testfile for 
# Source directory: /home/nikita/repos/calc_cli
# Build directory: /home/nikita/repos/calc_cli/build-package
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[calc_cli_tests]=] "/home/nikita/repos/calc_cli/build-package/calc_cli_tests")
set_tests_properties([=[calc_cli_tests]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/nikita/repos/calc_cli/CMakeLists.txt;148;add_test;/home/nikita/repos/calc_cli/CMakeLists.txt;0;")
add_test([=[calc_cli_tests_valgrind]=] "/usr/bin/valgrind" "--leak-check=full" "--show-leak-kinds=all" "--track-origins=yes" "--errors-for-leak-kinds=definite,indirect" "--error-exitcode=1" "/home/nikita/repos/calc_cli/build-package/calc_cli_tests")
set_tests_properties([=[calc_cli_tests_valgrind]=] PROPERTIES  LABELS "valgrind" TIMEOUT "60" _BACKTRACE_TRIPLES "/home/nikita/repos/calc_cli/CMakeLists.txt;155;add_test;/home/nikita/repos/calc_cli/CMakeLists.txt;0;")
subdirs("_deps/safe_math-build")
subdirs("_deps/nlohmann_json-build")
subdirs("_deps/spdlog-build")
