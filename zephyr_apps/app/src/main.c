#include <zephyr.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(main);

void main(void)
{
	while (1) {

		LOG_INF("Running Main() \n");
		k_sleep(K_MSEC(1000));
	}
}