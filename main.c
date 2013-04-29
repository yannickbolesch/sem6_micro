/** 
 * @file main.c 
 * This program have been written as a solution to the first programming exercise  
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

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "stopwatch.h"

static const char* in_filename = "foobar.txt";
static const char* out_filename = "result.txt";

#define MAX_LINE_BUFFER  256
#define MAX_TEXT_LINES 1024 

static uint64_t result[MAX_TEXT_LINES];
static int32_t operands_a[MAX_TEXT_LINES];
static int32_t operands_b[MAX_TEXT_LINES];

/**
 * @brief Calculates S_n = S_n-1 + A * B
 * 
 * @param operand_a [in] Operand A
 * @param operand_b [in] Operand B
 * @param pre_result [in] Previous result (S_n-1)
 * 
 * @return result of the operation.
 */
uint64_t do_mac_operation(int32_t operand_a,uint32_t operand_b,uint64_t prev_result);

int main (int argc, char** argv)
{
	int32_t ret = EXIT_SUCCESS;
	FILE * in_file_hdl = NULL;
	FILE * out_file_hdl = NULL;
	if(EXIT_FAILURE == init_stop_watch())
	{
		printf("unable to setup a stop watch -> abort");
	}

	in_file_hdl = fopen (in_filename,"r");
	out_file_hdl = fopen (out_filename,"w");

	if ((NULL == in_file_hdl) || (NULL == out_filename) )
	{
		printf("Error while opening files: %s / %s",in_filename,out_filename);
		ret = EXIT_FAILURE;
	}
	else
	{
		uint32_t counter = 0; 
		char line_buffer[MAX_LINE_BUFFER];
		int operand_a = 0; 
		int operand_b = 0; 
		uint64_t prev_result = 0; 
		uint64_t delta_time = 0;
		/*
		 * Stop on end of file or on array boundary
		 */
		while((0 == feof(in_file_hdl)) && ( MAX_TEXT_LINES > counter ))
		{
			start_stop_watch();
			if(NULL == fgets(line_buffer,MAX_LINE_BUFFER,in_file_hdl))
			{
				if(!feof(in_file_hdl))
				{
					printf("Error while reading data from: %s",in_filename);
					ret = EXIT_FAILURE;
				}
				else
				{
					ret = EXIT_SUCCESS;
				}
				break;
 			}
			else
			{
				if (2 != sscanf(line_buffer,"A:<%d> B:<%d>",&operand_a,&operand_b))
				{
					ret = EXIT_FAILURE;
					break;
				}
				else
				{
					result[counter] = do_mac_operation(operand_a,operand_b,prev_result);
					operands_a[counter] = operand_a;
					operands_b[counter] = operand_b;
					fprintf(out_file_hdl,"%llu = %llu + (%d * %d)\n",result[counter],prev_result,operand_a, operand_b);
					printf("%llu = %llu + (%d * %d)\n",result[counter],prev_result,operand_a, operand_b);
					prev_result =  result[counter];
					counter++;
				}
			}
			delta_time = stop_stop_watch();
			printf("delta time us: %llu\n",delta_time);
		}
	}

	if(NULL != in_file_hdl)
	{
		fclose(in_file_hdl);
	}
	if(NULL != out_file_hdl)
	{
		fclose(out_file_hdl);
	}
	return ret;
}

uint64_t do_mac_operation(int32_t operand_a,uint32_t operand_b,uint64_t result)
{
	return result + (operand_a * operand_b);
}