#ifndef __LINUX_GPIO_CONSUMER_H
#define __LINUX_GPIO_CONSUMER_H

#include <linux/bug.h>
#include <linux/err.h>
#include <linux/kernel.h>

struct device;

/**
 * Opaque descriptor for a GPIO. These are obtained using gpiod_get() and are
 * preferable to the old integer-based handles.
 *
 * Contrary to integers, a pointer to a gpio_desc is guaranteed to be valid
 * until the GPIO is released.
 */
struct gpio_desc;

/**
 * Struct containing an array of descriptors that can be obtained using
 * gpiod_get_array().
 */
struct gpio_descs {
	unsigned int ndescs;
	struct gpio_desc *desc[];
};

#define GPIOD_FLAGS_BIT_DIR_SET		BIT(0)
#define GPIOD_FLAGS_BIT_DIR_OUT		BIT(1)
#define GPIOD_FLAGS_BIT_DIR_VAL		BIT(2)

/**
 * Optional flags that can be passed to one of gpiod_* to configure direction
 * and output value. These values cannot be OR'd.
 */
enum gpiod_flags {
	GPIOD_ASIS	= 0,
	GPIOD_IN	= GPIOD_FLAGS_BIT_DIR_SET,
	GPIOD_OUT_LOW	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT,
	GPIOD_OUT_HIGH	= GPIOD_FLAGS_BIT_DIR_SET | GPIOD_FLAGS_BIT_DIR_OUT |
			  GPIOD_FLAGS_BIT_DIR_VAL,
};

/* Return the number of GPIOs associated with a device / function */
int gpiod_count(struct device *dev, const char *con_id);

/* Acquire and dispose GPIOs */
static inline struct gpio_desc *
gpiod_get(struct device *dev, const char *con_id, enum gpiod_flags flags)
{
	UNIMPLEMENTED();
	return (ERR_PTR(-ENOENT));
}
static inline void
gpiod_put(struct gpio_desc *desc)
{
	UNIMPLEMENTED();
}

/* Value get/set from sleeping context */
int gpiod_get_value_cansleep(const struct gpio_desc *desc);

static inline void
gpiod_set_value_cansleep(struct gpio_desc *desc, int value)
{
	UNIMPLEMENTED();
}

void gpiod_set_array_value_cansleep(unsigned int array_size,
				    struct gpio_desc **desc_array,
				    int *value_array);

static inline struct gpio_desc *
devm_gpiod_get_index(struct device *dev,
		       const char *con_id,
		       unsigned int idx,
		       enum gpiod_flags flags)
{
	UNIMPLEMENTED();
	return ERR_PTR(-ENOSYS);
}

static inline void gpiod_set_value(struct gpio_desc *desc, int value)
{
	UNIMPLEMENTED();
	/* GPIO can never have been requested */
	WARN_ON(1);
}

#endif
