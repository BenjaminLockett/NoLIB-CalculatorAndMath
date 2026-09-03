# NoLib Math Workbench

**[Download NoLibMath — current parser version](downloads/NoLibMath.zip?raw=true)**

**[Download NoLibMath Legacy — original basic code](downloads/NoLibMath-Legacy.zip?raw=true)**

These are separate source downloads. The current version is also fully browsable
below in `Header/` and `Source/`. The legacy ZIP is the original supplied archive,
unchanged, with its own solution and original numbered menu. It does not include
the new workbench parser or console. Download links above work on GitHub.

To get just one version, use its link above. GitHub's **Code > Download ZIP**
downloads the entire repository, including the two packaged downloads.

A keyboard-driven C++17 math console for Visual Studio. No third-party libraries,
packages, downloads, or network access are needed by the application.

## Start here

1. Extract the entire ZIP into a folder.
2. Open **NoLibMath.sln** in Visual Studio.
3. Choose **x64 / Release**, and set **NoLibMath** as the startup project if needed.
4. Press **Ctrl+F5** to build and run.

The solution uses the C++ desktop workload and Windows SDK. Visual Studio 2022
selects v143; Visual Studio 2026 selects v145. The solution was built and tested
with the installed Visual Studio 2026 compiler on Windows, in x64 Release.

After building, double-click **Run.cmd** to launch the x64 executable.
**Run-Tests.cmd** runs the tests after building. These downloads contain source
code; Visual Studio produces the executables locally. The Release projects link
the C++ runtime statically.

## Your command syntax

```text
action[dirive(x^3 + 2x + 1)]
```

Result:

```text
3x^2 + 2
```

`action[...]` selects the operation. Its parentheses contain the expression that
the parser reads. `dirive` is deliberately accepted, as are `derive` and `diff`.
The shorter `derive(x^3 + 2x + 1)` also works. Type `help` for the complete list.

| Command | Purpose / example |
| --- | --- |
| `derive(expr)` | Differentiate in x. Aliases: `dirive`, `diff`. |
| `integrate(expr)` | Supported antiderivative, with `+ C`. Alias: `int`. |
| `simplify(expr)` | Expand and collect polynomials, up to degree 32. Alias: `expand`. |
| `eval(expr, value)` | Evaluate at x = value, e.g. `eval(x^3 + 2x + 1, 2)` -> `13`. |
| `eval(expr)` | Evaluate a constant expression. Alias: `evaluate`. |
| `help`, `commands` | Command list and syntax. `action[help()]` also works. |
| `examples` | Copyable examples. |
| `history` | Last 12 calculation inputs, including failed calculations. |
| `clear`, `cls` | Clear and redraw the console. |
| `theme(cyan)` | Color choices: `cyan`, `amber`, `plain`. |
| `quit`, `exit` | Exit. End-of-input also exits cleanly. |

All commands may be wrapped in `action[...]`. Utility commands accept empty
parentheses, e.g. `action[examples()]`. Command and function names are insensitive
to case. Theme values are lowercase. A bare expression such as `2x + 3x + 1`
uses `simplify`. Only one operation per input line is supported.

```text
action[simplify((x+1)(x-1))]        -> x^2 - 1
action[integrate(3x^2 + 2)]        -> x^3 + 2x + C
action[eval(sin(x), pi/2)]         -> approximately 1
derive(sin(x))                    -> cos(x)
eval(2^3^2)                       -> 512
eval(1.5e2 + .25)                 -> 150.25
```

## What the parser understands

- Operators: `+ - * / ^`; grouping with parentheses; unary `+` and `-`.
- Implicit multiplication: `2x`, `3(x+1)`, `x(x+1)`, `(x+1)(x-1)`, `2sin(x)`.
- Decimals and scientific notation: `.5`, `1.25`, `1e-3`.
- Variable `x`; constants `pi` and `e`.
- Functions: `sin`, `cos`, `tan`, `atan`, `exp`, `ln`, `lnabs`.
- Constant integer exponents: `x^3`, `x^-2`, `x^(1+2)`.

Powers associate to the right: `2^3^2` means `2^(3^2)`. Powers bind more tightly
than unary signs: `-x^2` means `-(x^2)`. Multiplication and division, including
implicit multiplication, have equal precedence and run left to right:
`6/2x` means `(6/2)*x`. Write `6/(2x)` for a denominator of `2x`.

