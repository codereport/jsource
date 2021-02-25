### Livestream #1 2021-01-15
* ... under VSCode Find for file filter
* `-Wfatal-errors`
* `-fdiagnostics-color=always`
* [Intel Intrinsics](https://software.intel.com/sites/landingpage/IntrinsicsGuide/)
* Remember local machine has git shortcuts like `gs` for `git status`
* `git diff` `U` for # of lines
* Remembered `git diff` `--stat` for totall number of line changes
* Remembered `gitk` for visual diffs
* :star: `dlerror()` to diagnose `dlopen()` failure
* `extern "C"` wrapper to fix C++ issue
* Can't search in repo forks
* How to turn on issues in a forked repo
* Learned of [SLEEF](https://sleef.org/)

### Livestream #2 2021-01-17
* :star: `F4` & `Shift + F4` for navigating search results
* Prounounciation of Joa~o
* Commute adverb in J in ~
* We also learn John Scholes started ~~co-dfns~~ dfns
* :star: Learned about Dex !! (very interesting)
* :star: `Ctrl+Shift+[` to collapse region
* Got `git lg` setup
* Learned the `netdefs` folder can't be deleted (at least for the moment)

### Livestream #3 2021-01-23
* Learned about Travis-CI
* To avoid crazy Travis-CI issues: HOMEBREW_NO_AUTO_UPDATE=1 brew install
* `filename:` to filter for files on GitHub search
* `path:` to filter for path on GitHub search
* We also learned CircleCI is a PITA (and to just use Travis-CI)
* :star: We learned how to run tests in the jconsole:
```ijs
testpath=: '/home/cph/jsource/test/'
0!:0 <testpath,'tsu.ijs'
0!:2 <testpath,'<insert test name>.ijs'
```
* Learned the following about j
```ijs
-: NB. this is match
|. NB. reverse
]  NB. identity
}. NB. drop
{. NB. take

palindrome =. (|. -: ])  NB. fork version
palindrome =. (-: |.)    NB. hook version
```
* That Travis-CI is not free, you have 10,000 credits which get used per minute per OS 
* We learned the following about CI pricing:

|    Platform     | Free Linux Minutes |
| :-------------: | :----------------: |
|    Travis CI    |        1000        |
| Azure Pipelines |        1800        |
| Github Actions  |        2000        |
|    Buildkite    |     Infinity?      |
|    Appveyor     |     Infinity?      |

* We learned for some reason macOS is 5-10x more costly when it comes to CI
* ~~We should checkout GitHub actions~~
* We should also look into [Buildkite](https://buildkite.com/)
* Maybe also [Appveyor](https://www.appveyor.com)

### Livestream #4 2021-01-25
* Ctrl+Shift+\ is select to matching brace
* `git commit -m`
* `git branch -m oldname newname` to rename branch
*  j$(nproc)
* Need to review [Mixing C and C++ Code](https://isocpp.org/wiki/faq/mixing-c-and-cpp)
* We learned J code base has the egregious local variable seemingly not used but its use is hidden in a macro
* :star: Learned how to correctly set up repo merge conditions (aka build / reviews - summary: only one, use *)

### Livestream #5 2021-01-30
* Highlight section of Github Insights
* How to add an GitHub action 
* -term (read minus term) emits that term from a Google search
* J:
```ijs
{. NB. Head
{: NB. Tail
tally =. {. @ $
```
* Regex \w for word (0-9, a-z, A-Z, _ etc) \W is for non-word 
* `k & ~1` is equivalent to `k != 0 && k != 1`
* A bunch of doxygen / breathe / sphinx stuff

### Livestream #6 2021-02-06

* `x:` [Converts number y to extended precision, either extended integer or rational.](https://code.jsoftware.com/wiki/Vocabulary/xco)
* To get custom TOC in sphinx RST files, just use `.. contents::` + markdown
* :star: `Alt + R` to toggle between regex search (on both local and global)
* :star: `\b` is regex word boundary (helpful for avoiding need for captures)
* countIf is ALSO THE BLACKBIRD!!!
```hs
Prelude> let countIf      = blackbird length filter
Prelude> let innerProduct = blackbird sum (zipWith (*))
```
* Learned about partial application in J: `add5 =. 5&+`

### Off-stream 2021-02-07
* `-type f` to filter files on `find .` command
* Single line run test in `jconsole`
```ijs
0!:2 <'./test/g___.ijs' [ 0!:0 <'./test/tsu.ijs'
```
* You can treat the `[` (see :arrow_up:) as a statement separator
* `3!:0 ]` will get you the type of a array/pronoun ([docs](https://www.jsoftware.com/help/dictionary/dx003.htm))

### Livestream #7 2021-02-10

* :star: `datatype` is a builtin function that gives us a English type of an array
* Need to use `*` in the global VSCode search
* :star: Learned you can't use `x` and `y` in script if you are going to invoke them from a explicitly defined verb
   * And therefore it probably should be best practice not to
* Learned `names ''` to get a local list of verbs defined
* Learned about raze `;` (similar to ravel `,` but only one level of structure)
* Learned when searching on J for functions, search for `verbs` (technically the correct terminology in J speak)
* Learned that MicroAPL refers to "tack" as pass (and Dyalog APL refers to it as same)
* Learned about the [P Programming Language](https://github.com/p-org/P)
* Learned for a multi-line J function to use `f =. 3 : 0` or `g =. 4 : 0`

### Livestream #8 2021-02-18

* `x y z | value` is the sparse array view
* We learned `=` has lower precedence that equality comparision in C++
* IRS = Integral/Integrated Rank Support (optimizations for certain verbs - see [this paper](https://www.jsoftware.com/papers/rank.htm))
* Discrovered [Try J](http://tryj.freeddns.org/) site set up by Nathan
* Used our first C++ Standard Algorithm (`std::mismatch` x2)

### Livestream #9 2021-02-24

* Learned about J's `trace` function from Bob's video
* Learned this fancy incantation `((void)++fs,...)`
* Learned can't be in a deduced context then variadic template is not the last template
* Added `algorithm.hpp` with `zip_find`, `zip_found` and `is_mismatched`
* `git reset --merge` to abort `git stash pop`
