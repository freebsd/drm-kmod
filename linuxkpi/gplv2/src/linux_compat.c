#include <sys/param.h>
#include <sys/kernel.h>

#include <machine/specialreg.h>
#include <machine/md_var.h>

#include <linux/bitops.h>
#include <linux/idr.h>

#include <asm/processor.h>

struct cpuinfo_x86 boot_cpu_data;

struct ida *hwmon_idap;
DEFINE_IDA(hwmon_ida);

static void
linux_compat_init(void *arg __unused)
{

	if ((cpu_feature & CPUID_CLFSH) != 0)
		set_bit(X86_FEATURE_CLFLUSH, &boot_cpu_data.x86_capability);
	if ((cpu_feature & CPUID_PAT) != 0)
		set_bit(X86_FEATURE_PAT, &boot_cpu_data.x86_capability);
	boot_cpu_data.x86_clflush_size = cpu_clflush_line_size;
	boot_cpu_data.x86 = ((cpu_id & 0xf0000) >> 12) | ((cpu_id & 0xf0) >> 4);

	hwmon_idap = &hwmon_ida;
}
SYSINIT(linux_compat, SI_SUB_VFS, SI_ORDER_ANY, linux_compat_init, NULL);
