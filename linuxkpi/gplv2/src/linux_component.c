#include <linux/component.h>

DEFINE_MUTEX(component_mutex);

LIST_HEAD(linux_component_list);
LIST_HEAD(linux_component_masters);
