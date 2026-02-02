
cmake_minimum_required(VERSION 3.15)

Project(nb_np VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

### first making a target for main.cpp so we can run tests and confirm things work ###
# add_executable(main src/main.cpp src/math_core.cpp)
# target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# find tools
find_package(Python 3.8 COMPONENTS Interpreter Development REQUIRED)
# note (apparently):
# For embedding, you technically need Development.Embed, but 
# nanobind usually handles linking correctly with the standard find.

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_VARIABLE NB_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

# define c++ library
add_library(math_core_lib STATIC src/math_core.cpp)
target_include_directories(math_core_lib PUBLIC include)

# main cpp test executable
add_executable(main src/main.cpp)
# link the library to main test executable since it uses it
target_link_libraries(main PRIVATE math_core_lib)

# Python extension module
nanobind_add_module(my_math src/bindings.cpp)
target_link_libraries(my_math PRIVATE math_core_lib)

###### FOR EMBEDDING EXAMPLE ######
# for creating executables with nanobind, we will compile source code into the executable itself
set(NB_INC_PATH "${NB_DIR}/include")
set(NB_SRC_FILE "${NB_DIR}/src/nb_combined.cpp")

# define Nanobind as a proper static library
# this forces CMake to compile nb_combined.cpp once, with the right flags.
add_library(nanobind_lib STATIC "${NB_SRC_FILE}")
target_include_directories(nanobind_lib PUBLIC 
    "${NB_INC_PATH}"
    "/opt/homebrew/include")

# Nanobind itself needs to know about Python
target_link_libraries(nanobind_lib PUBLIC Python::Python)

# embedding example executable
add_executable(cpp_host src/embed_main.cpp "${NB_SRC_PATH}")
target_link_libraries(cpp_host PRIVATE 
    Python::Python 
    nanobind_lib)