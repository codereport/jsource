### Getting started & Building:
1. Checkout the repository:
    
    `git clone https://github.com/codereport/jsource.git`
2. Build jconsole:

    Prerequisites:
       
       * CMake version 3.17.0+ -> Ubuntu systems seem to have a problem getting the latest version, please build from sources or use another package such as snap. 
       * Ninja
    
    `cd jsource`

    `mkdir build && cmake -G "Ninja Multi-Config" -B build`

    `ninja -C build` or `ninja -C build -f build-Release.ninja` if you are building for release.

    To run the debug build: `./build/jsrc/Debug/jconsole` 

    To run the release build: `./build/jsrc/Release/jconsole`

    To run tests: `ninja -C build test`
