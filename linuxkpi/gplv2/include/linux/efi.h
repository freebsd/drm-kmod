#ifndef _LINUX_EFI_H_
#define _LINUX_EFI_H_

#include <linux/compiler.h>

#define EFI_BOOT 0

static inline int efi_enabled(int enabled){
	UNIMPLEMENTED();
	return (0);
}

#endif
