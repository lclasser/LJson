#pragma once
#include "ljson_fundamental.h"

#ifdef _UNICODE
typedef wchar_t*	LJString;
#else
typedef char*		LJString;
#endif // _UNICODE

class LJSON_EXPORT LJItem
{
public:
	LJItem();
	LJItem(LJsonObject* pObject);
	~LJItem();

	// getter
	LJsonObject*	GetObject(LPCTSTR lpszName);
	LJString		GetString(LPCTSTR lpszName);
	long			GetInteger(LPCTSTR lpszName);
	double			GetDouble(LPCTSTR lpszName);

	LJsonObject*	GetObject(int pos);
	LJString		GetString(int pos);
	long			GetInteger(int pos);
	double			GetDouble(int pos);
	int				GetCount();

	// setter
	LJsonObject*	CreateObject(LPCTSTR lpszName);
	LJsonObject*	CreateArray(LPCTSTR lpszName);

	int				SetValue(LPCTSTR lpszName, LJItem* ptrObj);
	int				SetValue(LPCTSTR lpszName, LJString strVal);
	int				SetValue(LPCTSTR lpszName, long nVal);
	int				SetValue(LPCTSTR lpszName, double dVal);

	int				AddValue(LJItem* ptrObj);
	int				AddValue(LJString strVal);
	int				AddValue(long nVal);
	int				AddValue(double dVal);

	// operator
	LJItem& operator= (LJsonObject* obj);

	// utilize
	LJString toString();
	BOOL isValid() { return m_pObject == NULL ? FALSE : TRUE; }

protected:
	LJsonObject*	m_pObject;
	friend class LJson;
};

class LJSON_EXPORT LJson
{
public:
	LJson();
	virtual ~LJson();

protected:
	LJsonStruct		m_struct;
	LJString		m_strError;

public:
	int fromString(LJString message);
	LJString toString();
	LJString getError() { return m_strError; }

	LJItem GetRoot();

protected:
	virtual LJLong onHashSetAt(void* owner, LJChar* key, LJsonObject*  val) { return -1; }
	virtual LJLong onHashLookup(void* owner, LJChar* key, LJsonObject**  val) { return -1; }
	virtual LJLong onScriptRun(void* owner, LJsonObject*  val) { return -1; }

	static LJLong hashSetAt (void* owner, LJChar* key, LJsonObject*  val);
	static LJLong hashLookup(void* owner, LJChar* key, LJsonObject** val);
	static LJLong scriptRun(void* owner, LJsonObject* val);
};
