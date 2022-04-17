cmake_minimum_required(VERSION 3.5)

project(openocdrtt LANGUAGES CXX C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
option(BUILD_WITH_ASAN "Enable ASAN" ON)

add_library(openocdrtt SHARED OpenOCDRTT/openocdrtt.c OpenOCDRTT/openocdrtt.h OpenOCDRTT/openocdrtt_config.h)
add_executable(openocdrtt_fuzz tests/fuzz/main.cpp)
add_executable(openocdrtt_test tests/unit/test.cpp)

target_include_directories(openocdrtt_fuzz PRIVATE OpenOCDRTT)
target_include_directories(openocdrtt_test PRIVATE OpenOCDRTT)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(COMPILER_FLAGS -O2 -Werror=logical-not-parentheses -pedantic
    -Wall -Wextra -Wno-comment -Wcast-align -Wcast-qual
    -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations
    -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion
    -Wstrict-overflow=2 -Wswitch-default -Wundef -Werror -Wunused
    -Wno-deprecated-register -Werror=return-type -Werror=uninitialized -funroll-loops
     -fbounds-check -Wstrict-aliasing=2)

if (BUILD_WITH_ASAN)
    message("Building with ASAN")
    set(COMPILER_FLAGS_EXTRA -fsanitize=address,leak,undefined -fsanitize-address-use-after-scope
        -fsanitize-undefined-trap-on-error)
    set(LINKER_FLAGS_EXTRA -fsanitize=address,leak,undefined)
endif()

message(${BUILD_WITH_ASAN})

target_compile_options(openocdrtt PRIVATE ${COMPILER_FLAGS} -fanalyzer)
target_compile_options(openocdrtt_fuzz PRIVATE ${COMPILER_FLAGS} -O2)
target_compile_options(openocdrtt_test PRIVATE ${COMPILER_FLAGS})

target_link_options(openocdrtt_fuzz PRIVATE ${LINKER_FLAGS_EXTRA})
target_link_options(openocdrtt_test PRIVATE ${LINKER_FLAGS_EXTRA} ${Boost_LIBRARIES})

target_link_libraries(openocdrtt_fuzz openocdrtt)
target_link_libraries(openocdrtt_test openocdrtt)
