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
#include <math.h>
#include <Windows.h>
#include "stopwatch.h"

static const char* in_filename = "foobar.txt";
static const char* out_filename = "result.txt";

#define MAX_LINE_BUFFER  256
#define MAX_TEXT_LINES 1024 

static uint64_t result[MAX_TEXT_LINES];
static int32_t operands_a[MAX_TEXT_LINES];
static int32_t operands_b[MAX_TEXT_LINES];

typedef struct statistics 
{
	uint32_t min_operand_a;
	uint32_t max_operand_a;
	uint32_t min_operand_b;
	uint32_t max_operand_b;
	uint32_t avg_operand_a;
	uint32_t avg_operand_b;
	uint32_t sum_operand_a;
	uint32_t sum_operand_b;
	uint32_t counter;
}statistics_t;

/**
 * @struct listNode 
 * @brief This struct defines a list element or
 *        somethimes called a list node.
 */
typedef struct listNode 
{
	/**@brief Pointer to previous Element */
	struct listNode *pPrev;
	/**@brief Pointer to next Element */
	struct listNode *pNext;
	/**@brief Operant A for the MAC Operation */
	uint32_t operand_a;
	/**@brief Operant B for the MAC Operation */
	uint32_t operand_b;
	/**@brief Result the MAC Operation */
	uint64_t result;
} listNode_t ;

typedef struct listNodeMem 
{
	listNode_t node;
	uint32_t inUse;
} listNodeMem_t;

#define STATIC_MEM 1 /* use static memory pool instead of malloc/free*/
#define MEM_POOL_SIZE 124 /* memory pool size */

/** @brief the static memory pool itself */
listNodeMem_t list_node_mem_pool[MEM_POOL_SIZE];
/**
 * @struct doubleLinkedList 
 * @brief Helper struct to hold the head and Tail of the list
 */
typedef struct doubleLinkedList 
{
	/** @brief Pointer to the head of the list. NULL if list is empty. */
	listNode_t *headOfList;
	/** @brief Pointer to the tail of the list. NULL if list is empty. */
	listNode_t *tailOfList;
}doubleLinkedList_t;

void calculate_statistics(statistics_t * stat,uint32_t operand_a,uint32_t operand_b);
/**
 * @brief create a new list Element and setup the initial values
 * 
 * @param [in] operand_a The operand a for the MAC operation
 * @param [in] operand_b The operand b for the MAC operation
 * @param [in] result The result of the MAC operation
 * @result he new element or NULL in case of error
 */
listNode_t* list_get_new_element(uint32_t operand_a,uint32_t operand_b,uint64_t result);

/**
 * @brief free a list Node this returns the memory to the os
 * 
 * @param elem [in] Pointer to a list node
 */
void list_free_element(listNode_t* elem);

/**
 * @brief insert element before another one
 * 
 * @param list [in] The list where the element should be linked in
 * @param old [in] The element where to insert in front of.
 * @param elem [in] The element to be added
 * @return EXIT_SUCCESS in case of success otherwise EXIT_FAILURE
 */
int list_insert_before(doubleLinkedList_t *list , listNode_t* old ,listNode_t* elem);

/**
 * @brief insert element after another one
 * 
 * @param list [in] The list where the element should be linked in
 * @param old [in] The element where to insert after.
 * @param elem [in] The element to be added
 * @return EXIT_SUCCESS in case of success otherwise EXIT_FAILURE
 */
int list_insert_after(doubleLinkedList_t *list , listNode_t* old ,listNode_t* elem);

/**
 * @brief Add element to the beginning of the list.
 * 
 * @param list [inout] list where element should be appended to.
 * @param elem [in] element to be appended to the list
 * @return EXIT_SUCCESS in case of success
 */
int list_push_front(doubleLinkedList_t *list , listNode_t* elem);

/**
 * @brief Add element to the end of the list.
 * 
 * @param list [inout] list where element should be appended to.
 * @param elem [in] element to be appended to the list
 * @return EXIT_SUCCESS in case of success
 */
int list_push_back(doubleLinkedList_t *list, listNode_t* elem);


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

listNode_t* static_malloc(); 

void static_free(listNode_t* elem);

