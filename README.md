[![build](https://github.com/alterware/master-server/workflows/Build/badge.svg)](https://github.com/alterware/master-server/actions)


# AlterWare: Master Server
This is the master server our clients use. It is based on the DP Master Server (ID Tech) protocol

## Build
- Install [*Premake5*](https://premake.github.io) and add it to your system PATH
- Clone this repository
- Update the submodules using ``git submodule update --init --recursive``
- Run Premake with either of these two options ``premake5 vs2022`` (Windows) or ``premake5 gmake2`` (Linux/macOS)

**IMPORTANT**
For Unix systems, you must use Clang to compile this project. You will also be required to install the LLVM C++ Standard library to run this program.
If you need to use another compiler, like GCC, you must use the [Mold](https://github.com/rui314/mold) linker. Additionally, you may have to tweak the Premake5.lua script.
I only support a few platforms, for more details see [build.yml](https://github.com/alterware/master-server/blob/master/.github/workflows/build.yml)
