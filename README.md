# kms-drm
The DRM part of the linuxkpi-based KMS

## Development branches
For testing the 4.11 development branch please use 

#### FreeBSD 12-CURRENT
Needed for changes in non-gplv2 part of linuxkpi.  
https://github.com/FreeBSDDesktop/freebsd-base-graphics/tree/linuxkpi411

#### Kernel Modules
**Important!** Build with SYSDIR pointing to the above branch.  
https://github.com/FreeBSDDesktop/kms-drm/tree/linuxkpi411

#### GPU Firmware 
https://github.com/FreeBSDDesktop/kms-firmware/tree/linuxkpi411

### Known bugs
- i915 is leaking memory (gem objects).
- DMC & HuC firmware loading seem fine but GuC loading sometimes fails but it is also disabled by default in Linux so might be upstream error.  
Enable by adding to your /boot/loader.conf  
compat.linuxkpi.enable_guc_submission=2  
compat.linuxkpi.enable_guc_loading=2
- ???
