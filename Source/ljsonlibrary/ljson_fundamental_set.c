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


LJLong ljson_set_object(LJsonObject* pOwner, LJChar* name, LJsonObject* pValue)
{
	if( pValue == NULL )
		return LJSON_ERROR_ARG_OBJECT_NULL;

	if( pOwner == NULL )
		return LJSON_ERROR_ARG_OWNER_NULL;

	if( name )
	{
		pValue->name = ljson_make_string(pOwner->master, name);
		if( pValue->name == NULL )
		{
			return LJSON_ERROR_STRING_ALLOC_FAIL;
		}
	}

	if( pOwner->master->config.is_ownercheck )
	{
		LJsonObject* chk_owner = pOwner->owner;
		while( chk_owner )
		{
			if( chk_owner == pValue )
				return LJSON_ERROR_OWNER_CHILD_OWNERCHECK;
			chk_owner = chk_owner->owner;
		}
	}

	if( _callback_hashfunc_setat && pOwner->master->joint )
	{
		if( name )
		{
			if( _callback_hashfunc_setat(pOwner, name, pValue) < 0 )
				return LJSON_ERROR_SETAT_DENY;
		}
	}

	pValue->owner = pOwner;
	if( ljson_list_addtail(pOwner->master, &pOwner->value.child, pValue) < 0 )
		return LJSON_ERROR_OWNER_CHILD_ADD_FAIL;

	return ljson_list_getcount(&pOwner->value.child);
}

LJLong ljson_set_string(LJsonObject* pOwner, LJChar* name, LJChar* value)
{
	LJsonObject* pObject = ljson_make_object(pOwner->master, pOwner, LJSON_TYPE_STRING);
	if( pObject == NULL )
	{
		return LJSON_ERROR_OBJECT_ALLOC_FAIL;
	}
	pObject->value.str  = ljson_make_string(pOwner->master, value);
	if( pObject->value.str == NULL )
	{
		return LJSON_ERROR_STRING_ALLOC_FAIL;
	}
	return ljson_set_object(pOwner, name, pObject);
}

LJLong ljson_set_integer(LJsonObject* pOwner, LJChar* name, LJLong value)
{
	LJChar tmp_val[64];
	_LJ_snprintf(tmp_val, sizeof(tmp_val), FIXED_STRING("%ld"), value);

	LJsonObject* pObject = ljson_make_object(pOwner->master, pOwner, LJSON_TYPE_INTEGER);
	if( pObject == NULL )
	{
		return LJSON_ERROR_OBJECT_ALLOC_FAIL;
	}
	pObject->value.str = ljson_make_string(pOwner->master, tmp_val);
	if( pObject->value.str == NULL )
	{
		return LJSON_ERROR_STRING_ALLOC_FAIL;
	}
	return ljson_set_object(pOwner, name, pObject);
}

LJLong ljson_set_double(LJsonObject* pOwner, LJChar* name, double value)
{
	LJChar tmp_val[64];
	_LJ_snprintf(tmp_val, sizeof(tmp_val), FIXED_STRING("%lf"), value);

	LJsonObject* pObject = ljson_make_object(pOwner->master, pOwner, LJSON_TYPE_FLOAT);
	if( pObject == NULL )
	{
		return LJSON_ERROR_OBJECT_ALLOC_FAIL;
	}
	pObject->value.str = ljson_make_string(pOwner->master, tmp_val);
	if( pObject->value.str == NULL )
	{
		return LJSON_ERROR_STRING_ALLOC_FAIL;
	}
	return ljson_set_object(pOwner, name, pObject);
}

LJLong ljson_add_array_object(LJsonObject* pArray, LJsonObject* pValue)
{
	return ljson_set_object(pArray, NULL, pValue);
}

LJLong ljson_add_array_string(LJsonObject* pArray, LJChar* value)
{
	return ljson_set_string(pArray, NULL, value);
}

LJLong ljson_add_array_integer(LJsonObject* pArray, LJLong value)
{
	return ljson_set_integer(pArray, NULL, value);
}

LJLong ljson_add_array_double(LJsonObject* pArray, double value)
{
	return ljson_set_double(pArray, NULL, value);
}
