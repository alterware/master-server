[![build](https://github.com/alterware/master-server/workflows/Build/badge.svg)](https://github.com/alterware/master-server/actions)


# AlterWare: Master Server
This is the master server our clients use. It is based on the DP Master Server (ID Tech) protocol

## Build
- Install [Premake5][premake5-link] and add it to your system PATH
- Clone this repository using [Git][git-link]
- Update the submodules using ``git submodule update --init --recursive``
- Run Premake with either of these two options ``premake5 vs2022`` (Windows) or ``premake5 gmake2`` (Linux/macOS)

**IMPORTANT**
Requirements for Unix systems:
- Compilation: Please use Clang as the preferred compiler
- Dependencies: Ensure the LLVM C++ Standard library is installed
- Alternative compilers: If you opt for a different compiler such as GCC, use the [Mold][mold-link] linker
- Customization: Modifications to the Premake5.lua script may be required
- Platform support: Details regarding supported platforms are available in [build.yml][build-link]

[premake5-link]:          https://premake.github.io
[git-link]:               https://git-scm.com
[mold-link]:              https://github.com/rui314/mold
[build-link]:             https://github.com/alterware/master-server/blob/master/.github/workflows/build.yml
