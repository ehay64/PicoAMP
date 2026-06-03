/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 * Copyright (c) 2026 Eric Hay
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/ipm.h>

static int i = 0;

static const struct device *ipm_dev;

static void ipm_callback(const struct device *dev, void *context, uint32_t id, volatile void *data)
{
	int received_value = *(int *)data;
	printk("Received IPM message with value: %d\n", received_value);
}

int main(void)
{
	int status;

	printk("Hello World from Core 0!\n");

	ipm_dev = DEVICE_DT_GET(DT_NODELABEL(ipm_mbox0));
	if (!ipm_dev) {
		printk("Failed to get IPM device\n");
		return -1;
	}

	ipm_register_callback(ipm_dev, ipm_callback, NULL);

	status = ipm_set_enabled(ipm_dev, 1);
	if (status != 0) {
		printk("Failed to enable IPM device\n");
		return -1;
	}

	while (1) {
		k_msleep(5000);
		printk("Running main loop iteration %d\n", i++);

		status = ipm_send(ipm_dev, -1, 0, &i, sizeof(i));
		if (status != 0) {
			printk("Failed to send IPM message\n");
			return -1;
		}
	}

	return 0;
}
