#if these were Header Only you don't need to include the filenames just
#add_library(adverbs INTERFACE)

# https://code.jsoftware.com/wiki/Vocabulary/Glossary#Adverb
# An adverb modifies the word or phrase to its left to produce a derived entity which is usually a verb
# (though it may be any of the four primary parts of speech). More...

add_library(lib_adverbs STATIC
        a.c
        #a.h
        ab.c
        af.c
        ai.c
        am.c
        am1.c
        amn.c
        ao.c
        ap.c
        ar.c
        ar.h
        as.c
        au.c
        )
target_include_directories(lib_adverbs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lib_adverbs lib_j_parent)