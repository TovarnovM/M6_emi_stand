
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>


#include "RSS_unit.h"

#pragma warning(disable : 4996)


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


/*********************************************************************/
/*								     */
/*		      ���������� ������ "���������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*		       ����������� ������ "���������"      	     */

     RSS_Unit::RSS_Unit(void)

{
     Owner=NULL ;
}


/*********************************************************************/
/*								     */
/*		        ���������� ������ "���������"      	     */

    RSS_Unit::~RSS_Unit(void)

{
}

