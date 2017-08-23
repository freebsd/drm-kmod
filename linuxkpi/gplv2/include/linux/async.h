/*
 * async.h: Asynchronous function calls for boot performance
 *
 * (C) Copyright 2009 Intel Corporation
 * Author: Arjan van de Ven <arjan@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#ifndef _LINUX_GPLV2_ASYNC_H_
#define _LINUX_GPLV2_ASYNC_H_

#include <linux/types.h>
#include <linux/list.h>
#include <linux/workqueue.h>

typedef u64 async_cookie_t;

typedef void (*async_func_t) (void *data, async_cookie_t cookie);

struct async_domain {
	struct list_head pending;
	unsigned registered:1;
};

struct async_entry {
	struct list_head	domain_list;
	struct list_head	global_list;
	struct work_struct	work;
	async_cookie_t		cookie;
	async_func_t		func;
	void			*data;
	struct async_domain	*domain;
};

extern async_cookie_t async_schedule(async_func_t func, void *data);

static inline void
async_synchronize_full(void)
{
	UNIMPLEMENTED();
}

static inline void
async_synchronize_cookie(async_cookie_t cookie)
{
	UNIMPLEMENTED();
}

static inline bool
current_is_async(void)
{
	UNIMPLEMENTED();
	return (false);
}

#endif /* _LINUX_GPLV2_ASYNC_H_ */
