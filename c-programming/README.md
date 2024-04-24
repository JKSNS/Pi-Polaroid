# Data

## Experience Summary

### How many hours did it take to complete assignment?

1.5 hours. 

### What did you enjoy about this lab?

Debugging and compiling my code. 

### What were the major challenges you had with this lab? Try to be as detailed as possible.

Again, what I enjoyed was the part of the lab that I had challenges with. Several times during this lab, my code wouldn't compile because of syntactical errors. 
In order to debug this, I had to look into the specific syntax of 'C' that's specific to assembly. 

## Lab Specific Tasks

### A Simple C Program
1. What are the specific types of comments that exist in the C programming language?

Block/multi-line and single line comments. 
<br> 

2. What is a compiler directive? When do they get used? 

Compiler directives are statements called at the beginning of a program in order to handle particular input, data types, etc... 
<br> 

3. Define the following compiler directives and when they are used: `#define`, `#if`, `#else`, `#endif`.

'#define': A constant value that can be reused. 

Conditional statements: <br> 
'#if': Checks if the condition is true. 

'#else': If the condition isn't true, else provides an alternate set of instructions. 

'#endif': Considering a condition is true, the program is prompted to end. 
<br> 

4. Investigate other compiler directives that exist in C. Name two more and explain what they do and when they are used.

'#include': A directive used to include particular libraries or source code, referenced at the beginning of a program. 

'#pragma': A directive used to turn particular features on or off within a particular program. 
<br> 

5. What is the `main()` function and why is it important in C programming?

The 'main()' function is the heart of every C program. All functions that need to be executed are done so through the main function which makes it so important. 
<br> 

6. What is a return value? Why are they used?

A return value signifies the end of a sequence of code, prompting to end the program or return a value. Without them, programs would run forever, looping and taking up 
unnecessary computational power. 
<br> 

7. Why does the `main()` function have a return value? Where can you access the return value of the `main()` function?

The 'main()' function has a return value because similar to every other variable function, it must have an end/ result. You can access the return value of the 'main()' 
function by looking at computer processes being ran as a result of the program, in program terminal output, data logs associated with your code, etc. 
<br> 

8. Modify the `simple.c` file that you previously created (if you haven't yet, go read the lab webpage!). Modify your program to do the following:

- Add two functions with two different return value data types.

- Call those functions from within your main function and print the values to the terminal using `printf`.

[Simple C Program](./simple.c)

Done. 
<br> 

1. Compile your updated `simple.c` program above in `gcc`. Change the name of the executable to `simple`.  Compile the code into assembly instead of an executable. Google search, "gcc output assembly" to find your answer.

[Simple Assembly](./simple.s)

[Simple Binary](./simple)

Done. 
<br> 

10. Look at one of the commands in your `simple.s` file. Google it and try to figure out what it means and report your findings below.

One of the commands that I found in my 'simple.s' file was: ".cfi_offset 30, -24". I found out that .cfi_offset is the Call From Information (CFI) 
directive, used for debugging. "30" is a register symbol and "-24" represents the offset specific to Canonical Frame Address (CFA), where the register's value 
is saved. Overall, this command is essential for unwinding the stack. 
<br> 

### Data Types

1.  Read the [man page for `stdint.h`](https://man7.org/linux/man-pages/man0/stdint.h.0p.html). You'll find that a lot of extra info about standard C libraries are available there. List below the all the different types this library provides.

All the different types this library provides are: 

int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t...
int_least8_t, uint_least8_t, int_least16_t, uint_least16_t, int_least32_t, uint_least32_t, int_least64_t, uint_least64_t...
int_fast8_t, uint_fast8_t, int_fast16_t, uint_fast16_t, int_fast32_t, uint_fast32_t, int_fast64_t, uint_fast64_t... 
intptr_t, uinptr_t...
intmax_t, uintmax_t... 

For all of which, stdint.h defines the limits. 
<br> 

12. Create a new file called `data.c`. This program should do the following. For each requirement, place a comment next to or above it so we know you have completed the required step:
    - Print out the hex equivalent of the `unsigned int`: 3735928559
Done. 
    - Create a function that takes in a `uint8_t` as a parameter and prints `char` equivalent. Use it at least 3 times in your `main()`.
Done. 
    - Use the `printf()` function at least once that has multiple formatting specifiers/placeholders.
Done. 
    - Use at least 5 different format specifier types in 5 different `printf()` statements.
Done. 
    - Use some format specifiers/placeholders in `printf()` in unexpected ways (i.e. pass in a `char` and format it with `%d`, or something similar). Your program must compile. In a comment next to or above this statement, explain the behavior and why you think it works that way.
Done. 

[Data C](./data.c)

1.  Compile `data.c` into a binary called `data`.

[Data Binary](./data)

Done. 
<br> 

### Miscellaneous Questions
The following questions will require some research outside of this lab.

14. What is the biggest number an `int` can hold?

The biggest number that an 'int' can hold, depends on the system and processor. Generally speaking, 2 to the 31st power. 
<br> 

15. Which is bigger, a `double` or a `float`?

A 'double' is generally larger than a 'float' because doubles occupy 8 bits whereas floats occupy 4 bits. 
<br> 

16. Arrange the following in data type size order (i.e., number of bytes): `int`, `double`, `long long`, `short`, `float`, `uint32_t`, `signed short`

'uint8_t',
'short', 'signed short',
'int', 'float', 'uint32_t', 
'double', 'long long'... 

