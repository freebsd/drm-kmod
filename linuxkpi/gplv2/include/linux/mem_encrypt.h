#ifndef __MEM_ENCRYPT_H__
#define __MEM_ENCRYPT_H__

#ifdef CONFIG_ARCH_HAS_MEM_ENCRYPT

// XXX: not yet

#include <asm/mem_encrypt.h>

#else	/* !CONFIG_ARCH_HAS_MEM_ENCRYPT */

#define sme_me_mask	0ULL

#endif	/* CONFIG_ARCH_HAS_MEM_ENCRYPT */


#ifndef pgprot_encrypted
#define pgprot_encrypted(prot)	(prot)
#endif

#ifndef pgprot_decrypted
#define pgprot_decrypted(prot)	(prot)
#endif


#endif
