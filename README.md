<h1 align="center">occutils</h1>

<div align="center">
  <p>A utility library providing high-level functions and algorithms to empower <a href="https://opencascade.com">OpenCASCADE</a>-based engineering tasks, such as mesh generation, shape modeling, Boolean operations, intersection and distance calculations, and CAD data import/export.</p> 
</div>

<div align="center">

<a href="https://github.com/paulbuechner/occutils">
<img alt="GitHub Workflow Status CI" src="https://img.shields.io/github/actions/workflow/status/paulbuechner/occutils/quality.yml?label=CI&style=for-the-badge">
</a>
<a href="https://github.com/paulbuechner/occutils">
<img alt="GitHub Workflow Status Release" src="https://img.shields.io/github/actions/workflow/status/paulbuechner/occutils/release.yml?style=for-the-badge">
</a>
<a href="https://github.com/paulbuechner/occutils/releases/latest">
<img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/paulbuechner/occutils?style=for-the-badge">
</a>
<a href="https://app.codacy.com/gh/paulbuechner/occutils/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade">
<img alt="Codacy Code Quality" src="https://img.shields.io/codacy/grade/f043a370fbb64edf96a2d8abc1fab4a0?style=for-the-badge"/>
</a>
<a href="https://github.com/paulbuechner/occutils/blob/main/LICENSE">
<img alt="Github occutils License" src="https://img.shields.io/github/license/paulbuechner/occutils?style=for-the-badge">
</a>

</div>

## Design goals

`occutils` strives to be a contemporary, user-friendly, and modular utility
library for OpenCASCADE, specifically designed for seamless integration and
enhanced functionality:

* **Simple to use**: Most tasks should be accomplishable in *one line of code*.
* **Aid rapid development**: No need to write tons of utility functions; no
  need to wait for long compile-times
* **Modular**: Pull in only what you need, or just copy the underlying
  sourcecode.
* **Clear**: What you write should be what you mean: `Edge::FromPoints()`
  instead of `BRepBuilderAPI_MakeEdge` three-liners.
* **High-Level**: Common tasks in 3D engineering should be accomplishable
  without diving into low level OpenCASCADE code
* **Modern**: Uses features from C++17, because those make your code more
  readable.
* **Minimal dependencies**: Only depends on OpenCASCADE and a few Boost
  packages.
* **Liberally licensed**: occutils is licensed under Apache License v2.0,
  allowing you to copy & modify the sourcecode for use in your commercial
  projects for free. Keep in mind that OpenCASCADE is still LGPL licensed.

Note that occutils is very young and, although it is used in multiple production
projects, it might not have all the functionality you want or need, and might
have some bugs.

