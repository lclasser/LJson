#include "stdafx.h"
#include "SimpleJson.h"


SimpleJson::SimpleJson()
{
}

SimpleJson::~SimpleJson()
{
	LJMemory* pMemory = &this->m_struct.mem_obj;

	int typearray_cnt = 0;
	int noname_cnt = 0;
	int pos;
	int cnt = pMemory->pos;
	for( pos=0; pos< cnt; pos++ )
	{
		LJsonObject* obj = &((LJsonObject*)pMemory->val)[pos];

		CString strType;
		LPTSTR lpszType = strType.GetBufferSetLength(16);
		ljson_get_typestring(obj, (LJChar*)lpszType, 16);

		if( obj->name == NULL )
		{
			if( obj->owner )
				ljson_get_typestring(obj->owner, (LJChar*)lpszType, 16);

			TRACE(L"OBJ owner-type[%-16.16s] name[null]\n", strType);
			noname_cnt++;
		}
		else
		{
			TRACE(L"OBJ       type[%-16.16s] name[%s]\n", strType, obj->name);
		}

		if( obj->type == LJSON_TYPE_ARRAY )
		{
			typearray_cnt++;
		}
		strType.ReleaseBuffer();
	}
	TRACE(L"noname_cnt    : %d\n", noname_cnt);
	TRACE(L"typearray_cnt : %d\n", typearray_cnt);

	{
		int pos = 0;
		int cnt = this->m_struct.mem_str.cnt;
		LJChar* ptr = (LJChar*)this->m_struct.mem_str.val;
		for( pos = 0; pos < cnt; pos++ )
		{
			TRACE(L"%3d STR: '%s'\n",
				   pos, ptr
			);
			ptr += wcslen((wchar_t*)ptr) + 2;
		}
	}
	m_mapHash.RemoveAll();
}

LJLong SimpleJson::onHashSetAt(void* owner, LJChar* key, LJsonObject*  val)
{
	if( key == NULL )
		return 0;

	CString sKey;
	sKey.Format(FIXED_STRING("%p.%s"), owner, key);
	m_mapHash.SetAt(sKey, val);
	return 0;
}

LJLong SimpleJson::onHashLookup(void* owner, LJChar* key, LJsonObject**  val)
{
	CString sKey;
	sKey.Format(FIXED_STRING("%p.%s"), owner, key);
	return m_mapHash.Lookup(sKey, (void*&)*val);
}

LJLong SimpleJson::onScriptRun(void* owner, LJsonObject*  val)
{
	return -1;
}
