/*
 * async.c: Asynchronous function calls for boot performance
 *
 * (C) Copyright 2009 Intel Corporation
 * Author: Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/async.h>
#include <linux/wait.h>

static atomic_t nextcookie;

static void
async_run_entry_fn(struct work_struct *work)
{
	struct async_entry *entry; 	

	linux_set_current(curthread);
	entry  = container_of(work, struct async_entry, work);
	entry->func(entry->data, entry->cookie);
	kfree(entry);
}

async_cookie_t
async_schedule(async_func_t func, void *data)
{
	struct async_entry *entry;
	async_cookie_t newcookie;

	DODGY();
	entry = kzalloc(sizeof(struct async_entry), GFP_ATOMIC);

	if (entry == NULL) {
		newcookie = atomic_inc_return(&nextcookie);
		func(data, newcookie);
		return (newcookie);
	}

	INIT_WORK(&entry->work, async_run_entry_fn);
	entry->func = func;
	entry->data = data;
	newcookie = entry->cookie = atomic_inc_return(&nextcookie);
	queue_work(system_unbound_wq, &entry->work);
	return (newcookie);
}
