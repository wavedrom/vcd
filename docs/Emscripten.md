# Emscripten
This file is a bit of notes on the Emscripten changes to the C code.

# Makefile
I added a `Makefile` to the project.  A few key things here.
* warnings flags etc to in `CLANG_WARN_FLAGS`
* compile time `#define` go in `CLANG_OTHER_FLAGS`
* Every .c .cpp file goes in `CPP_FILES`
* Every .h .hpp file goes in `HPP_FILES` however not required
  * This is only required if you want make "sensativity" to work correctly
* The emscripten `.js` file is minified by default. to disable this run
  * `NOOPT=1 make wasm`
  * Doing this will also compile the c code with `O0`
* Any c functions you want to access from javascript must be added to `EXPORT_STRING`
  * When you add them, you must add a `_` prefix
