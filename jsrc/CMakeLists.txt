
add_library(j)
target_include_directories(j PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(j PRIVATE C_NA=0 C_AVX=1 EMU_AVX=1)
target_link_libraries(j PRIVATE ${STANDARD_MATH_LIBRARY})
set_source_files_properties(aes-ni.c PROPERTIES COMPILE_FLAGS -maes)
target_sources(j PRIVATE
        a.c
        ab.c
        aes-c.c
        aes-ni.c
        af.c
        ai.c
        am.c
        am1.c
        amn.c
        ao.c
        ap.c
        ar.c
        as.c
        au.c
        c.c
        ca.c
        cc.c
        cf.c
        cg.c
        ch.c
        cip.c
        cl.c
        cp.c
        cpdtsp.c
        cpuinfo.c
        cr.c
        crs.c
        cu.c
        cv.c
        cx.c
        d.c
        dc.c
        dss.c
        dstop.c
        dsusp.c
        dtoa.c
        f.c
        f2.c
        fbu.c
        gemm.c
        i.c
        io.c
        j.c
        jdlllic.c
        k.c
        m.c
        p.c
        pv.c
        px.c
        r.c
        rl.c
        rt.c
        s.c
        sc.c
        sl.c
        sn.c
        t.c
        u.c
        v.c
        v0.c
        v1.c
        v2.c
        va1.c
        va1ss.c
        va2.c
        va2s.c
        va2ss.c
        vamultsp.c
        vb.c
        vbang.c
        vcant.c
        vcat.c
        vcatsp.c
        vchar.c
        vcomp.c
        vcompsc.c
        vd.c
        ve.c
        vf.c
        vfft.c
        vfrom.c
        vfromsp.c
        vg.c
        vgauss.c
        vgcomp.c
        vgranking.c
        vgsort.c
        vgsp.c
        vi.c
        viix.c
        visp.c
        vm.c
        vo.c
        vp.c
        vq.c
        vrand.c
        vrep.c
        vs.c
        vsb.c
        vt.c
        vu.c
        vx.c
        vz.c
        w.c
        wc.c
        wn.c
        ws.c
        x.c
        x15.c
        xa.c
        xaes.c
        xb.c
        xc.c
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
        crc32c.c
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

        )
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/jlibrary/bin" J_BINPATH)
file(TO_NATIVE_PATH "/profile.ijs" J_PROFILE_SCRIPT)
file(GENERATE
        OUTPUT "$<${is_multi_config}:$<CONFIG>/>profile.ijs"
        CONTENT "NB. loaded under debug
BINPATH_z_=: '${J_BINPATH}'
0!:0<BINPATH,'${J_PROFILE_SCRIPT}'"
        )
