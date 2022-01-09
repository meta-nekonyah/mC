# mC
Meta C. It's C but more pleasant to work with.

# C without the need for:
  - Terminating the end of lines with semicolons/;.
  - Brackets to group things, grouping is done via indentation liike Python.

# To use it to compile say... helloworld in metaC sytax:
- gcc -o mC mc.c    // Compile metaC to a binary
- mC helloworld.mc  // Use metaC binary to convert metaC syntax file to regular C syntax
- gcc test.c        // *test.c* is it's current hardcoded name, still working on it
- ./a.out           // Run helloworld : )

# helloworld.mc
- #include <stdio.h>

- int main():
-   printf("Hello, World!\n")
-   return 0;



# What needs to be worked on
- // comments inline of code
- /**/ comments in line of code

# Some more Nicities coming later
  -One will not need to prototype functions.
