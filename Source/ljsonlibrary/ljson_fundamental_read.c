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

LJLong ljson_read_name  (LJsonStruct* pStruct, LJsonObject* pObject);
LJLong ljson_read_value (LJsonStruct* pStruct, LJsonObject* pObject);

LJLong ljson_read_object(LJsonStruct* pStruct, LJsonObject* pOwner);
LJLong ljson_read_string(LJsonStruct* pStruct, LJsonObject* pOwner);
LJLong ljson_read_array (LJsonStruct* pStruct, LJsonObject* pOwner);

LJLong ljson_read_name(LJsonStruct* pStruct, LJsonObject* pObject)
{
	LJChar quot_str = 0;
	LJLong spos = -1;
	if( pStruct->rtext[pStruct->rpos] == '\"' || pStruct->rtext[pStruct->rpos] == '\'' )
	{
		quot_str = pStruct->rtext[pStruct->rpos];
		pStruct->rpos++; // " skip
	}

	pObject->name = NULL;
	while( pStruct->rpos < pStruct->rlen )
	{
		switch( pStruct->rtext[pStruct->rpos] )
		{
		case 0x00:
			if( quot_str == 0 ) return LJSON_ERROR_NAME_TEXT_ENDED;
			break;
		case ',':
			if( quot_str == 0 ) return LJSON_ERROR_NAME_DELIMITER_COMMA;
			break;
		case '{': case '}':
			if( quot_str == 0 ) return LJSON_ERROR_NAME_CURLY_BRACKET;
			break;
		case '[': case ']':
			if( quot_str == 0 ) return LJSON_ERROR_NAME_SQUARE_BRAKET;
			break;
		case ':':
			if( quot_str == 0 ) {
				if( pObject->name == NULL )
					return LJSON_ERROR_NAME_NOT_EXIST;

				ljson_trim_right(pObject->name, (pStruct->rpos - spos - 1));
				return pStruct->rpos;
			}
			break;
		case '\'':
		case '\"':
			if( quot_str == pStruct->rtext[pStruct->rpos] ) {
				if( pObject->name == NULL )
					return LJSON_ERROR_NAME_DUPLICATE_QUOTATION;

				pStruct->rtext[pStruct->rpos] = 0x00;
				pStruct->rpos++;
				return pStruct->rpos;
			}
			break;
		case ' ':
		case '\t':
			break;
		case '\r':
		case '\n':
			{
				if( pObject->name == NULL )
					return LJSON_ERROR_NAME_DUPLICATE_QUOTATION;

				pStruct->rtext[pStruct->rpos] = 0x00;
				pStruct->rpos++;

				ljson_trim_right(pObject->name, 0);
				return pStruct->rpos;
			}
			break;
		default:
			if( pObject->name == NULL ) {
				pObject->name = &pStruct->rtext[pStruct->rpos];
				spos = pStruct->rpos;
			}
			break;
		}
		pStruct->rpos++;
	}
	return LJSON_ERROR_NAME_TEXT_ENDED;
}

LJLong ljson_read_value(LJsonStruct* pStruct, LJsonObject* pObject)
{
	LJLong dot_cnt = 0;
	LJLong opr_cnt = 0;

	pObject->type = LJSON_TYPE_NONE;

	while( pStruct->rpos < pStruct->rlen )
	{
		switch( pStruct->rtext[pStruct->rpos] )
		{
		case 0x00:
		case '}': // in ljson_read_object
		case ']': // in ljson_read_array
		case ',':
			if( pObject->type == LJSON_TYPE_NONE ) {
				pObject->type = LJSON_TYPE_NULL;
			}
			return pStruct->rpos;
		case '\"': // string value
			return ljson_read_string(pStruct, pObject);
		case '{':
			if( pObject->type == LJSON_TYPE_NONE )
				return ljson_read_object(pStruct, pObject);
			break;
		case '[':
			if( pObject->type == LJSON_TYPE_NONE )
				return ljson_read_array(pStruct, pObject);
			break;
		case '(':
			{
				pObject->type = LJSON_TYPE_SCRIPT;
				LJLong is_continue = 1;
				while( is_continue ) {
					switch( pStruct->rtext[pStruct->rpos] )
					{
					case 0x00:
						return LJSON_ERROR_VALUE_TEXT_ENDED;
					case ' ':
					case '\t':
					case '\r':
					case '\n':
						pStruct->rtext[pStruct->rpos] = ' ';
						break;
					case ')':
						is_continue = 0;
						continue;
					}
					pStruct->rpos++;
				}
			}
			break;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			pStruct->rtext[pStruct->rpos] = ' ';
			break;
		default:
			if( pObject->value.str == NULL )
				pObject->value.str = &pStruct->rtext[pStruct->rpos];

			if( ljson_is_number(pStruct->rtext[pStruct->rpos]) ) {
				if( pObject->type == LJSON_TYPE_NONE )
					pObject->type = LJSON_TYPE_INTEGER;
			}
			else {
				switch( pStruct->rtext[pStruct->rpos] ) {
				case '+':
				case '-':
					if( (opr_cnt > 0) && (pObject->type == LJSON_TYPE_INTEGER) )
						pObject->type = LJSON_TYPE_SCRIPT;
					opr_cnt++;
					break;
				case '.':
					if( (dot_cnt == 0) && (pObject->type == LJSON_TYPE_INTEGER) )
						pObject->type = LJSON_TYPE_FLOAT;
					else
						pObject->type = LJSON_TYPE_SCRIPT;
					dot_cnt++;
					break;
				default:
					pObject->type = LJSON_TYPE_SCRIPT;
					break;
				}
			}
			break;
		}
		pStruct->rpos++;
	}
	return LJSON_ERROR_VALUE_TEXT_ENDED;
}

