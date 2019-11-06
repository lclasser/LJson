#pragma once 

#define LJSON_IMPORT

#ifdef _LJSON_USING_DYNAMIC_
#	ifndef _LJSON_LIBRARY_INTERNAL_
#		define LJSON_EXPORT       __declspec(dllimport)
#	else //	_LIBRARY_LBROKER_
#		define LJSON_EXPORT       __declspec(dllexport)
#	endif
#else // _USING_DYNAMIC_LIBRARY_
#		define LJSON_EXPORT
#endif // _USING_DYNAMIC_LIBRARY_

#ifdef _UNICODE
typedef unsigned short		LJChar;

#define FIXED_STRING(val)	L##val
#define _LJ_strlen			wcslen
#define _LJ_stricmp			_wcsicmp  
#define _LJ_atol(val)		wcstol(val, NULL, 10)
#define _LJ_atod(val)		wcstod(val, NULL)
#define _LJ_snprintf		swprintf
#define _LJ_memset(d,c,l)	wmemset(d,c,l)
#define _LJ_memcpy(d,s,l)	wmemcpy(d,s,l)
#else
typedef char				LJChar;

#ifdef _LINUX
#	define FIXED_STRING(val)	val
#	define _LJ_strlen			strlen
#	define _LJ_stricmp			strcasecmp
#	define _LJ_atol(val)		atoi(val)
#	define _LJ_atod(val)		atof(val)
#	define _LJ_snprintf			snprintf
#	define _LJ_memset(d,c,l)	memset(d,c,l)
#	define _LJ_memcpy(d,s,l)	memcpy(d,s,l)
#else
#	define FIXED_STRING(val)	val
#	define _LJ_strlen			strlen
#	define _LJ_stricmp			_stricmp
#	define _LJ_atol(val)		atoi(val)
#	define _LJ_atod(val)		atof(val)
#	define _LJ_snprintf			snprintf
#	define _LJ_memset(d,c,l)	memset(d,c,l)
#	define _LJ_memcpy(d,s,l)	memcpy(d,s,l)
#endif // _LINUX

#endif // _UNICODE

#ifndef NULL
#define NULL 0
#endif // NULL

#ifndef TRUE
#define TRUE 1
#endif // TRUE

#ifndef FALSE
#define FALSE 0
#endif // TRUE

#define LJSON_TYPE_NONE		0
#define LJSON_TYPE_NULL		1
#define LJSON_TYPE_STRING	2
#define LJSON_TYPE_INTEGER	3
#define LJSON_TYPE_FLOAT	4
#define LJSON_TYPE_SCRIPT	5
#define LJSON_TYPE_OBJECT	11
#define LJSON_TYPE_ARRAY	12

typedef struct _LJLinkItem		LJLinkItem;
typedef struct _LJLinkList		LJLinkList;

typedef struct _LJMemory		LJMemory;

typedef struct _LJsonValue     LJsonValue;
typedef struct _LJsonObject    LJsonObject;
typedef struct _LJsonStruct    LJsonStruct;

typedef unsigned char		LJByte;
typedef long				LJLong;
// typedef unsigned long long	LJPointer;

//////////////////////////////////////////////////////////////////
typedef struct _LJLinkItem {
	void*	next;
	LJByte*	data;
} __LJLinkItem;

typedef struct _LJLinkList {
	LJLinkItem*	head;
	LJLinkItem*	tail;

	LJLong		count;
} __LJLinkList;

typedef struct _LJMemory {
	unsigned short		idx;
	LJLong				size;	// item size
	LJLong				pos;	// item position
	LJLong				block;	// block position
	LJLong				cnt;	// calling count

	void*				head;
	void*				next;
	LJByte*				val;
	LJByte				_data[8];
} __LJMemory;

//////////////////////////////////////////////////////////////////
typedef struct _LJsonValue {
	union {
		LJChar*				str;
		LJsonObject*		obj;
	};
	LJLinkList			child;
} __LJsonValue;

typedef struct _LJsonObject { // count = object + array 
	char				temporary[6];
	char				used  : 1;
	char				alloc : 1;
	char				reserved : 6;
	LJByte				type;

	LJsonStruct*		master;
	LJsonObject*		owner;
	LJChar*				name;
	LJsonValue			value;
} __LJsonObject;

typedef struct {
	char		is_newline;
	char		is_indent;
	char		is_tabindent;
	char		is_ownercheck;
	char		is_scriptrun;
	char		cnt_space;
} LJsonConfig;

