/*
 create    : 20190819
 modify    : 20190819
 developer : lclasser@gmail.com
*/
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include "ljson_fundamental.h"

#ifdef _UNICODE
#include <wchar.h>
#endif // _UNICODE

LJLong ljson_write_realloc(LJsonStruct* pStruct, LJLong inc_len)
{
	if( inc_len < 4096 )
		inc_len = 4096;

	LJChar* tmp_text = malloc(sizeof(LJChar) * (pStruct->wsize + inc_len + 8));
	if( pStruct->wtext )
		_LJ_memcpy(tmp_text, pStruct->wtext, pStruct->wpos);
	tmp_text[pStruct->wpos] = 0x00;

	LJChar* del_text = pStruct->wtext;
	pStruct->wtext = tmp_text;
	pStruct->wsize += inc_len;
	if( del_text )
		free(del_text);

	return 0;
}

LJLong ljson_write_clear(LJsonStruct* pStruct)
{
	if( pStruct->wtext )
		free(pStruct->wtext);
	pStruct->wtext = NULL;
	pStruct->wpos = 0;
	pStruct->wsize = 0;
	return 0;
}

LJLong ljson_write_addstring(LJsonStruct* pStruct, LJChar* str_val)
{
	LJLong len = (LJLong)_LJ_strlen(str_val);
	if( len <= 0 )
		return 0;

	if( (pStruct->wtext == NULL) || ((pStruct->wpos + len) >= pStruct->wsize) )
		ljson_write_realloc(pStruct, len);

	_LJ_memcpy(&pStruct->wtext[pStruct->wpos], str_val, len);
	pStruct->wtext[pStruct->wpos + len] = 0x00;
	pStruct->wpos += len;
	return len;
}

LJLong ljson_write_addindent(LJsonStruct* pStruct, LJLong depth)
{
	if( pStruct->config.is_indent )
	{
		LJLong len = pStruct->config.cnt_space * depth;
		if( (pStruct->wtext == NULL) || ((pStruct->wpos + len) >= pStruct->wsize) )
			ljson_write_realloc(pStruct, len);

		if( pStruct->config.is_tabindent )
		{
			_LJ_memset(&pStruct->wtext[pStruct->wpos], '\t', depth);
			pStruct->wpos += depth;
		}
		else
		{
			_LJ_memset(&pStruct->wtext[pStruct->wpos], ' ', pStruct->config.cnt_space * depth);
			pStruct->wpos += (pStruct->config.cnt_space * depth);
		}
	}
	return 0;
}

