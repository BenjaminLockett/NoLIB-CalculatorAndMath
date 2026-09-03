# NoLib Mathematics

A C++17 mathematics and symbolic-calculus project that deliberately avoids the
standard library's mathematics, container and string facilities. `<iostream>` is
used only by the console application.

Previously this was one 4,710-line header plus one `main.cpp`. It is now a
Visual Studio project with declarations in `Include/` and definitions in
`Source/`.

## Building

**Visual Studio** — open `NoLibMath.sln`, pick a configuration, build. The
project sets `/std:c++17`, `/W4`, `/permissive-`, and adds `$(ProjectDir)Include`
to the include path. Solution Explorer shows `Header Files` and `Source Files`
mirroring the folder tree.

**GCC / Clang** — `./build.sh`, or

```
g++ -std=c++17 -Wall -Wextra -Wpedantic -Wshadow -IInclude main.cpp $(find Source -name '*.cpp') -o NoLibMath
```

**CMake** — `cmake -S . -B build && cmake --build build`

## Layout

```
NoLibMath.sln
NoLibMath.vcxproj
NoLibMath.vcxproj.filters
main.cpp                      startup and menu dispatch only

Include/
  NoLibMath.hpp               umbrella header, kept for backwards compatibility
  NoLib/Core/                 Types, Utility, CheckedInteger, RealTraits
  NoLib/Text/                 String, stream adapters, parsing, formatting
  NoLib/Scalar/               scientific numbers, roots, constants, log, exp, powers, trigonometry
  NoLib/Complex/              representation, arithmetic, elementary functions, formatting
  NoLib/Polynomial/           representation, arithmetic, calculus, roots
  NoLib/Numerical/            differentiation, quadrature, ODE
  NoLib/Symbolic/             nodes, arena, builder, parser, evaluation, formatting,
                              differentiation, polynomial bridge, integration, Risch subset
  App/                        console configuration, input, output, menu
  App/Commands/               one header per menu command

Source/
  NoLib/...                   definitions for every non-template function
  App/...                     console implementation
```

Templates stay in headers because they must; every non-template function has its
definition in the matching `.cpp`.

## Layering

```
Core -> Text -> Scalar -> Complex -> Polynomial -> Numerical
     -> Symbolic representation -> Symbolic algorithms -> App
```

A header never includes anything from a layer above it, and every header
includes its own dependencies rather than relying on include order. Each header
has been compiled on its own to prove that.

The library never prints. `<iostream>` appears only in `Source/App/**` and
`main.cpp`.

## Source conventions

Allman braces, one statement per line, and one physical line per declaration,
signature, condition, loop header and call:

```cpp
bool functionName(int value_Int_Var)
{
    return value_Int_Var != 0;
}
```

Variables carry a type suffix: `count_UInt_Var`, `angle_Real_Var`,
`text_String_Var`, `converged_Bool_Var`, `root_Complex_Var`. Raw arrays say so:
`coefficients_RealArray_Var`, `roots_ComplexArray_Var`,
`MenuEntries_MenuEntryArray_Var`. Pointers use `_CharPtr_Var`, function pointers
`_FunctionPtr_Var`. Template parameters end in `_UInt` or `_T`.

`#pragma once` in every header. No macros for constants or functions, and no
`using namespace`.

## Compatibility

`#include "NoLibMath.hpp"` still compiles and still exposes every previous type
and function under `NoLib`, with unchanged names, signatures and default
arguments. Nothing was renamed. Old `main.cpp` code that called `NoLib::` free
functions needs no edits.

The console functions moved into namespace `App`, and `runRealPower` is now
`App::runDecimalPower` to match the menu wording. Menu option numbers are
unchanged.

## Known limitations, carried over unchanged

This refactor moved code; it did not change algorithms. The following behaviour
is identical to the previous build and still needs repair:

- `rationalPower(-8, 2, 6)` fails because the exponent is checked before it is
  reduced by the greatest common divisor. The reduced exponent `1/3` has the
  real result `-2`.
- The parser maps `ln(x)` and `lnabs(x)` to one absolute-log node, so `ln(-1)`
  evaluates to `0` instead of reporting a domain error.
- `squareRoot` uses a poor starting estimate for extreme magnitudes.
- `realIntegerPower` with a negative exponent can report success with infinity.
- Simpson's interval count can wrap when given the maximum odd `UInt`.
- Complex division squares the denominator components and overflows.
- Durand-Kerner does not converge for repeated roots such as `(x - 1)^4`.
- `reduceAngle` subtracts `2*pi` in a loop, so large angles are slow.
- The integrator's "verification" is sampled validation, not an identity proof.

The integrator is a restricted, Risch-inspired elementary integrator. It is not
a complete Risch algorithm: it has no differential fields and no exact decision
procedure.
