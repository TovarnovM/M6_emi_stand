
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <malloc.h>


#include "..\RSS_feature\RSS_feature.h"
#include "RSS_object.h"

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
/*	         ���������� ������ "������� ���������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       ����������� ������ "������� ���������"      	     */

     RSS_Transit::RSS_Transit(void)

{
   memset(action, 0, sizeof(action)) ;

          object=NULL ;
}


/*********************************************************************/
/*								     */
/*	        ���������� ������ "������� ���������"      	     */

    RSS_Transit::~RSS_Transit(void)

{
}


/********************************************************************/
/*								    */
/*	              ���������� ��������                           */

    int  RSS_Transit::vExecute(void)

{
   return(0) ;
}


/*********************************************************************/
/*								     */
/*		      ���������� ������ "������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*		       ����������� ������ "������"      	     */

     RSS_Object::RSS_Object(void)

{
     Parameters    =NULL ;
     Parameters_cnt=  0 ;
       Features    =NULL ;
       Features_cnt=  0 ;

        ErrorEnable=1 ;
}


/*********************************************************************/
/*								     */
/*		        ���������� ������ "������"      	     */

    RSS_Object::~RSS_Object(void)

{
    int  i ;

/*--------------------------------------------- �������� ����������� */

   for(i=0 ; i<this->Units.List_cnt ; i++)
                  delete this->Units.List[i].object ;

/*-------------------------------------------------------------------*/

}


/********************************************************************/
/*								    */
/*                        ���������� ������		            */

    class RSS_Object *RSS_Object::vCopy(char *name)

{
   return(NULL) ;
}


/********************************************************************/
/*								    */
/*		  ���./����. ��������� �� �������		    */

    void  RSS_Object::vErrorMessage(int  on_off)

{
         ErrorEnable=on_off ;
}


/********************************************************************/
/*								    */
/*		        ������� ������ �� ������		    */

    void  RSS_Object::vReadSave(std::string *text)

{
}


/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void  RSS_Object::vWriteSave(std::string *text)

{
     *text="" ;
}


/********************************************************************/
/*								    */
/*		      ���������� �������        		    */

    void  RSS_Object::vFree(void)

{
}


/********************************************************************/
/*								    */
/*		      ����������� ��������      		    */

    void  RSS_Object::vFormDecl(void)

{
   sprintf(Decl, "%s  %s", Name, Type) ;
}


/********************************************************************/
/*								    */
/*		      ������ � �������� �������   		    */
/*								    */
/*   Return: ����� ������� �����                                    */

     int  RSS_Object::vGetPosition(RSS_Point *points)

{
        points->x=x_base ;
        points->y=y_base ;
        points->z=z_base ;

        points->azim=a_azim ;
        points->elev=a_elev ;
        points->roll=a_roll ;

    return(0) ;
}

    void  RSS_Object::vSetPosition(RSS_Point *points)

{
        x_base=points->x ;
        y_base=points->y ;
        z_base=points->z ;

        a_azim=points->azim ;
        a_elev=points->elev ;
        a_roll=points->roll ;
}


/********************************************************************/
/*								    */
/*                    ��������� ������� ��������                    */

    int  RSS_Object::vGetVelocity(RSS_Vector *velocity)
{
        velocity->x=x_velocity ;
        velocity->y=y_velocity ;
        velocity->z=z_velocity ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		����� ��������� �������� �������                    */

     int  RSS_Object::vResetFeatures(void *data)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++)
                this->Features[i]->vResetCheck(data) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		���������� ������� � �������� ������������          */

     int  RSS_Object::vPrepareFeatures(void *data)

{
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++)
                this->Features[i]->vPreCheck(data) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		�������� ������������������ �������                 */

     int  RSS_Object::vCheckFeatures(void *data, RSS_Objects_List *checked)

{
  int  status ;
  int  i ;


   for(i=0 ; i<this->Features_cnt ; i++) {
           status=this->Features[i]->vCheck(data, checked) ;
        if(status)  break ;
                                         }
   return(status) ;
}


/********************************************************************/
/*								    */
/*		      ������ �� ����������                          */

    void  RSS_Object::vSetFeature(RSS_Feature *feature)

