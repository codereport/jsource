## J: From C to C++20

J is an [array programming language](https://en.wikipedia.org/wiki/Array_programming) created by Ken Iverson and Roger Hui (see image below). 

This is a fork of `jsoftware/jsource` and we will be porting it to C++20.

### Goals

* [ ] Compile with GCC 10+
* [ ] Compile with Clang 11+
* [ ] Remove all (most) of the macros
* [ ] Clang-format the code base
* [ ] Get both build / tests running in parallel
* [ ] Monitor compile & run time perf while refactoring

### Comparison of Languages

Calculating the first 10 odd numbers:
**Python**:
```python
[1 + 2 * i for i in range(10)]
```
**Haskell**:
```hs 
map (1+) $ map (2*) [0..9]
```
**APL**:
```apl
1+2×⍳10
```
**J**:
```ijs
1+2*i.10
```

### Getting started & Building:
1. Checkout the repository:
    
    `https://github.com/codereport/jsource.git`
2. Build jconsole:
    
    `cd jsource`

    `mkdir build && cmake -G "Ninja Multi-Config" -B build`

    `ninja -C build` or `ninja -C build -f build-Release.ninja` if you are building for release.

    To run the debug build: `./build/jsrc/Debug/jconsole` 

    To run the release build: `./build/jsrc/Release/jconsole`

![image](https://user-images.githubusercontent.com/36027403/104798929-e4311700-5798-11eb-859c-5a55738daf79.png)
