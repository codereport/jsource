
add_subdirectory(adverbs)
add_subdirectory(conjunctions)
#add_subdirectory(verbs)
add_library(j)
target_compile_definitions(j PRIVATE C_NA=0 C_AVX=1 C_AVX2=1 EMU_AVX=1)
target_link_libraries(j PRIVATE ${STANDARD_MATH_LIBRARY}
        lib_adverbs
        lib_conjunctions
        lib_verbs
        )
set_source_files_properties(aes-ni.c PROPERTIES COMPILE_FLAGS -maes)
target_sources(j PUBLIC
        aes-arm_table.h
        aes-c.c
        aes-c.h
        aes-ni.c
        cip_t.h
        cipfloatmm_t.h
        cpuinfo.c
        cpuinfo.h
        cr_t.h # templates
        d.c
        d.h
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
        gemm.h
        i.c
        io.c
        j.c
        j.h
        ja.h
        jc.h
        jdlllic.c
        je.h
        jerr.h
        jfex.h
        js.h
        jt.h
        jtype.h
        jversion.h
        k.c
        linenoise.h
        m.c
        m.h
        p.c
        p.h
        pv.c
        px.c
        r.c
        result.h
        rl.c
        rt.c
        s.c
        s.h
        sc.c
        sl.c
        sn.c
        t.c
        u.c # Interpreter Utilities
        #vu.c # Unicode (2-byte unsigned characters) Not included anywhere?
        vx.h # Extended Precision
        vz.h # Complex Numbers
        w.c
        w.h
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

target_sources(j PRIVATE
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
target_link_libraries(jconsole PRIVATE ${CMAKE_DL_LIBS})
target_sources(jconsole PRIVATE
        jconsole.c
        jeload.h
        jeload.cpp
        jlib.h
        )
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/jlibrary/bin" J_BINPATH)
file(TO_NATIVE_PATH "/profile.ijs" J_PROFILE_SCRIPT)
file(GENERATE
        OUTPUT "$<${is_multi_config}:$<CONFIG>/>profile.ijs"
        CONTENT "NB. loaded under debug
BINPATH_z_=: '${J_BINPATH}'
0!:0<BINPATH,'${J_PROFILE_SCRIPT}'"
        )
