/********************************************************************/
/*								    */
/*		������ ���������� ����������� "������� ���"	    */
/*								    */
/********************************************************************/


#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <malloc.h>
#include <io.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys\stat.h>

#include "gl\gl.h"
#include "gl\glu.h"

#include "..\Matrix\Matrix.h"

#include "..\RSS_Feature\RSS_Feature.h"
#include "..\RSS_Object\RSS_Object.h"
#include "..\RSS_Unit\RSS_Unit.h"
#include "..\RSS_Kernel\RSS_Kernel.h"
#include "..\RSS_Model\RSS_Model.h"
#include "..\F_Show\F_Show.h"

#include "U_EWunit.h"

#pragma warning(disable : 4996)

#define  SEND_ERROR(text)    SendMessage(RSS_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,      \
                                         (LPARAM) text)
#define  CREATE_DIALOG       CreateDialogIndirectParam


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


/********************************************************************/
/*								    */
/*		    	����������� ������                          */

     static   RSS_Module_EWunit  ProgramModule ;


/********************************************************************/
/*								    */
/*		    	����������������� ����                      */

 U_EWUNIT_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 U_EWUNIT_API RSS_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/********************************************************************/
/**							           **/
/**    �������� ������ ������ ���������� �������� "������� ���"	   **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*                            ������ ������                         */

  struct RSS_Module_EWunit_instr  RSS_Module_EWunit_InstrList[]={

 { "help",    "?",  "#HELP   - ������ ��������� ������", 
                     NULL,
                    &RSS_Module_EWunit::cHelp   },
 { "info",    "i",  "#INFO - ������ ���������� �� �������",
                    " INFO <���> \n"
                    "   ������ �������� ���������� �� ������� � ������� ����\n"
                    " INFO/ <���> \n"
                    "   ������ ������ ���������� �� ������� � ��������� ����",
                    &RSS_Module_EWunit::cInfo },
 { "event",   "e",  "#EVENT - ��������� ������� ����� ����������� ������",
                    " EVENT <���> <�������>\n"
                    "   ��������� ������� ����� ����������� ������",
                    &RSS_Module_EWunit::cEvent },
 { "range",   "r",  "#RANGE - ������ �������� ���������� ����������� ��",
                    " RANGE <���> <������� �������>  <������� �������>\n"
                    "   ������ �������� ���������� ����������� ��",
                    &RSS_Module_EWunit::cRange },
 { "velocity","v",  "#VELOCITY - ������ ��������� �������� ������������� ��",
                    " VELOCITY <���> <��������>\n"
                    "   ������ ��������� �������� ������������� ��",
                    &RSS_Module_EWunit::cVelocity },
 { "show",    "s",  "#SHOW - ���������� ��������� ���������� ������� ���",
                    " SHOW <���> \n"
                    "   ���������� ��������� ���������� ������� ���\n",
                    &RSS_Module_EWunit::cShow },
 {  NULL }
                                                            } ;

/********************************************************************/
/*								    */
/*		     ����� ����� ������             		    */

    struct RSS_Module_EWunit_instr *RSS_Module_EWunit::mInstrList=NULL ;


/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     RSS_Module_EWunit::RSS_Module_EWunit(void)

