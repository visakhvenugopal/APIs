/*
 * -----------------------------------------------------------------------+
 * file		: lgc_indexed_squeue.c
 * header	: lgc_indexed_squeue.h
 * date		: 02/31/2020
 * compiler	: gcc
 * author	: Visakh Venugopal [visakh.v@gadgeon.com]
 *
 * Implements indexed simple queue operations
 * -----------------------------------------------------------------------+
 * Copyright (c) 2020 By GadgEon Smart Systems Pvt. Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/queue.h>
#include "lgc_debug.h"
#include "lgc_common_def.h"
#include "lgc_idx_squeue.h"

#define LGC_DATAQUEUE_MAX_SIZE 32

extern int g_log_LEVEL;

typedef struct node
{
	void *data;
	int size;
	SIMPLEQ_ENTRY(node) nodes;
} node_t;

typedef SIMPLEQ_HEAD(head_s, node) head_t;

static struct lgc_queue_t {
	head_t *lgc_queue_head;
	int *lgc_queue_size;
	pthread_mutex_t *q_mtx;
	int lgc_queue_count;
} lgc_queue = { NULL, NULL, 0 };

/**
 * This api initializes the BSD queue. Here 'max_count' no of queues
 * will be initialized.
 * 
 * @param
 * 	max_count	: No of queues to be intialized
 * @return
 *	status
 */
int lgc_idx_squeue_init(int max_count)
{
	int i;

	lgc_queue.lgc_queue_count = max_count;
	lgc_queue.lgc_queue_head = (head_t *) (calloc(max_count, sizeof(head_t)));
	lgc_queue.lgc_queue_size = (int *) (calloc(max_count, sizeof(int)));
	lgc_queue.q_mtx = (pthread_mutex_t *) (calloc(max_count, sizeof(pthread_mutex_t)));
	for (i = 0; i < max_count; i++) {
		SIMPLEQ_INIT(&lgc_queue.lgc_queue_head[i]);
		pthread_mutex_init(&lgc_queue.q_mtx[i], 0);
	}
	DPRINTF(DBG_INFO, "[BSD_SQ] Queue table initialized. Count=%d\n", max_count);
	return SUCCESS;
}

/**
 * This api de-initializes the BSD queue and reclaim any resources.
 * 
 * @param
 * 	None
 * @return
 *	status
 */
int lgc_idx_squeue_destroy()
{
	int i;
	node_t *node = NULL;

	for (i = 0; i < lgc_queue.lgc_queue_count; i++) {
		if (lgc_queue.lgc_queue_head + i) {
			while ((node = SIMPLEQ_FIRST(&lgc_queue.lgc_queue_head[i])) != NULL) {
				SIMPLEQ_REMOVE_HEAD(&lgc_queue.lgc_queue_head[i], nodes);
				free(node->data);
				free(node);
			}
			pthread_mutex_destroy(&lgc_queue.q_mtx[i]);
		}
	}
	if (lgc_queue.lgc_queue_head)
		free(lgc_queue.lgc_queue_head);
	if (lgc_queue.lgc_queue_size)
		free(lgc_queue.lgc_queue_size);
	memset(&lgc_queue, 0, sizeof(struct lgc_queue_t));
	return 0;
}

/**
 * This api insert a node to the BSD queue.
 * 
 * @param
 *	index	: Index of the queue, where the node/payload to be inserted
 *	data	: Pointer to the data buffer
 *	size	: Size of the payload
 * @return
 *	status
 */
int lgc_idx_squeue_insert(int index, const void *data, const int size)
{
	struct timespec tspec = { 2, 0 };

	if (pthread_mutex_timedlock(&lgc_queue.q_mtx[index], &tspec) == 0) {
		if (lgc_queue.lgc_queue_size[index] < LGC_DATAQUEUE_MAX_SIZE) {
			node_t *node = (node_t *) calloc(1, sizeof(node_t));
			node->data = (void *) calloc(1, size + 1);
			memcpy(node->data, data, size);
			((unsigned char *) (node->data))[size] = '\0';
			node->size = size;
			SIMPLEQ_INSERT_TAIL(&lgc_queue.lgc_queue_head[index], node, nodes);
			lgc_queue.lgc_queue_size[index]++;
		} else {
			DPRINTF(DBG_ERROR, "[BSD_SQ] [idx:%d] max size exceeded (%d)\n",
					index, LGC_DATAQUEUE_MAX_SIZE);
		}
		pthread_mutex_unlock(&lgc_queue.q_mtx[index]);
	} else {
		DPRINTF(DBG_ERROR, "[BSD_SQ] [idx:%d] resource lock failed !busy\n", index);
		return FAILURE;
	}
	return SUCCESS;
}

/**
 * This api read and delete a node from the BSD queue.
 * 
 * @param
 *	index	: Index of the queue, from where the data to be read
 *	data	: Pointer to the data buffer
 *	size	: Size of the buffer
 * @return
 *	status
 */
int lgc_idx_squeue_remove(int index, void *data, const int size)
{
	node_t *node = NULL;
	int t_size = 0;
	struct timespec tspec = { 2, 0 };

	if (pthread_mutex_timedlock(&lgc_queue.q_mtx[index], &tspec) == 0) {
		if ((node = SIMPLEQ_FIRST(&lgc_queue.lgc_queue_head[index])) != NULL) {
			t_size = size > node->size ? node->size : size;
			memcpy(data, node->data, t_size);
			SIMPLEQ_REMOVE_HEAD(&lgc_queue.lgc_queue_head[index], nodes);
			lgc_queue.lgc_queue_size[index]--;
			free(node->data);
			free(node);
		} else {
			DPRINTF(DBG_DEBUG, "[BSD_SQ] [idx:%d] data queue empty\n", index);
		}
		pthread_mutex_unlock(&lgc_queue.q_mtx[index]);
	} else {
		DPRINTF(DBG_ERROR, "[BSD_SQ] [idx:%d] resource lock failed !busy\n", index);
		return FAILURE;
	}
	return t_size;
}

