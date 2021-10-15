#ifndef _ASM_SET_MEMORY_H
#define _ASM_SET_MEMORY_H

int set_memory_wb(unsigned long addr, int numpages);
int set_memory_uc(unsigned long addr, int numpages);
int set_memory_wc(unsigned long addr, int numpages);

#endif