LJLong ljson_read_string(LJsonStruct* pStruct, LJsonObject* pOwner)
{
	LJLong quot_str = 0;
	if( pStruct->rtext[pStruct->rpos] == '\"' || pStruct->rtext[pStruct->rpos] == '\'' )
	{
		quot_str = pStruct->rtext[pStruct->rpos];
		pStruct->rpos++; // " skip
	}

	pOwner->value.str = NULL;
	pOwner->type = LJSON_TYPE_STRING;

	LJsonValue* pValue = &pOwner->value;

	while( pStruct->rpos < pStruct->rlen )
	{
		switch( pStruct->rtext[pStruct->rpos] )
		{
		case 0x00:
			if( quot_str == 0 ) return LJSON_ERROR_VALUE_STRING_TEXT_ENDED;
			break;
		case '{': case '[':
			if( quot_str == 0 ) return LJSON_ERROR_VALUE_STRING_CURLY_BRACKET_START;
			break;
		case '}': case ']':
		case ',':
			if( quot_str == 0 ) {
				pStruct->rtext[pStruct->rpos] = 0x00;
				pStruct->rpos++;
				return pStruct->rpos;
			}
			break;
		case '\'':
		case '\"':
			if( quot_str == pStruct->rtext[pStruct->rpos] ) {
				if( pValue->str == NULL ) // ºó°ª
					pValue->str = &pStruct->rtext[pStruct->rpos];
				pStruct->rtext[pStruct->rpos] = 0x00;
				pStruct->rpos++;
				return pStruct->rpos;
			}
			break;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			pStruct->rtext[pStruct->rpos] = ' ';
			break;
		default:
			if( pValue->str == NULL )
				pValue->str = &pStruct->rtext[pStruct->rpos];
			break;
		}
		pStruct->rpos++;
	}
	return LJSON_ERROR_VALUE_STRING_TEXT_ENDED;
}

LJLong ljson_read_array(LJsonStruct* pStruct, LJsonObject* pOwner)
{
	LJLong len;

	if( pStruct->rtext[pStruct->rpos] != '[' )
		return LJSON_ERROR_VALUE_ARRAY_NOT_SQUARE_BRAKET;
	pStruct->rpos++; // [ skip

	pOwner->type = LJSON_TYPE_ARRAY;

	while( pStruct->rpos < pStruct->rlen )
	{
		switch( pStruct->rtext[pStruct->rpos] )
		{
		case 0x00:
			return LJSON_ERROR_VALUE_ARRAY_TEXT_ENDED;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			break;
		case ',':
			pStruct->rtext[pStruct->rpos] = 0x00;
			break;
		case ']':
			pStruct->rtext[pStruct->rpos] = 0x00;
			pStruct->rpos++;
			return pStruct->rpos;
		case '}':
			return LJSON_ERROR_VALUE_ARRAY_QUOTATION;
		default:
			{
				LJsonObject* pChild = ljson_make_object(pStruct, pOwner, LJSON_TYPE_NONE);
				if( pChild == NULL )
					return LJSON_ERROR_VALUE_ARRAY_ALLOC_CHILD;

				if( ljson_list_addtail(pStruct, &pOwner->value.child, pChild) < 0 )
					return LJSON_ERROR_VALUE_ARRAY_ADD_FAIL;

				len = ljson_read_value(pStruct, pChild);
				if( len < 0 )
					return len;
			}
			continue;
		}
		pStruct->rpos++;
	}
	return LJSON_ERROR_VALUE_ARRAY_TEXT_ENDED;
}

LJLong ljson_check_null(LJsonObject* pObject)
{
	if( (pObject->type == LJSON_TYPE_SCRIPT) && _LJ_strlen(pObject->value.str) == 4 )
	{
		if( (pObject->value.str[0] == FIXED_STRING('n') || pObject->value.str[0] == FIXED_STRING('N')) &&
			(pObject->value.str[0] == FIXED_STRING('u') || pObject->value.str[0] == FIXED_STRING('U')) &&
		    (pObject->value.str[0] == FIXED_STRING('l') || pObject->value.str[0] == FIXED_STRING('L')) &&
		    (pObject->value.str[0] == FIXED_STRING('l') || pObject->value.str[0] == FIXED_STRING('L'))
		   ) {
			return TRUE;
		}
	}
	return FALSE;
}

