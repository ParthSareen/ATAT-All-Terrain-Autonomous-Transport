#include <zephyr.h>
#include <logging/log.h>
#include <sys/printk.h>

#define PWM_NODE DT_ALIAS(pwm-servo) // gotta fix this alias. pwm device must be configured using pwm-servo alias. Set up via device tree overlay, adding pwm-servo = &some_pwm_node to the aliases section.

#define PERIOD_USEC (20U * USEC_PER_MSEC)
#define STEP_USEC 100
#define MIN_PULSE_USEC 700
#define MAX_PULSE_USEC 2300

enum direction {
	DOWN,
	UP,
};

LOG_MODULE_REGISTER(main);

void main(void)
{
	const struct device *pwm;
	uint32_t pulse_width = MIN_PULSE_USEC;
	enum direction dir = UP;
	int ret;

	printk("Motor control\n");

	pwm = DEVICE_DT_GET(PWM_NODE);
	if (!device_is_ready(pwm)) {
		printk("PWM device %s not ready\n", pwm->name);
		return;
	}

	while (1) {
		LOG_INF("Running Main() \n");

		ret = pwm_pin_set_usec(pwm, 0, PERIOD_USEC, pulse_width, 0);
		if (ret < 0){
			printk("Error %d: failed to set pulse width\n", ret);
			return;
		}

		if (dir == DOWN){
			if(pulse_width <= MIN_PULSE_USEC){
				dir = UP;
				pulse_width = MIN_PULSE_USEC;
			} else {
				pulse_width -= STEP_USEC;
			}
		} else { 
			pulse_width += STEP_USEC;

			if (pulse_width >= MAX_PULSE_USEC) {
				dir = DOWN;
				pulse_width = MAX_PULSE_USEC;
			}
		}

		k_sleep(K_MSEC(1000));
	}
}