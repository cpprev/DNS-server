cmake_minimum_required(VERSION 3.10)

project(client VERSION 1.0 LANGUAGES C)

include_directories(src)
include_directories(../src)

set(SRC
    ### Client code
    src/connect.c

    ### Server code
    # Config
    ../src/config/server_config.c
    ../src/config/zone_array.c
    ../src/config/zone.c
    ../src/config/record_array.c
    ../src/config/record.c
    ../src/config/record_type.c
    ../src/config/class_type.c

    # Utils
    ../src/utils/utils.c
    ../src/utils/file.c
    ../src/utils/error.c
    ../src/utils/string.c

    # Parser
    ../src/parser/input.c
    ../src/parser/parse_zone.c
)

set(CHECK_CFLAGS -fsanitize=address -g)

add_compile_options(${CHECK_CFLAGS} -Wall -Wextra -Werror -pedantic -std=c99)

add_executable(client src/main.c ${SRC})

target_link_libraries(client asan pthread)
