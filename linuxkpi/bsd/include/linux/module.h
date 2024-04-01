#ifndef _BSD_LKPI_LINUX_MODULE_H_
#define	_BSD_LKPI_LINUX_MODULE_H_

#include <sys/param.h>
#include <sys/module.h>

// We need to create a unique variable name that we can make a global of and
// point THIS_MODULE at. We can't use KBUILD_MODNAME as it is a string literal,
// so instead we use LINUXKPI_PARAM_PREFIX which also is the name of the
// module.
#define THIS_MODULE_CONCAT_SUB(a, b) a##b
#define THIS_MODULE_CONCAT(...) THIS_MODULE_CONCAT_SUB(__VA_ARGS__)
#define THIS_MODULE_VAL THIS_MODULE_CONCAT(__this_module_, LINUXKPI_PARAM_PREFIX)
extern char *THIS_MODULE_VAL;

#if __FreeBSD_version >= 1500018
#define THIS_MODULE ((struct module *)&THIS_MODULE_VAL)
#endif

#include_next <linux/module.h>

#define	LKPI_DRIVER_MODULE(mod, init, exit)				\
	static int mod##_evh(module_t m, int e, void *a)		\
	{								\
		switch (e) {						\
		case MOD_LOAD:						\
			(init)();					\
			break;						\
		case MOD_UNLOAD:					\
			(exit)();					\
			break;						\
		}							\
		return (0);						\
	}								\
	static moduledata_t mod##_md =					\
		{							\
		 .name = #mod,						\
		 .evhand = mod##_evh,					\
		};							\
	char *THIS_MODULE_VAL = KBUILD_MODNAME;	\
	DECLARE_MODULE(mod, mod##_md, SI_SUB_DRIVERS, SI_ORDER_ANY);


#define	LKPI_PNP_INFO(bus, name, table)					\
	MODULE_PNP_INFO("U32:vendor;U32:device;",			\
	    bus, name, table, nitems(table) - 1);

#endif /* _BSD_LKPI_LINUX_MODULE_H_ */
