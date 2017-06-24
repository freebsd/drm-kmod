#ifndef _ASM_CPUFEATURE_H_
#define _ASM_CPUFEATURE_H_


#define test_cpu_cap(c, bit)						\
	 test_bit(bit, (unsigned long *)((c)->x86_capability))


#define REQUIRED_MASK_BIT_SET(bit)					\
	 ( (((bit)>>5)==0  && (1UL<<((bit)&31) & REQUIRED_MASK0 )) ||	\
	   (((bit)>>5)==1  && (1UL<<((bit)&31) & REQUIRED_MASK1 )) ||	\
	   (((bit)>>5)==2  && (1UL<<((bit)&31) & REQUIRED_MASK2 )) ||	\
	   (((bit)>>5)==3  && (1UL<<((bit)&31) & REQUIRED_MASK3 )) ||	\
	   (((bit)>>5)==4  && (1UL<<((bit)&31) & REQUIRED_MASK4 )) ||	\
	   (((bit)>>5)==5  && (1UL<<((bit)&31) & REQUIRED_MASK5 )) ||	\
	   (((bit)>>5)==6  && (1UL<<((bit)&31) & REQUIRED_MASK6 )) ||	\
	   (((bit)>>5)==7  && (1UL<<((bit)&31) & REQUIRED_MASK7 )) ||	\
	   (((bit)>>5)==8  && (1UL<<((bit)&31) & REQUIRED_MASK8 )) ||	\
	   (((bit)>>5)==9  && (1UL<<((bit)&31) & REQUIRED_MASK9 )) ||	\
	   (((bit)>>5)==10 && (1UL<<((bit)&31) & REQUIRED_MASK10)) ||	\
	   (((bit)>>5)==11 && (1UL<<((bit)&31) & REQUIRED_MASK11)) ||	\
	   (((bit)>>5)==12 && (1UL<<((bit)&31) & REQUIRED_MASK12)) ||	\
	   (((bit)>>5)==13 && (1UL<<((bit)&31) & REQUIRED_MASK13)) ||	\
	   (((bit)>>5)==13 && (1UL<<((bit)&31) & REQUIRED_MASK14)) ||	\
	   (((bit)>>5)==13 && (1UL<<((bit)&31) & REQUIRED_MASK15)) ||	\
	   (((bit)>>5)==14 && (1UL<<((bit)&31) & REQUIRED_MASK16)) )


#define cpu_has(c, bit)							\
	(__builtin_constant_p(bit) && REQUIRED_MASK_BIT_SET(bit) ? 1 :	\
	 test_cpu_cap(c, bit))


#define boot_cpu_has(bit)	cpu_has(&boot_cpu_data, bit)

/*
 * Do not add any more of those clumsy macros - use static_cpu_has() for
 * fast paths and boot_cpu_has() otherwise!
 */

#if defined(CC_HAVE_ASM_GOTO) && defined(CONFIG_X86_FAST_FEATURE_TESTS)
/*
 * Static testing of CPU features.  Used the same as boot_cpu_has().
 * These will statically patch the target code for additional
 * performance.
 */
static __always_inline __pure bool _static_cpu_has(u16 bit)
{
		asm_volatile_goto("1: jmp 6f\n"
			 "2:\n"
			 ".skip -(((5f-4f) - (2b-1b)) > 0) * "
			         "((5f-4f) - (2b-1b)),0x90\n"
			 "3:\n"
			 ".section .altinstructions,\"a\"\n"
			 " .long 1b - .\n"		/* src offset */
			 " .long 4f - .\n"		/* repl offset */
			 " .word %P1\n"			/* always replace */
			 " .byte 3b - 1b\n"		/* src len */
			 " .byte 5f - 4f\n"		/* repl len */
			 " .byte 3b - 2b\n"		/* pad len */
			 ".previous\n"
			 ".section .altinstr_replacement,\"ax\"\n"
			 "4: jmp %l[t_no]\n"
			 "5:\n"
			 ".previous\n"
			 ".section .altinstructions,\"a\"\n"
			 " .long 1b - .\n"		/* src offset */
			 " .long 0\n"			/* no replacement */
			 " .word %P0\n"			/* feature bit */
			 " .byte 3b - 1b\n"		/* src len */
			 " .byte 0\n"			/* repl len */
			 " .byte 0\n"			/* pad len */
			 ".previous\n"
			 ".section .altinstr_aux,\"ax\"\n"
			 "6:\n"
			 " testb %[bitnum],%[cap_byte]\n"
			 " jnz %l[t_yes]\n"
			 " jmp %l[t_no]\n"
			 ".previous\n"
			 : : "i" (bit), "i" (X86_FEATURE_ALWAYS),
			     [bitnum] "i" (1 << (bit & 7)),
			     [cap_byte] "m" (((const char *)boot_cpu_data.x86_capability)[bit >> 3])
			 : : t_yes, t_no);
	t_yes:
		return true;
	t_no:
		return false;
}

#define static_cpu_has(bit)					\
(								\
	__builtin_constant_p(boot_cpu_has(bit)) ?		\
		boot_cpu_has(bit) :				\
		_static_cpu_has(bit)				\
)
#else
/*
 * Fall back to dynamic for gcc versions which don't support asm goto. Should be
 * a minority now anyway.
 */
#define static_cpu_has(bit)		boot_cpu_has(bit)
#endif

#endif