If you are missing functionality, feel free to submit an issue or pull request.
For more information on how to contribute, see the
[CONTRIBUTING](#contributing) section.

## Usage samples

#### Topology creation

```cpp
#include <occutils/occutils-edge.h>
#include <occutils/occutils-face.h>
#include <occutils/occutils-wire.h>

#include <gp_Pnt.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>

int main() {
  // Create points to build edges from
  gp_Pnt aPoint(0, 0, 0);
  gp_Pnt anotherPoint(1, 0, 0);
  gp_Pnt yetAnotherPoint(0, 1, 0);
  // Create edges from points
  TopoDS_Edge aEdge = occutils::edge::FromPoints(aPoint, anotherPoint);
  TopoDS_Edge anotherEdge = occutils::edge::FromPoints(anotherPoint, yetAnotherPoint);
  TopoDS_Edge yetAnotherEdge = occutils::edge::FromPoints(yetAnotherPoint, aPoint);
  // Create a wire from edges
  TopoDS_Wire aWire = occutils::wire::FromEdges({aEdge, anotherEdge, yetAnotherEdge});
  // Create a face from a wire
  TopoDS_Face aFace = occutils::face::FromWire(aWire);
}
```

---

#### Boolean operations

```cpp
#include <occutils/occutils-boolean.h>
#include <occutils/occutils-primitive.h>

#include <TopoDS_Shape.hxx>

int main() {
  // Create two shapes
  TopoDS_Shape aBox = occutils::primitive::MakeBox(2, 2, 2)
  TopoDS_Shape anotherBox = occutils::primitive::MakeBox(1, 1, 1)
  // Perform a boolean operation
  TopoDS_Shape aShape = occutils::boolean::Cut(aBox, anotherBox);
}
```

## Setup

### Prerequisites

To build occutils you need to have the following prerequisites set up:

#### Compiler

The project uses C++17 features and therefore requires a compiler that supports
this standard.

Compiler compatibility (tested):

- Clang/LLVM >= 6
- MSVC++ >= 14.11 / Visual Studio >= 2017
- GCC >= 9

#### CMake

The project uses CMake as a build system. CMake version 3.25 or higher is
required.

#### vcpkg

This project is build with CMake and uses the
[vcpkg](https://vcpkg.io/en/index.html) package manager to install the required
dependencies. The project is configured to use vcpkg as a submodule, allowing it
to integrate with the CI/CD pipeline. Feel free to use this setup or use your
own vcpkg installation.

To fetch the vcpkg submodule run:

```shell
git submodule update --init --recursive
```

You can follow the steps to set up vcpkg on your
system [here](https://vcpkg.io/en/getting-started.html).

> Note: For WSL users, make sure to install vcpkg in a directory which grants
> permission for all users. Otherwise, you will get an error when trying to
> access and install vcpkg packages through a non-root user.

#### Python

The OpenCascade build requires python with a version of at least 3.7. However,
this is only needed when building on Linux.

To ensure your python3 points to the correct version run:

```shell
ls -l /usr/bin/python3
lrwxrwxrwx 1 root root 9  7. Jan 15:51 /usr/bin/python3 -> python3.7 # required python version >= 3.7
```

#### OpenCascade

The project uses OpenCascade as a geometry kernel. It is automatically
downloaded and build by `vcpkg` package manager.

To build OpenCascade on UNIX like systems, install the following packages:

<details><summary>Ubuntu</summary>
<p>

```shell
sudo apt-get install software-properties-common
sudo apt-get install libtool autoconf automake gfortran gdebi
sudo apt-get install gcc-multilib libxi-dev libxmu-dev libxmu-headers
sudo apt-get install libx11-dev mesa-common-dev libglu1-mesa-dev
sudo apt-get install libfontconfig1-dev
```

The minimum requirements for third party dependencies to run OpenCascade itself
is Freetype 2.5 and Tcl/TK 8.6:

```shell
sudo apt-get install libfreetype6 libfreetype6-dev
sudo apt-get install tcl tcl-dev tk tk-dev
```

</p>
</details>

<details><summary>openSUSE</summary>
<p>

> Note: The documentation is not complete yet. Feel free to contribute.

```shell
sudo zypper install libX11-devel Mesa-libHL-devel libXmu-devel libXi-devel
sudo zypper install bison fontconfig-devel 
```

The minimum requirements for third party dependencies to run OpenCascade itself
is Freetype 2.5 and Tcl/TK 8.6:

```shell
sudo apt-get install libfreetype6 libfreetype6-devel
sudo apt-get install tcl tcl-devel tk tk-devel
```

</p>
</details>

<details><summary>OSX</summary>
<p>

> Note: The documentation is not complete yet. Feel free to contribute.

</p>
</details>

### Build

occutils uses CMake as a build system. There are two ways to build project:

#### Using CMakePresets

The first option is to make use of the preconfigured `CMakePresets.json` presets
to build the project. This is the recommended way to build the project, as it
reflects the CI/CD pipeline.

To configure the project run:

```shell
cmake --preset occutils-ninja-multiconfiguration-vcpkg-{{ $os }} # $os = linux, windows, or macos

# Example:
cmake --preset occutils-ninja-multiconfiguration-vcpkg-windows
```

**Note**: The default value for `-DCMAKE_TOOLCHAIN_FILE` refers to
the `submodule` initialized vcpkg installation. If you want to use your own
vcpkg installation, change the value of `-DCMAKE_TOOLCHAIN_FILE` to reflect the
path of your vcpkg installation:

```shell
cmake --preset occutils-ninja-multiconfiguration-vcpkg-{{ $os }} -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

To build the project run:

```shell
cmake --build --preset occutils-ninja-multiconfiguration-vcpkg-{{ $os }} # $os = linux, windows, or macos

# Example:
cmake --build --preset occutils-ninja-multiconfiguration-vcpkg-windows
```

**Note**: The default build type is `Release`. To build a different build type,
use the `--config` flag. For example, to build the `Debug` build type, run:

```shell
cmake --build --preset occutils-ninja-multiconfiguration-vcpkg-{{ $os }} --config Debug
```

#### Using plain CMake

Another option is to use plain CMake to build the project. This is useful if you
want to have full control over the build process and underlying configuration.

To configure the project run:

```shell
cmake -S . -B {{ $build_dir }}/{{ $build_type }} -DCMAKE_BUILD_TYPE={{ $build_type }} -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -DOCCUTILS_BUILD_TESTS=ON -DOCCUTILS_BUILD_WARNINGS=ON

# Example:
cmake -S . -B build/Release -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -DOCCUTILS_BUILD_TESTS=ON -DOCCUTILS_BUILD_WARNINGS=ON
```

> Note: Tweak the passed CMake options to your needs,
> whereas `-DOCCUTILS_BUILD_TESTS` decides whether to build the tests and
> `-DOCCUTILS_BUILD_WARNINGS` decides whether to build the project with compiler
> warnings enabled.

> Note: To configure the project on macOS, you need to pass
> the `-DVCPKG_INSTALL_OPTIONS=--allow-unsupported` flag to the CMake command.

To build the project run:

```shell
cmake --build {{ $build_dir }}/{{ $build_type }} --config {{ $build_type }}

# Example:
cmake --build build/Release --config Release
```

## Installation

### System-wide install

The following instructions guide you through the process of
installing `occutils` library system-wide. This is useful if you want to use the
library in other projects.

#### Official GitHub release

The first option is to download the latest release for your platform from the
libraries [GitHub releases page](https://github.com/paulbuechner/occutils/releases/latest).

#### Build and install from source

The second option is to build and install the library from source. To do so,
follow the instructions in the [Build](#build) section. After the build is
complete, run:

```shell
cmake --install {{ $build_dir }}/{{ $build_type }}

# Example:
cmake --install build/Release
```

This will install the library to the system-wide installation directory. On
Windows, this is `C:\Program Files\occutils`. On Linux and macOS, this is
`/usr/local/lib/occutils`.

#### vcpkg

Coming soon.

### Per project install

#### Git submodule

This method involves adding the repository and building it as a subproject of
your CMake-based main project.

To add the repository as a submodule, run:

```shell
git submodule add https://github.com/paulbuechner/occutils.git
```

Then include the library in your CMake project by adding the following lines to
your `CMakeLists.txt`:

```cmake
add_subdirectory(occutils)
```

Lastly, link your project with the `occutils` library. This will set up the
necessary include directories and link the library to your project:

```cmake
target_link_libraries(${PROJECT_NAME} PRIVATE occutils::occutils)
```

#### vcpkg

Coming soon.

# Contributing

Pull requests are welcome. Please open an issue first to discuss what you would
like to change for major changes.

Please read [CONTRIBUTING](CONTRIBUTING.md) for details on the code of conduct,
and the process for submitting pull requests.

# License

This project is licensed under the [Apache License 2.0](LICENSE).
