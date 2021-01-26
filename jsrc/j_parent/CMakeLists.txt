#if these were Header Only you don't need to include the filenames just
#add_library(j_parent INTERFACE)

# j.h is required by many files and it requires a few things. This library contains the things required.


add_library(lib_j_parent
        d.h
        j.h
        j.c
        ja.h
        jc.h
        jtype.h
        js.h
        jt.h
        jlib.h
        je.h
        jerr.h
        m.h
        p.c
        p.h
        result.h
        va.h
        vasm.h
        vcomp.h
        ve.h
        vq.h
        vx.h
        vz.h
        a.h
        s.h
        jversion.h
        gemm.h
        w.h
        w.c
        )
target_include_directories(lib_j_parent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(lib_j_parent)