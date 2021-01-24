# J: From C to C++20
[![Build Status](https://travis-ci.com/codereport/jsource.svg?branch=main)](https://travis-ci.com/codereport/jsource)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/codereport/jsource/issues)
[![GitHub followers](https://img.shields.io/github/followers/codereport.svg?style=social&label=Follow&maxAge=2592000)](https://github.com/codereport?tab=followers)
[![GitHub stars](https://img.shields.io/github/stars/codereport/jsource.svg?style=social&label=Star&maxAge=2592000)](https://GitHub.com/codereport/jsource/stargazers/)
[![Twitter](https://img.shields.io/twitter/follow/code_report.svg?style=social&label=@code_report)](https://twitter.com/code_report)

J is an [array programming language](https://en.wikipedia.org/wiki/Array_programming) created by [Ken Iverson](https://en.wikipedia.org/wiki/Kenneth_E._Iverson) and [Roger Hui](https://en.wikipedia.org/wiki/Roger_Hui) (see image below).

This is a fork of `jsoftware/jsource` and we will be porting it to C++20.

## Goals
* [ ] [Reduce complexity of build options](https://github.com/codereport/jsource/issues/13)
* [ ] Compile with GCC 10+
* [ ] Compile with Clang 11+
* [ ] Remove all (most) of the macros
* [ ] Clang-format the code base
* [ ] Clang-tidy the code base
* [ ] Set up Travis-CI
* [ ] Set up CodeCov
* [ ] Set up [badges](https://github.com/badges/shields)
* [ ] Get both build / tests running in parallel
   * [x] Parallel build (for free off of zhihaoy branch)
   * [ ] Parallel tests
* [ ] Monitor compile & run time perf while refactoring

## Non-Codebase Goals

* [ ] Learn to not use mouse

## Comparison of Languages

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
-1+2*seq(10) # thanks to Roi Barkan
```
**APL**:
```apl
1+2×⍳10
```
**J**:
```ijs
1+2*i.10
```

## Getting started & Building:
For building this repository, please see [`CONTRIBUTING.md`](https://github.com/codereport/jsource/blob/main/CONTRIBUTING.md).

![image](https://user-images.githubusercontent.com/36027403/104798929-e4311700-5798-11eb-859c-5a55738daf79.png)
