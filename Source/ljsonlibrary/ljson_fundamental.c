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

#ifdef _UNICODE
#include <wchar.h>
#endif // _UNICODE

LJsonHashSetAt		_callback_hashfunc_setat = NULL;
LJsonHashLookup		_callback_hashfunc_lookup = NULL;
LJsonScriptRun		_callback_script_run = NULL;

LJsonStruct* ljson_initialize(LJsonStruct* pStruct)
{
	memset(pStruct, 0x00, sizeof(LJsonStruct));

	pStruct->config.is_indent = TRUE;
	pStruct->config.is_newline = TRUE;
	pStruct->config.is_tabindent = FALSE;
	pStruct->config.cnt_space = 4;

	pStruct->root.master = pStruct;
	pStruct->root.type = LJSON_TYPE_OBJECT;

	ljson_memory_initial(&pStruct->mem_list, sizeof(LJLinkItem)	, 1024);
	ljson_memory_initial(&pStruct->mem_obj , sizeof(LJsonObject), 1024);
	ljson_memory_initial(&pStruct->mem_str , sizeof(LJChar)		, 4096);

	return pStruct;
}

LJLong ljson_destroy(LJsonStruct* pStruct)
{
	if( pStruct )
	{
		if( pStruct->wtext )
			free(pStruct->wtext);
		pStruct->wtext = NULL;

		if( pStruct->rtext )
			free(pStruct->rtext);
		pStruct->rtext = NULL;

		ljson_memory_destroy(&pStruct->mem_list);
		ljson_memory_destroy(&pStruct->mem_obj);
		ljson_memory_destroy(&pStruct->mem_str);
	}

	return 0;
}

LJLong ljson_set_cbhash(LJsonHashSetAt _setat, LJsonHashLookup _lookup)
{
	_callback_hashfunc_setat = _setat;
	_callback_hashfunc_lookup = _lookup;
	return 0;
}

LJLong ljson_set_cbscript(LJsonScriptRun _run)
{
	_callback_script_run = _run;
	return 0;
}

LJLong ljson_is_number(LJChar val)
{
	if( ((val >= '0') && (val <= '9')) )
		return 1;
	return 0;
}

LJLong ljson_is_character(LJChar val)
{
	if( ((val >= 'A') && (val <= 'Z')) || ((val >= 'a') && (val <= 'z')) )
		return 1;
	return 0;
}

LJChar* ljson_trim_left(LJChar* str, LJLong len)
{
	if( len <= 0 )
		len = (LJLong)_LJ_strlen(str);

	LJLong pos = 0;
	while( pos < len )
	{
		switch( str[pos] )
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			// str[len] = 0x00;
			break;
		default:
			if( pos > 0 ) {
				memmove(&str[0], &str[pos], len - pos);
				str[len - pos] = 0x00;
			}
			return str;
		}
		pos++;
	}
	return str;
}

LJChar* ljson_trim_right(LJChar* str, LJLong len)
{
	if( len <= 0 )
		len = (LJLong)(_LJ_strlen(str) - 1);

	while( len >= 0 )
	{
		switch( str[len] )
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			str[len] = 0x00;
			break;
		default:
			return str;
		}
		len--;
	}
	return str;
}

LJChar* ljson_make_string(LJsonStruct* pStruct, LJChar* value)
{
	LJLong arg_len = 0;
	if( value == NULL )
		arg_len = 0;
	else
		arg_len = (LJLong)_LJ_strlen(value);

	LJChar* pString = (LJChar*)ljson_memory_alloc(&pStruct->mem_str, arg_len + 1); // string null
	if( pString == NULL )
		return NULL;

	if( value )
		_LJ_memcpy(pString, value, arg_len);
	pString[arg_len] = 0x00;
	return pString; 
}

LJsonObject* ljson_make_object(LJsonStruct* pStruct, LJsonObject* pOwner, LJByte type)
{
	LJsonObject* pObject = NULL;
	switch( type )
	{
	case LJSON_TYPE_ARRAY:
	case LJSON_TYPE_OBJECT:
		pObject = (LJsonObject*)ljson_memory_alloc(&pStruct->mem_obj, 1);
		break;
	default:
		pObject = (LJsonObject*)ljson_memory_alloc(&pStruct->mem_obj, 1);
		break;;
	}

	pObject->master = pStruct;
	pObject->owner = pOwner;
	pObject->type = type;
	return pObject;
}

LJLong ljson_count_object(LJsonStruct* pStruct) // object + array
{
	return pStruct->mem_obj.cnt;
}

LJLong ljson_count_string(LJsonStruct* pStruct) // string
{
	return pStruct->mem_str.cnt;
}

LJLong ljson_size_object(LJsonStruct* pStruct) // object + array
{
	return 0;
}

LJLong ljson_size_string(LJsonStruct* pStruct) // string
{
	return 0;
}
