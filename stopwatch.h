/** 
 * @file stopwatch.h
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

#include <stdint.h>

/* include guard */
#ifndef STOP_WATCH_H__
#define STOP_WATCH_H__

/**
 * @brief Initializes the stop watch.
 * 
 * This function checks also if everything is available to work as expected.
 * The init function also starts the stop watch.
 * 
 * @return EXIT_SUCCESS on success otherwise EXIT_FAILURE
 */
int init_stop_watch(void);

/**
 * @brief Starts the stop watch
 */
void start_stop_watch(void);

/**
 * @brief Stops the measurement and return the elapsed time in micro (10^-6) Seconds.
 * 
 * @eturn the ellapsed time in micro (10^-6) Seconds since the lat start.
 */
uint64_t stop_stop_watch(void);

#endif
