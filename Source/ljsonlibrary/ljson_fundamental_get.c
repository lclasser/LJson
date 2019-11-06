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

LJLong ljson_get_typestring(LJsonObject* obj, LJChar* value, LJLong size)
{
	int rtn = 8;
	switch( obj->type )
	{
	case LJSON_TYPE_NONE	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("None"	)); break;
	case LJSON_TYPE_NULL	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("Null"	)); break;
	case LJSON_TYPE_STRING	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("String"	)); break;
	case LJSON_TYPE_INTEGER	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("Integer")); break;
	case LJSON_TYPE_FLOAT	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("Float"	)); break;
	case LJSON_TYPE_SCRIPT	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("Script"	)); break;
	case LJSON_TYPE_OBJECT	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("Object"	)); break;
	case LJSON_TYPE_ARRAY	: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("Array"	)); break;
	default					: if( value ) rtn = _LJ_snprintf(value, size, FIXED_STRING("unkwon"	)); break;
	}
	return rtn;
}

LJsonStruct* ljson_get_struct(LJsonObject* pObject)
{
	return pObject->master;
}

LJsonObject* ljson_get_root(LJsonStruct* pStruct)
{
	if( pStruct == NULL )
		return NULL;
	return &pStruct->root;
}

LJsonObject* ljson_get_object(LJsonObject* pOwner, LJChar* name)
{
	if( pOwner == NULL )
		return NULL;

	if( _callback_hashfunc_lookup && pOwner->master->joint )
	{
		LJsonObject* pFound = NULL;
		if( _callback_hashfunc_lookup(pOwner, name, &pFound) )
		{
			return pFound;
		}
	}
	else
	{
		LJLinkItem* ptr_next = ljson_list_gethead(&pOwner->value.child);
		while( ptr_next )
		{
			LJsonObject* pObject = ljson_list_getnext(&pOwner->value.child, &ptr_next);
			if( pObject->name && _LJ_stricmp(pObject->name, name) == 0 )
			{
				return pObject;
			}
		}
	}
	return NULL;
}

LJsonObject* ljson_get_object_item(LJsonObject* pOwner, LJLong pos)
{
	if( pOwner == NULL )
		return NULL;

	LJLong loop = 0;
	LJLinkItem* ptr_next = ljson_list_gethead(&pOwner->value.child);
	while( ptr_next )
	{
		LJsonObject* pObject = ljson_list_getnext(&pOwner->value.child, &ptr_next);
		if( loop == pos )
		{
			return pObject;
		}
		loop++;
	}

	return NULL;
}

LJLinkItem* ljson_get_object_head(LJsonObject* pOwner)
{
	if( pOwner == NULL )
		return NULL;
	return ljson_list_gethead(&pOwner->value.child);
}

LJsonObject* ljson_get_object_next(LJsonObject* pOwner, LJLinkItem** ptr_next)
{
	if( pOwner == NULL )
		return NULL;
	return ljson_list_getnext(&pOwner->value.child, ptr_next);
}

LJLong ljson_get_object_count(LJsonObject* pOwner)
{
	if( pOwner == NULL )
		return 0;
	return ljson_list_getcount(&pOwner->value.child);
}

LJLong ljson_get_type(LJsonObject* pObject)
{
	if( pObject == NULL )
		return -1;
	return pObject->type;
}

LJChar* ljson_get_name(LJsonObject* pObject)
{
	if( pObject == NULL )
		return NULL;
	return pObject->name;
}

LJChar* ljson_get_value(LJsonObject* pObject)
{
	if( pObject == NULL )
		return NULL;
	return pObject->value.str;
}

LJChar* _ljson_return_string(LJLong type, LJChar* value)
{
	static LJChar* rtn_object = FIXED_STRING("[object]");
	static LJChar* rtn_array = FIXED_STRING("[array]");
	switch( type )
	{
	case LJSON_TYPE_STRING:
	case LJSON_TYPE_INTEGER:
	case LJSON_TYPE_FLOAT:
	case LJSON_TYPE_SCRIPT:
		return value;
	case LJSON_TYPE_OBJECT:
		return rtn_object;
	case LJSON_TYPE_ARRAY:
		return rtn_array;
	}
	return NULL;
}

long _ljson_return_integer(LJLong type, LJChar* value)
{
	switch( type )
	{
	case LJSON_TYPE_STRING:
	case LJSON_TYPE_INTEGER:
	case LJSON_TYPE_FLOAT:
		return _LJ_atol(value);
	}
	return 0;
}

double _ljson_return_double(LJLong type, LJChar* value)
{
	switch( type )
	{
	case LJSON_TYPE_STRING:
	case LJSON_TYPE_INTEGER:
	case LJSON_TYPE_FLOAT:
		return _LJ_atod(value);
	}
	return 0;
}

LJChar* ljson_get_string(LJsonObject* pObject, LJChar* name)
{
	LJsonObject* pChild = (LJsonObject*)ljson_get_object(pObject, name);
	if( pChild )
		return _ljson_return_string(pChild->type, pChild->value.str);
	return NULL;
}

LJLong ljson_get_integer(LJsonObject* pObject, LJChar* name)
{
	LJsonObject* pChild = (LJsonObject*)ljson_get_object(pObject, name);
	if( pChild )
		return _ljson_return_integer(pChild->type, pChild->value.str);
	return 0;
}

double ljson_get_double(LJsonObject* pObject, LJChar* name)
{
	LJsonObject* pChild = (LJsonObject*)ljson_get_object(pObject, name);
	if( pChild )
		return _ljson_return_double(pChild->type, pChild->value.str);
	return 0;
}

LJLong ljson_get_array_count(LJsonObject* pOwner)
{
	return ljson_get_object_count(pOwner);
}

LJsonObject* ljson_get_array_object(LJsonObject* pOwner, LJLong pos)
{
	LJsonObject* pChild = (LJsonObject*)ljson_get_object_item(pOwner, pos);
	if( pChild )
		return pChild;
	return NULL;
}

LJChar* ljson_get_array_string(LJsonObject* pOwner, LJLong pos)
{
	LJsonObject* pChild = (LJsonObject*)ljson_get_object_item(pOwner, pos);
	if( pChild )
		return _ljson_return_string(pChild->type, pChild->value.str);
	return NULL;
}

LJLong ljson_get_array_integer(LJsonObject* pOwner, LJLong pos)
{
	LJsonObject* pChild = (LJsonObject*)ljson_get_object_item(pOwner, pos);
	if( pChild )
		return _ljson_return_integer(pChild->type, pChild->value.str);
	return 0;
}

double ljson_get_array_double(LJsonObject* pOwner, LJLong pos)
{
	LJsonObject* pChild = (LJsonObject*)ljson_get_object_item(pOwner, pos);
	if( pChild )
		return _ljson_return_double(pChild->type, pChild->value.str);
	return 0;
}
