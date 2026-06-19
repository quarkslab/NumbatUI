---
icon: lucide/file-terminal
---
# Installation

## Windows

Download and open the zip file and extract its contents into a temporary folder of your choice. Run the `setup.exe` and go through the wizard. You can now launch Sourcetrail from your start menu.

## macOS
Download and open the Sourcetrail.dmg file and drag Sourcetrail.app into the applications folder. You can now launch Sourcetrail from your Applications.

<img src="documentation/installation_mac.png" width="800" alt="Installation macOS">

## Linux

### Tarball

Download the `.tar.gz` file and extract it. To start Sourcetrail run the `Sourcetrail.sh` script. Sourcetrail creates a folder `~/.config/sourcetrail` at the first run, this is the folder for Sourcetrail settings.

#### Install
To install Sourcetrail run the `install.sh` script with `sudo`. It will install Sourcetrail to `/opt/sourcetrail` and create the `/usr/bin/sourcetrail` symlink.

#### Uninstall
To uninstall Sourcetrail run the `/opt/sourcetrail/uninstall.sh` script with `sudo`.

### AppImage

Download the `.AppImage` file. Give it permission to execute with `chmod a+x` or via the context menu. To start Sourcetrail double click it or execute it from the Terminal. Sourcetrail creates a folder `~/.config/sourcetrail` at the first run, this is the folder for Sourcetrail settings.

For more information on AppImages please visit [appimage.org](https://appimage.org/).

## Data folder

The data folder holds certain files that are used by Sourcetrail to run the program. After following the [installation instructions](#installation) the data folder should be located in the following locations on your platform.

| Platform | Location
| --- | ---
| Windows | `C:/Users/You/AppData/Local/Coati Software/Sourcetrail` _(used for dynamic data and settings)_ `install_directory/Coati Software/Sourcetrail/data` _(used for static app data)_
| macOS | `~/Library/Application Support/Sourcetrail`
| Linux | `~/.config/sourcetrail`

## Finding System Header Locations

### Windows

These files usually ship with your compiler. For the Visual Studio IDE the system headers can be found at:
`<path_to_visual_studio>/VC/include/`
If you don't use the Visual Studio IDE you can also try to find your system headers in a subdirectory of:
`C:/Program Files (x86)/Windows Kits/`

### macOS

Run this command in your terminal:
`gcc -x c++ -v -E /dev/null`
You will find the header search paths your compiler uses in the output between these two lines:

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
You will find the header search paths your compiler uses in the output between these two lines:

```
#include <...> search starts here:
.
.
.
End of search list.
```