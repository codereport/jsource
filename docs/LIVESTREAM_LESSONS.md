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

### Livestream #4 2021-01-23
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

|    Platform    | Free Linux Minutes |
| :------------: | :----------------: |
|   Travis CI    |        1000        |
| Azure Pipelines|        1800        |
| Github Actions |        2000        |
|   Buildkite    |     Infinity?      |
|   Appveyor     |     Infinity?      |

* We learned for some reason macOS is 5-10x more costly when it comes to CI
* ~~We should checkout GitHub actions~~
* We should also look into [Buildkite](https://buildkite.com/)
* Maybe also [Appveyor](https://www.appveyor.com)

### Livestream #3 2021-01-25
* Ctrl+Shift+\ is select to matching brace
* `git commit -m`
* `git branch -m oldname newname` to rename branch
*  j$(nproc)
* Need to review [Mixing C and C++ Code](https://isocpp.org/wiki/faq/mixing-c-and-cpp)
* We learned J code base has the egregious local variable seemingly not used but its use is hidden in a macro
* :star: Learned how to correctly set up repo merge conditions (aka build / reviews - summary: only one, use *)
