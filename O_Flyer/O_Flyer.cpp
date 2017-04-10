/********************************************************************/
/*								    */
/*		������ ���������� �������� "�����"  		    */
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
#include "..\RSS_Kernel\RSS_Kernel.h"
#include "..\RSS_Model\RSS_Model.h"
#include "..\F_Show\F_Show.h"

#include "O_Flyer.h"

#pragma warning(disable : 4996)

#define  SEND_ERROR(text)    SendMessage(RSS_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,      \
                                         (LPARAM) text)

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

     static   RSS_Module_Flyer  ProgramModule ;


/********************************************************************/
/*								    */
/*		    	����������������� ����                      */

 O_FLYER_API char *Identify(void)

{
	return(ProgramModule.keyword) ;
}


 O_FLYER_API RSS_Kernel *GetEntry(void)

{
	return(&ProgramModule) ;
}

/********************************************************************/
/********************************************************************/
/**							           **/
/**	  �������� ������ ������ ���������� �������� "�����"	   **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*                            ������ ������                         */

  struct RSS_Module_Flyer_instr  RSS_Module_Flyer_InstrList[]={

 { "help",    "?",  "#HELP   - ������ ��������� ������", 
                     NULL,
                    &RSS_Module_Flyer::cHelp   },
 { "create",  "cr", "#CREATE - ������� ������",
                    " CREATE <���> [<������> [<������ ����������>]]\n"
                    "   ������� ����������� ������ �� ����������������� ������",
                    &RSS_Module_Flyer::cCreate },
 { "info",    "i",  "#INFO - ������ ���������� �� �������",
                    " INFO <���> \n"
                    "   ������ �������� ��������� �� ������� � ������� ����\n"
                    " INFO/ <���> \n"
                    "   ������ ������ ���������� �� ������� � ��������� ����",
                    &RSS_Module_Flyer::cInfo },
 { "base",    "b", "#BASE - ������ ������� ����� �������",
                    " BASE <���> <x> <y> <z>\n"
                    "   ������ ������� ����� �������\n"
                    " BASE/x <���> <x>\n"
                    "   ������ ���������� X ������� ����� �������\n"
                    "       (���������� ��� Y � Z)\n"
                    " BASE/+x <���> <x>\n"
                    "   ������ ���������� ���������� X ������� ����� �������\n"
                    "       (���������� ��� Y � Z)\n"
                    " BASE> <���>\n"
                    "   ������ ������������ ���������� ������� ������ �������\n",
                    &RSS_Module_Flyer::cBase },
 { "angle",   "a", "#ANGLE - ������ ���� ���������� �������",
                    "           A (AZIMUTH)   - ������\n"
                    "           E (ELEVATION) - ����������\n"
                    "           R (ROLL)      - ����\n"
                    " ANGLE <���> <a> <e> <r>\n"
                    "   ������ ���� ���������� �������\n"
                    " ANGLE/a <���> <a>\n"
                    "   ������ ���� ���������� A �������\n"
                    "       (���������� ��� E � R)\n"
                    " ANGLE/+a <���> <a>\n"
                    "   ������ ���������� ���� ���������� A �������\n"
                    "       (���������� ��� E � R)\n"
                    " ANGLE> <���>\n"
                    "   ������ ������������ ���������� ������ ���������� �������\n",
                    &RSS_Module_Flyer::cAngle },
 { "velocity", "v", "#VELOCITY - ������� �������� �������� �������",
                    " VELOCITY <���> <��������>\n"
                    "   ������� ����������� �������� �������� �������"
                    " VELOCITY <���> <X-��������> <Y-��������> <Z-��������>\n"
                    "   ������� �������� �������� �������",
                    &RSS_Module_Flyer::cVelocity },
 { "control",  "c", "#CONTROL - ���������� ��������",
                    " CONTROL <���> <���� �����> [<����������>]\n"
                    "   ������� ������� ������ ���������� ����������\n"
                    " CONTROL/G <���> <����������>\n"
                    "   ������� ������� ������ ���������� ����������\n"
                    " CONTROL> <���> [<��� �����> [<��� ���������>]]\n"
                    "   ���������� �����������\n",
                    &RSS_Module_Flyer::cControl },
 { "program",  "p", "#PROGRAM - ������� ��������� ���������� ��������",
                    " PROGRAM <���> <��� ����� ���������>\n"
                    "   ������� ��������� ���������� ��������\n",
                    &RSS_Module_Flyer::cProgram },
 { "trace",    "t", "#TRACE - ����������� ���������� �������",
                    " TRACE <���> [<������������>]\n"
                    "   ����������� ���������� ������� � �������� �������\n"
                    " TRACE/P <���> <����� ����������> [<������������>]\n"
                    "   ����������� ���������� ������� ��� ����� ����������\n",
                    &RSS_Module_Flyer::cTrace },
 {  NULL }
                                                            } ;

/********************************************************************/
/*								    */
/*		     ����� ����� ������             		    */

    struct RSS_Module_Flyer_instr *RSS_Module_Flyer::mInstrList=NULL ;


/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     RSS_Module_Flyer::RSS_Module_Flyer(void)