int main (int argc, char** argv)
{
	int32_t ret = EXIT_SUCCESS;
	FILE* in_file_hdl = NULL;
	FILE* out_file_hdl = NULL;
	doubleLinkedList_t result_list;
	//listNode_t *curr = result_list->headOfList;
	//int idx = 0;
	
	statistics_t stat;
	stat.avg_operand_a = 0;
	stat.avg_operand_b = 0;
	stat.sum_operand_a = 0;
	stat.sum_operand_b = 0;
	stat.max_operand_a = 0;
	stat.max_operand_b = 0;
	stat.min_operand_a = 0xFFFFFFFF;
	stat.min_operand_b = 0xFFFFFFFF;
	stat.counter = 0;

	result_list.headOfList = NULL;
	result_list.tailOfList = NULL;

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
		uint32_t operand_a = 0; 
		uint32_t operand_b = 0; 
		uint64_t result = 0; 
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
					// MAC
					if(result_list.tailOfList == NULL)
					{
						prev_result = 0;
					}
					else
					{
						prev_result = result_list.tailOfList->result;
					}
					result = do_mac_operation(operand_a, operand_b, prev_result);
					fprintf(out_file_hdl,"%llu = %llu + (%d * %d)\n",result, prev_result, operand_a, operand_b);
					printf("%llu = %llu + (%d * %d)\n",result, prev_result,operand_a, operand_b);					
					ret = list_push_back(&result_list,list_get_new_element(operand_a,operand_b,result));
					if(EXIT_FAILURE == ret)
					{
						break;
					}
					
					counter++;
					
					// min
					if(operand_a < stat.min_operand_a)
					{
						stat.min_operand_a = operand_a;
					}
					if(operand_b < stat.min_operand_b)
					{
						stat.min_operand_b = operand_b;
					}

					// max
					if(operand_a > stat.max_operand_a)
					{
						stat.max_operand_a = operand_a;
					}
					if(operand_b > stat.max_operand_b)
					{
						stat.max_operand_b = operand_b;
					}

					// sum
					stat.sum_operand_a += operand_a;
					stat.sum_operand_b += operand_b;

					// avg
					stat.avg_operand_a = stat.sum_operand_a / counter;
					stat.avg_operand_b = stat.sum_operand_b / counter;
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


void calculate_statistics(statistics_t * stat,uint32_t operand_a,uint32_t operand_b)
{
	if(NULL != stat)
	{
		stat->max_operand_a = max(stat->max_operand_a,operand_a);
		stat->max_operand_b = max(stat->max_operand_b,operand_b);
		stat->min_operand_a = min(stat->min_operand_a,operand_a);
		stat->min_operand_b = min(stat->min_operand_b,operand_b);

		stat->sum_operand_a += operand_a;
		stat->sum_operand_b += operand_b;
		stat->counter++;
		stat->avg_operand_a = stat->sum_operand_a/stat->counter;
		stat->avg_operand_b = stat->sum_operand_b/stat->counter;
	}
}


int list_insert_before(doubleLinkedList_t *list , listNode_t* old ,listNode_t* elem)
{
	if(old->pPrev == NULL)
	{
		// old ist das erste element
		elem->pNext = old;
		old->pPrev = elem;
		elem->pPrev = NULL;
	}
	else
	{
		// old ist irgendein element
		listNode_t* pHelp = old->pPrev;
		pHelp->pNext = elem;
		elem->pNext = old;
		elem->pPrev = pHelp;
		old->pPrev = elem;
	}
	return EXIT_SUCCESS;
}


int list_insert_after(doubleLinkedList_t *list , listNode_t* old ,listNode_t* elem)
{
	if(old->pNext == NULL)
	{
		// old ist das letze element
		old->pNext = elem;
		elem->pNext = NULL;
		elem->pPrev = old;
	}
	else
	{
		// old ist irgendein element
		listNode_t* pHelp = old->pNext;
		old->pNext = elem;
		elem->pNext = pHelp;
		elem->pPrev = old;
		pHelp->pPrev = elem;
	}
	return EXIT_SUCCESS;
}


void list_free_element(listNode_t* elem)
{
	if(NULL != elem)
	{
		// elem aus der liste entfernen
		if(elem->pNext == NULL)
		{
			// elem ist das letze
			elem->pPrev->pNext = NULL;
		}
		else if(elem->pPrev == NULL)
		{
			// elem das erste
			elem->pNext->pPrev = NULL;
		}
		else
		{
			// elem ist in der Mitte
			elem->pPrev->pNext = elem->pNext;
			elem->pNext->pPrev = elem->pPrev;
		}
		// elem freigeben
#if STATIC_MEM == 0
		free(elem);
#else
		static_free(elem);
#endif
	}
}


int list_push_front(doubleLinkedList_t *list , listNode_t* elem)
{
	if(list->headOfList == NULL && list->tailOfList == NULL)
	{
		// list initialisieren
		list->headOfList = elem;
		list->tailOfList = elem;
	}
	else if(list->headOfList == NULL || list->tailOfList == NULL)
	{
		// Fehler
		return EXIT_FAILURE;
	}
	else
	{
		elem->pNext = list->headOfList;
		elem->pPrev = NULL;
		list->headOfList->pPrev = elem;
		list->headOfList = elem;
	}

	return EXIT_SUCCESS;
}


int list_push_back(doubleLinkedList_t *list, listNode_t* elem)
{
	if(list->headOfList == NULL && list->tailOfList == NULL)
	{
		// list initialisieren
		list->headOfList = elem;
		list->tailOfList = elem;
	}
	else if(list->headOfList == NULL || list->tailOfList == NULL)
	{
		// Fehler
		return EXIT_FAILURE;
	}
	else
	{
		elem->pPrev = list->tailOfList;
		elem->pNext = NULL;
		list->tailOfList->pNext = elem;
		list->tailOfList = elem;
	}

	return EXIT_SUCCESS;
}


listNode_t* list_get_new_element(uint32_t operand_a,uint32_t operand_b,uint64_t result)
{
	listNode_t* pHelp = NULL;
#if STATIC_MEM == 0
	 pHelp = (listNode_t*)malloc(sizeof(listNode_t));
#else
	pHelp = static_malloc();
#endif
	if(NULL == pHelp)
	{
		printf("error allocating memory!");
	}
	else
	{
		pHelp->operand_a = operand_a;
		pHelp->operand_b = operand_b;
		pHelp->result = result;
		pHelp->pNext = NULL;
		pHelp->pPrev = NULL;
	}

	return pHelp;
}


listNode_t* static_malloc()
{
	int i;
	listNode_t* elem = NULL;

	// �ber ganzen mempool iterieren und freies suchen
	for(i = 0; i < MEM_POOL_SIZE; i++)
	{
		if(0 == list_node_mem_pool[i].inUse)
		{
			elem = &list_node_mem_pool[i].node;
			list_node_mem_pool[i].inUse == 1;
			i = MEM_POOL_SIZE;
		}
	}
	
	return elem;
}

void static_free(listNode_t* elem)
{
	int i;

	// �ber ganzen mempool iterieren und freies suchen
	for(i = 0; i < MEM_POOL_SIZE; i++)
	{
		if(elem == &list_node_mem_pool[i].node)
		{
			list_node_mem_pool[i].inUse == 0;
			i = MEM_POOL_SIZE;
		}
	}
}


