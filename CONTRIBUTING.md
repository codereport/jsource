### Getting Started & Building:
1. Prerequisites:
   * CMake version 3.18.0+
     * Ubuntu systems may have a problem getting the latest cmake version
       * [Ubuntu CMake update instructions](https://apt.kitware.com/)
   * Ninja
   * Additional prerequisites for building the documentation:
     * python packages: sphinx, breathe, divio-docs-theme
     * doxygen
2. Checkout the repository:
    ```sh
    # --recursive flag is needed to pull in the submodule
    git clone https://github.com/codereport/jsource.git --recursive
    ```
3. Build jconsole:
   * Run cmake:
    ```sh
    cd jsource
    mkdir build
    cmake -G "Ninja Multi-Config" -B build
    # If you want documentation built, modify your cmake command to:
    cmake -G "Ninja Multi-Config" -B build -DDOCS:STRING=YES
    ```
   * To build debug:
    ```sh
    ninja -C build
    ```
   * To build release:
    ```sh
    ninja -C build -f build-Release.ninja
    ```
4. Run jconsole:
   * To run the debug build:
    ```sh
    ./build/jsrc/Debug/jconsole
    ```
   * To run the release build:
    ```sh
    ./build/jsrc/Release/jconsole
    ```
5. Run tests:
    ```sh
    ninja -C build test
    ```
   * Filter tests:

    ```sh
   cd ./build && /usr/bin/ctest --force-new-ctest-process -C Debug -R hare_ && cd .. #fast
   cd ./build && /usr/bin/ctest --force-new-ctest-process -C Debug -R tortoise_ && cd .. #slow
   # this one requires to uncomment the disabled tests in ./test/CMakeLists.txt
   cd ./build && /usr/bin/ctest --force-new-ctest-process -C Debug -R disabled_ && cd ..
   ```

Extra: Get JHS Web IDE
```
./build/jsrc/Debug/jconsole ./jlibrary/addons/ide/jhs/config/jhs.cfg
```
