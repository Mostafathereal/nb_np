
cmake_minimum_required(VERSION 3.15)

Project(nb_np VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

### first making a target for main.cpp so we can run tests and confirm things work ###
# add_executable(main src/main.cpp src/math_core.cpp)
# target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# find tools
find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)
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

