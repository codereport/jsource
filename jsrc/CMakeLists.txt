add_subdirectory(j_parent)
add_subdirectory(adverbs)
add_subdirectory(conjunctions)
add_subdirectory(verbs)
add_library(j)
target_compile_definitions(j PRIVATE C_NA=0 C_AVX=1 C_AVX2=1 EMU_AVX=1)
target_link_libraries(j
        PRIVATE ${STANDARD_MATH_LIBRARY}
        PUBLIC lib_j_parent
        PUBLIC lib_adverbs
        PUBLIC lib_conjunctions
        PUBLIC lib_verbs
        )
set_source_files_properties(aes-ni.c PROPERTIES COMPILE_FLAGS -maes)
target_sources(j PUBLIC
        ./adverbs/a.c
        #./adverbs/a.h
        ./adverbs/ab.c
        ./adverbs/af.c
        ./adverbs/ai.c
        ./adverbs/am.c
        ./adverbs/am1.c
        ./adverbs/amn.c
        ./adverbs/ao.c
        ./adverbs/ap.c
        ./adverbs/ar.c
        ./adverbs/ar.h
        ./adverbs/as.c
        ./adverbs/au.c
        ./conjunctions/ca.c
        ./conjunctions/c.c
        ./conjunctions/cc.c
        ./conjunctions/cf.c
        ./conjunctions/cg.c
        ./conjunctions/ch.c
        ./conjunctions/cip.c
        ./conjunctions/cip_t.h
        ./conjunctions/cl.c
        ./conjunctions/cp.c
        ./conjunctions/cpdtsp.c
        ./conjunctions/cr.c
        ./conjunctions/crs.c
        ./conjunctions/cu.c
        ./conjunctions/cv.c
        ./conjunctions/cx.c
        ./j_parent/d.h
        ./j_parent/j.h
        ./j_parent/j.c
        ./j_parent/ja.h
        ./j_parent/jc.h
        ./j_parent/jtype.h
        ./j_parent/js.h
        ./j_parent/jt.h
        ./j_parent/jlib.h
        ./j_parent/je.h
        ./j_parent/jerr.h
        ./j_parent/m.h
        ./j_parent/p.c
        ./j_parent/p.h
        ./j_parent/result.h
        ./j_parent/va.h
        ./j_parent/vasm.h
        ./j_parent/vcomp.h
        ./j_parent/ve.h
        ./j_parent/vq.h
        ./j_parent/vx.h
        ./j_parent/vz.h
        ./j_parent/a.h
        ./j_parent/s.h
        ./j_parent/jversion.h
        ./j_parent/gemm.h
        ./j_parent/w.h
        ./j_parent/w.c
        ./verbs/crc32c.c
        ./verbs/crc32c.h
        ./verbs/crc32ctables.h
        ./verbs/v.c
        ./verbs/v0.c
        ./verbs/v1.c
        ./verbs/v2.c
        #./verbs/va.h
        ./verbs/va1.c
        ./verbs/va1ss.c
        ./verbs/va2.c
        ./verbs/va2s.c
        ./verbs/va2ss.c
        ./verbs/vamultsp.c
        #./verbs/vasm.h
        ./verbs/vb.c
        ./verbs/vbang.c
        ./verbs/vcant.c
        ./verbs/vcat.c
        ./verbs/vcatsp.c
        ./verbs/vchar.c
        ./verbs/vcomp.c
        #./verbs/vcomp.h
        ./verbs/vcompsc.c
        ./verbs/vd.c
        ./verbs/ve.c
        #./verbs/ve.h
        ./verbs/vf.c
        ./verbs/vfft.c
        ./verbs/vfrom.c
        ./verbs/vfromsp.c
        ./verbs/vg.c
        ./verbs/vg.h
        ./verbs/vgauss.c
        ./verbs/vgcomp.c
        ./verbs/vgmerge.h
        ./verbs/vgmergemincomp.h
        ./verbs/vgranking.c
        ./verbs/vgsort.c
        ./verbs/vgsort.h
        ./verbs/vgsortq.h
        ./verbs/vgsp.c
        ./verbs/vi.c
        ./verbs/viix.c
        ./verbs/visp.c
        ./verbs/vm.c
        ./verbs/vo.c
        ./verbs/vp.c
        ./verbs/vq.c
        #./verbs/vq.h
        ./verbs/vrand.c
        ./verbs/vrep.c
        ./verbs/vs.c
        ./verbs/vsb.c
        ./verbs/vt.c
        ./verbs/vx.c
        ./verbs/vz.c
        aes-arm_table.h
        aes-c.c
        aes-c.h
        aes-ni.c
        #cip_t.h
        cipfloatmm_t.h
        cpuinfo.c
        cpuinfo.h
        cr_t.h # templates
        d.c
        #d.h
        dc.c
        dss.c
        dstop.c
        dsusp.c
        dtoa.c
        dtoa.h
        f.c
        f2.c
        fbu.c
        fnmatch.h
        gemm.c
        #gemm.h
        i.c
        io.c
        #ja.h
        #jc.h
        jdlllic.c
        #je.h
        #jerr.h
        jfex.h
        #js.h
        #jt.h
        #jtype.h
        #jversion.h
        k.c
        linenoise.h
        m.c
        #m.h
        #p.c
        #p.h
        pv.c
        px.c
        r.c
        #result.h
        rl.c
        rt.c
        s.c
        #s.h
        sc.c
        sl.c
        sn.c
        t.c
        u.c # Interpreter Utilities
        vu.c # Unicode (2-byte unsigned characters)
        #vx.h # Extended Precision
        #vz.h # Complex Numbers
        #w.c
        #w.h
        wc.c
        wn.c
        ws.c
        x.c
        x.h
        x15.c
        xa.c
        xaes.c
        xb.c
        xc.c
        xc.h
        xcrc.c
        xd.c
        xf.c
        xfmt.c
        xh.c
        xi.c
        xl.c
        xo.c
        xs.c
        xsha.c
        xt.c
        xu.c
        )

# configure_file(jversion-x.h ${CMAKE_CURRENT_SOURCE_DIR}/jversion.h COPYONLY)

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

target_sources(j PUBLIC
        blis.h
        blis/gemm_vec-ref.c
        )

if (NOT BUILD_SHARED_LIBS)
    return()
endif ()

add_library(linenoise OBJECT EXCLUDE_FROM_ALL)
target_compile_definitions(linenoise INTERFACE USE_LINENOISE)
target_sources(linenoise PRIVATE
        linenoise.h
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
target_link_libraries(jconsole PRIVATE ${CMAKE_DL_LIBS} j)

target_sources(jconsole PRIVATE
        jconsole.c
        jeload.h
        jeload.cpp
        #jlib.h
        )
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/jlibrary/bin" J_BINPATH)
file(TO_NATIVE_PATH "/profile.ijs" J_PROFILE_SCRIPT)
file(GENERATE
        OUTPUT "$<${is_multi_config}:$<CONFIG>/>profile.ijs"
        CONTENT "NB. loaded under debug
BINPATH_z_=: '${J_BINPATH}'
0!:0<BINPATH,'${J_PROFILE_SCRIPT}'"
        )
