---
icon: lucide/file-terminal
---
# Installation

!!! warning
    NumbatUI is in active development and still unstable. We recommend waiting
    for our first releases before relying on it for production work.

NumbatUI does not yet ship prebuilt installers. There are three supported ways
to get it running, in increasing order of integration with your system:

* the **Docker image**, which bundles every dependency;
* a **manual build** from source on Linux or macOS;
* a native **Debian package** (`.deb`) on Debian/Ubuntu.

## Docker 

The Docker image contains all required dependencies. Either build it locally:

```bash
cd docker
docker build -t numbatui .
```

or pull it from the GitHub Container Registry:

```bash
docker pull ghcr.io/quarkslab/numbatui:main
```

NumbatUI is a Qt GUI application, so the container needs access to your display
server. Open a `*.srctrlprj` database by mounting its directory and forwarding
X11 or Wayland.

=== "X11"

    You may need to allow `docker` to use your Xserver first:

    ```bash
    xhost +local:docker
    ```

    ```bash
    docker run -it --rm \
               --env DISPLAY=$DISPLAY \
               --env QT_X11_NO_MITSHM=1 \
               --volume /tmp/.X11-unix:/tmp/.X11-unix \
               --volume /path/to/:/data/ \
               numbatui:latest /data/my_database.srctrlprj
    ```

    After usage you can revoke the authorization with `xhost -local:docker`.

=== "Wayland"

    ```bash
    docker run -it --rm \
               --env QT_QPA_PLATFORM=wayland \
               --env XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
               --env WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
               --volume /run/user/1000/$WAYLAND_DISPLAY:/run/user/1000/$WAYLAND_DISPLAY \
               --volume /usr/share/wayland-sessions:/usr/share/wayland-sessions \
               --volume /path/to/:/data/ \
               numbatui:latest /data/my_database.srctrlprj
    ```

!!! note "Docker on macOS"
    The image is Linux-only and talks to X11/Wayland, neither of which is
    native on macOS. Running it there requires installing
    [XQuartz](https://www.xquartz.org/) and forwarding the display over TCP —
    workable but slow. On macOS, prefer the native build below.

## Manual build

Build instructions are provided for Linux and macOS.

### Requirements

=== "Linux (Ubuntu/Debian)"

    * base tooling: `build-essential`, `cmake`, `git`, `unzip`, `wget`
    * LLVM/Clang 19: `clang-19`, `llvm-19-dev`, `libclang-19-dev`,
      `liblld-19-dev`, `libc++-19-dev`
    * Boost: `libboost-filesystem-dev`, `libboost-program-options-dev`,
      `libboost-system-dev`, `libboost-date-time-dev`
    * Qt6: `qt6-base-dev`, `qt6-svg-dev`, `qt6-5compat-dev`

=== "macOS (Homebrew)"

    Tested on Apple Silicon (arm64) with macOS 26 / Tahoe and Homebrew Qt 6.11.

    ```bash
    brew install cmake boost qt@6
    # Only needed if you re-enable BUILD_CXX_LANGUAGE_PACKAGE:
    # brew install llvm@19
    ```

### Compilation

=== "Linux"

    ```bash
    git clone --recurse-submodules https://github.com/quarkslab/NumbatUI.git
    cd NumbatUI

    cmake -B build \
          -DCMAKE_BUILD_TYPE="Release" \
          -DBUILD_CXX_LANGUAGE_PACKAGE=OFF \
          -DBUILD_PYTHON_LANGUAGE_PACKAGE=OFF

    cmake --build build --target NumbatUI -j $(nproc)
    ```

=== "macOS"

    Point CMake at the Homebrew Qt6 install and use `sysctl` for the parallel
    job count:

    ```bash
    git clone --recurse-submodules https://github.com/quarkslab/NumbatUI.git
    cd NumbatUI

    cmake -B build \
          -DCMAKE_BUILD_TYPE="Release" \
          -DBUILD_CXX_LANGUAGE_PACKAGE=OFF \
          -DBUILD_PYTHON_LANGUAGE_PACKAGE=OFF \
          -DCMAKE_PREFIX_PATH=$(brew --prefix qt@6)

    cmake --build build --target NumbatUI -j $(sysctl -n hw.logicalcpu)
    ```

The compiled binary is available at `build/app/NumbatUI` on Unix (or
`build/Release/app/` on Windows). Launch it directly:

```bash
./build/app/NumbatUI                                # macOS / Linux
./build/app/NumbatUI /path/to/my_database.srctrlprj # open a project
```

On macOS the binary is a native arm64 (or x86_64) Mach-O — no X11 / XQuartz
forwarding required, unlike the Docker image.

!!! note
    This build purposely disables the C++ and Python language indexing
    features. C++ indexing is being re-enabled (see
    [Supported Languages](!index#supported-languages)); when built with
    `-DBUILD_CXX_LANGUAGE_PACKAGE=ON` it targets LLVM/Clang 19 as a hard
    minimum.

## Debian package (.deb)

On Debian/Ubuntu, NumbatUI can be installed as a native `.deb` package. This
installs the GUI and indexer to `/usr/bin`, the runtime data to
`/usr/share/numbatui`, and registers the desktop entry, MIME association
(`*.srctrlprj`) and application icons.

### Install a prebuilt package

Every push to the `main` and `dev` branches builds a `.deb` in CI. Download it
from the **Actions** tab: open the latest *Build Debian package* run and grab
the `numbatui-deb-<branch>-<sha>` artifact (tagged `v*` releases also attach the
`.deb` to the GitHub release). Then:

```bash
sudo apt install ./numbatui_*_amd64.deb
```

`apt` pulls in the required Qt 6 and Boost runtime libraries automatically.

### Build the package yourself

From a checkout of the repository:

```bash
# one-off: install the build dependencies declared in debian/control
sudo apt build-dep .
# or, without a deb-src entry:
#   sudo apt install devscripts equivs && sudo mk-build-deps -i debian/control

# build the binary package (the .deb is written to the parent directory)
dpkg-buildpackage -us -uc -b
ls ../numbatui_*_amd64.deb
```

The packaging lives in `debian/` and drives the same CMake build as the manual
build through `debhelper`. See
[`debian/README.source`](https://github.com/quarkslab/NumbatUI/blob/dev/debian/README.source)
for the packaging design and how to bump the package version.

## Data folder

On first launch NumbatUI creates a configuration folder holding settings and a
copy of its user templates. After installation the data folder is located here:

| Platform | Location
| --- | ---
| Linux   | `~/.config/numbatui`
| macOS   | `~/Library/Application Support/numbatui`

A `.deb` install additionally ships the static runtime data under
`/usr/share/numbatui` and the per-user template tree that is copied into
`~/.config/numbatui` on first launch.

## Finding System Header Locations

When indexing C/C++ projects without a compilation database, NumbatUI needs to
know where your compiler's system headers live.

### macOS

Run this command in your terminal:
`gcc -x c++ -v -E /dev/null`
You will find the header search paths your compiler uses in the output between
these two lines:

```
#include <...> search starts here:
.
.
.
End of search list.
```

### Linux

`gcc -x c++ -v -E /dev/null`
or
`clang -x c++ -v -E /dev/null`
You will find the header search paths your compiler uses in the output between
these two lines:

```
#include <...> search starts here:
.
.
.
End of search list.
```
