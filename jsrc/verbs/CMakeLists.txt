#if these were Header Only you don't need to include the filenames just
#add_library(lib_verbs INTERFACE)

# https://code.jsoftware.com/wiki/Vocabulary/Verbs
# Verbs are programs. Verbs operate on one or two noun operands, and produce a result which is always a noun.
# The simplest verbs are the primitives, such as +. Larger verbs are created by connecting other verbs together.

add_library(lib_verbs STATIC
        crc32c.c
        crc32c.h
        crc32ctables.h
        v.c
        v0.c
        v1.c
        v2.c
        #va.h
        va1.c
        va1ss.c
        va2.c
        va2s.c
        va2ss.c
        vamultsp.c
        #vasm.h
        vb.c
        vbang.c
        vcant.c
        vcat.c
        vcatsp.c
        vchar.c
        vcomp.c
        #vcomp.h
        vcompsc.c
        vd.c
        ve.c
        #ve.h
        vf.c
        vfft.c
        vfrom.c
        vfromsp.c
        vg.c
        vg.h
        vgauss.c
        vgcomp.c
        vgmerge.h
        vgmergemincomp.h
        vgranking.c
        vgsort.c
        vgsort.h
        vgsortq.h
        vgsp.c
        vi.c
        viix.c
        visp.c
        vm.c
        vo.c
        vp.c
        vq.c
        #vq.h
        vrand.c
        vrep.c
        vs.c
        vsb.c
        vt.c
        vx.c
        vz.c
        )
target_include_directories(lib_verbs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lib_verbs lib_j_parent)