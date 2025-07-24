#include "stdafx.h"
#include "TestJson.h"
#include "TestJsonDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _UNICODE
#define CHAR_SET(str) _T(str)
#else
#define CHAR_SET(str) str
#endif 
#include "SimpleJson.h"



CString CTestJsonDlg::m_text =
_T("{ \
	\"array_1\" : [ \
		\"value 1\", \
		\"value 2\", \
		\"value 3\", \
		{ \
			\"obj_1\" : { \
				\"txt 1\" : 1, \
				\"txt 2\" : 2.3, \
				\"txt 3\" : false, \
				\"txt 4\" : null, \
			} \
		} \
	], \
\"web-app\":{ \
\"servlet\": [ \
		{ \
	\"servlet-name\": \"cofaxCDS\", \
		\"servlet-class\" : \"org.cofax.cds.CDSServlet\", \
		\"init-param\" : { \
		\"configGlossary:installationAt\": \"Philadelphia, PA\", \
		\"configGlossary:adminEmail\" : \"ksm@pobox.com\", \
		\"configGlossary:poweredBy\" : \"Cofax\", \
		\"configGlossary:poweredByIconv\" : \"/images/cofax.gifv\", \
					\"configGlossary:staticPath\" : \"/content/static\", \
					\"templateProcessorClass\" : \"org.cofax.WysiwygTemplate\", \
					\"templateLoaderClass\" : \"org.cofax.FilesTemplateLoader\", \
					\"templatePath\" : \"templates\", \
					\"templateOverridePath\" : \"\", \
					\"defaultListTemplate\" : \"listTemplate.htm\", \
					\"defaultFileTemplate\" : \"articleTemplate.htm\", \
					\"useJSP\" : false, \
					\"jspListTemplate\" : \"listTemplate.jsp\", \
					\"jspFileTemplate\" : \"articleTemplate.jsp\", \
					\"cachePackageTagsTrack\" : 200, \
					\"cachePackageTagsStore\" : 200, \
					\"cachePackageTagsRefresh\" : 60, \
					\"cacheTemplatesTrack\" : 100, \
					\"cacheTemplatesStore\" : 50, \
					\"cacheTemplatesRefresh\" : 15, \
					\"cachePagesTrack\" : 200, \
					\"cachePagesStore\" : 100, \
					\"cachePagesRefresh\" : 10, \
					\"cachePagesDirtyRead\" : 10, \
					\"searchEngineListTemplate\" : \"forSearchEnginesList.htm\", \
					\"searchEngineFileTemplate\" : \"forSearchEngines.htm\", \
					\"searchEngineRobotsDb\" : \"WEB-INF/robots.db\", \
					\"useDataStore\" : true, \
					\"dataStoreClass\" : \"org.cofax.SqlDataStore\", \
					\"redirectionClass\" : \"org.cofax.SqlRedirection\", \
					\"dataStoreName\" : \"cofax\", \
					\"dataStoreDriver\" : \"com.microsoft.jdbc.sqlserver.SQLServerDriver\", \
					\"dataStoreUrl\" : \"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\", \
					\"dataStoreUser\" : \"sa\", \
					\"dataStorePassword\" : \"dataStoreTestQuery\", \
					\"dataStoreTestQuery\" : \"SET NOCOUNT ON;select test='test';\", \
					\"dataStoreLogFile\" : \"/usr/local/tomcat/logs/datastore.log\", \
					\"dataStoreInitConns\" : 10, \
					\"dataStoreMaxConns\" : 100, \
					\"dataStoreConnUsageLimit\" : 100, \
					\"dataStoreLogLevel\" : \"debug\", \
					\"maxUrlLength\" : 500 \
			} \
		}, \
			{ \
				\"servlet-name\": \"cofaxEmail\", \
				\"servlet-class\" : \"org.cofax.cds.EmailServlet\", \
				\"init-param\" : { \
					\"mailHost\": \"mail1\", \
					\"mailHostOverride\" : \"mail2\" \
				} \
			}, \
			{ \
				\"servlet-name\": \"cofaxAdmin\", \
				\"servlet-class\" : \"org.cofax.cds.AdminServlet\" \
			}, \
			{ \
				\"servlet-name\": \"fileServlet\", \
				\"servlet-class\" : \"org.cofax.cds.FileServlet\" \
			}, \
			{ \
				\"servlet-name\": \"cofaxTools\", \
				\"servlet-class\" : \"org.cofax.cms.CofaxToolsServlet\", \
				\"init-param\" : { \
					\"templatePath\": \"toolstemplates/\", \
					\"log\" : 1, \
					\"logLocation\" : \"/usr/local/tomcat/logs/CofaxTools.log\", \
					\"logMaxSize\" : \"\", \
					\"dataLog\" : 1, \
					\"dataLogLocation\" : \"/usr/local/tomcat/logs/dataLog.log\", \
					\"dataLogMaxSize\" : \"\", \
					\"removePageCache\" : \"/content/admin/remove?cache=pages&id=\", \
					\"removeTemplateCache\" : \"/content/admin/remove?cache=templates&id=\", \
					\"fileTransferFolder\" : \"/usr/local/tomcat/webapps/content/fileTransferFolder\", \
					\"lookInContext\" : 1, \
					\"adminGroupID\" : 4, \
					\"betaServer\" : true \
				}  \
			} \
		], \
			\"servlet-mapping\": { \
				\"cofaxCDS\": \"/\", \
					\"cofaxEmail\" : \"/cofaxutil/aemail/*\", \
					\"cofaxAdmin\" : \"/admin/*\", \
					\"fileServlet\" : \"/static/*\", \
					\"cofaxTools\" : \"/tools/*\" \
			}, \
				\"taglib\": { \
					\"taglib-uri\": \"cofax.tld\", \
						\"taglib-location\" : \"/WEB-INF/tlds/cofax.tld\" \
				} \
	} \
} \
");

