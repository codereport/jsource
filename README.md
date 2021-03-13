# <p align="center">J: From C to C++20</p>

<p align="center">
    <a href="https://buildkite.com/codereport/jsource" alt="Build Status">
        <img src="https://badge.buildkite.com/836b4e62ac875246eb46bcf44cc5522af1ffca85451354c87d.svg?branch=main" /></a>
    <a href="https://github.com/codereport/jsource/issues" alt="contributions welcome">
        <img src="https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat" /></a>
    <a href="https://github.com/codereport?tab=followers" alt="GitHub followers">
        <img src="https://img.shields.io/github/followers/codereport.svg?style=social&label=Follow" /></a>
    <a href="https://GitHub.com/codereport/jsource/stargazers/" alt="GitHub stars">
        <img src="https://img.shields.io/github/stars/codereport/jsource.svg?style=social&label=Star" /></a>
    <a href="https://twitter.com/code_report" alt="Twitter">
        <img src="https://img.shields.io/twitter/follow/code_report.svg?style=social&label=@code_report" /></a>
</p>

J is an [array programming language](https://en.wikipedia.org/wiki/Array_programming) created by [Ken Iverson](https://en.wikipedia.org/wiki/Kenneth_E._Iverson) and [Roger Hui](https://en.wikipedia.org/wiki/Roger_Hui) ([see image below](https://github.com/codereport/jsource#image-of-ken-iverson-and-roger-hui)).

This is a fork of [`jsoftware/jsource`](https://github.com/jsoftware/jsource) and we will be porting it to [C++20](https://en.cppreference.com/w/cpp/20).

## Motivation

The motivation for this is to learn about how Ken Iverson / Roger Hui implemented the J language (and similarly APL as J is the successor to APL). The end goal is to learn as much as possible.

## Try J

If you would like to try J without building from source, check out this site: http://tryj.freeddns.org/

## Goals
* [x] [Reduce complexity of build options](https://github.com/codereport/jsource/issues/13)
* [ ] Compile with GCC 10+
* [ ] Compile with Clang 11+
* [ ] Remove all (most) of the macros
* [x] Clang-format the code base
* [ ] Clang-tidy the code base
* [x] Set up CI (Buildkite)
* [ ] Set up Code Coverage
* [x] Set up [badges](https://github.com/badges/shields)
* [x] Get both build / tests running in parallel
   * [x] Parallel build (for free off of zhihaoy branch)
   * [x] Parallel tests
* [ ] Monitor compile & run time perf while refactoring

## Non-Codebase Goals

* [ ] Learn to not use mouse

## Comparison of Languages

Calculating the first 10 odd numbers:

| Language |                                Code                                |                                       Runnable Link                                       |
| :------: | :----------------------------------------------------------------: | :---------------------------------------------------------------------------------------: |
|  **J**   |                             `1+2*i.10`                             |                                                                                           |
|   APL    |                             `1+2×⍳10`                              |             [TryAPL](https://tryapl.org/?clear&q=1%2B2%C3%97%E2%8D%B310&run)              |
|    R     |                           `-1+2*seq(10)`                           |                                                                                           |
|  Python  |                  `[1 + 2 * i for i in range(10)]`                  |                      [Godbolt](https://python.godbolt.org/z/dEqv3s)                       |
| Haskell  |                    `map (1+) $ map (2*) [0..9]`                    |                 [OneCompiler](https://onecompiler.com/haskell/3wmm2ykqy)                  |
| Haskell  |                     `map ((+1) . (*2)) [0..9]`                     |                 [OneCompiler](https://onecompiler.com/haskell/3wmm2ykqy)                  |
| Haskell  |                         `take 10 [1,3..]`                          |                 [OneCompiler](https://onecompiler.com/haskell/3wmm2ykqy)                  |
|   C++    |     `transform(iota(0, 10), [](auto e) { return e * 2 + 1; })`     |                          [Godbolt](https://godbolt.org/z/5r7aEo)                          |
|    C#    | `Enumerable.Range(0, 10).Select((int i) => { return i * 2 + 1; })` |                                                                                           |
|   Java   |      `IntStream.range(0, 10).map(x -> x * 2 + 1).toArray();`       |                                                                                           |
|   Ruby   |                  `(0..9).map { \|i\| i * 2 + 1 }`                  | [Try It Online](https://tio.run/##KypNqvz/v0BBw0BPz1JTLzexQKFaoSazRiFTQUvBSEFbwVCh9v9/AA) |

## Getting started & Building:
For building this repository, please see [`CONTRIBUTING.md`](https://github.com/codereport/jsource/blob/main/CONTRIBUTING.md).


## Image of Ken Iverson and Roger Hui
![image](https://user-images.githubusercontent.com/36027403/104798929-e4311700-5798-11eb-859c-5a55738daf79.png)
