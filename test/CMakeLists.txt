if (BUILD_TESTING)
    add_library(tsdll SHARED)
    target_link_libraries(tsdll PRIVATE ${STANDARD_MATH_LIBRARY})
    target_sources(tsdll PRIVATE
            ../jsrc/tsdll.c
            )
endif ()
set(hare_test_cases #fast
        g0        # 0.170 seconds
        g000      # 0.170 seconds
        g000a     # 0.150 seconds
        g000i     # 0.160 seconds
        g000p     # 0.170 seconds
        g000s     # 0.170 seconds
        g010      # 0.160 seconds
        g010a     # 0.170 seconds
        g010i     # 0.170 seconds
        g010p     # 0.210 seconds
        g010s     # 0.220 seconds
        g011      # 0.180 seconds
        g011a     # 0.170 seconds
        g011i     # 0.180 seconds
        g011p     # 0.570 seconds
        g011s     # 0.180 seconds
        g012      # 0.160 seconds
        g012a     # 0.170 seconds
        g012i     # 0.170 seconds
        g012p     # 0.170 seconds
        g012s     # 0.200 seconds
        g020      # 0.180 seconds
        g020a     # 0.180 seconds
        g020i     # 0.170 seconds
        g020p     # 0.160 seconds
        g020s     # 0.170 seconds
        g021      # 0.160 seconds
        g021a     # 0.160 seconds
        g021i     # 0.180 seconds
        g021p     # 0.550 seconds
        g021s     # 0.190 seconds
        g022      # 0.210 seconds
        g022a     # 0.170 seconds
        g022i     # 0.160 seconds
        g022p     # 0.160 seconds
        g022s     # 0.160 seconds
        g030      # 0.170 seconds
        g030a     # 0.160 seconds
        g031      # 0.160 seconds
        g032      # 0.170 seconds
        g0a       # 0.160 seconds
        g0x       # 0.160 seconds
        g100      # 0.230 seconds
        g100a     # 0.210 seconds
        g100i     # 0.210 seconds
        g100p     # 0.170 seconds
        g100s     # 0.190 seconds
        g101      # 0.190 seconds
        g101a     # 0.170 seconds
        g101i     # 0.160 seconds
        g101p     # 0.170 seconds
        g101s     # 0.160 seconds
        g102      # 0.150 seconds
        g102a     # 0.160 seconds
        g102i     # 0.170 seconds
        g102p     # 0.170 seconds
        g102s     # 0.160 seconds
        g110      # 0.240 seconds
        g110a     # 0.160 seconds
        g110i     # 0.160 seconds
        g110p     # 0.160 seconds
        g110s     # 0.180 seconds
        g111      # 0.190 seconds
        g111a     # 0.160 seconds
        g111i     # 0.180 seconds
        g111p     # 0.160 seconds
        g111s     # 0.170 seconds
        g112      # 0.160 seconds
        g112a     # 0.160 seconds
        g112i     # 0.160 seconds
        g112p     # 0.160 seconds
        g112s     # 0.160 seconds
        g120      # 0.160 seconds
        g120a     # 0.160 seconds
        g120i     # 0.170 seconds
        g120p     # 0.170 seconds
        g120s     # 0.180 seconds
        g121      # 0.170 seconds
        g122      # 0.920 seconds
        g128x     # 0.160 seconds
        g128x3    # 0.160 seconds
        g128x5    # 0.230 seconds
        g130      # 0.160 seconds
        g130a     # 0.160 seconds
        g130i     # 0.180 seconds
        g130p     # 0.160 seconds
        g132      # 0.190 seconds
        g13x      # 0.170 seconds
        g15x      # 0.180 seconds
        g1x       # 0.170 seconds
        g1x0      # 0.350 seconds
        g1x1      # 0.240 seconds
        g1x11     # 0.180 seconds
        g1x12     # 0.180 seconds
        g1x2      # 0.170 seconds 95 - hare_g1x2 (Failed) rerunning test
        g1x20     # 0.170 seconds
        g1x3      # 0.180 seconds 97 - hare_g1x3 (Failed) rerunning test
        g1x30     # 0.170 seconds
        g1x4      # 0.240 seconds
        g1x43     # 0.160 seconds
        g1x5      # 0.170 seconds
        g1x55     # 0.170 seconds
        g1x7      # 0.170 seconds
        g200      # 0.800 seconds
        g200a     # 0.170 seconds
        g200i     # 0.160 seconds
        g200m     # 0.160 seconds
        g200p     # 0.150 seconds
        g201      # 0.150 seconds
        g202      # 0.160 seconds
        g202b     # 0.180 seconds
        g210      # 0.180 seconds
        g211      # 0.300 seconds
        g212      # 0.170 seconds
        g220      # 0.160 seconds
        g220t     # 0.210 seconds
        g221      # 0.580 seconds
        g222      # 1.940 seconds
        g222a     # 0.150 seconds
        g222i     # 0.170 seconds
        g222p     # 0.160 seconds
        g222s     # 0.160 seconds
        g230      # 0.180 seconds
        g230a     # 0.160 seconds
        g230i     # 0.160 seconds
        g230p     # 0.180 seconds
        g230s     # 0.180 seconds
        g231      # 0.160 seconds
        g232      # 2.150 seconds
        g2x       # 0.200 seconds
        g300b     # 0.170 seconds
        g300t     # 0.160 seconds
        g301      # 0.160 seconds
        g310      # 0.160 seconds
        g310a     # 0.160 seconds
        g310names # 0.150 seconds
        g310r     # 0.160 seconds
        g310t     # 0.160 seconds
        g311      # 0.160 seconds
        g312      # 0.150 seconds
        g320      # 0.170 seconds
        g320ip    # 0.320 seconds
        g321      # 0.170 seconds
        g321t     # 0.230 seconds
        g322      # 0.170 seconds
        g330      # 0.160 seconds
        g330f     # 0.210 seconds
        g330t     # 0.180 seconds
        g331bx    # 0.190 seconds
        g331col   # 0.170 seconds
        g331ins   # 0.290 seconds
        g331ps    # 0.230 seconds
        g331sp    # 0.180 seconds
        g332      # 0.160 seconds
        g332s     # 0.160 seconds
        g3x       # 0.860 seconds
        g3x4      # 0.160 seconds
        g3x5      # 0.160 seconds
        g3x6      # 0.170 seconds
        g400e     # 0.180 seconds
        g401      # 0.170 seconds
        g402      # 0.200 seconds
        g410      # 0.220 seconds
        g410a     # 0.160 seconds
        g410i     # 0.160 seconds
        g410p     # 0.170 seconds
        g410s     # 0.170 seconds
        g411      # 0.160 seconds
        g412      # 0.180 seconds
        g420ce    # 0.180 seconds
        g420fg    # 0.860 seconds
        g420r2    # 0.160 seconds
        g420stch  # 0.230 seconds
        g420t     # 1.940 seconds
        g421      # 0.390 seconds
        g421c     # 0.290 seconds
        g421d     # 0.210 seconds
        g421e     # 0.290 seconds
        g421i     # 0.570 seconds
        g421p     # 0.170 seconds
        g421t     # 0.210 seconds
        g422      # 1.310 seconds
        g422tao   # 0.150 seconds
        g430      # 0.580 seconds
        g430a2    # 0.160 seconds
        g430avg   # 0.240 seconds
        g430b     # 0.190 seconds
        g430d     # 0.170 seconds
        g430fin   # 0.300 seconds
        g430inv   # 0.170 seconds
        g431      # 1.200 seconds
        g431a     # 0.350 seconds
        g432      # 1.130 seconds
        g4x       # 0.200 seconds
        g4x5      # 0.170 seconds
        g500      # 0.160 seconds
        g502      # 0.160 seconds
        g510      # 0.150 seconds
        g520      # 2.090 seconds
        g520b     # 0.170 seconds
        g520p     # 0.410 seconds
        g521      # 0.190 seconds
        g522      # 0.160 seconds
        g530      # 1.010 seconds
        g530t     # 0.320 seconds
        g531      # 0.180 seconds
        g532      # 0.160 seconds
        g5x       # 0.150 seconds
        g5x0      # 0.160 seconds
        g5x1      # 0.160 seconds
        g5x2      # 0.190 seconds
        g5x30     # 0.160 seconds
        g5x4      # 0.170 seconds
        g5x5      # 0.180 seconds
        g5x6      # 0.160 seconds
        g5x7      # 0.160 seconds
        g601      # 1.620 seconds
        g602      # 0.170 seconds
        g610      # 0.160 seconds
        g612      # 0.150 seconds
        g620      # 0.160 seconds
        g620a     # 0.150 seconds
        g621      # 0.190 seconds
        g622      # 0.150 seconds
        g630      # 0.160 seconds
        g631c     # 0.160 seconds
        g632      # 0.160 seconds
        g640k     # 0.550 seconds
        g640r     # 0.180 seconds
        g641      # 0.150 seconds
        g6x       # 0.160 seconds
        g6x0      # 0.160 seconds
        g6x14     # 0.150 seconds
        g7x       # 0.160 seconds
        g7x5      # 0.200 seconds
        g7x6      # 0.160 seconds
        g8x       # 0.180 seconds
        g9x       # 0.160 seconds
        g9x40     # 0.160 seconds
        ga        # 0.190 seconds
        gassert   # 0.150 seconds
        gb        # 1.030 seconds
        gbpar     # 0.180 seconds
        gc        # 0.170 seconds
        gchar     # 0.150 seconds
        gct       # 0.170 seconds
        gctrl     # 0.160 seconds
        gd        # 0.160 seconds
        gddot     # 0.160 seconds
        gdll      # 1.750 seconds
        gdll_df   # 0.160 seconds
        gebar     # 1.390 seconds
        gesc      # 0.490 seconds
        gf        # 0.160 seconds
        gfor      # 0.260 seconds
        gft       # 0.320 seconds
        ggoto     # 0.180 seconds
        gh        # 0.190 seconds
        gi0       # 0.160 seconds
        gibs      # 2.120 seconds
        gibst     # 0.980 seconds
        gicap     # 0.450 seconds
        gicap2    # 0.260 seconds
        gico      # 0.410 seconds
        giconv    # 0.160 seconds
        gif       # 0.640 seconds
        gimaxmin  # 0.190 seconds
        gintdiv   # 0.360 seconds
        gintg     # 0.160 seconds
        gintovfl  # 0.280 seconds
        giph      # 1.080 seconds
        gipht     # 0.900 seconds
        giscode   # 0.160 seconds
        git       # 1.630 seconds
        gix       # 0.160 seconds
        gj        # 0.160 seconds
        glco      # 0.160 seconds
        gldot     # 0.520 seconds
        glocale   # 0.670 seconds
        gmean     # 0.430 seconds
        gmemo     # 0.300 seconds
        gmnom     # 0.160 seconds
        gn        # 0.150 seconds
        gnan      # 0.170 seconds
        gnum      # 0.160 seconds
        gnvv      # 0.150 seconds
        go        # 0.170 seconds
        goi       # 0.160 seconds
        goox      # 0.160 seconds
        gop       # 0.160 seconds
        gos       # 0.150 seconds
        gparse    # 1.560 seconds
        gpco      # 0.350 seconds
        gpco2     # 0.890 seconds
        gpcoinv   # 0.630 seconds
        gpdd      # 0.160 seconds
        gpi       # 0.150 seconds
        gpick     # 0.170 seconds
        gpoly     # 0.240 seconds
        gq        # 0.300 seconds
        gq101     # 0.490 seconds
        gq132     # 0.160 seconds
        gq201     # 0.230 seconds
        gqco      # 2.550 seconds
        gqnonrat  # 0.240 seconds
        gr        # 0.160 seconds
        gr1       # 0.160 seconds
        grefcount # 0.160 seconds
        gs        # 2.000 seconds
        gsco      # 0.180 seconds
        gsco1     # 0.980 seconds
        gsco1u    # 0.940 seconds
        gsco1w    # 0.780 seconds
        gsco2     # 0.180 seconds
        gsco2u    # 0.170 seconds
        gsco2w    # 0.170 seconds
        gscou     # 0.160 seconds
        gsdot     # 0.160 seconds
        gselect   # 0.170 seconds
        gsp       # 0.190 seconds
        gsp0      # 0.450 seconds
        gsp000    # 0.320 seconds
        gsp1      # 0.170 seconds
        gsp100    # 0.500 seconds
        gsp101    # 0.180 seconds
        gsp102    # 0.160 seconds
        gsp110    # 0.380 seconds
        gsp111    # 0.210 seconds
        gsp112    # 0.170 seconds
        gsp120    # 0.420 seconds
        gsp122    # 0.210 seconds
        gsp130    # 0.320 seconds
        gsp2      # 0.250 seconds
        gsp221    # 0.280 seconds
        gsp222    # 0.290 seconds
        gsp231    # 0.170 seconds
        gsp320    # 0.410 seconds
        gsp321    # 0.630 seconds
        gsp322    # 0.580 seconds
        gsp331    # 0.170 seconds
        gsp400    # 1.470 seconds
        gsp410    # 0.210 seconds
        gsp412    # 0.160 seconds
        gsp420    # 0.970 seconds
        gsp421    # 0.430 seconds
        gsp422    # 1.120 seconds
        gsp431    # 0.290 seconds
        gsp432    # 1.110 seconds
        gsp520    # 0.350 seconds
        gsp520sd  # 0.770 seconds
        gsp520ss  # 1.740 seconds
        gsp521    # 0.170 seconds
        gsp530i   # 0.290 seconds
        gsp530l   # 1.070 seconds
        gsp530n   # 0.800 seconds
        gsp531    # 0.190 seconds
        gsp5x5    # 0.310 seconds
        gsp6      # 0.180 seconds
        gsp600    # 0.780 seconds
        gsp600a   # 0.350 seconds
        gspi      # 0.750 seconds
        gspj      # 0.160 seconds
        gspo      # 0.170 seconds
        gspr      # 0.160 seconds
        gspx      # 0.210 seconds
        gstack    # 0.190 seconds, Exception: SegFault, Only on Windows because of reduced stack size
        gt        # 0.170 seconds
        gthrow    # 0.160 seconds
        gtrain    # 0.170 seconds
        gtry      # 0.160 seconds
        gu        # 0.350 seconds, Failed on CI before and passed after reran.
        gu0       # 0.590 seconds
        gunderai  # 0.760 seconds
        gwhile    # 0.160 seconds
        gx        # 0.150 seconds
        gx132     # 1.260 seconds
        gxco      # 0.660 seconds
        gxco1     # 0.700 seconds
        gxco2     # 0.260 seconds
        gxinf     # 0.160 seconds
        )
