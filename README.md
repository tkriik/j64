# Status

In-progress

# What

A JSON library written in C, where all values are represented as 64-bit words.
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

For fun. You should **not** use this in production.

# How

Integrate `j64.h` and `j64.c` directly into your project with a C99-compliant compiler.
For testing, you should compile and run `j64_test.c` with the same compilation flags as
used in your code base.
