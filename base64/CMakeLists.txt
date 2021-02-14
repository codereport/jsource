add_library(j-base64 OBJECT)
target_compile_definitions(j-base64 PRIVATE)
target_sources(j-base64 PRIVATE
    include/libbase64.h
    lib/codec.c
    lib/codec.h
    lib/env.h
    lib/lib.c
    lib/tables/tables.c
    lib/tables/tables.h
)