typedef LJLong (*LJsonScriptRun )(void* owner, LJsonObject*  val);
typedef LJLong (*LJsonHashSetAt )(void* owner, LJChar* key, LJsonObject*  val);
typedef LJLong (*LJsonHashLookup)(void* owner, LJChar* key, LJsonObject** val);
typedef struct _LJsonStruct {
	LJChar*				rtext;
	LJLong				rlen;
	LJLong				rpos;

	LJChar*				wtext;
	LJLong				wpos;
	LJLong				wsize;

	LJsonConfig			config;
	LJByte*				joint;

	LJsonObject			root;

	LJMemory			mem_list;
	LJMemory			mem_obj;
	LJMemory			mem_str;
} __LJsonStruct;

extern LJsonHashSetAt		_callback_hashfunc_setat;
extern LJsonHashLookup		_callback_hashfunc_lookup;
extern LJsonScriptRun		_callback_script_run;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// internal function only
LJSON_IMPORT LJLong			ljson_memory_initial(LJMemory* pMemory, LJLong size, LJLong block);
LJSON_IMPORT LJLong			ljson_memory_destroy(LJMemory* pMemory);
LJSON_IMPORT LJByte*		ljson_memory_alloc  (LJMemory* pMemory, LJLong cnt);

LJSON_IMPORT LJLong			ljson_list_addtail (LJsonStruct* pStruct, LJLinkList* pList, void* data);
LJSON_IMPORT LJLong			ljson_list_getcount(LJLinkList* pList);
LJSON_IMPORT LJLinkItem*	ljson_list_gethead (LJLinkList* pList);
LJSON_IMPORT void*			ljson_list_getnext (LJLinkList* pList, LJLinkItem** ptr_item);

// ljson_fundamental.c
LJSON_EXPORT LJsonStruct*	ljson_initialize(LJsonStruct* pStruct);
LJSON_EXPORT LJLong			ljson_destroy   (LJsonStruct* pStruct);
LJSON_EXPORT LJLong			ljson_set_cbhash(LJsonHashSetAt _setat, LJsonHashLookup _lookup);
LJSON_EXPORT LJLong			ljson_set_cbscript(LJsonScriptRun _run);

LJSON_IMPORT LJLong			ljson_is_number   (LJChar val);
LJSON_IMPORT LJLong			ljson_is_character(LJChar val);
LJSON_IMPORT LJChar*		ljson_trim_left   (LJChar* str, LJLong len);
LJSON_IMPORT LJChar* 		ljson_trim_right  (LJChar* str, LJLong len);

LJSON_EXPORT LJChar*		ljson_make_string(LJsonStruct* pStruct, LJChar* value);
LJSON_EXPORT LJsonObject*	ljson_make_object(LJsonStruct* pStruct, LJsonObject* pOwner, LJByte type);

LJSON_IMPORT LJLong			ljson_count_object(LJsonStruct* pStruct); // object + array
LJSON_IMPORT LJLong			ljson_count_string(LJsonStruct* pStruct); // string
LJSON_IMPORT LJLong			ljson_size_object(LJsonStruct* pStruct); // object + array
LJSON_IMPORT LJLong			ljson_size_string(LJsonStruct* pStruct); // string

// ljson_fundamental_get.c
LJSON_EXPORT LJLong			ljson_get_typestring(LJsonObject* obj, LJChar* value, LJLong size);
LJSON_EXPORT LJsonStruct*	ljson_get_struct(LJsonObject* pObject);
LJSON_EXPORT LJsonObject*	ljson_get_root(LJsonStruct* pStruct);

LJSON_EXPORT LJsonObject* 	ljson_get_object      (LJsonObject* pOwner, LJChar* name);
LJSON_EXPORT LJsonObject* 	ljson_get_object_item (LJsonObject* pOwner, LJLong pos);
LJSON_EXPORT LJLong			ljson_get_object_count(LJsonObject* pOwner);
LJSON_EXPORT LJLinkItem*	ljson_get_object_head (LJsonObject* pOwner);
LJSON_EXPORT LJsonObject*	ljson_get_object_next (LJsonObject* pOwner, LJLinkItem** ptr_next);

LJSON_EXPORT LJLong  		ljson_get_type   (LJsonObject* pObject);
LJSON_EXPORT LJChar* 		ljson_get_name   (LJsonObject* pObject);
LJSON_EXPORT LJChar*		ljson_get_value  (LJsonObject* pObject);

LJSON_EXPORT LJChar*		ljson_get_string (LJsonObject* pObject, LJChar* name);
LJSON_EXPORT LJLong			ljson_get_integer(LJsonObject* pObject, LJChar* name);
LJSON_EXPORT double			ljson_get_double (LJsonObject* pObject, LJChar* name);

