#ifndef _ASM_X86_MTRR_H
#define _ASM_X86_MTRR_H


#define MTRR_TYPE_UNCACHABLE 0
#define MTRR_TYPE_WRCOMB     1

#define MTRR_TYPE_WRTHROUGH  4
#define MTRR_TYPE_WRPROT     5
#define MTRR_TYPE_WRBACK     6
#define MTRR_NUM_TYPES       7


int arch_phys_wc_add(unsigned long base, unsigned long size);
void arch_phys_wc_del(int handle);

#define arch_phys_wc_index(x) (x)

#endif
