### Getting started & Building:
1. Checkout the repository:
    
    `https://github.com/codereport/jsource.git`
2. Build jconsole:
    
    `cd jsource`

    `mkdir build && cmake -G "Ninja Multi-Config" -B build`

    `ninja -C build` or `ninja -C build -f build-Release.ninja` if you are building for release.

    To run the debug build: `./build/jsrc/Debug/jconsole` 

    To run the release build: `./build/jsrc/Release/jconsole`
