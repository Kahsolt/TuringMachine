# TuringMachine

    Standard k-tape Turing Machine simulator
    homework project of NJU course 'Preliminary Introduction to the Theory of Computation'

----

The problem description is here: [TM-Problem.pdf](/TM-Problem.pdf)

## Quickstart

  - have a look at [config.h](/src/config.h) for customization, and [/Makefile](Makefile) if you wanna modify executable filename, then do `make`
  - run demos, e.g. `make test CASE=programs/palindrome`
  - create a directory for your case, containing:
    - a TM description filenamed `test.tm` (referring to [syntax.tm](/programs/syntax.tm) and [example programs](/programs))
    - your test inputs in **one line** filenamed `input.txt`
  - run `make run CASE=<case-directory-path>` to start your TM on given input, you should now see:
    - console logs
    - log file `console.txt` and `results.txt` generated under your case-directory

### Example Programs

  - [incr](/programs/incr/test.tm): increase a binary number by 1, minimal example for 1-tape TM syntax demo
  - [add_3](/programs/add_3/test.tm): add up two binary numbers, minimal example for multi-tape TM syntax demo
  - [reverse](/programs/reverse/test.tm): reverse a binary string
  - [reverse_2](/programs/reverse_2/test.tm): reverse, on 2-tape TM
  
  - [palindrome](/programs/palindrome/test.tm): decide a palindrome binary string
  - [palindrome_2](/programs/palindrome_2/test.tm): decide palindrome, on 2-tape TM
  - [mult](/programs/mult/test.tm): decide a unary multiply equation string
  - [fib_4](/programs/fib_4/test.tm): decide a unary string in fibonacci number length, on 4-tape TM
  - [ww](/programs/ww/test.tm): decide a binary string in form of 'ww'

----

by Armit
2019/10/02 
2019-11-26