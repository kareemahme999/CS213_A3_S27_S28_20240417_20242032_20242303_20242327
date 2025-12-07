# Assignment-3 - Board Game Collection

This C++ project contains several board game implementations for the CS213
assignment. It uses CMake as the build system.

## What I changed to prepare for Doxygen

- Added a `Doxyfile` with reasonable defaults and recursive input.
- Added `docs/mainpage.h` to provide a Doxygen main page.
- Added Doxygen `@file` headers to the project's public header files so they
  appear in the generated documentation.
- Added a `doxygen` CMake target (if Doxygen is available on the build system).

## Building

From a command prompt (Windows cmd.exe):

```bat
mkdir build
cd build
cmake ..
cmake --build .
```

The executable `Assignment_3` will be produced in the build output.

## Generating documentation with Doxygen (tested)

I ran Doxygen locally with the included `Doxyfile` and confirmed it generates
HTML documentation. There are two ways to generate docs:

A) Using Doxygen directly from the project root:

```bat
doxygen Doxyfile
```

B) Using the CMake `doxygen` target (recommended if you use CMake builds):

```bat
mkdir build
cd build
cmake ..
cmake --build . --target doxygen
```

Output location (tested): `build_docs/html/` in the project root.

> Note: If `doxygen` is not installed or not on PATH, the CMake configuration
> will show a message and the `doxygen` target will be unavailable.

## Quick troubleshooting

- If the CMake configure step doesn't create the `doxygen` target, install
  Doxygen (https://www.doxygen.nl) and re-run CMake.
- To enable call graphs and class diagrams, install Graphviz and set
  `HAVE_DOT = YES` in `Doxyfile`.

## Next steps / improvements (optional)

- Add more Doxygen comments to `.cpp` functions to improve the generated docs.
- Enable Graphviz (`HAVE_DOT = YES`) if you want call graphs/diagrams.
- Tweak `Doxyfile` values (PROJECT_BRIEF, PROJECT_LOGO) for nicer output.


## Where the generated docs were produced during testing

When I ran `doxygen Doxyfile` from the project root, Doxygen created
`build_docs/html/index.html`. Open that file in a browser to see the docs.
