# Xe-Release

#### Xe-Release--a tool to generate version

##### I'm always on the way to find a breakthrough

How to compile: You can use Cmake or meson build system to compile it, but before the cmake command, the submodule should be initalized.

build with cmake:

```shell
mkdir build
cd build
cmake ..
make (or ninja, depends on the Generater of cmake)
```

build with meson:

```shell
meson build
cd build
ninja
```

~~Notice : The build system switched to cmake,use cmake to build binaries for any platform~~

~~this repository contain files for win64 and linux on amd64 and arm64~~

(The binaries have moved to release page)

Current version is xe-release 11(Gtk3 and Gtk4 Version);XeRelease 12(Gtkmm3 Version)

Notice:The Gtk4 version only supports linux

Current releases:

~~Longterm(5.x):2019-01-11 to now~~

Stable : Before 2021-02-07 : The start time is 2017-06-17

After 2021 : The date is changed to 2017-05-18

Development : 2017-05-18 to now
