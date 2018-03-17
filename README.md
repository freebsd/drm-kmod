# kms-drm
The DRM part of the linuxkpi-based KMS

## Contributing

### Linux source code
Folders `amd`, `drm`, `i915`, `radeon`

These folders contain source code from Linux, patched (minimally) to run on FreeBSD with LinuxKPI.
Try to keep changes to the code to a minimum but if you have to patch it, leave the Linux source code intact like so:

```
#ifdef __linux__
...intact linux code...
#else
bsd code
#endif
```

When updating and patching this code with code from Linux there are often merge conflicts where the code has been changed. If there are no markers it is difficult to know what code to keep and what to throw away.

Pull requests that do not follow this will not be accepted. 

Use `#if(n)def __linux__` and not `__FreeBSD__` to respect the other BSDs.

Unless obvious what your code does, please leave a comment to explain to fellow developers why you patched it. The source code is the documentation!

If the patch can be avoided by adding functionality to LinuxKPI, please consider the latter. Sooner or later, there will be more places where the functionality is used and having it in LinuxKPI mean we don't have to patch twice.

### FreeBSD source code
Folders `lindebugfs`, `linuxkpi`

Code style and rules same as FreeBSD kernel. If a GPL'd file is copy-paste from Linux, it's OK to leave style as is.



## Testing
For testing work in progress branches please see the wiki:  
https://github.com/FreeBSDDesktop/kms-drm/wiki/Testing  
