#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/ljson_fundamental.h"

#define CHAR_SET(val) val

int main(int argc, char** argv)
{
	printf("hello~\n");

	// TODO: Add your control notification handler code here

	LJsonStruct jsons;
	LJsonStruct* pStruct = ljson_initialize(&jsons);
	if( pStruct == NULL )
	{
		printf("초기화 오류가 발생했습니다.");
		return 0;
	}

	pStruct->config.is_ownercheck = 1;

	LJsonObject* ptr_root = ljson_get_root(pStruct);
	if( ptr_root )
	{
		LJsonObject* ptr_obj1 = ljson_make_object(pStruct, NULL, LJSON_TYPE_OBJECT);
		if( ptr_obj1 )
		{
			// ljson_set_object(ptr_obj1, "prop", ptr_test);
			ljson_set_string (ptr_obj1, "name", "test");
			ljson_set_integer(ptr_obj1, "int", 1);
			ljson_set_double (ptr_obj1, "dbl", 2.3);

			ljson_set_object(ptr_root, "ptr_obj1", ptr_obj1);
		}

		LJsonObject* ptr_array = ljson_make_object(pStruct, NULL, LJSON_TYPE_ARRAY);
		if( ptr_array )
		{
			ljson_add_array_object (ptr_array, ptr_obj1);
			ljson_add_array_string (ptr_array, "array_2");
			ljson_add_array_integer(ptr_array, 12);
			ljson_add_array_double (ptr_array, 34.56);

			ljson_set_object(ptr_root, (LJChar*)CHAR_SET("array"), ptr_array);
		}
	}
	
	printf("Output: \n%s \n", ljson_write_tostring(ptr_root));

	ptr_root = ljson_get_root(pStruct);
	if( ptr_root )
	{
		LJsonObject* ptr_obj1 = ljson_get_object(ptr_root, (LJChar*)CHAR_SET("ptr_obj1"));
		if( ptr_obj1 )
		{
			char* v1 = ljson_get_string (ptr_obj1, (LJChar*)CHAR_SET("name"));
			int   v2 = ljson_get_integer(ptr_obj1, (LJChar*)CHAR_SET("int"));
			printf("%s = %s, %d \n", "name", v1, v2);
		}

		LJsonObject* ptr_array = ljson_get_object(ptr_root, (LJChar*)CHAR_SET("array"));
		if( ptr_array )
		{
			LJsonObject*  v1 = ljson_get_array_object(ptr_array, 0);
			char*         v2 = ljson_get_array_string(ptr_array, 1);
			char*		  v3 = ljson_get_array_string(ptr_array, 2);
			
			printf(CHAR_SET("%s = %s, %s, %s\n"), CHAR_SET("array"),
				  ljson_get_string(v1, (LJChar*)CHAR_SET("dbl")),
				  v2, v3
			);
		}
	}

	printf(CHAR_SET("obj count: %ld\n"), ljson_count_object(pStruct));
	printf(CHAR_SET("str count: %ld\n"), ljson_count_string(pStruct));

	{
		int pos;
		int cnt = pStruct->mem_str.cnt;
		char* ptr = pStruct->mem_str.val;
		for( pos=0; pos< cnt; pos++ )
		{
			printf("%3d STR: '%s'\n",
				pos, ptr
			);
			ptr += strlen(ptr) + 1;
		}
	}

	ljson_destroy(pStruct);

	return 0;
}