{
}

    void  RSS_Object::vGetFeature(RSS_Feature *feature)

{
}


/********************************************************************/
/*								    */
/*                        ����������� ��������                      */

     int  RSS_Object::vSpecial(char *oper, void *data)
{
  return(-1) ;
}


/********************************************************************/
/*								    */
/*             ���������� ������� ��������� ���������               */

     int  RSS_Object::vCalculateStart(double  t)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                   ������ ��������� ���������                     */

     int  RSS_Object::vCalculate(double t1, double t2, char *callback, int callback_size)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*      ����������� ���������� ������� ��������� ���������          */

     int  RSS_Object::vCalculateShow(void)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                        ��������� �������                         */

     int  RSS_Object::vEvent(char *event_name, double  t)
{
  return(-1) ;
}


/********************************************************************/
/*								    */
/*                �������� ������ ���������� ����������             */

   int  RSS_Object::vListControlPars(RSS_ControlPar *list)

{
   return(0) ;
}


/********************************************************************/
/*								    */
/*               ���������� �������� ��������� ����������           */

   int  RSS_Object::vSetControlPar(RSS_ControlPar *par)   

{
   return(-1) ;
}


/*********************************************************************/
/*                                                                   */
/*               �������� ��������� ���� � ��������                  */

  int  RSS_Object::iAngleInCheck(double  value, 
                                 double  value_min, 
                                 double  value_max )
{
  int  i ;


  for(i=0 ; i<2 ; i++) {

    if(value_min<=value_max) {
	 if(value>=value_min &&
	    value<=value_max   ) return(0) ;
			     }
    else                     {
	 if(value<=value_min &&
	    value>=value_max   ) return(0) ;
			     }

    if(value>0)  value-=360. ;
    else         value+=360. ;

                       }

  return(1) ;
}


/********************************************************************/
/*								    */
/*              ������ ���������� ��������� � ������                */

  int  RSS_Object::iReplaceText(char *buff, char *name, char *value, int  count)

{
   char *entry ;
    int  shift ;
    int  i ;


     if(count==0)  count=10000 ;

             entry=buff ;
             shift=strlen(value)-strlen(name) ;

     for(i=0 ; i<count ; i++) {

             entry=strstr(entry, name) ;
          if(entry==NULL)  break ;
                         
               memmove(entry+strlen(value), entry+strlen(name), 
                                       strlen(entry+strlen(name))+1) ;
                memcpy(entry, value, strlen(value)) ;

                              }

  return(i) ;
}


/*********************************************************************/
/*								     */
/*	      ���������� ������ "������ ��������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       ����������� ������ "������ ��������"      	     */

     RSS_Objects_List::RSS_Objects_List(void)

{
     List    =NULL ;
     List_cnt=  0 ;
     List_max=  0 ;
}


/*********************************************************************/
/*								     */
/*	        ���������� ������ "������ ��������"      	     */

    RSS_Objects_List::~RSS_Objects_List(void)

{
    if(List!=NULL)  free(List) ;
}


/********************************************************************/
/*								    */
/*		              ������� ������		            */

    void  RSS_Objects_List::Clear(void)

{
    List_cnt=0 ;
}


/********************************************************************/
/*								    */
/*		      ���������� ������ � ������	            */

    int  RSS_Objects_List::Add(class RSS_Object *object, char *relation)

{
   int  i ;

/*------------------------------------------------ ������ ���������� */

   for(i=0 ; i<List_cnt ; i++)
     if(!stricmp(object->Name, List[i].object->Name))  break ;

      if(i<List_cnt) {
                               List[i].object->vFree() ;
                        delete List[i].object ;
                     }
/*------------------------------------- ���������� ������ ���������� */

   if(i>=List_max) {
                       List_max+=10 ;
                       List     =(RSS_Objects_List_Elem *)
                                  realloc(List, List_max*sizeof(*List)) ;
                   }

   if(List==NULL)  return(-1) ;


               List[i].object=object ;
       strncpy(List[i].relation, relation, sizeof(List[0].relation)-1) ;

    if(i>=List_cnt)  List_cnt++ ;

/*-------------------------------------------------------------------*/

  return(0) ;
}

