/*
 create    : 20190819
 modify    : 20190819
 developer : lclasser@gmail.com
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "ljson_fundamental.h"

LJLong ljson_memory_initial(LJMemory* pMemory, LJLong size, LJLong block)
{
	memset(pMemory, 0x00, sizeof(LJMemory));
	pMemory->size = size;
	pMemory->block = block;
	return 0;
}

LJLong ljson_memory_destroy(LJMemory* pMemory)
{
	void* ptr_next = pMemory->head;
	while( ptr_next )
	{
		LJMemory* ptr_alloc = ptr_next;
		ptr_next = ptr_alloc->next;

		free(ptr_alloc);
	}
	return 0;
}

LJByte* ljson_memory_alloc(LJMemory* pMemory, LJLong cnt)
{
	LJLong extend_cnt = (pMemory->pos + cnt + 8);

	if( (pMemory->val == NULL) || (extend_cnt >= pMemory->block) )
	{
		LJLong alloc_cnt = pMemory->block;
		if( alloc_cnt < cnt )
			alloc_cnt = cnt;

		LJMemory* alloc_ptr = malloc(sizeof(LJMemory) + (pMemory->size * (alloc_cnt + 8)));
		if( alloc_ptr == NULL )
		{
			return NULL;
		}
		memset(alloc_ptr, 0x00, sizeof(LJMemory));
		alloc_ptr->idx = pMemory->idx++;

		if( pMemory->head == NULL ) {
			pMemory->head = alloc_ptr;
			pMemory->next = alloc_ptr;
		} else {
			LJMemory* pLast = pMemory->next;
			pLast->next = alloc_ptr;

			pMemory->next = alloc_ptr;
		}

		pMemory->val = alloc_ptr->_data;
		pMemory->pos = 0;
	}

	LJByte* pItem = &pMemory->val[pMemory->size*pMemory->pos];
	memset(pItem, 0x00, (pMemory->size * cnt));
	pMemory->pos += cnt;
	pMemory->cnt++;
	return pItem;
}
