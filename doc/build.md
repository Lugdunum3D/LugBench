---
title: Building LugBench
menu:
- title: Documentation
  href: /doc
  class: documentation button button-green align-right
---

# How to build the LugBench application

## Dependencies for the LugBench application

### Introduction

Lugbench depends on many different libraries / projects in order to work properly.
You can find on our [ThirdParty repository](https://github.com/Lugdunum3D/LugBench-ThirdParty "third party lugbench repository") all the compiled versions, ready to use to compile Lugbench and get started quickly.

### List of dependencies

* [Lugdunum](https://github.com/Lugdunum3D/Lugdunum): Lugdunum is an open-source 3D engine using Vulkan as backend. Lugudunum's goal is to provide a free, modern, cross-platform (mobile and desktop) 3D engine for everyone.
* [Json _(from nlohmann)_](https://github.com/nlohmann/json) is a header-only Json library for Modern C++.
* [libcurl](https://curl.haxx.se/libcurl/) is a free and easy-to-use client-side URL transfer library
* [Restclient-cpp](https://github.com/mrtazz/restclient-cpp) This is a simple REST client for C++. It wraps libcurl for HTTP requests.

:::info
libcurl and restclient are not needed to build Lugbench on Android.
:::


## Cloning the repository

First, clone Lugbench repository:

```
git clone git@github.com:Lugdunum3D/LugBench.git
```

Now, to build Lugbench, you'll need to either have some dependencies installed, or you can automatically pull them from the `thirdparty` submodule, that regroups their pre-compiled versions to set you up more quickly:

```
git submodule update --init --recursive
```

:::info
You must first compile the Lugdunum libraries, as shown earlier in this document
:::


## <img src="https://upload.wikimedia.org/wikipedia/commons/3/35/Tux.svg" width="24"> Linux

### General prerequisites 

Target | Toolchain
-------|--------------
Linux  | gcc >= 6
Linux  | clang >= 3.8

### Building

The commands below should be distribution independant, hopefully. What we do is create a "build" directory (out-of-source build), `cd` in it and run `cmake` with the appropriate compiler versions and the location of the Lugdunum library.

```
mkdir build
cd build
cmake
    -DCMAKE_C_COMPILER=gcc-6
    -DCMAKE_CXX_COMPILER=g++-6
    -DLUG_ROOT=PATH_TO_LUGDUNUM_LIBRARY
    ../
make
```

:::info
Of course, CMAKE_C_COMPILER and CMAKE_CXX_COMPILER can be set to clang and clang++
:::


## <img src="https://upload.wikimedia.org/wikipedia/commons/e/ee/Windows_logo_%E2%80%93_2012_%28dark_blue%29.svg" width="24"> Windows

### General prerequisites

Target      | Toolchain
------------|--------------------
Windows 10  | Visual Studio 2015
Windows 10  | [Visual Studio 2017](https://www.visualstudio.com/downloads/)
 

### Building

To build Lugbench on Windows, you'll need [CMake](https://cmake.org/download/). CMake will generate a Visual Studio solution that you can then open, and build the project from.

In command line, you can generate the solution with:

```
mkdir build
cd build
cmake
    -G"Visual Studio 2017 15 Win64"
    -DLUG_ROOT=PATH_TO_LUGDUNUM_LIBRARY
    ../
```

`LUG_ROOT` designates the location of the Lugdunum library, which is required to build Lugbench. Steps for building the Lugdunum libraries were describes in the first part of this document.

Then, open the generated `Lugbench.sln` with Visual Studio and compile it.

#### Visual studio 2017
With the [recent support of CMake](https://blogs.msdn.microsoft.com/vcblog/2016/10/05/cmake-support-in-visual-studio/) in Visual Studio 2017, building and installing CMake projects is now possible directly within Visual Studio.
Just modify the CMake configuration file called `CMakeSettings.json` to change the install path.

```json
{
  "configurations": [
   {
    "name": "my-config",
    "generator": "Visual Studio 15 2017",
    "buildRoot": "${env.LOCALAPPDATA}\\CMakeBuild\\${workspaceHash}\\build\\${name}",
    "cmakeCommandArgs": "",
    "variables": [
     {
      "name": "LUG_ROOT",
      "value": "PATH_TO_LUGDUNUM_LIBRARY"
     }
    ]
  }
 ]
}
```

## <img src="https://upload.wikimedia.org/wikipedia/commons/d/d7/Android_robot.svg" width="24"> Android

### General prerequisites
* You must compile and install [Lugdunum](https://lugdunum3d.github.io) for Android.

:::info
We suppose that Lugdunum libraries for Android are built in _`ANDROID_NDK/sources/lugdunum`_
In case you specified a different path with `CMAKE_INSTALL_PREFIX`, you must modify the build.gradle accordingly.
:::


### Compiling

Open the folder `Lugbench/android` with Android Studio and let gradle configure the project.

:::info
If the NDK isn’t configured properly, you’ll have to tell Android Studio where to find it :
_`File > Project Structure > SDK Location > Android NDK Location`_
:::

The project should now be available as a target and be buildable from Android Studio.


## <img src="https://upload.wikimedia.org/wikipedia/commons/f/fa/Apple_logo_black.svg" width="24"> Apple macOS & iOS

These platforms are not yet supported, but they might be one day if Apple decides to support Vulkan on their systems.