{
  static  WNDCLASS  Indicator_wnd ;

/*---------------------------------------------------- ������������� */

	   keyword="EmiStand" ;
    identification="EWunit" ;
          category="Unit" ;

        mInstrList=RSS_Module_EWunit_InstrList ;

/*----------------------------- ����������� ������ ������� Indicator */

	Indicator_wnd.lpszClassName="Unit_EWunit_Indicator_class" ;
	Indicator_wnd.hInstance    = GetModuleHandle(NULL) ;
	Indicator_wnd.lpfnWndProc  = Unit_EWunit_Indicator_prc ;
	Indicator_wnd.hCursor      = LoadCursor(NULL, IDC_ARROW) ;
	Indicator_wnd.hIcon        =  NULL ;
	Indicator_wnd.lpszMenuName =  NULL ;
	Indicator_wnd.hbrBackground=  NULL ;
	Indicator_wnd.style        =    0 ;
	Indicator_wnd.hIcon        =  NULL ;

            RegisterClass(&Indicator_wnd) ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    RSS_Module_EWunit::~RSS_Module_EWunit(void)

{
}


/********************************************************************/
/*								    */
/*		      �������� �������                              */

  RSS_Object *RSS_Module_EWunit::vCreateObject(RSS_Model_data *data)

{
   RSS_Unit_EWunit *unit ;
               int  i ;
 
/*---------------------------------------------- �������� ���������� */

       unit=new RSS_Unit_EWunit ;
    if(unit==NULL) {
               SEND_ERROR("������ EWUNIT: ������������ ������ ��� �������� ����������") ;
                        return(NULL) ;
                   }

             unit->Module=this ;

      strcpy(unit->Decl, "������������ ������� ����������� ������ �����") ;

/*------------------------------- �������� ������ ������� ���������� */

        unit->Features_cnt=this->feature_modules_cnt ;
        unit->Features    =(RSS_Feature **)
                             calloc(this->feature_modules_cnt, 
                                     sizeof(unit->Features[0])) ;

   for(i=0 ; i<this->feature_modules_cnt ; i++)
        unit->Features[i]=this->feature_modules[i]->vCreateFeature(unit, NULL) ;

/*-------------------------------------------------------------------*/

  return(unit) ;
}


/********************************************************************/
/*								    */
/*		        �������� ��������       		    */

     int  RSS_Module_EWunit::vGetParameter(char *name, char *value)

{
/*-------------------------------------------------- �������� ������ */

    if(!stricmp(name, "$$MODULE_NAME")) {

         sprintf(value, "%-20.20s -  ������� ������� ���", identification) ;
                                        }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		        ��������� �������       		    */

  int  RSS_Module_EWunit::vExecuteCmd(const char *cmd)

{
  static  int  direct_command ;   /* ���� ������ ������ ������� */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

#define  _SECTION_FULL_NAME   "EWUNIT"
#define  _SECTION_SHRT_NAME   "EWU"

/*--------------------------------------------- ������������� ������ */

             memset(command, 0, sizeof(command)) ;
            strncpy(command, cmd, sizeof(command)-1) ;

   if(!direct_command) {

         end=strchr(command, ' ') ;
      if(end!=NULL) {  *end=0 ;  end++ ;  }

      if(stricmp(command, _SECTION_FULL_NAME) &&
         stricmp(command, _SECTION_SHRT_NAME)   )  return(1) ;
                       }
   else                {
                             end=command ;
                       }
/*----------------------- ���������/���������� ������ ������ ������� */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -  ��������� */
   if(end==NULL || end[0]==0) {
     if(!direct_command) {
                            direct_command=1 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Object EWunit:") ;
        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DIRECT_COMMAND, (LPARAM)identification) ;
                         }
                                    return(0) ;
                              }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - ���������� */
   if(end!=NULL && !strcmp(end, "..")) {

                            direct_command=0 ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"") ;
        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DIRECT_COMMAND, (LPARAM)"") ;

                                           return(0) ;
                                       }
/*--------------------------------------------- ��������� ���������� */

       instr=end ;                                                  /* �������� ����� � ��������� ������� */

     for(end=instr ; *end!= 0  &&
                     *end!=' ' &&
                     *end!='>' &&
                     *end!='/'    ; end++) ;

      if(*end!= 0 &&
         *end!=' '  )  memmove(end+1, end, strlen(end)+1) ;

      if(*end!=0) {  *end=0 ;  end++ ;  }
      else            end="" ;

   for(i=0 ; mInstrList[i].name_full!=NULL ; i++)                   /* ���� ������� � ������ */
     if(!stricmp(instr, mInstrList[i].name_full) ||
        !stricmp(instr, mInstrList[i].name_shrt)   )   break ;

     if(mInstrList[i].name_full==NULL) {                            /* ���� ����� ������� ���... */

          status=this->kernel->vExecuteCmd(cmd) ;                   /*  �������� �������� ������ ����... */
       if(status)  SEND_ERROR("������ EWunit: ����������� �������") ;
                                            return(-1) ;
                                       }
 
     if(mInstrList[i].process!=NULL)                                /* ���������� ������� */
                status=(this->*mInstrList[i].process)(end) ;
     else       status=  0 ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*								    */
/*		        ������� ������ �� ������		    */

    void  RSS_Module_EWunit::vReadSave(std::string *data)

