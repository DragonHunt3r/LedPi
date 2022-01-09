cmake_minimum_required(VERSION 3.16.3)

# Project name and version
project(LEDPI VERSION 1.1.0)

# Apparantly this is not ideal
# I do not know the proper way to do this however
configure_file("src/version.h.in" "${PROJECT_BINARY_DIR}/version.h")
include_directories(${PROJECT_BINARY_DIR})

# Debug flag for assertions
option(DEBUG "If assertions should be enabled" OFF)

# Effect file
# This source file should implement the effect.h header
set(EFFECT "src/effects/no_effect.c" CACHE FILEPATH "The effect to compile")

# Compiler flags
set(FLAGS_GNUCC -Wall -Wextra)
set(FLAGS_MSVC /W4)

# Executable
add_executable(LEDPI
    "src/main.c"
    "src/color.h"
    "src/color.c"
    "src/coords.h"
    "src/coords.c"
    "src/effects/effect.h"
    ${EFFECT}
    "src/leds.h"
    "src/leds.c"
    "src/random.h"
    "src/random.c"
    "src/vector.h"
    "src/vector.c"
)

# Libraries
target_link_libraries(LEDPI PRIVATE m)
target_link_libraries(LEDPI PRIVATE ws2811)

# Compiler flags
if (NOT DEBUG)
    set(FLAGS_GNUCC ${FLAGS_GNUCC} -DNDEBUG)
    set(FLAGS_MSVC ${FLAGS_MSVC} /DNDEBUG)
endif()
if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(LEDPI PRIVATE ${FLAGS_GNUCC})
endif()
if (MSVC)
    target_compile_options(LEDPI PRIVATE ${FLAGS_MSVC})
endif()
