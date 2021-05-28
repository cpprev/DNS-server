cmake_minimum_required(VERSION 3.10)

project(client VERSION 1.0 LANGUAGES C)

include_directories(src)

set(SRC

)

set(CHECK_CFLAGS -fsanitize=address -g)

add_compile_options(${CHECK_CFLAGS} -Wall -Wextra -Werror -pedantic -std=c99)

add_executable(client src/main.c ${SRC})

target_link_libraries(client asan pthread)