{
	   keyword="EmiStand" ;
    identification="Flyer_object" ;

        mInstrList=RSS_Module_Flyer_InstrList ;

        a_step=15. ;
        g_step= 1. ;
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    RSS_Module_Flyer::~RSS_Module_Flyer(void)

{
}


/********************************************************************/
/*								    */
/*		        ��������� �������       		    */

  int  RSS_Module_Flyer::vExecuteCmd(const char *cmd)

{
  static  int  direct_command ;   /* ���� ������ ������ ������� */
         char  command[1024] ;
         char *instr ;
         char *end ;
          int  status ;
          int  i ;

#define  _SECTION_FULL_NAME   "FLYER"
#define  _SECTION_SHRT_NAME   "FLYER"

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
                     (WPARAM)_USER_COMMAND_PREFIX, (LPARAM)"Object Flyer:") ;
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
       if(status)  SEND_ERROR("������ FLYER: ����������� �������") ;
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

    void  RSS_Module_Flyer::vReadSave(std::string *data)

{
             char *buff ;
              int  buff_size ;
   RSS_Model_data  create_data ;
 RSS_Object_Flyer *object ;
             char  name[128] ;
              int  status ;
             char *entry ;
             char *end ;
              int  i ;

/*----------------------------------------------- �������� ��������� */

   if(memicmp(data->c_str(), "#BEGIN MODULE FLYER\n", 
                      strlen("#BEGIN MODULE FLYER\n")) &&
      memicmp(data->c_str(), "#BEGIN OBJECT FLYER\n", 
                      strlen("#BEGIN OBJECT FLYER\n"))   )  return ;

/*------------------------------------------------ ���������� ������ */

              buff_size=data->size()+16 ;
              buff     =(char *)calloc(1, buff_size) ;

       strcpy(buff, data->c_str()) ;

/*------------------------------------------------- �������� ������� */

   if(!memicmp(buff, "#BEGIN OBJECT FLYER\n", 
              strlen("#BEGIN OBJECT FLYER\n"))) {                   /* IF.1 */
/*- - - - - - - - - - - - - - - - - - - - - -  ���������� ���������� */
              memset(&create_data, 0, sizeof(create_data)) ;

                                     entry=strstr(buff, "NAME=") ;  /* ��������� ��� ������� */
           strncpy(create_data.name, entry+strlen("NAME="), 
                                       sizeof(create_data.name)-1) ;
        end=strchr(create_data.name, '\n') ;
       *end= 0 ;

                                     entry=strstr(buff, "MODEL=") ; /* ��������� ������ ������� */
           strncpy(create_data.path, entry+strlen("MODEL="),
                                       sizeof(create_data.path)-1) ;
        end=strchr(create_data.path, '\n') ;
       *end= 0 ;

    for(i=0 ; i<_MODEL_PARS_MAX ; i++) {
             sprintf(name, "PAR_%d=", i) ;
        entry=strstr(buff, name) ;
     if(entry!=NULL) {
           strncpy(create_data.pars[i].value, entry+strlen(name), 
                                        sizeof(create_data.pars[i].value)-1) ;
        end=strchr(create_data.pars[i].value, '\n') ;
       *end= 0 ;
                     }
                                       } 
/*- - - - - - - - - - - - - - - �������� ���������� �������� ������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - �������� ������� */
                status=CreateObject(&create_data) ;
             if(status)  return ;

        object=(RSS_Object_Flyer *)this->kernel->kernel_objects[this->kernel->kernel_objects_cnt-1] ;
/*- - - - - - - - - - - - ������� ������� ����� � ���������� ������� */
       entry=strstr(buff, "X_BASE=") ; object->x_base=atof(entry+strlen("X_BASE=")) ;
       entry=strstr(buff, "Y_BASE=") ; object->y_base=atof(entry+strlen("Y_BASE=")) ;
       entry=strstr(buff, "Z_BASE=") ; object->z_base=atof(entry+strlen("Z_BASE=")) ;
       entry=strstr(buff, "A_AZIM=") ; object->a_azim=atof(entry+strlen("A_AZIM=")) ;
       entry=strstr(buff, "A_ELEV=") ; object->a_elev=atof(entry+strlen("A_ELEV=")) ;
       entry=strstr(buff, "A_ROLL=") ; object->a_roll=atof(entry+strlen("A_ROLL=")) ;

   for(i=0 ; i<object->Features_cnt ; i++) {
        object->Features[i]->vBodyBasePoint(NULL, object->x_base, 
                                                  object->y_base, 
                                                  object->z_base ) ;
        object->Features[i]->vBodyAngles   (NULL, object->a_azim, 
                                                  object->a_elev, 
                                                  object->a_roll ) ;
                                           }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                                                }                   /* END.1 */
/*-------------------------------------------- ������������ �������� */

                free(buff) ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*		        �������� ������ � ������		    */

    void  RSS_Module_Flyer::vWriteSave(std::string *text)

{
  std::string  buff ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN MODULE FLYER\n" ;

/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� HELP                    */

  int  RSS_Module_Flyer::cHelp(char *cmd)

{ 
    DialogBoxIndirect(GetModuleHandle(NULL),
			(LPCDLGTEMPLATE)Resource("IDD_HELP", RT_DIALOG),
			   GetActiveWindow(), Object_Flyer_Help_dialog) ;

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� CREATE                  */
/*								    */
/*      CREATE <���> [<������> [<������ ����������>]]               */

  int  RSS_Module_Flyer::cCreate(char *cmd)

{
 RSS_Model_data  data ;
           char *name ;
           char *model ;
           char *pars[10] ;
           char *end ;
           char  tmp[1024] ;
            int  status ;
            int  i ;

/*-------------------------------------- ���������� ��������� ������ */

                             model ="" ;
     for(i=0 ; i<10 ; i++)  pars[i]="" ;

   do {                                                             /* BLOCK.1 */
                  name=cmd ;                                        /* ��������� ��� ������� */
                   end=strchr(name, ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;

                 model=end+1 ;                                      /* ��������� �������� ������ */
                   end=strchr(model, ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
          
     for(i=0 ; i<10 ; i++) {                                        /* ��������� ��������� */
               pars[i]=end+1 ;            
                   end=strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                           }
      } while(0) ;                                                  /* BLOCK.1 */

/*--------------------------------- ���������� ����� ������ �������� */

         memset(&data, 0, sizeof(data)) ;

        strncpy(data.name,  name,  sizeof(data.name)) ;
        strncpy(data.model, model, sizeof(data.model)) ;

    for(i=0 ; *pars[i]!=0 ; i++) {
        strncpy(data.pars[i].value,  pars[i], sizeof(data.pars[i].value)) ;
         strcpy(data.pars[i].text, "") ;
                                 }

        sprintf(data.lib_path, "%s\\Body.lib", getcwd(tmp, sizeof(tmp))) ;

/*---------------------- �������� ������������� ��������� ���������� */

   if(data.name[0]!=0) {
                            status=CreateObject(&data) ;
                         if(status==0) {
                                          this->kernel->vShow(NULL) ;
                                             return(0) ;
                                       }
                       }
/*----------------------------------------------- ���������� ������� */

      status=DialogBoxIndirectParam( GetModuleHandle(NULL),
                                    (LPCDLGTEMPLATE)Resource("IDD_CREATE", RT_DIALOG),
			             GetActiveWindow(), 
                                     Object_Flyer_Create_dialog, 
                                    (LPARAM)&data               ) ;
   if(status)  return(status) ;

            this->kernel->vShow(NULL) ;

/*-------------------------------------------------------------------*/

   return(status) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� INFO                    */
/*								    */
/*        INFO   <���>                                              */
/*        INFO/  <���>                                              */

  int  RSS_Module_Flyer::cInfo(char *cmd)

{
             char  *name ;
 RSS_Object_Flyer  *object ;
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

/*------------------------------------------- �������� ����� ������� */

    if(name   ==NULL ||
       name[0]==  0    ) {                                          /* ���� ��� �� ������... */
                           SEND_ERROR("�� ������ ��� �������. \n"
                                      "��������: INFO <���_�������> ...") ;
                                     return(-1) ;
                         }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*-------------------------------------------- ������������ �������� */

      sprintf(text, "%s\r\n%s\r\n"
                    "Base X % 8.2lf\r\n" 
                    "     Y % 8.2lf\r\n" 
                    "     Z % 8.2lf\r\n"
                    "Ang. A % 8.2lf\r\n" 
                    "     E % 8.2lf\r\n" 
                    "     R % 8.2lf\r\n"
                    "Vel. V % 8.2lf\r\n" 
                    "     X % 8.2lf\r\n" 
                    "     Y % 8.2lf\r\n" 
                    "     Z % 8.2lf\r\n"
                    "G-ctrl % 8.2lf\r\n" 
                    "\r\n",
                        object->Name, object->Type, 
                        object->x_base, object->y_base, object->z_base,
                        object->a_azim, object->a_elev, object->a_roll,
                        object->v_abs,
                        object->x_velocity, object->y_velocity, object->z_velocity,
                        object->g_ctrl
                    ) ;

           info=text ;

/*----------------------------------------------- ������ �� �������� */

   for(i=0 ; i<object->Features_cnt ; i++) {

                      object->Features[i]->vGetInfo(&f_info) ;
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
/*		      ���������� ���������� BASE                    */
/*								    */
/*        BASE    <���> <X> <Y> <Z>                                 */
/*        BASE/X  <���> <X>                                         */
/*        BASE/+X <���> <X>                                         */
/*        BASE>   <���> <��� ���������> <���>                       */
/*        BASE>>  <���> <��� ���������> <���>                       */

  int  RSS_Module_Flyer::cBase(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

             char  *pars[_PARS_MAX] ;
             char  *name ;
             char **xyz ;
           double   coord[_COORD_MAX] ;
              int   coord_cnt ;
           double   inverse ;
 RSS_Object_Flyer  *object ;
              int   xyz_flag ;          /* ���� ������ ����� ���������� */
              int   delta_flag ;        /* ���� ������ ���������� */
              int   arrow_flag ;        /* ���� ����������� ������ */
             char  *arrows ;
             char  *error ;
             char  *end ;
              int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                        xyz_flag=0 ;
                      delta_flag=0 ;
                      arrow_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, '+')!=NULL   )  delta_flag= 1 ;

                if(strchr(cmd, 'x')!=NULL ||
                   strchr(cmd, 'X')!=NULL   )    xyz_flag='X' ;
           else if(strchr(cmd, 'y')!=NULL ||
                   strchr(cmd, 'Y')!=NULL   )    xyz_flag='Y' ;
           else if(strchr(cmd, 'z')!=NULL ||
                   strchr(cmd, 'Z')!=NULL   )    xyz_flag='Z' ;

                           cmd=end+1 ;
                        }

  else if(*cmd=='>') {
                           delta_flag=1 ;
                           arrow_flag=1 ;

                          cmd=strchr(cmd, ' ') ;
                       if(cmd==NULL)  return(0) ;
                          cmd++ ;
                     } 
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
                  inverse=   1. ; 

/*------------------------------------- ��������� ����������� ������ */

    if(arrow_flag) {                        
                         arrows=pars[1] ;

      if(strstr(arrows, "left" )!=NULL   ) {  xyz_flag='X' ;  inverse=-1. ;  }
      if(strstr(arrows, "right")!=NULL   ) {  xyz_flag='X' ;  inverse= 1. ;  }  
      if(strstr(arrows, "up"   )!=NULL &&
         strstr(arrows, "ctrl" )==NULL   ) {  xyz_flag='Y' ;  inverse= 1. ;  }
      if(strstr(arrows, "down" )!=NULL &&
         strstr(arrows, "ctrl" )==NULL   ) {  xyz_flag='Y' ;  inverse=-1. ;  }
      if(strstr(arrows, "up"   )!=NULL &&
         strstr(arrows, "ctrl" )!=NULL   ) {  xyz_flag='Z' ;  inverse= 1. ;  }
      if(strstr(arrows, "down" )!=NULL &&
         strstr(arrows, "ctrl" )!=NULL   ) {  xyz_flag='Z' ;  inverse=-1. ;  }
           
                          xyz=&pars[2] ;
                   }
/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: BASE <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*------------------------------------------------- ������ ��������� */

    for(i=0 ; xyz[i]!=NULL && i<_COORD_MAX ; i++) {

             coord[i]=strtod(xyz[i], &end) ;
        if(*end!=0) {  
                       SEND_ERROR("������������ �������� ����������") ;
                                       return(-1) ;
                    }
                                                  }

                             coord_cnt=  i ;

                                 error= NULL ;
      if(xyz_flag) {
               if(coord_cnt==0)  error="�� ������� ���������� ��� �� ����������" ;
                   }
      else         {
               if(coord_cnt <3)  error="������ ���� ������� 3 ����������" ;
                   }

      if(error!=NULL) {  SEND_ERROR(error) ;
                               return(-1) ;   }

      if(arrow_flag && coord_cnt>1)  coord[0]=coord[coord_cnt-1] ;  /* ��� ����������� ������ ����� � �������� ���� */
                                                                    /*  ����� ��������� ��������                    */
/*------------------------------------------------ ������� ��������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - ���������� */
   if(delta_flag) {

          if(xyz_flag=='X')   object->x_base+=inverse*coord[0] ;
     else if(xyz_flag=='Y')   object->y_base+=inverse*coord[0] ;                 
     else if(xyz_flag=='Z')   object->z_base+=inverse*coord[0] ;
                  }
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� �������� */
   else           {

          if(xyz_flag=='X')   object->x_base=coord[0] ;
     else if(xyz_flag=='Y')   object->y_base=coord[0] ;                 
     else if(xyz_flag=='Z')   object->z_base=coord[0] ;
     else                   {
                              object->x_base=coord[0] ;
                              object->y_base=coord[1] ;
                              object->z_base=coord[2] ;
                            }
                  }
/*---------------------------------------------- ������� �� �������� */

   for(i=0 ; i<object->Features_cnt ; i++)
     object->Features[i]->vBodyBasePoint("Flyer.Body", object->x_base, 
                                                       object->y_base, 
                                                       object->z_base ) ;

/*------------------------------------------------------ ����������� */

                      this->kernel->vShow(NULL) ;

/*-------------------------------------------------------------------*/

#undef  _COORD_MAX   
#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� ANGLE                   */
/*								    */
/*       ANGLE    <���> <A> <E> <R>                                 */
/*       ANGLE/A  <���> <A>                                         */
/*       ANGLE/+A <���> <A>                                         */
/*       ANGLE>   <���> <��� ���������> <���>                       */
/*       ANGLE>>  <���> <��� ���������> <���>                       */

  int  RSS_Module_Flyer::cAngle(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

             char  *pars[_PARS_MAX] ;
             char  *name ;
             char **xyz ;
           double   coord[_COORD_MAX] ;
              int   coord_cnt ;
           double   inverse ;
 RSS_Object_Flyer  *object ;
              int   xyz_flag ;          /* ���� ������ ����� ���������� */
              int   delta_flag ;        /* ���� ������ ���������� */
              int   arrow_flag ;        /* ���� ����������� ������ */
             char  *arrows ;
         Matrix2d  Sum_Matrix ;
         Matrix2d  Oper_Matrix ;  
         Matrix2d  Velo_Matrix ;  
             char  *error ;
             char  *end ;
              int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                        xyz_flag=0 ;
                      delta_flag=0 ;
                      arrow_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, '+')!=NULL   )  delta_flag= 1 ;

                if(strchr(cmd, 'a')!=NULL ||
                   strchr(cmd, 'A')!=NULL   )    xyz_flag='A' ;
           else if(strchr(cmd, 'e')!=NULL ||
                   strchr(cmd, 'E')!=NULL   )    xyz_flag='E' ;
           else if(strchr(cmd, 'r')!=NULL ||
                   strchr(cmd, 'R')!=NULL   )    xyz_flag='R' ;

                           cmd=end+1 ;
                        }

  else if(*cmd=='>') {
                           delta_flag=1 ;
                           arrow_flag=1 ;

                          cmd=strchr(cmd, ' ') ;
                       if(cmd==NULL)  return(0) ;
                          cmd++ ;
                     } 
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
                  inverse=   1. ; 

/*------------------------------------- ��������� ����������� ������ */

    if(arrow_flag) {                        
                         arrows=pars[1] ;

      if(strstr(arrows, "left" )!=NULL &&
         strstr(arrows, "ctrl" )==NULL   ) {  xyz_flag='A' ;  inverse=-1. ;  }
      if(strstr(arrows, "right")!=NULL &&  
         strstr(arrows, "ctrl" )==NULL   ) {  xyz_flag='A' ;  inverse= 1. ;  }  

      if(strstr(arrows, "up"   )!=NULL   ) {  xyz_flag='E' ;  inverse= 1. ;  }
      if(strstr(arrows, "down" )!=NULL   ) {  xyz_flag='E' ;  inverse=-1. ;  }

      if(strstr(arrows, "left" )!=NULL &&
         strstr(arrows, "ctrl" )!=NULL   ) {  xyz_flag='R' ;  inverse=-1. ;  }
      if(strstr(arrows, "right")!=NULL &&  
         strstr(arrows, "ctrl" )!=NULL   ) {  xyz_flag='R' ;  inverse= 1. ;  }  
           
                          xyz=&pars[2] ;   
                   }
/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: ANGLE <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*------------------------------------------------- ������ ��������� */

    for(i=0 ; xyz[i]!=NULL && i<_COORD_MAX ; i++) {

             coord[i]=strtod(xyz[i], &end) ;
        if(*end!=0) {  
                       SEND_ERROR("������������ �������� ����������") ;
                                       return(-1) ;
                    }
                                                  }

                             coord_cnt=  i ;

                                 error= NULL ;
      if(xyz_flag) {
               if(coord_cnt==0)  error="�� ������� ���������� ��� �� ����������" ;
                   }
      else         {
               if(coord_cnt <3)  error="������ ���� ������� 3 ����������" ;
                   }

      if(error!=NULL) {  SEND_ERROR(error) ;
                               return(-1) ;   }

      if(arrow_flag && coord_cnt>1)  coord[0]=coord[coord_cnt-1] ;  /* ��� ����������� ������ ����� � �������� ���� */
                                                                    /*  ����� ��������� ��������                    */
/*------------------------------------------------ ������� ��������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - ���������� */
   if(delta_flag) {

          if(xyz_flag=='A')   object->a_azim+=inverse*coord[0] ;
     else if(xyz_flag=='E')   object->a_elev+=inverse*coord[0] ;                 
     else if(xyz_flag=='R')   object->a_roll+=inverse*coord[0] ;
                  }
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� �������� */
   else           {

          if(xyz_flag=='A')   object->a_azim=coord[0] ;
     else if(xyz_flag=='E')   object->a_elev=coord[0] ;                 
     else if(xyz_flag=='R')   object->a_roll=coord[0] ;
     else                   {
                              object->a_azim=coord[0] ;
                              object->a_elev=coord[1] ;
                              object->a_roll=coord[2] ;
                            }
                  }
/*- - - - - - - - - - - - - - - - - - - - - -  ������������ �������� */
     while(object->a_azim> 180.)  object->a_azim-=360. ;
     while(object->a_azim<-180.)  object->a_azim+=360. ;

     while(object->a_elev> 180.)  object->a_elev-=360. ;
     while(object->a_elev<-180.)  object->a_elev+=360. ;

     while(object->a_roll> 180.)  object->a_roll-=360. ;
     while(object->a_roll<-180.)  object->a_roll+=360. ;

/*---------------------------------------------- ���������� �������� */

       Velo_Matrix.LoadZero   (3, 1) ;
       Velo_Matrix.SetCell    (2, 0, object->v_abs) ;
        Sum_Matrix.Load3d_azim(-object->a_azim) ;
       Oper_Matrix.Load3d_elev(-object->a_elev) ;
        Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
       Velo_Matrix.LoadMul    (&Sum_Matrix, &Velo_Matrix) ;

         object->x_velocity=Velo_Matrix.GetCell(0, 0) ;
         object->y_velocity=Velo_Matrix.GetCell(1, 0) ;
         object->z_velocity=Velo_Matrix.GetCell(2, 0) ;

/*---------------------------------------------- ������� �� �������� */

   for(i=0 ; i<object->Features_cnt ; i++)
     object->Features[i]->vBodyAngles("Flyer.Body", object->a_azim, 
                                                    object->a_elev, 
                                                    object->a_roll ) ;

/*------------------------------------------------------ ����������� */

                this->kernel->vShow(NULL) ;

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
/*       VELOCITY <���> <X-��������> <Y-��������> <Z-��������>      */

  int  RSS_Module_Flyer::cVelocity(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

             char  *pars[_PARS_MAX] ;
             char  *name ;
             char **xyz ;
           double   coord[_COORD_MAX] ;
              int   coord_cnt ;
 RSS_Object_Flyer  *object ;
             char  *error ;
         Matrix2d  Sum_Matrix ;
         Matrix2d  Oper_Matrix ;  
         Matrix2d  Velo_Matrix ;  
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

/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: VELOCITY <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

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
      if(coord_cnt==0)  error="�� ������� �������� ��� �� ��������" ;
      if(coord_cnt==2)  error="������ ���� ������� 3 �������� ��������" ;

      if(error!=NULL) {  SEND_ERROR(error) ;
                               return(-1) ;   }

/*------------------------------------------------- ������� �������� */
/*- - - - - - - - - - - - - - - - - - - ���������� �������� �������� */
   if(coord_cnt==1) {
                          object->v_abs=coord[0] ;

                Velo_Matrix.LoadZero   (3, 1) ;
                Velo_Matrix.SetCell    (2, 0, object->v_abs) ;

                 Sum_Matrix.Load3d_azim(-object->a_azim) ;
                Oper_Matrix.Load3d_elev(-object->a_elev) ;
                 Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
                Velo_Matrix.LoadMul    (&Sum_Matrix, &Velo_Matrix) ;

                       object->x_velocity=Velo_Matrix.GetCell(0, 0) ;
                       object->y_velocity=Velo_Matrix.GetCell(1, 0) ;
                       object->z_velocity=Velo_Matrix.GetCell(2, 0) ;
                    }
/*- - - - - - - - - - - - - - - - - - - - - - - -  �������� �������� */
   else             {
                       object->x_velocity=coord[0] ;
                       object->y_velocity=coord[1] ;
                       object->z_velocity=coord[2] ;

                       object->v_abs     =sqrt(object->x_velocity*object->x_velocity+
                                               object->y_velocity*object->y_velocity+
                                               object->z_velocity*object->z_velocity ) ;
                    }
/*-------------------------------------------------------------------*/

#undef  _COORD_MAX   
#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� CONTROL                 */
/*								    */
/*       CONTROL   <���> <���� �����> [<����������>]                */
/*       CONTROL/G <���> <����������>                               */
/*       CONTROL>  <���> <��� ���������>                            */
/*       CONTROL>> <���> <��� ���������>                            */

  int  RSS_Module_Flyer::cControl(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

             char  *pars[_PARS_MAX] ;
             char  *name ;
             char **xyz ;
           double   coord[_COORD_MAX] ;
              int   coord_cnt ;
           double   inverse ;
 RSS_Object_Flyer  *object ;
              int   xyz_flag ;          /* ���� ������ ����� ���������� */
              int   g_flag ;            /* ���� ������� ���������� */
              int   s_flag ;            /* ���� ������� ����� ��������� */
              int   arrow_flag ;        /* ���� ����������� ������ */
             char  *arrows ;
             char  *error ;
             char  *end ;
              int   i ;

/*---------------------------------------- �������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                        xyz_flag=0 ;
                          g_flag=0 ;
                          s_flag=0 ;
                      arrow_flag=0 ;

       if(*cmd=='/') {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, 'g')!=NULL ||
                   strchr(cmd, 'G')!=NULL   )  g_flag=1 ;

                           cmd=end+1 ;
                     }

  else if(*cmd=='>') {
                           arrow_flag=1 ;

                          cmd=strchr(cmd, ' ') ;
                       if(cmd==NULL)  return(0) ;
                          cmd++ ;
                     } 
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
                  inverse=   1. ; 

/*------------------------------------- ��������� ����������� ������ */

    if(arrow_flag) {                        
                         arrows=pars[1] ;

      if(strstr(arrows, "left" )!=NULL) {  xyz_flag='R' ;  inverse= 1. ;  }
      if(strstr(arrows, "right")!=NULL) {  xyz_flag='R' ;  inverse=-1. ;  }  

      if(strstr(arrows, "up"   )!=NULL) {  xyz_flag='G' ;  inverse=-1. ;  }
      if(strstr(arrows, "down" )!=NULL) {  xyz_flag='G' ;  inverse= 1. ;  }
           
                          xyz=&pars[2] ;   
                   }
/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: CONTROL <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*------------------------------------------------- ������ ��������� */

    for(i=0 ; xyz[i]!=NULL && i<_COORD_MAX ; i++) {

             coord[i]=strtod(xyz[i], &end) ;
        if(*end!=0) {  
                       SEND_ERROR("������������ �������� ����������") ;
                                       return(-1) ;
                    }
                                                  }

                             coord_cnt=  i ;

                        error= NULL ;
      if(coord_cnt==0)  error="�� ������� ���������� ��� �� ����������" ;

      if(error!=NULL) {  SEND_ERROR(error) ;
                               return(-1) ;   }

/*----------------------------------- ������� ����������� ���������� */

   if(g_flag) {
                  object->g_ctrl=coord[0] ;

                      return(0) ;
              }
/*------------------------------------------------ ������� ��������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - ���������� */
   if(arrow_flag) {

          if(xyz_flag=='G')   object->g_ctrl+=inverse*this->g_step ;
     else if(xyz_flag=='R')   object->a_roll+=inverse*this->a_step ;
                  }
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� �������� */
   else           {
                               object->a_roll=coord[0] ;
              if(coord_cnt>1)  object->g_ctrl=coord[1] ;
                  }
/*- - - - - - - - - - - - - - - - - - - - - -  ������������ �������� */
     while(object->a_roll> 180.)  object->a_roll-=360. ;
     while(object->a_roll<-180.)  object->a_roll+=360. ;

/*---------------------------------------------- ������� �� �������� */

   for(i=0 ; i<object->Features_cnt ; i++)
     object->Features[i]->vBodyAngles("Flyer.Body", object->a_azim, 
                                                    object->a_elev, 
                                                    object->a_roll ) ;

/*------------------------------------------------------ ����������� */

                this->kernel->vShow(NULL) ;

/*-------------------------------------------------------------------*/

#undef  _COORD_MAX   
#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� PROGRAM                 */
/*								    */
/*      PROGRAM <���> <���� ���������>                              */

  int  RSS_Module_Flyer::cProgram(char *cmd)

{
#define   _PARS_MAX   4
 RSS_Object_Flyer *object ;
             char *pars[_PARS_MAX] ;
             char *name ;
             char *end ;
              int  status ;
              int  i ;

/*-------------------------------------- ���������� ��������� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {

                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }

                     name= pars[0] ;

/*------------------------------------------- �������� ����� ������� */

    if(name   ==NULL ||
       name[0]==  0    ) {                                          /* ���� ��� �� ������... */
                           SEND_ERROR("�� ������ ��� �������.\n"
                                      "��������: PROGRAM <���> ...") ;
                                     return(-1) ;
                         }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*--------------------------------------- ���������� ������ �� ����� */

  if(pars[1]==NULL) {
                       SEND_ERROR("�� ������ ��� ����� ���������.\n"
                                  "��������: PROGRAM <���> <���� ���������>") ;
                                     return(-1) ;
                    }

      status=iReadProgram(object, pars[1]) ;

/*-------------------------------------------------------------------*/

#undef   _PARS_MAX

   return(status) ;
}


/********************************************************************/
/*								    */
/*		      ���������� ���������� TRACE                   */
/*								    */
/*       TRACE <���> [<������������>]                               */
/*       TRACE/P <���> <����� ����������>                           */

  int  RSS_Module_Flyer::cTrace(char *cmd)

{
#define  _COORD_MAX   3
#define   _PARS_MAX  10

             char  *pars[_PARS_MAX] ;
             char  *name ;
           double   trace_time ;
           double   time_0 ;        /* ��������� ����� ������� */ 
           double   time_1 ;        /* ������� ����� */ 
           double   time_c ;        /* ���������� ����� ������� */ 
           double   time_s ;        /* ��������� ����� ��������� */ 
           double   time_w ;        /* ����� �������� */ 
 RSS_Object_Flyer  *object ;
              int   program_flag ;  /* ����� ������������ ���������� */ 
             char  *end ;
              int   i ;

/*---------------------------------------- �������� ��������� ������ */

                     trace_time=0. ;
/*- - - - - - - - - - - - - - - - - - -  ��������� ������ ���������� */
                   program_flag=0 ;

       if(*cmd=='/' ||
          *cmd=='+'   ) {
 
                if(*cmd=='/')  cmd++ ;

                   end=strchr(cmd, ' ') ;
                if(end==NULL) {
                       SEND_ERROR("������������ ������ �������") ;
                                       return(-1) ;
                              }
                  *end=0 ;

                if(strchr(cmd, 'p')!=NULL ||
                   strchr(cmd, 'P')!=NULL   )  program_flag=1 ;

                           cmd=end+1 ;
                        }
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������ ���������� */        
    for(i=0 ; i<_PARS_MAX ; i++)  pars[i]=NULL ;

    for(end=cmd, i=0 ; i<_PARS_MAX ; end++, i++) {
      
                pars[i]=end ;
                   end =strchr(pars[i], ' ') ;
                if(end==NULL)  break ;
                  *end=0 ;
                                                 }


                     name=pars[0] ;

      if(program_flag) {

           if( pars[1]==NULL ||
              *pars[1]==  0    ) {
                                     SEND_ERROR("�� ������ ��������� ����������") ;
                                         return(-1) ;
                                 }
                       }
      else             {

           if( pars[1]!=NULL &&
              *pars[1]!=  0    ) {
                                      trace_time=strtod(pars[1], &end) ;
                                   if(trace_time<=0.) {
                                         SEND_ERROR("������ ������������ ����� �����������") ;
                                                           return(-1) ;
                                                      }
                                 }
           else                  {
                                       trace_time=60. ;
                                         SEND_ERROR("����� ����������� - 60 ������") ;
                                 }
                       }
/*------------------------------------------- �������� ����� ������� */

    if(name==NULL) {                                                /* ���� ��� �� ������... */
                      SEND_ERROR("�� ������ ��� �������. \n"
                                 "��������: ANGLE <���_�������> ...") ;
                                     return(-1) ;
                   }

       object=FindObject(name) ;                                    /* ���� ������ �� ����� */
    if(object==NULL)  return(-1) ;

/*------------------------------------ �������� ��������� ���������� */

                  object->program=NULL ;

    if(program_flag) {

        for(i=0 ; i<_PROGRAMS_MAX ; i++)
          if(object->programs[i]!=NULL)
           if(!stricmp(object->programs[i]->name, pars[1])) {
                    object->program=object->programs[i] ;
                             break ;
                                                            } 

      if(object->program==NULL) {
            SEND_ERROR("����������� ��������� - �������������� �������� PROGRAM ��� �������� ����� ���������") ;
                                     return(-1) ;
                                }

                memset(&object->p_controls, 0, sizeof(object->p_controls)) ;
                strcpy( object->p_controls.used, ";") ;
                        object->p_frame=0 ;
                     }
/*------------------------------------------------------ ����������� */

              time_0=this->kernel->vGetTime() ;
              time_c=0. ;
              time_s=0. ;

         object->iSaveTracePoint("CLEAR") ;

    do {                                                            /* CIRCLE.1 - ���� ����������� */
           if(this->kernel->stop)  break ;                          /* ���� ������� ���������� ������ */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ��������� ������� */
              time_c+=RSS_Kernel::calc_time_step ;
              time_1=this->kernel->vGetTime() ;

         if(object->program==NULL)
           if(time_1-time_0>trace_time)  break ;                    /* ���� ����� ����������� ����������� */

              time_w=time_c-(time_1-time_0) ;

           if(time_w>=0)  Sleep(time_w*1000) ;
/*- - - - - - - - - - - - - - - - - - - - - - ������������� �������� */
         object->vCalculate(time_c-RSS_Kernel::calc_time_step, time_c) ;
         object->iSaveTracePoint("ADD") ;
/*- - - - - - - - - - - - - - - - - - - - - - ����������� ���������� */
         object->iShowTrace_() ;
/*- - - - - - - - - - - - - - - - - - - - - - -  ����������� ������� */
   for(i=0 ; i<object->Features_cnt ; i++) {
     object->Features[i]->vBodyBasePoint("Flyer.Body", object->x_base, 
                                                       object->y_base, 
                                                       object->z_base ) ;
     object->Features[i]->vBodyAngles   ("Flyer.Body", object->a_azim, 
                                                       object->a_elev, 
                                                       object->a_roll ) ;
                                            }
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ��������� ����� */
          time_1=this->kernel->vGetTime() ;
       if(time_1-time_s>=this->kernel->show_time_step) {

                 time_s=time_1 ;

              this->kernel->vShow(NULL) ;
                                                       }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
       } while(1) ;                                                 /* END CIRCLE.1 - ���� ����������� */

/*-------------------------------------------------------------------*/

#undef  _COORD_MAX   
#undef   _PARS_MAX    

   return(0) ;
}


/********************************************************************/
/*								    */
/*		   ����� ������� ���� FLYER �� �����                */

  RSS_Object_Flyer *RSS_Module_Flyer::FindObject(char *name)

{
     char   text[1024] ;
      int   i ;

#define   OBJECTS       this->kernel->kernel_objects 
#define   OBJECTS_CNT   this->kernel->kernel_objects_cnt 

/*------------------------------------------- ����� ������� �� ����� */ 

       for(i=0 ; i<OBJECTS_CNT ; i++)                               /* ���� ������ �� ����� */
         if(!stricmp(OBJECTS[i]->Name, name))  break ;

    if(i==OBJECTS_CNT) {                                            /* ���� ��� �� �������... */
                           sprintf(text, "������� � ������ '%s' "
                                         "�� ����������", name) ;
                        SEND_ERROR(text) ;
                            return(NULL) ;
                       }
/*-------------------------------------------- �������� ���� ������� */ 

     if(strcmp(OBJECTS[i]->Type, "Flyer")) {

           SEND_ERROR("������ �� �������� �������� ���� FLYER") ;
                            return(NULL) ;
                                           }
/*-------------------------------------------------------------------*/ 

   return((RSS_Object_Flyer *)OBJECTS[i]) ;
  
#undef   OBJECTS
#undef   OBJECTS_CNT

}


/********************************************************************/
/*								    */
/*		      �������� �������                              */

  int  RSS_Module_Flyer::CreateObject(RSS_Model_data *data)

{
  RSS_Object_Flyer *object ;
              char  models_list[4096] ;
              char *end ;
               int  i ;
               int  j ;

#define   OBJECTS       this->kernel->kernel_objects 
#define   OBJECTS_CNT   this->kernel->kernel_objects_cnt 

#define       PAR             object->Parameters
#define       PAR_CNT         object->Parameters_cnt
 
/*--------------------------------------------------- �������� ����� */

    if(data->name[0]==0) {                                           /* ���� ��� �� ������ */
              SEND_ERROR("������ FLYER: �� ������ ��� �������") ;
                                return(-1) ;
                         }

       for(i=0 ; i<OBJECTS_CNT ; i++)
         if(!stricmp(OBJECTS[i]->Name, data->name)) {
              SEND_ERROR("������ FLYER: ������ � ����� ������ ��� ����������") ;
                                return(-1) ;
                                                    }
/*-------------------------------------- ���������� �������� ������� */
/*- - - - - - - - - - - - ���� ������ ������ ��������� � ����������� */
   if(data->path[0]==0) {

    if(data->model[0]==0) {                                         /* ���� ������ �� ������ */
              SEND_ERROR("������ FLYER: �� ������ ������ �������") ;
                                return(-1) ;
                          }

        RSS_Model_list(data->lib_path, models_list,                 /* ������������ ������ ������� */
                                sizeof(models_list)-1, "BODY" );

        for(end=models_list ; *end ; ) {                            /* ���� ������ �� ������ */
                        if(!stricmp(data->model, end))  break ;
                                         end+=strlen(end)+1 ;
                                         end+=strlen(end)+1 ;
                                       }

           if(*end==0) {
              SEND_ERROR("������ FLYER: ����������� ������ ����") ;
                                return(-1) ;
                       }

                                    end+=strlen(end)+1 ;            /* ��������� ��� ����� */

                      sprintf(data->path, "%s\\%s", data->lib_path, end) ;
           RSS_Model_ReadPars(data) ;                               /* ��������� ��������� ������ */
                        }
/*- - - - - - - - - - - - - - - - -  ���� ������ ������ ������ ����� */
   else                 {
                              RSS_Model_ReadPars(data) ;            /* ��������� ��������� ������ */
                        }
/*--------------------------------------- �������� ������ ���������� */

     for(i=0 ; i<5 ; i++)
       if((data->pars[i].text [0]==0 &&
           data->pars[i].value[0]!=0   ) ||
          (data->pars[i].text [0]!=0 &&
           data->pars[i].value[0]==0   )   ) {

              SEND_ERROR("������ FLYER: �������������� ����� ���������� ������") ;
                                return(-1) ;
                                             }
/*------------------------------------------------- �������� ������� */

       object=new RSS_Object_Flyer ;
    if(object==NULL) {
              SEND_ERROR("������ FLYER: ������������ ������ ��� �������� �������") ;
                        return(-1) ;
                     }
/*------------------------------------- ���������� ������ ���������� */
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������� ��������� */
     for(i=0 ; i<5 ; i++)
       if(data->pars[i].text[0]!=0) {

           PAR=(struct RSS_Parameter *)
                 realloc(PAR, (PAR_CNT+1)*sizeof(*PAR)) ;
        if(PAR==NULL) {
                         SEND_ERROR("������ FLYER: ������������ ������") ;
                                            return(-1) ;
                      }

             memset(&PAR[PAR_CNT], 0, sizeof(PAR[PAR_CNT])) ;
            sprintf( PAR[PAR_CNT].name, "PAR%d", i+1) ;
                     PAR[PAR_CNT].value=strtod(data->pars[i].value, &end) ;
                     PAR[PAR_CNT].ptr  = NULL ;
                         PAR_CNT++ ;
                                    }
/*- - - - - - - - - - - - - - - - - - - - ����������� ������ ������� */
           PAR=(struct RSS_Parameter *)
                 realloc(PAR, (PAR_CNT+1)*sizeof(*PAR)) ;
        if(PAR==NULL) {
                         SEND_ERROR("������ FLYER: ������������ ������") ;
                                            return(-1) ;
                      }

             memset(&PAR[PAR_CNT], 0, sizeof(PAR[PAR_CNT])) ;

/*---------------------------------- �������� ������ ������� ������� */

      object->Features_cnt=this->feature_modules_cnt ;
      object->Features    =(RSS_Feature **)
                             calloc(this->feature_modules_cnt, 
                                     sizeof(object->Features[0])) ;

   for(i=0 ; i<this->feature_modules_cnt ; i++)
      object->Features[i]=this->feature_modules[i]->vCreateFeature(object) ;

/*-------------------------------------- ���������� �������� ������� */

           RSS_Model_ReadSect(data) ;                               /* ��������� ������ �������� ������ */

   for(i=0 ; data->sections[i].title[0] ; i++) {

     for(j=0 ; j<object->Features_cnt ; j++) {

          object->Features[j]->vBodyPars(NULL, PAR) ;
          object->Features[j]->vReadSave(data->sections[i].title, 
                                         data->sections[i].decl, "Flyer.Body") ;
                                             }

                                         data->sections[i].title[0]= 0 ;
                                        *data->sections[i].decl    ="" ;
                                               }
/*---------------------------------- �������� ������� � ����� ������ */

       OBJECTS=(RSS_Object **)
                 realloc(OBJECTS, (OBJECTS_CNT+1)*sizeof(*OBJECTS)) ;
    if(OBJECTS==NULL) {
              SEND_ERROR("������ FLYER: ������������ ������") ;
                                return(-1) ;
                      }

              OBJECTS[OBJECTS_CNT]=object ;
                      OBJECTS_CNT++ ;

       strcpy(object->Name,       data->name) ;
       strcpy(object->model_path, data->path) ;

        SendMessage(this->kernel_wnd, WM_USER,
                     (WPARAM)_USER_DEFAULT_OBJECT, (LPARAM)data->name) ;

/*-------------------------------------------------------------------*/

#undef   OBJECTS
#undef   OBJECTS_CNT

#undef   PAR
#undef   PAR_CNT

  return(0) ;
}


/*********************************************************************/
/*								     */
/*              ���������� ����� �������� ���������                  */
/*								     */
/*  PROGRAM <��� ���������>                                          */
/*  OBJECT <���������� ��� �������>                                  */
/*								     */
/*  ������������ ������ ��������:				     */
/*    <������������-1> <������������-2> ... <������������-N>         */
/*      T=<��������>      - ����� �������    			     */
/*   ���������� �������:                                             */
/*      X=<��������>                                                 */
/*      Y=<��������>                                                 */
/*      Z=<��������>                                                 */
/*   ���������� ������� (������, ����������, ����)                   */
/*      A=<��������>                                                 */
/*      E=<��������>                                                 */
/*      R=<��������>                                                 */
/*   �������� ��������� ���������� ������� (������, ����������, ����)*/
/*     DA=<��������>[:<�� ��������>]                                 */
/*     DE=<��������>[:<�� ��������>]                                 */
/*     DR=<��������>[:<�� ��������>]                                 */
/*   ���������� �������                                              */
/*      G=<��������>                                                 */
/*   �������� ��������� ���������� �������                           */
/*     DG=<��������>[:<�� ��������>]                                 */
/*   ��������                                                        */
/*      V=<��������>                                                 */
/*   ���������                                                       */
/*     DV=<���������>[:<�� ��������>]                                */

  int  RSS_Module_Flyer::iReadProgram(RSS_Object_Flyer *object, char *path)
{
                    FILE *file ;
 RSS_Object_FlyerProgram *program ;
  RSS_Object_FlyerPFrame  frame ;
                    char  text[1024] ;
                    char  error[1024] ;
                    char *words[20] ;
                    char *name ;
                    char *data ;
                  double  value ;
                  double  target ;
                     int  target_flag ;
                    char *end ;
                     int  row ;
                     int  n ;
                     int  i ;

/*--------------------------------------------------- �������� ����� */

       file=fopen(path, "rt") ;
    if(file==NULL) {
                          sprintf(error, "������ %d �������� ����� %s", errno, path) ;
                       SEND_ERROR(error) ;
                                       return(-1) ;
                   }
/*------------------------------------------------ ���������� ������ */

                      row=0 ;

   while(1) {                                                       /* CIRCLE.1 - ��������� ������ ���� */

/*-------------------------------------- ���������� ��������� ������ */

                      row++ ;

                      memset(text, 0, sizeof(text)) ;
                   end=fgets(text, sizeof(text)-1, file) ;          /* ��������� ������ */
                if(end==NULL)  break ;

            if(text[0]==';')  continue ;                            /* �������� ����������� */

               end=strchr(text, '\n') ;                             /* ������� ������ ����� ������ */
            if(end!=NULL)  *end=0 ;
               end=strchr(text, '\r') ;
            if(end!=NULL)  *end=0 ;

/*---------------------------------- ��������� ������������� PROGRAM */

#define  _KEY_WORD  "PROGRAM "
      if(!memicmp(text, _KEY_WORD, strlen(_KEY_WORD))) {

               end=text+strlen(_KEY_WORD) ;
            if(*end==0) {
                          sprintf(error, "������ %d - �������� PROGRAM �� �������� ��� ���������", row) ;
                       SEND_ERROR(error) ;
                             return(-1) ;
                        }  

               program=NULL ;

        do {

         for(i=0 ; i<_PROGRAMS_MAX ; i++)
           if(object->programs[i]!=NULL)
            if(!stricmp(object->programs[i]->name, end)) {
                                                            program=object->programs[i] ;
                                                               break ;
                                                         }

            if(program!=NULL)  break ;

         for(i=0 ; i<_PROGRAMS_MAX ; i++)
            if(object->programs[i]==NULL) {
                                             program    =new RSS_Object_FlyerProgram ;
                                     object->programs[i]=program ;        
                                             break ;
                                          }
           } while(0) ;  

            if(program==NULL) {
                          sprintf(error, "������ %d - ���������� �������� ��� ������� ��������� ���������� ������ - %d", row, _PROGRAMS_MAX) ;
                       SEND_ERROR(error) ;
                             return(-1) ;
                              }

               strncpy(program->name, end, sizeof(program->name)-1) ;
                       program->frames_cnt=0 ;

                                       continue ;
                                                       }
#undef   _KEY_WORD

/*----------------------------------- ��������� ������������� OBJECT */

#define  _KEY_WORD  "OBJECT "
      if(!memicmp(text, _KEY_WORD, strlen(_KEY_WORD))) {

               end=text+strlen(_KEY_WORD) ;
            if(*end==0) {
                          sprintf(error, "������ %d - �������� OBJECT �� �������� ��� �������", row) ;
                       SEND_ERROR(error) ;
                             return(-1) ;
                        }  

            if(stricmp(end, "Flyer")) {
                          sprintf(error, "������ %d - ��������� �� ������������� ��� �������� ������� ����", row) ;
                       SEND_ERROR(error) ;
                             return(-1) ;
                                      }

                                       continue ;
                                                       }
#undef   _KEY_WORD

/*------------------------------------------- ������ ������ �� ����� */

            if(program==NULL) {
                                sprintf(error, "������ %d - ��������� ������ ���������� � ��������� PROGRAM", row) ;
                             SEND_ERROR(error) ;
                                  return(-1) ;
                              }
        
           memset(words, 0, sizeof(words)) ;

                   i = 0 ;
             words[i]=strtok(text, " \t") ;

       while(words[i]!=NULL && i<30) {
                   i++ ;
             words[i]=strtok(NULL, " \t") ;
                                    }
/*---------------------------------------------- ������������ ������ */

          memset(&frame, 0, sizeof(frame)) ;

          strcpy( frame.used, ";") ;

       for(i=0 ; i<30 ; i++) {
                                 if(words[i]==NULL)  break ;

             end=strchr(words[i], '=') ;
          if(end==NULL) {
                             sprintf(error, "������ %d - � ������������� %d ����������� ������ ����������� '='", row, i+1) ;
                          SEND_ERROR(error) ;
                                return(-1) ;
                        }
            *end=0 ;

              name=words[i] ;
              data=end+1 ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - -  ����� ������� */
          if(!stricmp(name, "T")) {

               frame.t=strtod(data, &end) ;
            if(*end!=0) {
                             sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                          SEND_ERROR(error) ;
                                return(-1) ;
                        }
                                  }
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� ������� */
          else
          if(!stricmp(name, "X") ||
             !stricmp(name, "Y") ||
             !stricmp(name, "Z")   ) {

               value=strtod(data, &end) ;
            if(*end!=0) {
                             sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                          SEND_ERROR(error) ;
                                return(-1) ;
                        }

            if(!stricmp(name, "X"))  frame.x=value ;
            if(!stricmp(name, "Y"))  frame.y=value ;
            if(!stricmp(name, "Z"))  frame.z=value ;

                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;
                                     }
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� ������� */
          else
          if(!stricmp(name, "A") ||
             !stricmp(name, "E") ||
             !stricmp(name, "R")   ) {

               value=strtod(data, &end) ;
            if(*end!=0) {
                             sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                          SEND_ERROR(error) ;
                                return(-1) ;
                        }

            if(!stricmp(name, "A"))  frame.a=value ;
            if(!stricmp(name, "E"))  frame.e=value ;
            if(!stricmp(name, "R"))  frame.r=value ;

                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;
                                     }
/*- - - - - - - - - - - - - - �������� ��������� ���������� ������� */
          else
          if(!stricmp(name, "DA") ||
             !stricmp(name, "DE") ||
             !stricmp(name, "DR")   ) {

                             target_flag= 0 ;
                              value     =strtod(data, &end) ;
            if(*end==':') {
                             target     =strtod(end+1, &end) ;
                             target_flag= 1  ;
                          }

            if(*end!=0) {
                          sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                       SEND_ERROR(error) ;
                             return(-1) ;
                        }

            if(!stricmp(name, "DA"))  frame.d_a=value ;
            if(!stricmp(name, "DE"))  frame.d_e=value ;
            if(!stricmp(name, "DR"))  frame.d_r=value ;

                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;

            if(target_flag==0)  continue ; 

                        name[0]='T' ;

            if(!stricmp(name, "TA"))  frame.t_a=target ;
            if(!stricmp(name, "TE"))  frame.t_e=target ;
            if(!stricmp(name, "TR"))  frame.t_r=target ;

                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;

                                      }
/*- - - - - - - - - - - - - - - - - - - - - - -  ���������� ������� */
          else
          if(!stricmp(name, "G")) {

               value=strtod(data, &end) ;
            if(*end!=0) {
                             sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                          SEND_ERROR(error) ;
                                return(-1) ;
                        }

                                      frame.g=value ;

                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;
                                     }
/*- - - - - - - - - - - - - -  �������� ��������� ���������� ������� */
          else
          if(!stricmp(name, "DG")) {

                             target_flag= 0 ;
                              value     =strtod(data, &end) ;
            if(*end==':') {
                             target     =strtod(end+1, &end) ;
                             target_flag= 1  ;
                          }

            if(*end!=0) {
                          sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                       SEND_ERROR(error) ;
                             return(-1) ;
                        }

                                      frame.d_g=value ;
                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;

            if(target_flag==0)  continue ;

                                      frame.t_g=target ;
                               strcat(frame.used, "TG;") ;
                                   }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -  �������� */
          else
          if(!stricmp(name, "V")) {

               value=strtod(data, &end) ;
            if(*end!=0) {
                             sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                          SEND_ERROR(error) ;
                                return(-1) ;
                        }

                                      frame.v=value ;

                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;
                                     }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -  ��������� */
          else
          if(!stricmp(name, "DV")) {

                             target_flag= 0 ;
                              value     =strtod(data, &end) ;
            if(*end==':') {
                             target     =strtod(end+1, &end) ;
                             target_flag= 1  ;
                          }

            if(*end!=0) {
                          sprintf(error, "������ %d - � ������������� %s ������ ������������ ��������", row, name) ;
                       SEND_ERROR(error) ;
                             return(-1) ;
                        }

                                      frame.d_v=value ;
                               strcat(frame.used, name) ;
                               strcat(frame.used, ";" ) ;

            if(target_flag==0)  continue ;

                                      frame.t_v=target ;
                               strcat(frame.used, "TV;") ;
                                   }
/*- - - - - - - - - - - - - - - - - - - - - ����������� ������������ */
          else                    {

                             sprintf(error, "������ %d - ����������� ������������ %s", row, name) ;
                          SEND_ERROR(error) ;
                                return(-1) ;

                                  } 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                             }
/*--------------------------------------- ���������� ����� ��������� */

       if(program->frames_cnt>=_PFRAMES_MAX) {
               sprintf(error, "������ %d - ���������� ������ ��������� ��������� ���������� ������ - %d", row, _PFRAMES_MAX) ;
            SEND_ERROR(error) ;
                  return(-1) ;
                                             }

       memcpy(&program->frames[program->frames_cnt], &frame, sizeof(frame)) ;
                               program->frames_cnt++ ;

/*-------------------------------------------------------------------*/
            }                                                       /* CONTINUE.1 */
/*--------------------------------------------------- �������� ����� */

                fclose(file) ;

/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/********************************************************************/
/**							           **/
/**		  �������� ������ ������� "�����"	           **/
/**							           **/
/********************************************************************/
/********************************************************************/

/********************************************************************/
/*								    */
/*		       ����������� ������			    */

     RSS_Object_Flyer::RSS_Object_Flyer(void)

{
   strcpy(Type, "Flyer") ;

    Context        =new RSS_Transit_Flyer ;
    Context->object=this ;

   Parameters    =NULL ;
   Parameters_cnt=  0 ;

      x_base    = 0. ;
      y_base    = 0. ;
      z_base    = 0. ;
      a_azim    = 0. ;
      a_elev    = 0. ;
      a_roll    = 0. ;
      x_velocity= 0. ;
      y_velocity= 0. ;
      z_velocity= 0. ;
      v_abs     = 0. ;
      g_ctrl    = 0. ;

      r_ctrl    = 0. ;
      m_ctrl    =NULL ;

    memset(programs, 0, sizeof(programs)) ;

      mTrace      =NULL ;
      mTrace_cnt  =  0 ;  
      mTrace_max  =  0 ; 
      mTrace_color=  0 ;  
      mTrace_dlist=  0 ;  
}


/********************************************************************/
/*								    */
/*		        ���������� ������			    */

    RSS_Object_Flyer::~RSS_Object_Flyer(void)

{
      vFree() ;

   delete Context ;
}


/********************************************************************/
/*								    */
/*		       ������������ ��������                        */

  void   RSS_Object_Flyer::vFree(void)

{
  int  i ;


   for(i=0 ; i<_PROGRAMS_MAX ; i++)  
     if(programs[i]!=NULL) {
                              delete programs[i] ;
                                     programs[i]=NULL ;
                           }

  if(this->mTrace!=NULL) {
                             free(this->mTrace) ;
                                  this->mTrace    =NULL ;
                                  this->mTrace_cnt=  0 ;  
                                  this->mTrace_max=  0 ;  
                         }

  if(this->Features!=NULL) {

    for(i=0 ; i<this->Features_cnt ; i++) {
                this->Features[i]->vBodyDelete(NULL) ;
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

    void  RSS_Object_Flyer::vWriteSave(std::string *text)

{
  char  field[1024] ;
   int  i ;

/*----------------------------------------------- ��������� �������� */

     *text="#BEGIN OBJECT FLYER\n" ;

/*----------------------------------------------------------- ������ */

    sprintf(field, "NAME=%s\n",       this->Name      ) ;  *text+=field ;
    sprintf(field, "X_BASE=%.10lf\n", this->x_base    ) ;  *text+=field ;
    sprintf(field, "Y_BASE=%.10lf\n", this->y_base    ) ;  *text+=field ;
    sprintf(field, "Z_BASE=%.10lf\n", this->z_base    ) ;  *text+=field ;
    sprintf(field, "A_AZIM=%.10lf\n", this->a_azim    ) ;  *text+=field ;
    sprintf(field, "A_ELEV=%.10lf\n", this->a_elev    ) ;  *text+=field ;
    sprintf(field, "A_ROLL=%.10lf\n", this->a_roll    ) ;  *text+=field ;
    sprintf(field, "MODEL=%s\n",      this->model_path) ;  *text+=field ;

  for(i=0 ; i<this->Parameters_cnt ; i++) {
    sprintf(field, "PAR_%d=%.10lf\n", 
                          i, this->Parameters[i].value) ;  *text+=field ;
                                          }
/*------------------------------------------------ �������� �������� */

     *text+="#END\n" ;

/*-------------------------------------------------------------------*/
}


/********************************************************************/
/*								    */
/*                   ������ ��������� ���������                     */

     int  RSS_Object_Flyer::vCalculate(double t1, double t2)
{
  Matrix2d  Sum_Matrix ;
  Matrix2d  Oper_Matrix ;
  Matrix2d  Vect_Matrix ;  
  Matrix2d  Gold_Matrix ;  
  Matrix2d  Gnew_Matrix ;  
    double  r ;             /* ������ ������� */
    double  a ;             /* ����������� ���� ������� */
    double  s1, s2 ;
    double  x0, y0, z0 ;    /* ����� ������� */
    double  x1, y1, z1 ;    /* ����� ������ ������������ ���� ������� */
    double  n1_azim, n1_elev ;
    double  n2_azim, n2_elev ;
    double  x2, y2, z2 ;    /* ����� ������ ������������ ���� ������� */
    double  k ;

/*---------------------------------------------- ��������� ��������� */

   if(program!=NULL)  iExecuteProgram(t1, t2) ;

/*---------------------------------------------- ���������� �������� */

   if(this->g_ctrl==0) {
                          x_base+=x_velocity*(t2-t1) ;
                          y_base+=y_velocity*(t2-t1) ;
                          z_base+=z_velocity*(t2-t1) ;
                       }
/*-------------------------------------------- ���������� ���������� */

   else                {
/*- - - - - - - - - - - - - - - - - - - - - - - -  ��������� ������� */
                           r      = v_abs*v_abs/g_ctrl ;
                           a      =(360.*v_abs*(t2-t1))/(2.*_PI*r) ;
/*- - - - - - - - - - - - - - - - - - - - - - - -  ������� �������������� */
                 Sum_Matrix.Load3d_azim(-a_azim) ;
                Oper_Matrix.Load3d_elev(-a_elev) ;
                 Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
                Oper_Matrix.Load3d_roll(-a_roll) ;
                 Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;

    if(r_ctrl!=a_roll ||
       m_ctrl==  NULL   ) {
                  if(m_ctrl==NULL) m_ctrl=new Matrix2d ;
 
                             m_ctrl->Copy(&Sum_Matrix) ;
                             r_ctrl=a_roll ;
                             a_ctrl=   0. ;
                          }
/*- - - - - - - - - - - - - - - - - - - - - -  ������ �������� ����� */
                         x1=0 ;
                         y1=r-r*cos(a*_GRD_TO_RAD) ;
                         z1=  r*sin(a*_GRD_TO_RAD) ;

                Vect_Matrix.LoadZero(3, 1) ;
                Vect_Matrix.SetCell (0, 0, x1) ;
                Vect_Matrix.SetCell (1, 0, y1) ;
                Vect_Matrix.SetCell (2, 0, z1) ;

                Vect_Matrix.LoadMul (&Sum_Matrix, &Vect_Matrix) ;

                    x_base+=Vect_Matrix.GetCell(0, 0) ;
                    y_base+=Vect_Matrix.GetCell(1, 0) ;
                    z_base+=Vect_Matrix.GetCell(2, 0) ;
/*- - - - - - - - - - - - - - - - - - - - - ������ �������� �������� */
                     a_ctrl+=a ;
                         x1 =0. ;
                         y1 =this->v_abs*sin(a_ctrl*_GRD_TO_RAD) ;
                         z1 =this->v_abs*cos(a_ctrl*_GRD_TO_RAD) ;

                Vect_Matrix.LoadZero(3, 1) ;
                Vect_Matrix.SetCell (0, 0, x1) ;
                Vect_Matrix.SetCell (1, 0, y1) ;
                Vect_Matrix.SetCell (2, 0, z1) ;

                Vect_Matrix.LoadMul (m_ctrl, &Vect_Matrix) ;

                    x_velocity=Vect_Matrix.GetCell(0, 0) ;
                    y_velocity=Vect_Matrix.GetCell(1, 0) ;
                    z_velocity=Vect_Matrix.GetCell(2, 0) ;
/*- - - - - - - - - - - - - - - - - - - - ��������� ����� ���������� */
                Gold_Matrix.LoadZero(3, 1) ;
                Gold_Matrix.SetCell (1, 0, 1.) ;
                Gold_Matrix.LoadMul (&Sum_Matrix, &Gold_Matrix) ;

                  n1_azim=atan2(x_velocity, z_velocity)*_RAD_TO_GRD ;
                  n1_elev=atan2(y_velocity, sqrt(x_velocity*x_velocity+z_velocity*z_velocity))*_RAD_TO_GRD ;
                  n2_azim=180.+n1_azim ;
                  n2_elev=180.-n1_elev ;

                 Sum_Matrix.Load3d_azim(-n1_azim) ;
                Oper_Matrix.Load3d_elev(-n1_elev) ;
                 Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
                Oper_Matrix.Load3d_roll(-a_roll) ;
                 Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
                
                Gnew_Matrix.LoadZero(3, 1) ;
                Gnew_Matrix.SetCell (1, 0, 1.) ;
                Gnew_Matrix.LoadMul (&Sum_Matrix, &Gnew_Matrix) ;

              x1=Gold_Matrix.GetCell(0, 0)-Gnew_Matrix.GetCell(0, 0) ;
              y1=Gold_Matrix.GetCell(1, 0)-Gnew_Matrix.GetCell(1, 0) ;
              z1=Gold_Matrix.GetCell(2, 0)-Gnew_Matrix.GetCell(2, 0) ;

              s1=sqrt(x1*x1+y1*y1+z1*z1) ;
           if(s1>0.) {  x1/=s1 ;  y1/=s1 ;  z1/=s1 ;  }

                 Sum_Matrix.Transpose(m_ctrl) ;
                Vect_Matrix.LoadZero (3, 1) ;
                Vect_Matrix.SetCell  (0, 0, x1) ;
                Vect_Matrix.SetCell  (1, 0, y1) ;
                Vect_Matrix.SetCell  (2, 0, z1) ;
                Vect_Matrix.LoadMul  (&Sum_Matrix, &Vect_Matrix) ;

             x2=Vect_Matrix.GetCell  (0, 0) ;
             y2=Vect_Matrix.GetCell  (1, 0) ;
             z2=Vect_Matrix.GetCell  (2, 0) ;

                 Sum_Matrix.Load3d_azim(-n2_azim) ;
                Oper_Matrix.Load3d_elev(-n2_elev) ;
                 Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
                Oper_Matrix.Load3d_roll(-a_roll) ;
                 Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
                
                Gnew_Matrix.LoadZero(3, 1) ;
                Gnew_Matrix.SetCell (1, 0, 1.) ;
                Gnew_Matrix.LoadMul (&Sum_Matrix, &Gnew_Matrix) ;

              x2=Gold_Matrix.GetCell(0, 0)-Gnew_Matrix.GetCell(0, 0) ;
              y2=Gold_Matrix.GetCell(1, 0)-Gnew_Matrix.GetCell(1, 0) ;
              z2=Gold_Matrix.GetCell(2, 0)-Gnew_Matrix.GetCell(2, 0) ;

              s2=sqrt(x2*x2+y2*y2+z2*z2) ;
//           if(s2>0.) {  x2/=s2 ;  y2/=s2 ;  z2/=s2 ;  }

                 Sum_Matrix.Transpose(m_ctrl) ;
                Vect_Matrix.LoadZero (3, 1) ;
                Vect_Matrix.SetCell  (0, 0, x2) ;
                Vect_Matrix.SetCell  (1, 0, y2) ;
                Vect_Matrix.SetCell  (2, 0, z2) ;
                Vect_Matrix.LoadMul  (&Sum_Matrix, &Vect_Matrix) ;

             x2=Vect_Matrix.GetCell  (0, 0) ;

            if(     s1 > 1.     ) {
                                      a_azim=n2_azim ;
                                      a_elev=n2_elev ;
                                  }
            else
            if(     s2 > 1.     ) {
                                      a_azim=n1_azim ;
                                      a_elev=n1_elev ;
                                  }
            else
            if(fabs(x1)<fabs(x2)) {
                                      a_azim=n1_azim ;
                                      a_elev=n1_elev ;
                                  }
            else                  {
                                      a_azim=n2_azim ;
                                      a_elev=n2_elev ;
                                  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                       }
/*-------------------------------------------------------------------*/

  return(0) ;
}


/********************************************************************/
/*								    */
/*               ��������� ������������ ����������                  */

     int  RSS_Object_Flyer::iExecuteProgram(double t1, double t2)
{
   Matrix2d  Sum_Matrix ;
   Matrix2d  Oper_Matrix ;  
   Matrix2d  Velo_Matrix ;  
        int  v_flag ;    /* ���� ����������� �������� �������� */
       char  used[1024] ;
     double  a_new, e_new, r_new, g_new, v_new ;
        int  status ;
        int  i ;

#define  FRAME program->frames[i]

/*----------------------------------- �������� ���������� ���������� */

       if(p_frame>=program->frames_cnt)  return(0) ;

/*--------------------------------------- ��������� ������ ��������� */

             v_flag=0 ;

   for(i=p_frame ; i<program->frames_cnt ; i++) {
/*- - - - - - - - - - - - - - - - - - - - -  ��������� ����� ������� */
     if(program->frames[i].t>t2)  break ;
/*- - - - - - - - - - - - - - - - - - - - - - -  ��������� ��������� */
     if(strstr(FRAME.used, ";X;"))    this->x_base=FRAME.x ;
     if(strstr(FRAME.used, ";Y;"))    this->y_base=FRAME.y ;
     if(strstr(FRAME.used, ";Z;"))    this->z_base=FRAME.z ;

     if(strstr(FRAME.used, ";A;")) {  this->a_azim=FRAME.a ;  v_flag=1 ;  }
     if(strstr(FRAME.used, ";E;")) {  this->a_elev=FRAME.e ;  v_flag=1 ;  }
     if(strstr(FRAME.used, ";R;"))    this->a_roll=FRAME.r ;
/*- - - - - - - - - - - - - - - - - -  ��������� ��������� ��������� */
     if(strstr(FRAME.used, ";DA;")) {
                        iReplaceText(this->p_controls.used, ";DA;", ";", 1) ;
                        iReplaceText(this->p_controls.used, ";TA;", ";", 1) ;

                                     this->p_controls.d_a=FRAME.d_a ;
         if(FRAME.d_a!=0.)    strcat(this->p_controls.used, "DA;") ;
                                    }
     if(strstr(FRAME.used, ";DE;")) {
                        iReplaceText(this->p_controls.used, ";DE;", ";", 1) ;
                        iReplaceText(this->p_controls.used, ";TE;", ";", 1) ;

                                     this->p_controls.d_e=FRAME.d_e ;
         if(FRAME.d_e!=0.)    strcat(this->p_controls.used, "DE;") ;
                                    }
     if(strstr(FRAME.used, ";DR;")) {
                        iReplaceText(this->p_controls.used, ";DR;", ";", 1) ;
                        iReplaceText(this->p_controls.used, ";TR;", ";", 1) ;

                                     this->p_controls.d_r=FRAME.d_r ;
         if(FRAME.d_r!=0.)    strcat(this->p_controls.used, "DR;") ;
                                    }

     if(strstr(FRAME.used, ";TA;")) {
                                       this->p_controls.t_a=FRAME.t_a ;
                                strcat(this->p_controls.used, "TA;") ;
                                    }
     if(strstr(FRAME.used, ";TE;")) {
                                       this->p_controls.t_e=FRAME.t_e ;
                                strcat(this->p_controls.used, "TE;") ;
                                    }
     if(strstr(FRAME.used, ";TR;")) {
                                       this->p_controls.t_r=FRAME.t_r ;
                                strcat(this->p_controls.used, "TR;") ;
                                    }
/*- - - - - - - - - - - - - - - - - - - ��������� ���������� ������� */
     if(strstr(FRAME.used, ";G;"))    this->g_ctrl=FRAME.g ;

     if(strstr(FRAME.used, ";DG;")) {
                        iReplaceText(this->p_controls.used, ";DG;", ";", 1) ;
                        iReplaceText(this->p_controls.used, ";TG;", ";", 1) ;

                                     this->p_controls.d_g=FRAME.d_g ;
         if(FRAME.d_g!=0.)    strcat(this->p_controls.used, "DG;") ;
                                    }

     if(strstr(FRAME.used, ";TG;")) {
                                       this->p_controls.t_g=FRAME.t_g ;
                                strcat(this->p_controls.used, "TG;") ;
                                    }
/*- - - - - - - - - - - - - - - - - - - - - - - - ��������� �������� */
     if(strstr(FRAME.used, ";V;")) {  this->v_abs=FRAME.v ;  v_flag=1 ;  }

     if(strstr(FRAME.used, ";DV;")) {
                        iReplaceText(this->p_controls.used, ";DV;", ";", 1) ;
                        iReplaceText(this->p_controls.used, ";TV;", ";", 1) ;

                                     this->p_controls.d_v=FRAME.d_v ;
           if(FRAME.d_v!=0.)  strcat(this->p_controls.used, "DV;") ;
                                    }

     if(strstr(FRAME.used, ";TV;")) {
                                       this->p_controls.t_v=FRAME.t_v ;
                                strcat(this->p_controls.used, "TV;") ;
                                    }
/*- - - - - - - - - - - - - - - - - - - - ��������� ������ ��������� */
                 p_frame++ ;
                                                }
/*------------------------------------ ���������� ���������� ������� */

         memset(used,   0,                   sizeof(used)  ) ;
        strncpy(used, this->p_controls.used, sizeof(used)-1) ;

     if(strstr(this->p_controls.used, ";DA;"))  a_new=this->a_azim+this->p_controls.d_a*(t2-t1) ;
     if(strstr(this->p_controls.used, ";DE;"))  e_new=this->a_elev+this->p_controls.d_e*(t2-t1) ;
     if(strstr(this->p_controls.used, ";DR;"))  r_new=this->a_roll+this->p_controls.d_r*(t2-t1) ;
     if(strstr(this->p_controls.used, ";DG;"))  g_new=this->g_ctrl+this->p_controls.d_g*(t2-t1) ;
     if(strstr(this->p_controls.used, ";DV;"))  v_new=this->v_abs +this->p_controls.d_v*(t2-t1) ;

/*----------------------------- �������� ��������� ������� ��������� */

     if(strstr(this->p_controls.used, ";TA;")) {

       if(a_new>this->a_azim)  status=iAngleInCheck(this->p_controls.t_a, this->a_azim, a_new) ;
       else                    status=iAngleInCheck(this->p_controls.t_a, a_new, this->a_azim) ;

       if(!status) {
                            a_new=this->p_controls.t_a ;
                     iReplaceText(this->p_controls.used, ";DA;", ";", 1) ;
                     iReplaceText(this->p_controls.used, ";TA;", ";", 1) ;
                   }
                                               }

     if(strstr(this->p_controls.used, ";TE;")) {

       if(e_new>this->a_elev)  status=iAngleInCheck(this->p_controls.t_e, this->a_elev, e_new) ;
       else                    status=iAngleInCheck(this->p_controls.t_e, e_new, this->a_elev) ;

       if(!status) {
                            e_new=this->p_controls.t_e ;
                     iReplaceText(this->p_controls.used, ";DE;", ";", 1) ;
                     iReplaceText(this->p_controls.used, ";TE;", ";", 1) ;
                   }
                                               }

     if(strstr(this->p_controls.used, ";TR;")) {

       if(r_new>this->a_roll)  status=iAngleInCheck(this->p_controls.t_r, this->a_roll, r_new) ;
       else                    status=iAngleInCheck(this->p_controls.t_r, r_new, this->a_roll) ;

       if(!status) {
                            e_new=this->p_controls.t_r ;
                     iReplaceText(this->p_controls.used, ";DR;", ";", 1) ;
                     iReplaceText(this->p_controls.used, ";TR;", ";", 1) ;
                   }
                                               }

     if(strstr(this->p_controls.used, ";TG;")) {

       if(g_new>this->g_ctrl)  status=iAngleInCheck(this->p_controls.t_g, this->g_ctrl, g_new) ;
       else                    status=iAngleInCheck(this->p_controls.t_g, g_new, this->g_ctrl) ;

       if(!status) {
                            g_new=this->p_controls.t_g ;
                     iReplaceText(this->p_controls.used, ";DG;", ";", 1) ;
                     iReplaceText(this->p_controls.used, ";TG;", ";", 1) ;
                   }
                                               }

     if(strstr(this->p_controls.used, ";TV;")) {

       if(v_new>this->v_abs)  status=iAngleInCheck(this->p_controls.t_v, this->v_abs, v_new) ;
       else                   status=iAngleInCheck(this->p_controls.t_v, v_new, this->v_abs) ;

       if(!status) {
                            v_new=this->p_controls.t_v ;
                     iReplaceText(this->p_controls.used, ";DV;", ";", 1) ;
                     iReplaceText(this->p_controls.used, ";TV;", ";", 1) ;
                   }
                                               }
/*------------------------------------ ���������� ���������� ������� */

     if(strstr(used, ";DA;")) {  this->a_azim=a_new ;  v_flag=1 ;  }
     if(strstr(used, ";DE;")) {  this->a_elev=e_new ;  v_flag=1 ;  }
     if(strstr(used, ";DR;"))    this->a_roll=r_new ;
     if(strstr(used, ";DG;"))    this->g_ctrl=g_new ;
     if(strstr(used, ";DV;")) {  this->v_abs =v_new ;  v_flag=1 ;  }

/*------------------------------------- ���������� �������� �������� */

     if(v_flag) {
                   Velo_Matrix.LoadZero   (3, 1) ;
                   Velo_Matrix.SetCell    (2, 0, this->v_abs) ;
                    Sum_Matrix.Load3d_azim(-this->a_azim) ;
                   Oper_Matrix.Load3d_elev(-this->a_elev) ;
                    Sum_Matrix.LoadMul    (&Sum_Matrix, &Oper_Matrix) ;
                   Velo_Matrix.LoadMul    (&Sum_Matrix, &Velo_Matrix) ;

                     x_velocity=Velo_Matrix.GetCell(0, 0) ;
                     y_velocity=Velo_Matrix.GetCell(1, 0) ;
                     z_velocity=Velo_Matrix.GetCell(2, 0) ;
                }
/*-------------------------------------------------------------------*/

#undef   FRAME

  return(0) ;
}


/*********************************************************************/
/*								     */
/*                   ���������� ����� ����������                     */

  int  RSS_Object_Flyer::iSaveTracePoint(char *action)

{
/*------------------------------------------------- ����� ���������� */

   if(!stricmp(action, "CLEAR")) {
                                     mTrace_cnt=0 ;
                                       return(0) ;
                                 }
/*----------------------------------------------- ����������� ������ */

   if(mTrace_cnt==mTrace_max) {

          mTrace_max+= 1000 ;
          mTrace     =(RSS_Object_FlyerTrace *)
                            realloc(mTrace, mTrace_max*sizeof(RSS_Object_FlyerTrace)) ;

       if(mTrace==NULL) {
                   SEND_ERROR("FLYER.iSaveTracePoint@"
                              "Memory over for trajectory") ;
                                  return(-1) ;
                        }
                              }
/*------------------------------------------------- ���������� ����� */
                  
                  mTrace[mTrace_cnt].x_base    =this->x_base ;
                  mTrace[mTrace_cnt].y_base    =this->y_base ;
                  mTrace[mTrace_cnt].z_base    =this->z_base ;
                  mTrace[mTrace_cnt].a_azim    =this->a_azim ;
                  mTrace[mTrace_cnt].a_elev    =this->a_elev ;
                  mTrace[mTrace_cnt].a_roll    =this->a_roll ;
                  mTrace[mTrace_cnt].x_velocity=this->x_velocity ;
                  mTrace[mTrace_cnt].y_velocity=this->y_velocity ;
                  mTrace[mTrace_cnt].z_velocity=this->z_velocity ;
                         mTrace_cnt++ ;

/*-------------------------------------------------------------------*/

   return(0) ;
}


/********************************************************************/
/*								    */
/*           ����������� ���������� � ��������� ���������           */

  void  RSS_Object_Flyer::iShowTrace_(void)

{
    strcpy(Context->action, "SHOW_TRACE") ;

  SendMessage(RSS_Kernel::kernel_wnd, 
                WM_USER, (WPARAM)_USER_CHANGE_CONTEXT, 
                         (LPARAM) this->Context       ) ;
}


/*********************************************************************/
/*								     */
/*                     ����������� ����������                        */

  void  RSS_Object_Flyer::iShowTrace(void)

{
   int  status ;
   int  i ;

/*-------------------------------- �������������� ����������� ������ */

     if(mTrace_dlist==0) {
           mTrace_dlist=RSS_Kernel::display.GetList(2) ;
                         }

     if(mTrace_dlist==0)  return ;

/*----------------------------------- ������� ���������� ����������� */

          status=RSS_Kernel::display.SetFirstContext("Show") ;
    while(status==0) {                                              /* CIRCLE.1 */

/*---------------------------------- ��������� ��������� ����������� */

             glNewList(mTrace_dlist, GL_COMPILE) ;                  /* �������� ������ */
          glMatrixMode(GL_MODELVIEW) ;

/*--------------------------------------------- ��������� ���������� */

             glColor4d(GetRValue(mTrace_color)/256., 
                       GetGValue(mTrace_color)/256.,
                       GetBValue(mTrace_color)/256., 1.) ;

               glBegin(GL_LINE_STRIP) ;

       for(i=0 ; i<mTrace_cnt ; i++)
            glVertex3d(mTrace[i].x_base, mTrace[i].y_base, mTrace[i].z_base) ;

                  glEnd();

/*----------------------------- �������������� ��������� ����������� */

             glEndList() ;                                          /* �������� ������ */

/*----------------------------------- ������� ���������� ����������� */

          status=RSS_Kernel::display.SetNextContext("Show") ;
                     }                                              /* CONTINUE.1 */
/*-------------------------------------------------------------------*/

}


/*********************************************************************/
/*								     */
/*	      ���������� ������ "������� ���������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       ����������� ������ "������� ���������"      	     */

     RSS_Transit_Flyer::RSS_Transit_Flyer(void)

{
}


/*********************************************************************/
/*								     */
/*	        ���������� ������ "������� ���������"      	     */

    RSS_Transit_Flyer::~RSS_Transit_Flyer(void)

{
}


/********************************************************************/
/*								    */
/*	              ���������� ��������                           */

    int  RSS_Transit_Flyer::vExecute(void)

{
   if(!stricmp(action, "SHOW_TRACE"))  ((RSS_Object_Flyer *)object)->iShowTrace() ;

   return(0) ;
}


/*********************************************************************/
/*								     */
/*	      ���������� ������ "��������� ����������"	             */
/*								     */
/*********************************************************************/

/*********************************************************************/
/*								     */
/*	       ����������� ������ "��������� ����������"      	     */

     RSS_Object_FlyerProgram::RSS_Object_FlyerProgram(void)

{
    memset(name, 0, sizeof(name)) ;
           frames_cnt=0 ;
}


/*********************************************************************/
/*								     */
/*	        ���������� ������ "��������� ����������"      	     */

    RSS_Object_FlyerProgram::~RSS_Object_FlyerProgram(void)

{
}


