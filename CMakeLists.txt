cmake_minimum_required(VERSION 3.25)
project(compilers-coursework)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER "gcc")

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(
        MPARSER
        src/lang.y
        ${CMAKE_CURRENT_BINARY_DIR}/gen/lang.tab.c)
FLEX_TARGET(MLEXER
        src/lang.l
        ${CMAKE_CURRENT_BINARY_DIR}/gen/lex.yy.c
        )
ADD_FLEX_BISON_DEPENDENCY(MLEXER MPARSER)

add_executable(compilers-coursework src/main.c src/ast.c src/vector.c src/ir.c ${BISON_MPARSER_OUTPUTS} ${FLEX_MLEXER_OUTPUTS})

target_include_directories(compilers-coursework PRIVATE src)
target_include_directories(compilers-coursework PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/gen)
target_link_libraries(compilers-coursework ${FLEX_LIBRARIES})
