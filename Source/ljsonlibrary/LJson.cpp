#include "stdafx.h"
#include "LJson.h"


////////////////////
// LJItem
LJItem::LJItem()
{
	m_pObject = NULL;
}

LJItem::LJItem(LJsonObject* pObject)
{
	m_pObject = pObject;
}

LJItem::~LJItem()
{
	m_pObject = NULL;
}

LJItem& LJItem::operator= (LJsonObject* obj)
{
	this->m_pObject = obj;
	return *this;
}

LJString LJItem::toString()
{
	return (LJString)ljson_write_tostring(this->m_pObject);
}

// getter : object
LJsonObject* LJItem::GetObject(LPCTSTR lpszName)
{
	LJsonObject* ptr_object = ljson_get_object(this->m_pObject, (LJChar*)lpszName);
	return ptr_object;
}

LJsonObject* LJItem::GetObject(int pos)
{
	return ljson_get_array_object(this->m_pObject, pos);
}

// getter : string
LJString LJItem::GetString(LPCTSTR lpszName)
{
	return (LJString)ljson_get_string(this->m_pObject, (LJChar*)lpszName);
}

LJString LJItem::GetString(int pos)
{
	return (LJString)ljson_get_array_string(this->m_pObject, pos);
}

// getter : integer
long LJItem::GetInteger(LPCTSTR lpszName)
{
	return ljson_get_integer(this->m_pObject, (LJChar*)lpszName);
}

long LJItem::GetInteger(int pos)
{
	return ljson_get_array_integer(this->m_pObject, pos);
}

// getter : double
double LJItem::GetDouble(LPCTSTR lpszName)
{
	return ljson_get_double(this->m_pObject, (LJChar*)lpszName);
}

double LJItem::GetDouble(int pos)
{
	return ljson_get_array_double(this->m_pObject, pos);
}

int LJItem::GetCount()
{
	return ljson_get_array_count(this->m_pObject);
}

int LJItem::GetType(LPCTSTR lpszName)
{
	LJsonObject* ptr_object = this->m_pObject;
	if( lpszName != NULL )
		ptr_object = ljson_get_object(this->m_pObject, (LJChar*)lpszName);

	return ljson_get_type(ptr_object);
}

LJString LJItem::GetName()
{
	return (LJString)ljson_get_name(this->m_pObject);
}

// setter
LJsonObject* LJItem::CreateObject(LPCTSTR lpszName)
{
	LJsonObject* pChild = this->GetObject(lpszName);
	if( pChild == NULL )
	{
		pChild = ljson_make_object(this->m_pObject->master, this->m_pObject, LJSON_TYPE_OBJECT);
		if( pChild )
			ljson_set_object(this->m_pObject, (LJChar*)lpszName, pChild);
	}
	return pChild;
}

LJsonObject* LJItem::CreateArray(LPCTSTR lpszName)
{
	LJsonObject* pChild = ljson_make_object(this->m_pObject->master, this->m_pObject, LJSON_TYPE_ARRAY);
	if( pChild )
		ljson_set_object(this->m_pObject, (LJChar*)lpszName, pChild);
	return pChild;
}

int LJItem::SetValue(LPCTSTR lpszName, LJItem* ptrObj)
{
	return ljson_set_object(this->m_pObject, (LJChar*)lpszName, ptrObj->m_pObject);
}

int LJItem::SetValue(LPCTSTR lpszName, LJString strVal)
{
	return ljson_set_string(this->m_pObject, (LJChar*)lpszName, (LJChar*)strVal);
}

int	LJItem::SetValue(LPCTSTR lpszName, LPCTSTR strVal)
{
	return ljson_set_string(this->m_pObject, (LJChar*)lpszName, (LJChar*)strVal);
}

int LJItem::SetValue(LPCTSTR lpszName, long nVal)
{
	return ljson_set_integer(this->m_pObject, (LJChar*)lpszName, nVal);
}

