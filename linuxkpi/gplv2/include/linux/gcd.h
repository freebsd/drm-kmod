#ifndef _LINUX_GCD_H_
#define _LINUX_GCD_H_

static inline unsigned long
gcd(unsigned long a, unsigned long b)
{

	if (b == 0)
		return (a);
	return (gcd(b, a % b));
}

#endif
