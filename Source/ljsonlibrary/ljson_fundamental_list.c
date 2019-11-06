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

LJLong ljson_list_addtail(LJsonStruct* pStruct, LJLinkList* pList, void* data)
{
	LJLinkItem* pItem = (LJLinkItem*)ljson_memory_alloc(&pStruct->mem_list, 1);
	if( pItem == NULL )
	{
		return LJSON_ERROR_LIST_ALLOC_FAIL;
	}
	pItem->data = data;

	if( pList->tail == NULL )
	{
		pList->head = pItem;
		pList->tail = pItem;
	}
	else
	{
		LJLinkItem* pTail = pList->tail;
		pTail->next = pItem;
		pList->tail = pItem;
	}
	pList->count++;
	return pList->count;
}

LJLong ljson_list_getcount(LJLinkList* pList)
{
	return pList->count;
}

LJLinkItem* ljson_list_gethead(LJLinkList* pList)
{
	LJLinkItem* item = pList->head;
	return item;
}

void* ljson_list_getnext(LJLinkList* pList, LJLinkItem** ptr_item)
{
	if( ptr_item == NULL )
	{
		return NULL;
	}

	LJLinkItem* item = *ptr_item;
	if( item == NULL )
	{
		return NULL;
	}
	*ptr_item = item->next;
	return item->data;
}