int LJItem::SetValue(LPCTSTR lpszName, double dVal)
{
	return ljson_set_double(this->m_pObject, (LJChar*)lpszName, dVal);
}

int LJItem::AddValue(LJItem* ptrObj)
{
	return ljson_add_array_object(this->m_pObject, ptrObj->m_pObject);
}

int LJItem::AddValue(LJString strVal)
{
	return ljson_add_array_string(this->m_pObject, (LJChar*)strVal);
}

int LJItem::AddValue(long nVal)
{
	return ljson_add_array_integer(this->m_pObject, nVal);
}

int LJItem::AddValue(double dVal)
{
	return ljson_add_array_double(this->m_pObject, dVal);
}

////////////////////
LJson::LJson()
{
	ljson_initialize(&m_struct);

	m_struct.config.is_indent = TRUE;
	m_struct.config.is_newline = TRUE;
	m_struct.config.is_tabindent = FALSE;
	m_struct.config.cnt_space = 4;

	m_struct.joint = (LJByte*)this;

#ifdef _WINDOWS
	ljson_set_cbhash(LJson::hashSetAt, LJson::hashLookup);
#endif // _WINDOWS
}

LJson::~LJson()
{
	ljson_destroy(&m_struct);
}

int LJson::fromString(LJString message)
{
	int rtn = ljson_read_fromstring(&m_struct, (LJChar*)message);
	if( rtn < 0 )
	{
		int npos = m_struct.rpos;
		if( npos < 10 )
			npos = 10;

		m_strError = &message[npos - 10];
		// m_strError.Format("[%d] 오류가 발생했습니다. pos(%d:%*.*s)", rtn, m_struct.rpos, 20, 20, er_mesg);
	}
	return rtn;
}

LJString LJson::toString()
{
	LJsonObject* pROOT = ljson_get_root(&m_struct);
	if( pROOT == NULL )
	{
		return NULL;
	}

	return (LJString)ljson_write_tostring(pROOT);
}

LJItem LJson::GetRoot()
{
	return ljson_get_root(&m_struct);
}

LJLong LJson::hashSetAt(void* owner, LJChar* key, LJsonObject* val)
{
	LJsonStruct* pStruct = ljson_get_struct((LJsonObject*)owner);
	if( pStruct == NULL )
		return -1;

	LJson* pJson = (LJson*)pStruct->joint;
	if( pJson == NULL )
		return -1;

	pJson->onHashSetAt(owner, key, val);
	return 0;
}

LJLong LJson::hashLookup(void* owner, LJChar* key, LJsonObject** val)
{
	LJsonStruct* pStruct = ljson_get_struct((LJsonObject*)owner);
	if( pStruct == NULL )
		return -1;

	LJson* pJson = (LJson*)pStruct->joint;
	if( pJson == NULL )
		return -1;

	return pJson->onHashLookup(owner, key, val);
}

LJLong LJson::scriptRun(void* owner, LJsonObject* val)
{
	LJsonStruct* pStruct = ljson_get_struct((LJsonObject*)owner);
	if( pStruct == NULL )
		return -1;

	LJson* pJson = (LJson*)pStruct->joint;
	if( pJson == NULL )
		return -1;

	return pJson->onScriptRun(owner, val);
}

LJLong LJson::onHashSetAt(void* owner, LJChar* key, LJsonObject*  val)
{
#ifdef _WINDOWS
	if( key == NULL )
		return -1;

	CString sKey;
	sKey.Format(FIXED_STRING("%p.%s"), owner, key);
	m_mapHash.SetAt(sKey, val);
	return 0;
#else
	return -1;
#endif // _WINDOWS
}

LJLong LJson::onHashLookup(void* owner, LJChar* key, LJsonObject**  val)
{
#ifdef _WINDOWS
	CString sKey;
	sKey.Format(FIXED_STRING("%p.%s"), owner, key);
	return m_mapHash.Lookup(sKey, (void*&)*val);
#else
	return NULL;
#endif // _WINDOWS
}
