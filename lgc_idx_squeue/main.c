/*
 * -----------------------------------------------------------------------+
 * file		: main.c
 * date		: 02/31/2020
 * compiler	: gcc
 * author	: Visakh Venugopal [visakh.vinayak@gmail.com]
 *
 * Entry point
 * -----------------------------------------------------------------------+
 */

#include <stdio.h>
#include "lgc_idx_squeue.h"

int g_log_LEVEL = 0;

int main(int argc, char *argv[])
{
	int i, j, len;
	char buffer[256];

	lgc_idx_squeue_init(3);
	for (i = 0; i < 3; i++) {
		for (j = 0; j < i * 2 + 1; j++) {
			len = sprintf(buffer, "%d-%d-%d", i, j, (i+j));
			lgc_idx_squeue_insert(i, (const void *) buffer, len);
		}
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < i * 2 + 1; j++) {
			lgc_idx_squeue_remove(i, (void *) buffer, sizeof(buffer));
			printf("%d:%d > %s\n", i, j, buffer);
		}
	}
	lgc_idx_squeue_destroy();
	return 0;
}

