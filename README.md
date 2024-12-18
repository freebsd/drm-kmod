# drm-kmod

The DRM drivers ported from Linux to FreeBSD using *linuxkpi*.

## Installing from sources

### Requirements

You need to have the sources of FreeBSD kernel available and they MUST
correspond to the installed version of FreeBSD.

There are several ways to get them:
* by using the FreeBSD installer
* by using the `FreeBSD-src-sys` package if you use PkgBase
* by cloning the Git repository from https://git.FreeBSD.org/src.git or any mirror

By default, Makefiles expect the sources of the kernel to be in `/usr/src/sys`.
The installer and the PkgBase package will install them in that directory. If
you clone from Git, you can set `SYSDIR=/path/to/FreeBSD/src/sys` on the
make(1) command line or in the environment when compiling the drivers and their
firmwares.

### Building

```sh
make -j12 DEBUG_FLAGS=-g SYSDIR=/usr/src/sys
```

As stated in the requirements section, set `SYSDIR` to the location where you
put the FreeBSD kernel sources. The example above shows the default value.

### Installing

```sh
sudo make install DEBUG_FLAGS=-g SYSDIR=/usr/src/sys KMODDIR=/boot/modules
```

As stated in the requirements section, set `SYSDIR` to the location where you
put the FreeBSD kernel sources. The example above shows the default value.

Likewise, set `KMODDIR` to the location of the kernel you want to install the
drivers for. The example above shows the default value which is the global
directory, used by all installed kernels

> [!IMPORTANT]
> The DRM drivers MUST be compiled against the kernel sources corresponding to
> the installed kernel you want to use them with.

### GPU firmwares

DRM drivers depend on binary firmwares. They are maintained in a separate Git
repository at https://github.com/freebsd/drm-kmod-firmware.

The binary firmwares are packaged into kernel modules and loaded automatically
by the DRM drivers when needed.

The kernel modules can be compiled by following the same instructions as the
DRM drivers. The same constraint applies: use the matching kernel sources.

## Contributing

> [!TIP]
> This part is a TL;DR version of the porting process. For the full version,
> please visit
> https://github.com/freebsd/drm-kmod/wiki/Porting-a-new-version-of-DRM-drivers-from-Linux.

1. Clone Linux:

    ```sh
    git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git /path/to/linux-src
    ```

2. Extract Linux patches:

    ```sh
    ./scripts/drmgeneratepatch \
        /path/to/linux-src \
        /path/to/patches-6.7 \
        v6.6..v6.7
    ```

3. Filter out already applied patches:

    ```sh
    ./scripts/drmcheckapplied /path/to/patches-6.7
    ```

4. Apply patches to `drm-kmod`:

    ```sh
    ./scripts/drmpatch /path/to/patches-6.7
    ```
