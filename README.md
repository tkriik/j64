# Status

In-progress

# What

A header-only JSON library written in C, where all values are represented as 64-bit words.
Values are either immediate or boxed.

An immediate value is stored directly in the word, which can be:

 * a literal (null, false, true, empty string, empty array, empty object)
 * a 61-bit floating point
 * a 62-bit integer
 * a short string (7 bytes or less)

A boxed value contains a pointer to the heap, which can be:

 * a long string (more than 7 bytes)
 * a non-empty array
 * a non-empty object

# Why

For fun. I got the idea when studying how
[Erlang terms](https://github.com/erlang/otp/blob/maint/erts/emulator/beam/erl_term.h)
were implemented.
You should **not** use this in production.

# How

Integrate `j64.h` directly into your project.
An ANSI-compliant compiler is enough, although it needs
to support `long long` integer literals.

For testing, you should compile and run `j64_test.c` with the same compilation flags as
used in your code base.
