#if these were Header Only you don't need to include the filenames just
#add_library(lib_conjunction INTERFACE)

# https://code.jsoftware.com/wiki/Vocabulary/Glossary#Conjunction
# A conjunction modifies the words or phrases to its left and right to produce a derived entity which is usually a verb,
# though it can be any of the four primary parts of speech. More...

add_library(lib_conjunction
        ca.c
        c.c
        cc.c
        cf.c
        cg.c
        ch.c
        cip.c
        cl.c
        cp.c
        cpdtsp.c
        cr.c
        crs.c
        cu.c
        cv.c
        cx.c
        )
target_include_directories(lib_conjunction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})