set(tortoise_test_cases #slow
        g001      #  4.890 seconds
        g122a     # 22.280 seconds
        g131      #  7.130 seconds
        g300      #  5.050 seconds
        g320ipt   # 33.130 seconds
        g400      #  4.090 seconds
        g422os    #  3.100 seconds
        g422rk    # 13.220 seconds
        g422sort  # 19.940 seconds
        g422sort1 #  6.870 seconds
        g600      #  5.950 seconds
        g600ip    # 11.970 seconds
        g631      # 11.720 seconds
        gcompsc   #  3.780 seconds
        ge        # 33.460 seconds
        gi        # 40.740 seconds
        gss       # 55.860 seconds
        )
set(disabled_test_cases # tests that fail on CI. Other tests might fail on other systems.
        g18x      # Failed Test, Both CI and Windows
        g210a     # Exception: SegFault, Both CI and Windows
        g331      # Exception: SegFault, Both CI and Windows
        g420      # Failed Test, Both CI and Windows
        # tests below are flaky (on Linux)
        gmmf      # 0.150 seconds, Exception: SegFault, Only on Windows, #bus errors, unstable on CI?
        gmmf1s    # 0.110 seconds, Exception: SegFault, Only on Windows
        gmmf1u    # 0.260 seconds, Exception: SegFault, Only on Windows
        gmmf1w    # 0.110 seconds, Exception: SegFault, Only on Windows
        )
function(add_tests test_list prefix)
foreach (test_name IN LISTS ${test_list})
    file(GENERATE
            OUTPUT "$<${is_multi_config}:$<CONFIG>/>${test_name}.ijs"
            CONTENT "0!:0 <'${CMAKE_CURRENT_SOURCE_DIR}/tsu.ijs'
LIBTSDLL=:'$<TARGET_FILE:tsdll> '
exit -.&(0!:3) <testpath,'${test_name}.ijs'"
            CONDITION $<BOOL:${BUILD_TESTING}>
            )
    add_test(
            NAME ${prefix}${test_name}
            COMMAND jconsole "$<${is_multi_config}:$<CONFIG>/>${test_name}.ijs"
    )
endforeach ()
endfunction()
add_tests(hare_test_cases hare_)
add_tests(tortoise_test_cases tortoise_)
#add_tests(disabled_test_cases disabled_)