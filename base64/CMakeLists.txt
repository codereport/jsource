add_library(j-base64 OBJECT)
target_compile_definitions(j-base64 PRIVATE HAVE_AVX2=1)
target_sources(j-base64 PRIVATE
    include/libbase64.h
    lib/arch/generic/codec-generic.c
    lib/codecs.h
    lib/codec_choose.c
    lib/env.h
    lib/lib.c
    lib/tables/tables.c
    lib/tables/tables.h
    lib/tables/table_dec_32bit.h
    lib/tables/table_enc_12bit.h
)
