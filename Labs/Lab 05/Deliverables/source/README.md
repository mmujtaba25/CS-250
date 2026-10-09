# cpp-template

C++ template with `vcpkg` as `git submodule` and setup with `CMake`.

- Project Works on MacOS and Windows
- Uses `vcpkg`

## Prerequisites

- Windows:
  - Visual Studio
  - x64 Command Lines Tools For Visual Studio
- MacOS:
  - `clang++` in CLI
- `ninja` in CLI
- `cmake` in CLI
- `git` in CLI
  
## How to Run

> On Windows use **x64 Command Lines Tools For Visual Studio**.

- Run `./setup` to setup the project *(installs fmt as example also)*
- use `./run` for running afterwards

## Installing `vcpkg` Packages

- For **Windows** run `.\external\vcpkg\vcpkg.exe install package_name`
- For **MacOS** run `./external/vcpkg/vcpkg install package_name`
