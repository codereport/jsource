## J: From C to C++20

J is an [array programming language](https://en.wikipedia.org/wiki/Array_programming) created by Ken Iverson and Roger Hui (see image below). 

This is a fork of `jsoftware/jsource` and we will be porting it to C++20.

### Goals
* [ ] Reduce complexity of build options
   * [x] Remove Android
   * [x] Remove AVX
   * [x] Remove SLEEF
   * [x] Remove Neon
   * [x] Remove SSE41
   * [x] Remove SSE42
   * [x] Remove SSSE3
   * [ ] Remove Windows
   * [ ] Remove Raspberry Pi
   * [x] Remove blis
* [ ] Compile with GCC 10+
* [ ] Compile with Clang 11+
* [ ] Remove all (most) of the macros
* [ ] Clang-format the code base
* [ ] Clang-tidy the code base
* [ ] Get both build / tests running in parallel
   * [x] Parallel build (for free off of zhihaoy branch)
   * [ ] Parallel tests
* [ ] Monitor compile & run time perf while refactoring

### Non-Codebase Goals

* [ ] Learn to not use mouse

### Comparison of Languages

Calculating the first 10 odd numbers:

**Python**:
```python
[1 + 2 * i for i in range(10)]
```
**Haskell**:
```hs 
map (1+) $ map (2*) [0..9]
map ((+1) . (*2)) [0..9] -- alternative thanks to Alexandru Dinu
```
**R**:
```R
-1+2*seq(10) -- thanks to Roi Barkan
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
For building this repository, please see `CONTRIBUTING.md`.

![image](https://user-images.githubusercontent.com/36027403/104798929-e4311700-5798-11eb-859c-5a55738daf79.png)