{
               char *buff ;
                int  buff_size ;
     RSS_Model_data  create_data ;
//  RSS_Unit_EWunit *unit ;
               char *entry ;
               char *end ;

/*----------------------------------------------- �������� ��������� */

   if(memicmp(data->c_str(), "#BEGIN MODULE EWUNIT\n", 
                      strlen("#BEGIN MODULE EWUNIT\n")) &&
      memicmp(data->c_str(), "#BEGIN UNIT EWUNIT\n", 
                      strlen("#BEGIN UNIT EWUNIT\n"  ))   )  return ;

/*------------------------------------------------ ���������� ������ */

              buff_size=data->size()+16 ;
              buff     =(char *)calloc(1, buff_size) ;

       strcpy(buff, data->c_str()) ;

/*---------------------------------------------- �������� ���������� */

   if(!memicmp(buff, "#BEGIN UNIT EWUNIT\n", 
              strlen("#BEGIN UNIT EWUNIT\n"))) {                    /* IF.1 */
/*- - - - - - - - - - - - - - - - - - - - - -  ���������� ���������� */
              memset(&create_data, 0, sizeof(create_data)) ;

                                     entry=strstr(buff, "NAME=") ;  /* ��������� ��� ���������� */
           strncpy(create_data.name, entry+strlen("NAME="), 
                                       sizeof(create_data.name)-1) ;
        end=strchr(create_data.name, '\n') ;
       *end= 0 ;
/*- - - - - - - - - - - - -  �������� ���������� �������� ���������� */
/*- - - - - - - - - - - - - - - - - - - - - - -  �������� ���������� */
/*
                status=vCreateObject(NULL) ;
             if(status)  return ;
*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                               }                    /* END.1 */
/*-------------------------------------------- ������������ �������� */

                free(buff) ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void  RSS_Module_EWunit::vWriteSave(std::string *text)

{
  std::string  buff ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN MODULE EWUNIT\n" ;

/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� HELP                    */

  int  RSS_Module_EWunit::cHelp(char *cmd)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Unit_EWunit_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� INFO                    */
/*								    */
/*        INFO   <���>                                              */
/*        INFO/  <���>                                              */

  int  RSS_Module_EWunit::cInfo(char *cmd)

{
               char  *name ;
    RSS_Unit_EWunit  *unit ;
                int   all_flag ;   /* ���� ������ ������ ���������� */
               char  *end ;
        std::string   info ;
        std::string   f_info ;
               char   text[4096] ;
                int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                           all_flag=0 ;

       if(*cmd=='/') {
                           all_flag=1 ;
 
                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }

                           cmd=end+1 ;
                        }
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */
                  name=cmd ;
                   end=strchr(name, ' ') ;
                if(end!=NULL)  *end=0 ;

/*----------------------------------------- �������� ����� ��������� */

    if(name   ==NULL ||
       name[0]==  0    ) {                                          /* ���� ��� �� ������... */
                           SEND_ERROR("�� ������ ��� ���������. \n"
                                      "��������: INFO <���_���������> ...") ;
                                     return(-1) ;
                         }

       unit=(RSS_Unit_EWunit *)FindUnit(name) ;                     /* ���� ��������� �� ����� */
    if(unit==NULL)  return(-1) ;

/*-------------------------------------------- ������������ �������� */

      sprintf(text, "%s\r\n%s\r\n"
                    "Range Min   % 5lf\r\n" 
                    "Range Max   % 5lf\r\n" 
                    "V-threshold % 5lf\r\n" 
                    "Owner       %s\r\n" 
                    "\r\n",
                        unit->Name,      unit->Type, 
                        unit->range_min, unit->range_max, unit->velocity,
                        unit->Owner->Name
                    ) ;

           info=text ;

/*----------------------------------------------- ������ �� �������� */

   for(i=0 ; i<unit->Features_cnt ; i++) {

                        unit->Features[i]->vGetInfo(&f_info) ;
                                               info+=f_info ;
                                               info+="\r\n" ;
                                           }
/*-------------------------------------------------- ������ �������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - � ��������� ���� */
     if(all_flag) {

                  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - � ������� ���� */
     else         {

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_SHOW_INFO, (LPARAM)info.c_str()) ;
                  }
/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� EVENT                   */
/*								    */
/*       EVENT <���> <�������>                                      */

  int  RSS_Module_EWunit::cEvent(char *cmd)

{
#define   _PARS_MAX  10

               char  *pars[_PARS_MAX] ;
               char  *name ;
               char  *event_name ;
    RSS_Unit_EWunit  *unit ;
               char  *end ;
                int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name=pars[0] ;
               event_name=pars[1] ;   

/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: EVENT <���_�������> ...") ;
                                     return(-1) ;
                   }

       unit=(RSS_Unit_EWunit *)FindUnit(name) ;                     /* ���� ������-���� �� ����� */
    if(unit==NULL)  return(-1) ;

/*-------------------------------------------------- ������� ������� */

    if(event_name==NULL) {                                          /* ���� ������� �� ������... */
                      SEND_ERROR("�� ������ �������. \n"
                                 "��������: EVENT <���_�������> <�������>") ;
                                     return(-1) ;
                         }

          strcpy(unit->event_name, event_name) ;

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� RANGE                   */
/*								    */
/*       RANGE <���> <������� �������> <������� �������>            */

  int  RSS_Module_EWunit::cRange(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

               char  *pars[_PARS_MAX] ;
               char  *name ;
               char **xyz ;
             double   coord[_COORD_MAX] ;
                int   coord_cnt ;
    RSS_Unit_EWunit  *unit ;
               char  *error ;
               char  *end ;
                int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name= pars[0] ;
                      xyz=&pars[1] ;   

/*---------------------------------------- �������� ����� ���������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� ����������. \n"
                                 "��������: RANGE <���_����������> ...") ;
                                     return(-1) ;
                   }

       unit=(RSS_Unit_EWunit *)FindUnit(name) ;                     /* ���� ���������� �� ����� */
    if(unit==NULL)  return(-1) ;

/*------------------------------------------------- ������ ��������� */

    for(i=0 ; xyz[i]!=NULL && i<_COORD_MAX ; i++) {

             coord[i]=strtod(xyz[i], &end) ;
        if(*end!=0) {  
                       SEND_ERROR("������������ �������� ���������") ;
                                       return(-1) ;
                    }
                                                  }

                             coord_cnt=i ;

                        error= NULL ;
      if(coord_cnt==0)  error="�� ������ �������� ���������� �����������" ;
      if(coord_cnt==1)  error="�� ������� ������� ������� �����������" ;

      if(error!=NULL) {  SEND_ERROR(error) ;
                               return(-1) ;   }

/*------------------------------------------------- ������� �������� */

                   unit->range_min=coord[0] ;
                   unit->range_max=coord[1] ;

/*-------------------------------------------------------------------*/

#undef  _COORD_MAX   
#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� VELOCITY                */
/*								    */
/*       VELOCITY <���> <��������>                                  */

  int  RSS_Module_EWunit::cVelocity(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

               char  *pars[_PARS_MAX] ;
               char  *name ;
               char **xyz ;
             double   coord[_COORD_MAX] ;
                int   coord_cnt ;
    RSS_Unit_EWunit  *unit ;
               char  *error ;
               char  *end ;
                int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name= pars[0] ;
                      xyz=&pars[1] ;   

/*---------------------------------------- �������� ����� ���������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� ����������.\n"
                                 "��������: VELOCITY <���_����������> ...") ;
                                     return(-1) ;
                   }

       unit=(RSS_Unit_EWunit *)FindUnit(name) ;                     /* ���� ���������� �� ����� */
    if(unit==NULL)  return(-1) ;

/*------------------------------------------------- ������ ��������� */

    for(i=0 ; xyz[i]!=NULL && i<_COORD_MAX ; i++) {

             coord[i]=strtod(xyz[i], &end) ;
        if(*end!=0) {  
                       SEND_ERROR("������������ �������� ��������") ;
                                       return(-1) ;
                    }
                                                  }

                             coord_cnt=i ;

                        error= NULL ;
      if(coord_cnt==0)  error="�� ������� ��������" ;

      if(error!=NULL) {  SEND_ERROR(error) ;
                               return(-1) ;   }

/*------------------------------------------------- ������� �������� */

                   unit->velocity=coord[0] ;

/*-------------------------------------------------------------------*/

#undef  _COORD_MAX   
#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� SHOW                    */
/*								    */
/*        SHOW <���>                                                */

  int  RSS_Module_EWunit::cShow(char *cmd)

{
               char  *name ;
    RSS_Unit_EWunit  *unit ;
               char  *end ;

/*---------------------------------------- �������� ��������� ������ */

                  name=cmd ;
                   end=strchr(name, ' ') ;
                if(end!=NULL)  *end=0 ;

/*---------------------------------------- �������� ����� ���������� */

    if(name   ==NULL ||
       name[0]==  0    ) {                                          /* ���� ��� �� ������... */
                           SEND_ERROR("�� ������ ��� ����������. \n"
                                      "��������: SHOW <���_����������> ...") ;
                                     return(-1) ;
                         }

       unit=(RSS_Unit_EWunit *)FindUnit(name) ;                     /* ���� ���������� �� ����� */
    if(unit==NULL)  return(-1) ;

/*----------------------------------------- �������� ���� ���������� */

          strcpy(unit->Context->action, "INDICATOR") ;

     SendMessage(RSS_Kernel::kernel_wnd, 
                 WM_USER, (WPARAM)_USER_CHANGE_CONTEXT, 
                          (LPARAM) unit->Context       ) ;

/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*		   ����� ������� ���� EWUNIT �� �����               */

  RSS_Unit *RSS_Module_EWunit::FindUnit(char *name)

{
 RSS_Object *object ;
 RSS_Object *unit ;
       char  o_name[128] ;
       char *u_name ;
       char  text[1024] ;
        int  i ;

#define   OBJECTS       this->kernel->kernel_objects 
#define   OBJECTS_CNT   this->kernel->kernel_objects_cnt 

/*-------------------------------------------------- ���������� ���� */ 

               memset(o_name, 0, sizeof(o_name)) ;
              strncpy(o_name, name, sizeof(o_name)-1) ;
        u_name=strchr(o_name, '.') ;

     if(u_name==NULL) {
                           sprintf(text, "��� '%s' �� �������� ������ ���������� �������", name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                      }

       *u_name=0 ;
        u_name++ ;
     
/*------------------------------------------- ����� ������� �� ����� */ 

             object=NULL ;

       for(i=0 ; i<OBJECTS_CNT ; i++)
         if(!stricmp(OBJECTS[i]->Name, o_name)) {
                                                   object=OBJECTS[i] ;
                                                     break ; 
                                                }

    if(object==NULL) {
                           sprintf(text, "������� � ������ '%s' �� ����������", o_name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                     }
/*---------------------------------------- ����� ���������� �� ����� */ 

       for(i=0 ; i<object->Units.List_cnt ; i++)
         if(!stricmp(object->Units.List[i].object->Name, u_name)) { 
                         unit=object->Units.List[i].object ;
                                               break ;
                                                                  }

    if(unit==NULL) {
                           sprintf(text, "������ '%s' �� �������� ��������� '%s'", o_name, u_name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                   }
/*-------------------------------------------- �������� ���� ������� */ 

     if(strcmp(unit->Type, "EWunit")) {

           SEND_ERROR("��������� �� �������� ����������� ���� EWunit") ;
                            return(NULL) ;
                                      }
/*-------------------------------------------------------------------*/ 

   return((RSS_Unit *)unit) ;

#undef   OBJECTS
#undef   OBJECTS_CNT

}


/********************************************************************/
/********************************************************************/
/**							           **/
/**	  �������� ������ ���������� "������� ���"	           **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     RSS_Unit_EWunit::RSS_Unit_EWunit(void)

{
   strcpy(Type, "EWunit") ;

  Context        =new RSS_Transit_EWunit ;
  Context->object=this ;

   Parameters    =NULL ;
   Parameters_cnt=  0 ;

       range_min= 100. ;
       range_max=5000. ;
       velocity =1000. ;

           hWnd =NULL ;
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    RSS_Unit_EWunit::~RSS_Unit_EWunit(void)

{
      vFree() ;
}


/********************************************************************/
/*								    */
/*		       ������������ ��������                        */

  void   RSS_Unit_EWunit::vFree(void)

{
  int  i ;


  if(this->Features!=NULL) {

    for(i=0 ; i<this->Features_cnt ; i++) {
           free(this->Features[i]) ;
                                          }

           free(this->Features) ;

                this->Features    =NULL ;
                this->Features_cnt=  0 ;
                           }
}


/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void  RSS_Unit_EWunit::vWriteSave(std::string *text)

{
  char  field[1024] ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN UNIT EWUNIT\n" ;

/*----------------------------------------------------------- ������ */

    sprintf(field, "NAME=%s\n", this->Name) ;  *text+=field ;

/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*                        ����������� ��������                      */

     int  RSS_Unit_EWunit::vSpecial(char *oper, void *data)
{
/*------------------------------------------ ������ �� ������ BATTLE */

    if(!stricmp(oper, "BATTLE")) {

                             this->battle=(RSS_Module_Battle *)data ;
                                      return(0) ;
                                 }
/*-------------------------------------------------------------------*/

  return(-1) ;
}


/********************************************************************/
/*								    */
/*             ���������� ������� ��������� ���������               */

     int  RSS_Unit_EWunit::vCalculateStart(double  t)
{
  return(0) ;
}


/********************************************************************/
/*								    */
/*                   ������ ��������� ���������                     */

     int  RSS_Unit_EWunit::vCalculate(double t1, double t2, char *callback, int cb_size)
{
  RSS_Object *object ;
      double  v_x, v_y, v_z ;
      double  d_x, d_y, d_z ;
      double  v ;
      double  d ;
      double  s ;
      double  alpha ;
         int  i ;

/*---------------------------------------------------- ������������� */

                threats_cnt =0 ;
                  event_send=0 ;

/*--------------------------------------------- ������� �������� ��� */

   for(i=0 ; i<battle->mObjects_cnt ; i++) {

         object=battle->mObjects[i].object ;

/*------------------------ ������ ������������� �������� � ��������� */

         v_x=object->x_velocity-this->Owner->x_velocity ;           /* �������� ������� ������������ �������� */
         v_y=object->y_velocity-this->Owner->y_velocity ;
         v_z=object->z_velocity-this->Owner->z_velocity ;

         v  =sqrt(v_x*v_x+v_y*v_y+v_z*v_z) ;

      if(v==0)  continue ;                                          /* ���� ������ ���������� ������������ ��������... */

         d_x=this->Owner->x_base-object->x_base ;                   /* ����������� �� ������� �� �������� (�� ��������!) */
         d_y=this->Owner->y_base-object->y_base ;
         d_z=this->Owner->z_base-object->z_base ;

         d  =sqrt(d_x*d_x+d_y*d_y+d_z*d_z) ;
      if(d==0.)  continue ;                                         /* ���� ����� ������� � �������� ���������... */

      if(d<range_min || d>range_max)  continue ;                    /* ���� ������ �� �������� � �������� ���������� �����������... */

         d_x=d_x*v/d ;
         d_y=d_y*v/d ;
         d_z=d_z*v/d ;

/*------------------------------------- ������ ������������ �������� */

         s=sqrt((d_x-v_x)*(d_x-v_x)+
                (d_y-v_y)*(d_y-v_y)+
                (d_z-v_z)*(d_z-v_z) ) ;

      if(s>1.414*v)  continue ;                                     /* ���� �������� ������� �� ���������� � ��������... */

     alpha=asin(0.5*s/v) ;

             v*=cos(alpha) ;

      if(v<this->velocity)  continue ;                              /* ���� ������������ �������� ������ ���������... */

/*----------------------------------------------- ����������� ������ */

      if(threats_cnt>=_EWUNIT_THREATS_MAX)  continue ;
 
         threats[threats_cnt]=object ;
                 threats_cnt++ ;

      if(t2-event_time>5) {                                         /* ������ �� ������ - �� ���� ���� � 5 ������ */
                             event_time=t2 ;
                             event_send= 1 ;
                          }
/*--------------------------------------------- ������� �������� ��� */
                                           }
/*-------------------------------------------------------------------*/

  return(0) ;
}


/********************************************************************/
/*								    */
/*      ����������� ���������� ������� ��������� ���������          */

     int  RSS_Unit_EWunit::vCalculateShow(void)
{
/*------------------------------------- �������� ������� �� �������� */

    if(event_send) {
                        this->Owner->vEvent(event_name, event_time) ;
                           event_send=0 ;
                   }
/*--------------------------------- ����������� ������ �� ���������� */

    if(this->hWnd!=NULL)  SendMessage(this->hWnd, WM_PAINT, NULL, NULL) ;

/*-------------------------------------------------------------------*/

  return(0) ;
}

/*********************************************************************/
/*								     */
/*	      ���������� ������ "������� ���������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       ����������� ������ "������� ���������"      	     */

     RSS_Transit_EWunit::RSS_Transit_EWunit(void)

{
}


/*********************************************************************/
/*								     */
/*	        ���������� ������ "������� ���������"      	     */

    RSS_Transit_EWunit::~RSS_Transit_EWunit(void)

{
}


/********************************************************************/
/*								    */
/*	              ���������� ��������                           */

    int  RSS_Transit_EWunit::vExecute(void)

{
  RSS_Unit_EWunit *unit ;


       unit=(RSS_Unit_EWunit *)this->object ;

   if(!stricmp(action, "INDICATOR")) {

       unit->hWnd=CREATE_DIALOG(GetModuleHandle(NULL),
                                (LPCDLGTEMPLATE)unit->Module->Resource("IDD_ROUND", RT_DIALOG),
	                         NULL, Unit_EWunit_Show_dialog, 
                                       (LPARAM)object) ;

                       ShowWindow(unit->hWnd, SW_SHOW) ;

                                     }

   return(0) ;
}
