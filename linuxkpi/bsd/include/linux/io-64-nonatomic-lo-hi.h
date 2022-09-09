#ifndef	_BSD_LKPI_LINUX_IO_64_NONATOMIC_LO_HI_H_
#define	_BSD_LKPI_LINUX_IO_64_NONATOMIC_LO_HI_H_

#include <sys/param.h>

#if __FreeBSD_version >= 1400067
#include_next <linux/io-64-nonatomic-lo-hi.h>
#else

#include <linux/io.h>

static inline uint64_t
lo_hi_readq(const volatile void *addr)
{
	const volatile uint32_t *p = addr;
	uint32_t l, h;

	__io_br();
	l = le32toh(__raw_readl(p));
	h = le32toh(__raw_readl(p + 1));
	__io_ar();

	return (l + ((uint64_t)h << 32));
}

static inline void
lo_hi_writeq(uint64_t v, volatile void *addr)
{
	volatile uint32_t *p = addr;

	__io_bw();
	__raw_writel(htole32(v), p);
	__raw_writel(htole32(v >> 32), p + 1);
	__io_aw();
}

#ifndef readq
#define readq(addr)	lo_hi_readq(addr)
#endif

#ifndef writeq
#define writeq(v, addr)	lo_hi_writeq(v, addr)
#endif

#endif /* __FreeBSD_version >= 1400067 */

#endif	/* _BSD_LKPI_LINUX_IO_64_NONATOMIC_LO_HI_H_ */