void CTestJsonDlg::OnBnClickedButtonCTest()
{
	UpdateData(TRUE);
	CString stext = m_JsonText.GetBuffer();
	m_EditOutput = "";

	SimpleJson json;
	if( json.fromString(stext.GetBuffer()) < 0 )
	{
		m_EditOutput = json.getError();
	}
	else
	{
		m_EditOutput = json.toString();
	}

	LJItem root = json.GetRoot();
	LJItem item_1 = root.GetObject((CString)"array_1");
	CString val = item_1.GetString(1);
	TRACE(L"VAL:%s\n", val);
	UpdateData(FALSE);
}

void CTestJsonDlg::OnBnClickedButtonCPPTest()
{

	LJsonStruct jsons;
	LJsonStruct* pStruct = ljson_initialize(&jsons);
	if( pStruct == NULL )
	{
		TRACE(L"초기화 오류가 발생했습니다.");
		return;
	}

	pStruct->config.is_ownercheck = 1;

	LJsonObject* ptr_root = ljson_get_root(pStruct);
	if( ptr_root )
	{
		LJsonObject* ptr_obj1 = ljson_make_object(pStruct, NULL, LJSON_TYPE_OBJECT);
		if( ptr_obj1 )
		{
			// ljson_set_object(ptr_obj1, "prop", ptr_test);
			ljson_set_string(ptr_obj1, (LJChar*)L"name", (LJChar*)L"test");
			ljson_set_integer(ptr_obj1, (LJChar*)L"int", 1);
			ljson_set_double(ptr_obj1, (LJChar*)L"dbl", 2.3);

			ljson_set_object(ptr_root, (LJChar*)L"ptr_obj1", ptr_obj1);
		}

		LJsonObject* ptr_array = ljson_make_object(pStruct, NULL, LJSON_TYPE_ARRAY);
		if( ptr_array )
		{
			ljson_add_array_object(ptr_array, ptr_obj1);
			ljson_add_array_string(ptr_array, (LJChar*)L"array_2");
			ljson_add_array_integer(ptr_array, 12);
			ljson_add_array_double(ptr_array, 34.56);

			ljson_set_object(ptr_root, (LJChar*)CHAR_SET("array"), ptr_array);
		}
	}

	TRACE(L"Output: \n%s \n", ljson_write_tostring(ptr_root));

	ptr_root = ljson_get_root(pStruct);
	if( ptr_root )
	{
		LJsonObject* ptr_obj1 = ljson_get_object(ptr_root, (LJChar*)CHAR_SET("ptr_obj1"));
		if( ptr_obj1 )
		{
			LJChar* v1 = ljson_get_string(ptr_obj1, (LJChar*)CHAR_SET("name"));
			int     v2 = ljson_get_integer(ptr_obj1, (LJChar*)CHAR_SET("int"));
			TRACE(L"[%s] %s = %d \n", L"name", v1, v2);
		}

		LJsonObject* ptr_array = ljson_get_object(ptr_root, (LJChar*)CHAR_SET("array"));
		if( ptr_array )
		{
			LJsonObject*  v1 = ljson_get_array_object(ptr_array, 0);
			LJChar*       v2 = ljson_get_array_string(ptr_array, 1);
			LJChar*		  v3 = ljson_get_array_string(ptr_array, 2);

			TRACE(CHAR_SET("%s = %s, %s, %s\n"), CHAR_SET("array"),
				ljson_get_string(v1, (LJChar*)CHAR_SET("dbl")),
				v2, v3
			);
		}
	}

	{
		int pos;
		int cnt = pStruct->mem_str.cnt;
		LJChar* ptr = (LJChar*)pStruct->mem_str.val;
		for( pos = 0; pos < cnt; pos++ )
		{
			TRACE(L"%3d STR: '%s'\n",
				pos, ptr
			);
			ptr += wcslen((wchar_t*)ptr) + 1;
		}
	}
	ljson_destroy(pStruct);
}