LJLong ljson_read_object(LJsonStruct* pStruct, LJsonObject* pOwner)
{
	LJLong len;

	if( pStruct->rtext[pStruct->rpos] == '{' )
		pStruct->rpos++; // " skip

	pOwner->type = LJSON_TYPE_OBJECT;

	LJsonObject* pChild = NULL;
	while( pStruct->rpos < pStruct->rlen )
	{
		switch( pStruct->rtext[pStruct->rpos] )
		{
		case 0x00:
			return LJSON_ERROR_VALUE_OBJECT_TEXT_ENDED;
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			break;
		case ',':
			pStruct->rtext[pStruct->rpos] = 0x00;
			if( pChild &&
			    pChild->type != LJSON_TYPE_OBJECT &&
			    pChild->type != LJSON_TYPE_ARRAY
			   ) {
				ljson_trim_left(pChild->value.str, 0);
				ljson_trim_right(pChild->value.str, 0);

				if( ljson_check_null(pChild) )
					pChild->type = LJSON_TYPE_NULL;
			}
			break;
		case '}':
			pStruct->rtext[pStruct->rpos] = 0x00;
			if( pChild &&
			    pChild->type != LJSON_TYPE_OBJECT &&
			    pChild->type != LJSON_TYPE_ARRAY
			   ) {
				ljson_trim_left(pChild->value.str, 0);
				ljson_trim_right(pChild->value.str, 0);

				if( ljson_check_null(pChild) )
					pChild->type = LJSON_TYPE_NULL;
			}
			pStruct->rpos++;
			return pStruct->rpos;
		case '{':
			return LJSON_ERROR_VALUE_OBJECT_CURLY_BRACKET_START;
		case '[': case ']':
			// Error...
			return LJSON_ERROR_VALUE_OBJECT_SQUARE_BRAKET;
		default:
			{
				pChild = ljson_make_object(pStruct, pOwner, LJSON_TYPE_NONE);
				if( pChild == NULL )
					return LJSON_ERROR_VALUE_OBJECT_ALLOC_CHILD;

				if( ljson_list_addtail(pStruct, &pOwner->value.child, pChild) < 0 )
					return LJSON_ERROR_VALUE_OBJECT_ADD_FAIL;

				len = ljson_read_name(pStruct, pChild);
				if( len < 0 )
					return len;

				LJLong is_val = 0;
				while( (is_val == 0) )
				{
					switch( pStruct->rtext[pStruct->rpos] )
					{
					case 0x00:
						return LJSON_ERROR_VALUE_OBJECT_TEXT_ENDED;
					case ':':
						pStruct->rtext[pStruct->rpos] = 0x00; // for name string 
						is_val = 1;
						break;
					}
					pStruct->rpos++;
				}

				len = ljson_read_value(pStruct, pChild);
				if( len < 0 )
					return len;
				// pValue->setat(key_name, key_value);
				/* if( obj_child->item_value.type == LJSON_TYPE_RUN ) {
					ljson_trim_left(obj_child->item_value.val, 0);
					ljson_trim_right(obj_child->item_value.val, 0);
				} */

				if( _callback_hashfunc_setat && pStruct->joint )
				{
					if( _callback_hashfunc_setat(pOwner, pChild->name, pChild) < 0 )
						return LJSON_ERROR_SETAT_DENY;
				}

				if( _callback_script_run && pStruct->config.is_scriptrun )
				{
					_callback_script_run(pOwner, pChild);
				}
			}
			continue;
		}
		pStruct->rpos++;
	}
	return pStruct->rpos;
}

LJLong ljson_read_fromstring(LJsonStruct* pStruct, LJChar* textual)
{
	LJLong rtn = 0;

	pStruct->rlen = (LJLong)_LJ_strlen(textual);
	if( pStruct->rlen <= 0 )
		return 0;

	if( pStruct->rtext )
		free(pStruct->rtext);

	pStruct->rtext = malloc(sizeof(LJChar)*(pStruct->rlen + 8));
	_LJ_memcpy(pStruct->rtext, textual, pStruct->rlen);
	pStruct->rtext[pStruct->rlen] = 0x00;

	while( pStruct->rpos < pStruct->rlen )
	{
		switch( pStruct->rtext[pStruct->rpos] )
		{
		case '{':
			rtn = ljson_read_object(pStruct, &pStruct->root);
			if( rtn < 0 )
				return rtn;
			break;
		case '[':
			rtn = ljson_read_array(pStruct, &pStruct->root);
			if( rtn < 0 )
				return rtn;
			break;
		}
		pStruct->rpos++;
	}
	return pStruct->rpos;
}