LJSON_EXPORT LJLong			ljson_get_array_count  (LJsonObject* pOwner);
LJSON_EXPORT LJsonObject*	ljson_get_array_object (LJsonObject* pOwner, LJLong pos);
LJSON_EXPORT LJChar*		ljson_get_array_string (LJsonObject* pOwner, LJLong pos);
LJSON_EXPORT LJLong			ljson_get_array_integer(LJsonObject* pOwner, LJLong pos);
LJSON_EXPORT double			ljson_get_array_double (LJsonObject* pOwner, LJLong pos);

// ljson_fundamental_set.c
LJSON_EXPORT LJLong			ljson_set_object (LJsonObject* pOwner, LJChar* name, LJsonObject* pValue);
LJSON_EXPORT LJLong			ljson_set_string (LJsonObject* pOwner, LJChar* name, LJChar* value);
LJSON_EXPORT LJLong			ljson_set_integer(LJsonObject* pOwner, LJChar* name, LJLong value);
LJSON_EXPORT LJLong			ljson_set_double (LJsonObject* pOwner, LJChar* name, double value);

LJSON_EXPORT LJLong			ljson_add_array_object (LJsonObject* pArray, LJsonObject* pValue);
LJSON_EXPORT LJLong			ljson_add_array_string (LJsonObject* pArray, LJChar* value);
LJSON_EXPORT LJLong			ljson_add_array_integer(LJsonObject* pArray, LJLong value);
LJSON_EXPORT LJLong			ljson_add_array_double (LJsonObject* pArray, double value);

// LJSON_EXPORT LJLong			ljson_del_object   (LJsonObject* pArray, LJChar* name);
// LJSON_EXPORT LJLong			ljson_del_array_pos(LJsonObject* pArray, LJLong pos);

// ljson_fundamental_read.c
LJSON_EXPORT LJLong			ljson_read_fromstring(LJsonStruct* pStruct, LJChar* textual);

// ljson_fundamental_write.c
LJSON_EXPORT LJChar*		ljson_write_tostring(LJsonObject* pObject);
LJSON_IMPORT LJLong			ljson_write_clear(LJsonStruct* pStruct);
#ifdef __cplusplus
};
#endif // __cplusplus

#define LJSON_ERROR_NAME_TEXT_ENDED						-101
#define LJSON_ERROR_NAME_DELIMITER_COMMA				-102
#define LJSON_ERROR_NAME_CURLY_BRACKET					-103 // {} 오류
#define LJSON_ERROR_NAME_SQUARE_BRAKET					-104 // [] 오류
#define LJSON_ERROR_NAME_NOT_EXIST						-105
#define LJSON_ERROR_NAME_DUPLICATE_QUOTATION			-106 // ''' 연속 오류

#define LJSON_ERROR_LIST_ALLOC_FAIL						-131
#define LJSON_ERROR_ARG_OBJECT_NULL						-132
#define LJSON_ERROR_STRING_ALLOC_FAIL					-133
#define LJSON_ERROR_ARG_OWNER_NULL						-134
#define LJSON_ERROR_OWNER_CHILD_OWNERCHECK				-135
#define LJSON_ERROR_OWNER_CHILD_ADD_FAIL				-136
#define LJSON_ERROR_OBJECT_ALLOC_FAIL					-137
#define LJSON_ERROR_SETAT_DENY							-138

#define LJSON_ERROR_VALUE_STRING_CURLY_BRACKET_START	-231
#define LJSON_ERROR_VALUE_STRING_TEXT_ENDED				-232

#define LJSON_ERROR_VALUE_ARRAY_NOT_SQUARE_BRAKET		-341
#define LJSON_ERROR_VALUE_ARRAY_ALLOC_OBJECT			-342
#define LJSON_ERROR_VALUE_ARRAY_TEXT_ENDED				-343
#define LJSON_ERROR_VALUE_ARRAY_ALLOC_CHILD				-344
#define LJSON_ERROR_VALUE_ARRAY_ADD_FAIL				-345
#define LJSON_ERROR_VALUE_ARRAY_QUOTATION				-346

#define LJSON_ERROR_VALUE_OBJECT_ALLOC_OBJECT			-451
#define LJSON_ERROR_VALUE_OBJECT_TEXT_ENDED				-452
#define LJSON_ERROR_VALUE_OBJECT_CURLY_BRACKET_START	-453
#define LJSON_ERROR_VALUE_OBJECT_SQUARE_BRAKET			-454
#define LJSON_ERROR_VALUE_OBJECT_ALLOC_CHILD			-455
#define LJSON_ERROR_VALUE_OBJECT_ADD_FAIL				-456
#define LJSON_ERROR_VALUE_TEXT_ENDED					-461
