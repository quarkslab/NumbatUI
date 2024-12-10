# NumbatUI: source code and graph explorer

<img align="right" src="logo/numbatui_1024.png" width="250">

Numbat UI is a fork of [Sourcetrail](https://github.com/CoatiSoftware/Sourcetrail), a source code explorer that was archived in 2021. Thanks to NumbatUI, you can analyze and explore a project based on the analysis of its source code. 

More generally, NumbatUI has been created to extend Sourcetrail to be able to visualize and explore any graph. 
We also provide a user friendly API, [Numbat](https://quarkslab.github.io/numbat/), to generate NumbatUI projects and so explore any kind of data.

## Installation
As this fork is currently a WIP, the application should be compiled by the user. It has only be tested for Linux distributions.

### Requirements
For Ubuntu/Debian distributions:
- compilation (LLVM 17): `build-essential`, `cmake`, `clang-17`, `libclang-17-dev`
- tooling: `git`, `unzip`, `wget`
- boost: `libboost-filesystem-dev`, `libboost-program-options-dev`,`libboost-system-dev`, `libboost-date-time-dev`
- QT6: `qt6-svg-dev`, `qt6-base-dev`, `qt6-5compat-dev`

### Compilation
From the NumbatUI directory:
```
mkdir -p build/Release
cd build/Release
cmake -DCMAKE_BUILD_TYPE="Release" -DBUILD_CXX_LANGUAGE_PACKAGE=ON -DBUILD_PYTHON_LANGUAGE_PACKAGE=ON ../.. && make NumbatUI -j $(nproc)
```
The compiled binary is available in `build/Release/app`, this directory contains symlinks to the `bin/app` directory.

## Documentation

For the moment you can consult the old Sourcetrail documentation [here](./DOCUMENTATION.md), we will update it soon.

## Publications
N/A

## Authors
TODO