#pragma once
#include "..\include\LJson.h"
class SimpleJson : public LJson
{
public:
	SimpleJson();
	virtual ~SimpleJson();

	CMapStringToPtr		m_mapHash;

protected:
	virtual LJLong onHashSetAt(void* owner, LJChar* key, LJsonObject*  val);
	virtual LJLong onHashLookup(void* owner, LJChar* key, LJsonObject**  val);
	virtual LJLong onScriptRun(void* owner, LJsonObject*  val);
};
