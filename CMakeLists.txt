cmake_minimum_required(VERSION 3.27)
project(code C)

set(CMAKE_C_STANDARD 11)

add_executable(insert
        code/insert.c
        code/defs.h
        code/reln.c
        code/tuple.c
        code/bits.c
        code/chvec.c
        code/page.c
        code/util.c
        code/hash.c
)

add_executable(create
        code/create.c
        code/defs.h
        code/reln.c
        code/tuple.c
        code/bits.c
        code/chvec.c
        code/page.c
        code/util.c
        code/hash.c
)

add_executable(select
        code/select.c
        code/query.c
        code/defs.h
        code/reln.c
        code/tuple.c
        code/bits.c
        code/chvec.c
        code/page.c
        code/util.c
        code/hash.c
)

add_executable(stats
        code/stats.c
        code/defs.h
        code/reln.c
        code/tuple.c
        code/bits.c
        code/chvec.c
        code/page.c
        code/util.c
        code/hash.c
)