LJLong ljson_write_object(LJsonStruct* pStruct, LJsonObject* pOwner, LJLong depth)
{
	LJLong is_last = 0;
	// LJLong nPos;
	// LJLong nCnt = ljson_get_object_count(pOwner->value.obj);
	// for( nPos = 0; nPos < nCnt; nPos++ )
	LJLinkItem* ptr_next = ljson_get_object_head(pOwner);
	while( ptr_next )
	{
		LJsonObject* pCHILD = ljson_get_object_next(pOwner, &ptr_next);

		if( ptr_next == NULL )
			is_last = 1;

		LJChar* str_name = NULL;
		if( ljson_get_type(pOwner) != LJSON_TYPE_ARRAY ) {
			str_name = ljson_get_name(pCHILD);
		}

		switch( ljson_get_type(pCHILD) )
		{
		case LJSON_TYPE_STRING:
			{
				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);

				LJChar* str_value = pCHILD->value.str;
				if( str_name ) {
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
					ljson_write_addstring(pStruct, str_name);
					ljson_write_addstring(pStruct, FIXED_STRING("\""));

					ljson_write_addstring(pStruct, FIXED_STRING(" : "));
				}

				if( str_value != NULL ) {
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
					ljson_write_addstring(pStruct, str_value);
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
				} else {
					ljson_write_addstring(pStruct, FIXED_STRING("NULL"));
				}
			}
			break;
		case LJSON_TYPE_FLOAT:
		case LJSON_TYPE_INTEGER:
			{
				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);

				LJChar* str_value = pCHILD->value.str;
				if( str_name ) {
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
					ljson_write_addstring(pStruct, str_name);
					ljson_write_addstring(pStruct, FIXED_STRING("\""));

					ljson_write_addstring(pStruct, FIXED_STRING(" : "));
				}

				if( str_value != NULL ) {
					ljson_write_addstring(pStruct, str_value);
				} else {
					ljson_write_addstring(pStruct, FIXED_STRING("NULL"));
				}
			}
			break;
		case LJSON_TYPE_OBJECT:
			{
				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);

				if( str_name ) {
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
					ljson_write_addstring(pStruct, str_name);
					ljson_write_addstring(pStruct, FIXED_STRING("\""));

					ljson_write_addstring(pStruct, FIXED_STRING(" : "));
				}
				ljson_write_addstring(pStruct, FIXED_STRING("{"));
				if( pStruct->config.is_newline ) ljson_write_addstring(pStruct, FIXED_STRING("\r\n"));

				ljson_write_object(pStruct, pCHILD, depth + 1);
				if( pStruct->config.is_newline ) ljson_write_addstring(pStruct, FIXED_STRING("\r\n"));

				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);
				ljson_write_addstring(pStruct, FIXED_STRING("}"));
			}
			break;
		case LJSON_TYPE_ARRAY:
			{
				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);

				if( str_name ) {
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
					ljson_write_addstring(pStruct, str_name);
					ljson_write_addstring(pStruct, FIXED_STRING("\""));

					ljson_write_addstring(pStruct, FIXED_STRING(" : "));
				}
				ljson_write_addstring(pStruct, FIXED_STRING("["));
				if( pStruct->config.is_newline ) ljson_write_addstring(pStruct, FIXED_STRING("\r\n"));

				ljson_write_object(pStruct, pCHILD, depth + 1);
				if( pStruct->config.is_newline ) ljson_write_addstring(pStruct, FIXED_STRING("\r\n"));

				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);
				ljson_write_addstring(pStruct, FIXED_STRING("]"));
			}
			break;
		case LJSON_TYPE_SCRIPT:
			{
				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);

				LJChar* str_value = pCHILD->value.str;
				if( str_name ) {
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
					ljson_write_addstring(pStruct, str_name);
					ljson_write_addstring(pStruct, FIXED_STRING("\""));

					ljson_write_addstring(pStruct, FIXED_STRING(" : "));
				}
				ljson_write_addstring(pStruct, str_value);
			}
			break;
		case LJSON_TYPE_NULL:
			{
				if( pStruct->config.is_indent ) ljson_write_addindent(pStruct, depth);

				if( str_name ) {
					ljson_write_addstring(pStruct, FIXED_STRING("\""));
					ljson_write_addstring(pStruct, str_name);
					ljson_write_addstring(pStruct, FIXED_STRING("\""));

					ljson_write_addstring(pStruct, FIXED_STRING(" : "));
				}
				ljson_write_addstring(pStruct, FIXED_STRING("NULL"));
			}
			break;
		}

		if( is_last == 0 ) {
			ljson_write_addstring(pStruct, FIXED_STRING(","));
			if( pStruct->config.is_newline ) ljson_write_addstring(pStruct, FIXED_STRING("\r\n"));
		} else {
		}
	}
	return 0;
}

LJChar* ljson_write_tostring(LJsonObject* pObject)
{
	LJsonStruct* pStruct = pObject->master;
	if( pStruct && pObject )
	{
		ljson_write_clear(pStruct);

		if( pObject->type == LJSON_TYPE_ARRAY )
			ljson_write_addstring(pStruct, FIXED_STRING("["));
		else
			ljson_write_addstring(pStruct, FIXED_STRING("{"));

		if( pStruct->config.is_newline ) ljson_write_addstring(pStruct, FIXED_STRING("\r\n"));

		if( ljson_write_object(pStruct, pObject, 1) >= 0 )
		{
			if( pStruct->config.is_newline ) ljson_write_addstring(pStruct, FIXED_STRING("\r\n"));

			if( pObject->type == LJSON_TYPE_ARRAY )
				ljson_write_addstring(pStruct, FIXED_STRING("]"));
			else
				ljson_write_addstring(pStruct, FIXED_STRING("}"));

			return pStruct->wtext;
		}
	}
	return NULL;
}
