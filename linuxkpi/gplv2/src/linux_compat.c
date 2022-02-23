#include <sys/param.h>
#include <sys/kernel.h>
#if defined(__i386__) || defined(__amd64__)
#include <sys/pcpu.h>
#include <machine/specialreg.h>
#include <machine/md_var.h>
#endif
#include <linux/bitops.h>
#include <linux/idr.h>
#include <linux/pci.h>

#include <asm/processor.h>


#if defined(__i386__) || defined(__amd64__)
struct cpuinfo_x86 boot_cpu_data;

static void
linux_compat_init(void *arg __unused)
{

	boot_cpu_data.x86_clflush_size = cpu_clflush_line_size;
	boot_cpu_data.x86 = ((cpu_id & 0xf0000) >> 12) | ((cpu_id & 0xf0) >> 4);
}
SYSINIT(linux_compat, SI_SUB_VFS, SI_ORDER_ANY, linux_compat_init, NULL);
#endif
