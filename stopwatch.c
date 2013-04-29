/** 
 * @file stopwatch.c
 * This program have been written as a abstract way to measure execution times
 * Copyright (C) 2011  Christian Ege
 */

/*
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "stopwatch.h"
#include <windows.h>
#include <stdlib.h>

/* Modul variables to keep track of time ticks */
static uint64_t frequency;
static uint64_t start_val;
static uint64_t stop_val;

int init_stop_watch(void)
{
	int ret = EXIT_SUCCESS;
	if(!QueryPerformanceFrequency((LARGE_INTEGER*)&frequency))
	{
		ret = EXIT_FAILURE;
	}
	else
	{
		start_stop_watch();
		ret = EXIT_SUCCESS;
	}
	return ret;
}


void start_stop_watch(void)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&start_val);
}

uint64_t stop_stop_watch(void)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&stop_val);
	return ((stop_val - start_val)*1e6 / frequency);
}
