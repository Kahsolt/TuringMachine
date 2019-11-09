# TuringMachine

    Stanard one-bidirectional-infinite-tape Turing Machine simulation
    homework project of NJU course 'Preliminary Introduction to the Theory of Computation'

----

The problem description is here: [Project-2018](https://github.com/ptc-nju/Project-2018)

## Quickstart

  - have a look at [config.h](/src/config.h), then `make`
  - `make run CASE=programs/palindrome` to see demo examples
  - create a directory for your case, containing a TM description filenamed `test.tm` according to the format descripted in [sample.tm](/sample.tm) with refer to [example programs](/programs), and your inputs in one line filenamed `input.txt`
  - `make run CASE=<case-directory>` to run your TM case
  - see console logs, `console.txt` and `results.txt` generated under your case-directory

### Example Programs

  - [incr](/programs/incr/test.tm): increase a binary string by 1, also a minimal example for syntax demo
  - [reverse](/programs/reverse/test.tm): reverse a binary string
  - [palindrome](/programs/palindrome/test.tm): decide a palindrome binary string
  - [mult_d](/programs/mult_d/test.tm): decide a mult equation on unary language
  - [ww](/programs/ww/test.tm): decide a binary string in form of 'ww'

----

by Armit
2019/10/02 
