
add_library(j)
target_include_directories(j PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(j PRIVATE)
target_link_libraries(j PRIVATE ${STANDARD_MATH_LIBRARY})
target_sources(j PRIVATE
        adverbs/a.c
        adverbs/ab.c
        adverbs/af.c
        adverbs/ai.c
        adverbs/am.c
        adverbs/am1.c
        adverbs/amn.c
        adverbs/ao.c
        adverbs/ap.c
        adverbs/ar.c
        adverbs/as.c
        adverbs/au.c
        conjunctions/c.c
        conjunctions/ca.c
        conjunctions/cc.c
        conjunctions/cf.c
        conjunctions/cg.c
        conjunctions/ch.c
        conjunctions/cip.c
        conjunctions/cl.c
        conjunctions/cp.c
        conjunctions/cpdtsp.c
        conjunctions/cr.c
        conjunctions/crs.c
        conjunctions/cu.c
        conjunctions/cv.c
        conjunctions/cx.c
        verbs/crc32c.c
        verbs/v.c
        verbs/v0.c
        verbs/v1.c
        verbs/v2.c
        verbs/va1.c
        verbs/va1ss.c
        verbs/va2.c
        verbs/va2s.c
        verbs/va2ss.c
        verbs/vamultsp.c
        verbs/vb.c
        verbs/vbang.c
        verbs/vcant.c
        verbs/vcat.c
        verbs/vcatsp.c
        verbs/vchar.c
        verbs/vcomp.c
        verbs/vcompsc.c
        verbs/vd.c
        verbs/ve.c
        verbs/vf.c
        verbs/vfft.c
        verbs/vfrom.c
        verbs/vfromsp.c
        verbs/vg.c
        verbs/vgauss.c
        verbs/vgcomp.c
        verbs/vgranking.c
        verbs/vgsort.c
        verbs/vgsp.c
        verbs/vi.c
        verbs/viix.c
        verbs/visp.c
        verbs/vm.c
        verbs/vo.c
        verbs/vp.c
        verbs/vq.c
        verbs/vrand.c
        verbs/vrep.c
        verbs/vs.c
        verbs/vsb.c
        verbs/vt.c
        verbs/vx.c
        verbs/vz.c
        cpuinfo.c
        debugging/d.c
        debugging/dc.c
        debugging/dss.c
        debugging/dstop.c
        debugging/dsusp.c
        dtoa.c
        format/f.c
        format/f2.c
        format/fbu.c
        gemm.c
        i.c
        io.c
        j.c
        jdlllic.c
        k.c
        m.c
        parsing/p.c
        parsing/pv.c
        px.c
        representations/r.c
        representations/rl.c
        representations/rt.c
        s.c
        sc.c
        sl.c
        sn.c
        t.c
        u.c
        vu.c
        words/w.c
        words/wc.c
        words/wn.c
        words/ws.c
        xenos/x.c
        xenos/x15.c
        xenos/xa.c
        xenos/xb.c
        xenos/xc.c
        xenos/xcrc.c
        xenos/xd.c
        xenos/xf.c
        xenos/xfmt.c
        xenos/xh.c
        xenos/xi.c
        xenos/xl.c
        xenos/xo.c
        xenos/xs.c
        xenos/xt.c
        xenos/xu.c
        )

# configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/ COPYONLY)

if (OpenMP_C_FOUND)
    target_link_libraries(j PRIVATE OpenMP::OpenMP_C)

    if (DEFINED OpenMP_libomp_DLL)
        add_custom_command(
                TARGET j POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${OpenMP_libomp_DLL}
                "${CMAKE_CURRENT_BINARY_DIR}$<${is_multi_config}:/$<CONFIG>>"
        )
    endif ()
endif ()

target_sources(j PRIVATE

        blis/gemm_vec-ref.c
        )

if (NOT BUILD_SHARED_LIBS)
    return()
endif ()

add_library(linenoise OBJECT EXCLUDE_FROM_ALL)
target_compile_definitions(linenoise INTERFACE USE_LINENOISE)
target_sources(linenoise PRIVATE

        linenoise.c
        )

add_executable(jconsole)
target_compile_definitions(jconsole PRIVATE READLINE)
if (NOT UNIX OR ${CMAKE_SYSTEM_PROCESSOR} MATCHES "^(aarch64|arm)")
    target_link_libraries(jconsole PRIVATE linenoise)
endif ()
if ("${CMAKE_C_COMPILER_FRONTEND_VARIANT}" STREQUAL "MSVC")
    target_link_options(jconsole PRIVATE /STACK:0x1000000)
endif ()
target_link_libraries(jconsole PRIVATE ${CMAKE_DL_LIBS})
target_sources(jconsole PRIVATE
        jconsole.c
        jeload.cpp
        util/string.cpp
        util/string.hpp
        )
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/jlibrary/bin" J_BINPATH)
file(TO_NATIVE_PATH "/profile.ijs" J_PROFILE_SCRIPT)
file(GENERATE
        OUTPUT "$<${is_multi_config}:$<CONFIG>/>profile.ijs"
        CONTENT "NB. loaded under debug
BINPATH_z_=: '${J_BINPATH}'
0!:0<BINPATH,'${J_PROFILE_SCRIPT}'"
        )
