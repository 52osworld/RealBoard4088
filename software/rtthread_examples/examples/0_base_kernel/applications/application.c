/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#include <rtthread.h>

void rt_init_thread_entry(void* parameter)
{
		while(1){
		
    rt_kprintf("Hello RT-Thread!\n");
		}
}
void rt_init_thread_entry1(void* parameter)
{
		while(1){
		
    rt_kprintf("Hello RT-Thread!I'm No.1\n");
		}
}
void rt_init_thread_entry2(void* parameter)
{
		while(1){
		
    rt_kprintf("Hello RT-Thread!I'm No.2\n");
		}
}
void rt_init_thread_entry3(void* parameter)
{
		while(1){
		
    rt_kprintf("Hello RT-Thread!I'm No.3\n");
		}
}

int rt_application_init()
{
    rt_thread_t tid,tid1,tid2,tid3;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 1);
		if (tid != RT_NULL)
        rt_thread_startup(tid);
		tid1 = rt_thread_create("init1",
					rt_init_thread_entry1, RT_NULL,
					2048, RT_THREAD_PRIORITY_MAX/3, 1);
// 		if (tid1 != RT_NULL)
//         rt_thread_startup(tid1);
		tid2 = rt_thread_create("init2",
					rt_init_thread_entry2, RT_NULL,
					2048, RT_THREAD_PRIORITY_MAX/3, 1);
		if (tid2 != RT_NULL)
        rt_thread_startup(tid2);
		tid3 = rt_thread_create("init3",
					rt_init_thread_entry3, RT_NULL,
					2048, RT_THREAD_PRIORITY_MAX/3, 1);
    if (tid3 != RT_NULL)
        rt_thread_startup(tid3);
		
    return 0;
}
