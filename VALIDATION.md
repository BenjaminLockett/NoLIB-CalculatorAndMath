# Packaging validation

The separate current source package was checked on 2026-09-03 using Visual Studio
2026 / MSVC v145, x64 Release, C++17 and Windows SDK 10.0.26100.0.

- Both current solution projects built successfully from the separated source.
- Parser suite: 3,120 passed, 0 failed.
- Requested command: `action[dirive(x^3 + 2x + 1)]` returns `3x^2 + 2`.
- A malformed command returns failure as expected.
- CMake 4.2.3 configured and built successfully; all 3 CTest groups passed.
- All packaged C++ files match the latest working copy byte-for-byte.
- 65 shared original C++ files match the original archive. The local edit to
  `SquareRoot.cpp` is preserved in the current version.
- The legacy ZIP is byte-for-byte identical to the original supplied ZIP and
  contains all 94 original C++ files. Its archive integrity check passed.
- Both downloads are separate: the current ZIP contains no legacy archive or
  legacy application, and the legacy ZIP contains no new workbench code.

The legacy ZIP retains its original Visual Studio 2022/v143 build configuration;
it was not rebuilt as part of this packaging check. Win32, Visual Studio 2022
and non-Windows configurations of the current source were not run here.

Host-specific verification overrides disabled optional MSBuild file tracking and
unrelated vcpkg integration. Those overrides are not embedded in the projects.

No third-party application libraries are required. Mathematical and parser limits
are described in README.md; this packaging change does not expand those limits.
