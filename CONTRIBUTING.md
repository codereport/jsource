### Getting started & Building:
1. Checkout the repository:
    
    `git clone https://github.com/codereport/jsource.git`
2. Build jconsole:

    Prerequisites:
   * CMake version 3.17.0+ -> Ubuntu systems seem to have a problem getting the latest version, please build from sources or use another package such as snap. 
     * [Ubuntu CMake update instructions](https://apt.kitware.com/)
   * Ninja
    
    ```sh
    cd jsource

    mkdir build && cmake -G "Ninja Multi-Config" -B build
    #building debug
    ninja -C build 
    #building release.
    ninja -C build -f build-Release.ninja` 
    ```

    To run the debug build:
    ```sh
    ./build/jsrc/Debug/jconsole
    ```

    To run the release build:
    ```sh
    ./build/jsrc/Release/jconsole
    ```

    To run tests:
    ```sh
    ninja -C build test
    ```