Functions require parentheses. Write `x*x`, not `xx`, and `2*pi`, not `pix`.
`2e3` is scientific notation; write `2*e` to multiply Euler's constant.
Angles use radians. `ln(x)` requires x > 0; `lnabs(x)` means ln(abs(x)) and
requires x != 0. Invalid input reports a message and a caret at its source column.

## Interface and command-line use

Cyan/amber accents, a command dashboard, labeled results, inline diagnostics,
examples, and a bounded history are included. Windows console color support is
detected automatically; redirected output falls back to readable plain text.
`clear` uses terminal escape sequences rather than launching a shell command.

```bat
bin\x64\Release\NoLibMath.exe --plain
bin\x64\Release\NoLibMath.exe --command "action[dirive(x^3 + 2x + 1)]"
bin\x64\Release\NoLibMath.exe --help
```

`--plain` disables color at startup. `--color` forces ANSI color for terminals
that support it. `--command` runs once and returns 0 on success, 1 for invalid
commands/calculations, or 2 for invalid process arguments. Piped lines and EOF
are supported. Overlong lines are fully consumed and rejected; a partial prefix
is never executed.

## Folder layout and preservation

```text
NoLibMath.sln                    Current application and test projects
NoLibMath.vcxproj                Current workbench startup project
NoLibMathTests.vcxproj           Standalone tests, no test framework
Header/
  NoLib/                        Shared math headers
  Workbench/                    Parser, command and console declarations
Source/
  main.cpp                      Current application entry point
  NoLib/                        Shared math implementation
  Workbench/                    Parser, commands and console
  Tests/ParserTests.cpp          Parser and calculation tests
downloads/
  NoLibMath.zip                 Current source only
  NoLibMath-Legacy.zip           Original basic source only
```

The `downloads/` folder is part of the repository, and is excluded from the
current source ZIP. The legacy application is contained only in its own ZIP;
the current solution builds the workbench and tests independently of it.

The current C++ files are copied from the latest working version without edits
during packaging. `Include/` becomes `Header/` in the current version. The separate legacy ZIP
retains the original project layout, all 94 original C++ files and original
build configuration. Open its `NoLibMath.sln` separately; that original project
targets Visual Studio 2022/v143.

The new workbench adds a recursive-descent parser and checked evaluation layer
over the original math APIs. The current working version also contains a local
edit to `Source/NoLib/Scalar/SquareRoot.cpp`, including diagnostic output. That
edit is preserved here; the legacy ZIP retains the original file.
`README.original.md` documents the original library; its layout and menu
instructions refer to the legacy version. See `SOURCE-SHA256.txt` for the
current C++ file hashes and their relationship to the original source.

## Limits

- This is a single-variable expression parser, not a complete computer algebra
  system. No equations, user variables, square roots, fractional/variable
  exponents, factorials, or nested action commands.
- Exponents must be constant integers from -1024 to 1024. Use `exp(x)` for
  variable exponentials.
- Polynomial expansion supports degree 32; polynomial integration supports
  input degree up to 31. General expressions are normalized rather than fully
  algebraically simplified.
- The original restricted integration engine is used for non-polynomials.
  Unsupported integrals fail clearly. Accepted symbolic candidates pass its
  guarded sample-point verification; that check is not an exact identity proof.
- Input is limited to 2048 characters, 512 nodes, and depth 48. Non-polynomial
  integration has a smaller complexity limit because of the original engine's
  recursive stack usage.
- Trig arguments must be within +/-1,000,000 radians. All results use floating
  point and are displayed to approximately 12 decimal places; underflow and
  rounding still apply. This is not exact rational arithmetic.
- Algebraic transformations are valid only where the original expression is
  defined. The original library's remaining numerical limitations, listed in
  `README.original.md`, are not silently claimed to be repaired.

The new layer checks arithmetic overflow and division by zero, distinguishes
`ln` from `lnabs`, rejects malformed numeric tokens, guards recursion, and keeps
small nonzero polynomial coefficients instead of trimming them to zero.

## Building and testing with CMake

CMake is optional build tooling, not an application dependency.

```text
cmake -S . -B build/cmake
cmake --build build/cmake --config Release
ctest --test-dir build/cmake -C Release --output-on-failure
```

On Linux/macOS with a C++17 compiler and CMake, `sh build.sh` runs these steps.
Windows and Visual Studio are the tested platform for this delivery.

Tests cover command dispatch and aliases, polynomial/chain/quotient derivatives,
implicit multiplication, scientific notation, grouping round trips, invalid
domains, small coefficients, source columns, input/depth limits, and 3,000
deterministic malformed inputs. See `VALIDATION.md` for the final build results.
