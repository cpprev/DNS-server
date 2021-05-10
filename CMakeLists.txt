cmake_minimum_required(VERSION 3.10)

project(dns VERSION 1.0 LANGUAGES C)

include_directories(src)

set(SRC

)

add_compile_options(-Wall -Wextra -Werror -pedantic -std=c99)

add_executable(dns src/main.c ${SRC})
