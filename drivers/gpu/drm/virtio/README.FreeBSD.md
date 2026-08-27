# virtio-gpu DRM driver for FreeBSD (2D / KMS)

A port of Linux's `drivers/gpu/drm/virtio/` paravirtual display driver
into the drm-kmod framework, for FreeBSD 15.1 guests under QEMU/UTM
with a `virtio-gpu-pci` device.  Developed and tested on aarch64
(Apple-silicon UTM, QEMU/hvf); the code is arch-neutral.

**Scope: 2D only.**  KMS atomic modesetting, dumb buffers, fbdev
console, PRIME self-import — enough for a wlroots compositor with
`WLR_RENDERER=pixman` (sway, confirmed) and for `vt(4)` console on the
DRM framebuffer.  Out of scope: virgl/3D, Vulkan, blob resources,
multi-head.

Upstream base: Linux v6.12 driver sources against drm-kmod
`drm_v6.12.85_2` (DRM core 6.12.85).  The upstream `virtgpu_*.c` files
carry minimal, commented diffs; all FreeBSD-specific code lives in
separate files (see Architecture).

## Build

Requirements: `/usr/src` matching the running kernel, drm-kmod checkout.

```sh
cd ~/src/drm-kmod
make SYSDIR=/usr/src/sys -C dmabuf
make SYSDIR=/usr/src/sys -C drm
make SYSDIR=/usr/src/sys -C virtio      # -> virtio/virtio_gpu_drm.ko
```

Note: BSD make does not rebuild on CFLAGS changes — `make clean` first
when switching options.

## Install on a guest

The base `vtgpu(4)` framebuffer driver is compiled into GENERIC and
claims the device at boot.  Disable it and let this driver take over:

1. `/boot/loader.conf`:

   ```
   hint.vtgpu.0.disabled="1"
   ```

   (Console falls back to `vt_efifb` until the module loads, then
   hands off to `vt_drmfb`.)

   No interrupt tunable is needed.  The driver is purely
   interrupt-driven, like every other virtio driver.  (On FreeBSD
   under UTM/hvf, MSI/MSI-X is not allocated for any PCI device — a
   firmware/IORT quirk unrelated to this driver — so every device,
   this one included, runs on shared wired INTx, which delivers
   completions correctly.)

2. Copy `dmabuf.ko`, `drm.ko`, `virtio_gpu_drm.ko` to the guest
   (e.g. `/usr/local/kmods`).

3. `/etc/rc.conf`:

   ```
   kld_list="/path/to/dmabuf.ko /path/to/drm.ko /path/to/virtio_gpu_drm.ko"
   seatd_enable="YES"
   ```

   Absolute paths keep dependency loading in order without touching
   `kern.module_path`.  Load via `kld_list`, not `_load` entries in
   loader.conf: at loader time DRM core has not initialized yet and
   the probe fails with -19.

4. `/etc/rc.local` — the vtgpu hint also disables the newbus child
   device, so re-enable it after the module is in:

   ```sh
   #!/bin/sh
   devctl enable vtgpu0
   ```

After reboot: `/dev/dri/card0` exists, the console is on the DRM
framebuffer, and `drm_info` enumerates the connector/modes.

On plain QEMU (non-UTM), configure the display as `-vga virtio`
rather than a bare `-device virtio-gpu-pci`: with the latter, QEMU
also instantiates its default std-VGA device and X picks the wrong
one as primary.

Manual load (same order, then `devctl enable vtgpu0`) works too.
DRM device-node teardown is not survivable on drm-kmod, so the driver
**refuses to unload once attached**: `kldunload` (and `devctl detach`)
return EBUSY — reboot to unload.  A FAILED kldload also requires a
reboot before retrying (stale driver registration).

## Running sway

```sh
mkdir -p /tmp/xdg && chmod 700 /tmp/xdg
env XDG_RUNTIME_DIR=/tmp/xdg WLR_RENDERER=pixman LIBSEAT_BACKEND=seatd sway
```

Confirmed working: sway + swaybar + swaybg render, foot opens, keyboard
input works, `grim` screencopy works, live mode switching and custom
modes work.

HiDPI on a Mac host (UTM): enable the display's "Retina Mode" (maps
guest pixels 1:1 to panel pixels), keep the UTM window at its normal
point size, and let the guest render at 2x in `~/.config/sway/config`:

```
output Virtual-1 mode --custom 2560x1600@60Hz
output Virtual-1 scale 2
```

(2x the window's point size — or 2x the Mac's "looks like" resolution
when running fullscreen.  UTM does not push window-size changes to
plain virtio-gpu-pci, so the resolution is chosen guest-side.  Also
raise the default fonts, which read tiny at this density: foot.ini
`font=monospace:size=12` and sway `font pango:DejaVu Sans 11`.)

## Architecture

Two-translation-unit shim — both kernels define `struct virtqueue`, so
no single file may see both:

- `drivers/gpu/drm/virtio/virtgpu_freebsd.c` — FreeBSD-native newbus
  virtio child driver (`BUS_PROBE_VENDOR`, outranks vtgpu).  Owns the
  virtio 1.x status ladder for kldload-time attach, virtqueue
  allocation and interrupt arming, and its own notify-window mapping.
- `drivers/gpu/drm/virtio/linux_virtio.c` — the Linux virtio API
  surface (`virtqueue_add_sgs`, `virtqueue_get_buf`, kicks,
  `virtio_find_vqs`, shim `virtio_device`).  Talks to the native TU
  only through the opaque wrappers in `freebsd/virtgpu_freebsd.h`.
- `drivers/gpu/drm/virtio/freebsd/linux/*.h` — include-path-override
  shim headers (virtio core, config space access, dma-buf export).
- `drivers/gpu/drm/drm_gem_shmem_helper.c`, `drm_fbdev_shmem.c`,
  `drm_format_helper.c`, `drm_simple_kms_helper.c` — DRM-core pieces
  virtio needs that drm-kmod did not build; ported/enabled from Linux
  6.12 under their upstream licenses (the tree already builds
  GPL-2.0 helpers such as drm_writeback.c and sync_file.c).

Platform quirks the driver works around:

- vtpci's boot-created notify sub-map faults on UTM/QEMU-hvf — the
  driver maps the notify window itself and never calls
  `virtqueue_notify(9)`.
- FreeBSD virtqueues are created with interrupts disabled and native
  drivers arm them explicitly; Linux vrings start enabled.  The glue
  bridges this by calling virtqueue_enable_intr() at setup — without
  it no completion interrupt is ever delivered.
- FreeBSD allocates no MSI-X for any PCI device under UTM/hvf (QEMU
  exposes an ITS IORT node on this GICv2-only machine and the ACPI
  PCI host prefers IORT over the GICv2m fallback — unrelated to this
  driver), so the device runs on shared wired INTx like the guest's
  other virtio devices.
- Physically-contiguous segments must not coalesce across the
  device-readable/device-writable boundary (upstream's inline command
  responses); the shim's flattener enforces the split.

Diagnostics: `sysctl dev.virtio_gpu_drm.0.{reaps,queue_state}`.

## Files

| Path | Role |
|---|---|
| `virtio/Makefile` | kmod build (`virtio_gpu_drm.ko`) |
| `drivers/gpu/drm/virtio/virtgpu_*.c,h` | upstream driver, minimal diff |
| `drivers/gpu/drm/virtio/virtgpu_freebsd.c` | native attach/transport TU |
| `drivers/gpu/drm/virtio/linux_virtio.c` | Linux virtio API shim TU |
| `drivers/gpu/drm/virtio/freebsd/` | shim headers + cross-TU contract |
