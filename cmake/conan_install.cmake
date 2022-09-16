# SPDX-License-Identifier: MIT Copyright (c) 2022 Jai Bellare See
# <https://opensource.org/licenses/MIT/> or LICENSE.md Project homepage:
# <https://github.com/strangeQuark1041/samarium>

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conan.lock AND RUN_CONAN)
    find_program(CONAN_EXE conan REQUIRED)

    set(DEPS_OPTION "")

    if(BUILD_UNIT_TESTS OR BUILD_BENCHMARKS)
        set(DEPS_OPTION "-o samarium:build_tests=True")
    endif()

    message(STATUS "Installing dependencies ... (this may take a few seconds)")
    execute_process(
        COMMAND
            ${CONAN_EXE} install . -b missing -if ${CMAKE_CURRENT_BINARY_DIR}
            -pr:b=cmake/clang_${CMAKE_BUILD_TYPE}.profile
            -pr=cmake/clang_${CMAKE_BUILD_TYPE}.profile ${DEPS_OPTION}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_QUIET
    )
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
else()
    message(STATUS "Dependencies already installed")
endif()
