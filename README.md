# ArbitraryInteger

A simple C++11 arbitrary length integer library for teaching.

Signed values use a growing vector of 32-bit unsigned words, with 64-bit
intermediates for arithmetic. Construction accepts signed native integers and
decimal strings.

The library favors simple loops and algorithms that are not highly optimized
so students can follow the implementation.

## Versions

| Version | Added features |
| --- | --- |
| `v1.0.0` | Arithmetic, comparisons, assignment, increment, and decrement |
| `v2.0.0` | Bitwise AND, XOR, OR, and compound assignments |
| `v3.0.0` | Left and right shifts |
| `v3.0.8` | GitHub Actions unit-test workflow |

## Build

Requires CMake 3.14 or newer and a C++11 compiler.

```sh
cmake -S . -B build -DCMAKE_CXX_STANDARD=11
cmake --build build
cd build
ctest --output-on-failure
```

Tests use GoogleTest 1.10.0. Disable them with `-DBUILD_TESTING=OFF`.
For CMake 4, also configure with `-DCMAKE_POLICY_VERSION_MINIMUM=3.5`.

Fetch this repository with CMake `FetchContent` and select a version tag from
the table. Link `ArbitraryInteger::ArbitraryInteger` and include
`ArbitraryInteger.hpp`.
