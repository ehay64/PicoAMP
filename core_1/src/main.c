/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 * Copyright (c) 2026 Eric Hay
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

static int i = 0;

int main(void)
{
	printk("Hello World from Core 1!\n");

	while (1) {
		k_msleep(1000);
		printk("Running main loop iteration %d\n", i++);
	}

	return 0;
}
