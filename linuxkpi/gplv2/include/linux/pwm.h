#ifndef _LINUX_PWM_H_
#define _LINUX_PWM_H_

#include <linux/mutex.h>
#include <linux/device.h>

enum pwm_polarity {
        PWM_POLARITY_NORMAL,
        PWM_POLARITY_INVERSED,
};

struct pwm_args {
	unsigned int period;
	enum pwm_polarity polarity;
};

enum {
	PWMF_REQUESTED = 1 << 0,
	PWMF_EXPORTED = 1 << 1,
};

struct pwm_state {
	unsigned int period;
	unsigned int duty_cycle;
	enum pwm_polarity polarity;
	bool enabled;
};

struct pwm_device {
	const char *label;
	unsigned long flags;
	unsigned int hwpwm;
	unsigned int pwm;
	struct pwm_chip *chip;
	void *chip_data;

	struct pwm_args args;
	struct pwm_state state;
};


static inline void pwm_get_state(const struct pwm_device *pwm,
				 struct pwm_state *state)
{
	*state = pwm->state;
}

static inline int pwm_apply_state(struct pwm_device *pwm,
				  const struct pwm_state *state)
{
	UNIMPLEMENTED();
	return -ENOTSUPP;
}

static inline void
pwm_put(struct pwm_device *pwm)
{
	UNIMPLEMENTED();
}

static inline struct pwm_device *
pwm_get(struct device *dev, const char *con_id)
{
	UNIMPLEMENTED();
	return (NULL);
}

static inline int
pwm_enable(struct pwm_device *pwm)
{
	UNIMPLEMENTED();
	return (0);
}

static inline void pwm_disable(struct pwm_device *pwm)
{
	struct pwm_state state;

	if (!pwm)
		return;

	pwm_get_state(pwm, &state);
	if (!state.enabled)
		return;

	state.enabled = false;
	pwm_apply_state(pwm, &state);
}


static inline unsigned int
pwm_get_duty_cycle(const struct pwm_device *pwm)
{
	UNIMPLEMENTED();
	return (0);
}

static inline int
pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns)
{
	UNIMPLEMENTED();
	return (0);
}

static inline int pwm_set_polarity(struct pwm_device *pwm,
				   enum pwm_polarity polarity)
{
	struct pwm_state state;

	if (!pwm)
		return -EINVAL;

	pwm_get_state(pwm, &state);
	if (state.polarity == polarity)
		return 0;

	/*
	 * Changing the polarity of a running PWM without adjusting the
	 * dutycycle/period value is a bit risky (can introduce glitches).
	 * Return -EBUSY in this case.
	 * Note that this is allowed when using pwm_apply_state() because
	 * the user specifies all the parameters.
	 */
	if (state.enabled)
		return -EBUSY;

	state.polarity = polarity;
	return pwm_apply_state(pwm, &state);
}


static inline void pwm_apply_args(struct pwm_device *pwm)
{
	/*
	 * PWM users calling pwm_apply_args() expect to have a fresh config
	 * where the polarity and period are set according to pwm_args info.
	 * The problem is, polarity can only be changed when the PWM is
	 * disabled.
	 *
	 * PWM drivers supporting hardware readout may declare the PWM device
	 * as enabled, and prevent polarity setting, which changes from the
	 * existing behavior, where all PWM devices are declared as disabled
	 * at startup (even if they are actually enabled), thus authorizing
	 * polarity setting.
	 *
	 * Instead of setting ->enabled to false, we call pwm_disable()
	 * before pwm_set_polarity() to ensure that everything is configured
	 * as expected, and the PWM is really disabled when the user request
	 * it.
	 *
	 * Note that PWM users requiring a smooth handover between the
	 * bootloader and the kernel (like critical regulators controlled by
	 * PWM devices) will have to switch to the atomic API and avoid calling
	 * pwm_apply_args().
	 */
	pwm_disable(pwm);
	pwm_set_polarity(pwm, pwm->args.polarity);
}



#endif
