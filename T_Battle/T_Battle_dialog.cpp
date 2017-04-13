/********************************************************************/
/*								    */
/*		������ ������ "��������� ���"    		    */
/*								    */
/*                   ���������� ���������                           */
/*                                                                  */
/********************************************************************/

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <math.h>

#include "d:\_Projects\_Libraries\controls.h"

#include "..\RSS_Feature\RSS_Feature.h"
#include "..\RSS_Object\RSS_Object.h"
#include "..\RSS_Kernel\RSS_Kernel.h"

#include "T_Battle.h"

#pragma warning(disable : 4996)


#define  SEND_ERROR(text)    SendMessage(RSS_Kernel::kernel_wnd, WM_USER,  \
                                         (WPARAM)_USER_ERROR_MESSAGE,      \
                                         (LPARAM) text) ;

#define  CREATE_DIALOG  CreateDialogIndirectParam

#include "resource.h"

/*--------------------------------------------------- ������� ������ */

#define    _FRAME_FORMAT  "% 6.2lf(% 5.2lf) %-10.10s %s"
#define   _OBJECT_FORMAT  "%s"

/*------------------------------------ ����������� ��������� ������� */

  union WndProc_par {
                        long            par ;
                     LRESULT (CALLBACK *call)(HWND, UINT, WPARAM, LPARAM) ; 
                    } ;

  static union WndProc_par  Scenario_WndProc ;
  static union WndProc_par  Tmp_WndProc ;

   LRESULT CALLBACK  Task_Battle_Scenario_WndProc(HWND, UINT, WPARAM, LPARAM) ;

/*---------------------------------------------------- ��������� �/� */


/*********************************************************************/
/*								     */
/* 	     ���������� ��������� ����������� ���� HELP	             */

    BOOL CALLBACK  Task_Battle_Help_dialog(  HWND hDlg,     UINT Msg, 
 		  	                   WPARAM wParam, LPARAM lParam) 
{
   RSS_Module_Battle  Module ;
                 int  elm ;         /* ������������� �������� ������� */
                 int  status ;
                 int  index ;
                 int  insert_flag ;
                char *help ;
                char  text[512] ;
                char *end ;
                 int  i ;

/*------------------------------------------------- ������� �������� */

  switch(Msg) {

/*---------------------------------------------------- ������������� */

    case WM_INITDIALOG: {
/*- - - - - - - - - - - - - - - - - - - - -  ������������� ��������� */
   for(i=0 ; Module.mInstrList[i].name_full!=NULL ; i++) {

     if(Module.mInstrList[i].help_row==NULL)  continue ;

         index=LB_ADD_ROW (IDC_HELP_LIST, Module.mInstrList[i].help_row) ;
               LB_SET_ITEM(IDC_HELP_LIST, index, Module.mInstrList[i].help_full) ;
                                                         }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}

/*------------------------------------------------ ��������� ������� */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - - -  ��������� ������� */
       if(elm==IDC_HELP_LIST) {

        if(status==LBN_DBLCLK) {

             index=        LB_GET_ROW (elm) ;
              help=(char *)LB_GET_ITEM(elm) ;
                           LB_GET_TEXT(elm, index, text) ;
           if(text[0]!='#' || 
              help   ==NULL  )  return(FALSE) ;

               status=LB_GET_TEXT(elm, index+1, text) ;
           if( status==LB_ERR ||
              text[0]==  '#'    )  insert_flag=1 ;
           else                    insert_flag=0 ;

         do {
                   memset(text, 0, sizeof(text)) ;
                  strncpy(text, help, sizeof(text)-1) ;
               end=strchr(text, '\n') ;
           if( end!=NULL)  *end=0 ;

           if(insert_flag) {  LB_INS_ROW(elm, index+1, text) ;
                                              index++ ;        }
              help=strchr(help, '\n') ;
           if(help==NULL)  break ;
              help++ ;
              
            } while(1) ;
                               }

			            return(FALSE) ;
                              }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- �������� */

    case WM_CLOSE:      {
                            EndDialog(hDlg, 0) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- ������ */

    default :        {
			  return(FALSE) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

    return(TRUE) ;
}


/*********************************************************************/
/*								     */
/* 	     ���������� ��������� ����������� ���� EDIT	             */

    BOOL CALLBACK  Task_Battle_View_dialog(  HWND hDlg,     UINT Msg, 
 		  	                   WPARAM wParam, LPARAM lParam) 
{
   static RSS_Module_Battle *Module ;
              static  HFONT  font ;        /* ����� */
                        int  elm ;         /* ������������� �������� ������� */
                        int  status ;
                        int  index ;
                       char  text[512] ;
                        int  i ;

/*------------------------------------------------- ������� �������� */

  switch(Msg) {

/*---------------------------------------------------- ������������� */

    case WM_INITDIALOG: {
                             Module=(RSS_Module_Battle *)lParam ;
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������� ������� */
        if(font==NULL)
           font=CreateFont(14, 0, 0, 0, FW_THIN, 
                                 false, false, false,
                                  ANSI_CHARSET,
                                   OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY,
                                      VARIABLE_PITCH,
                                       "Courier New Cyr") ;
         SendMessage(ITEM(IDC_SCENARIO), WM_SETFONT, (WPARAM)font, 0) ;
         SendMessage(ITEM(IDC_OBJECTS),  WM_SETFONT, (WPARAM)font, 0) ;
/*- - - - - - - - - - - - - - - - - - - - - ������ "������" �������� */
                       LB_CLEAR(IDC_SCENARIO) ;

        for(i=0 ; i<Module->mScenario_cnt ; i++) {

                  sprintf(text, _FRAME_FORMAT, Module->mScenario[i].t, 0.,
                                               Module->mScenario[i].action,
                                               Module->mScenario[i].command ) ;
               LB_ADD_ROW(IDC_SCENARIO, text) ;
                                                 }
/*- - - - - - - - - - - - - - - - - - - - - - ������ ������ �������� */
                       LB_CLEAR(IDC_OBJECTS) ;

        for(i=0 ; i<Module->mObjects_cnt ; i++) {

                  sprintf(text, _OBJECT_FORMAT, Module->mObjects[i].object->Name) ;
               LB_ADD_ROW(IDC_OBJECTS, text) ;
                                                }
/*- - - - - - - - - - - - - - - - -  �������� ������������ ��������� */
           Scenario_WndProc.par =GetWindowLong(ITEM(IDC_SCENARIO), GWL_WNDPROC) ;
                Tmp_WndProc.call=Task_Battle_Scenario_WndProc ;
          SetWindowLong(ITEM(IDC_SCENARIO), GWL_WNDPROC, Tmp_WndProc.par) ;
/*- - - - - - - - - - - - - - - - - - - - -  ������������� ��������� */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}
/*------------------------------------------------ ��������� ������� */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - ��������� ������ */
       if(   elm==IDC_SCENARIO &&
          status==LBN_DBLCLK     ) {

                     index=LB_GET_ROW (elm) ;
                           LB_GET_TEXT(elm, index, text) ;

			              return(FALSE) ;
                                   }
/*- - - - - - - - - - - - - - - - - - - - DEBUG: "����������" ������ */
       if(elm==IDC_STEP) {

            index=LB_GET_ROW(IDC_SCENARIO) ;                        /* ���.������ "�����" �������� */
         if(index==LB_ERR              ) {
              SEND_ERROR("������ BATTLE: �������� ����������� ����") ;
                                            return(FALSE) ;
                                         }

                      LB_GET_TEXT(IDC_SCENARIO, index, text) ;

         if(text[0]==0) {
              SEND_ERROR("������ BATTLE: �������� ��������� ��������") ;
                                            return(FALSE) ;
                        }

			              return(FALSE) ;
                         }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- �������� */

    case WM_CLOSE:      {
                            EndDialog(hDlg, 0) ;
  			       return(FALSE) ;
			              break ;
			}
/*----------------------------------------------------------- ������ */

    default :        {
			  return(FALSE) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

    return(TRUE) ;
}


/*********************************************************************/
/*								     */
/*        ���������� ��������� ����������� ���� VARIABLES            */

    BOOL CALLBACK  Task_Battle_Vars_dialog(  HWND hDlg,     UINT Msg, 
 		  	                   WPARAM wParam, LPARAM lParam) 
{
   RSS_Module_Battle  Module ;
       static  HFONT  font ;        /* ����� */
                 int  elm ;         /* ������������� �������� ������� */
                 int  status ;
                char  record[1024] ;
                char  iface_signal[1024] ;
                char  iface_data[1024] ;
                char  tmp[1024] ;
                 int  i ;

#define  VARS        Module.mVariables
#define  VARS_CNT    Module.mVariables_cnt

/*------------------------------------------------- ������� �������� */

  switch(Msg) {

/*---------------------------------------------------- ������������� */

    case WM_INITDIALOG: {
/*- - - - - - - - - - - - - - - - - - - - - - - - -  ������� ������� */
        if(font==NULL)
           font=CreateFont(14, 0, 0, 0, FW_THIN, 
                                 false, false, false,
                                  ANSI_CHARSET,
                                   OUT_DEFAULT_PRECIS,
                                    CLIP_DEFAULT_PRECIS,
                                     DEFAULT_QUALITY,
                                      VARIABLE_PITCH,
                                       "Courier New Cyr") ;
         SendMessage(ITEM(IDC_VARS_LIST), WM_SETFONT, (WPARAM)font, 0) ;
         SendMessage(ITEM(IDC_EXEC_LIST), WM_SETFONT, (WPARAM)font, 0) ;
/*- - - - - - - - - - - - - - - - -  ������������� ������ ���������� */
                      LB_CLEAR(IDC_VARS_LIST) ;

         for(i=0 ; i<VARS_CNT ; i++) {
                       sprintf(tmp, "%10.10s = %s", VARS[i].name, 
                                                    VARS[i].value) ;
                    LB_ADD_ROW(IDC_VARS_LIST, tmp) ;
                                     }
/*- - - - - - - - - - - - - - - - - ������������� ������ ����������� */
/*
            Module.mExecIFace.vDecode(iface_signal, iface_data) ;

          LB_CLEAR(IDC_EXEC_LIST) ;
           sprintf(tmp, "%10.10s = %s:%s",  "@Buff",  iface_signal, iface_data) ;
        LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
           sprintf(tmp, "%10.10s = %.10lf", "@Value", Module.mExecValue) ;
        LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
           sprintf(tmp, "%10.10s = %d",     "@Error", Module.mExecError) ;
        LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
           sprintf(tmp, "%10.10s = %d",     "@Fail",  Module.mExecFail) ;
        LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  			  return(FALSE) ;
  			     break ;
  			}
/*------------------------------------ ��������� ���������� �������� */

    case WM_USER:  {
/*- - - - - - - - - - - - - - - - - - ���������� ������ � ���������� */
        if(wParam==_USER_SHOW) {

         for(i=0 ; i<VARS_CNT ; i++) {                              /* ���������� ������ ���������� */
                       sprintf(tmp, "%10.10s = %s", VARS[i].name, 
                                                    VARS[i].value) ;

                        memset(record, 0, sizeof(record)) ;
                   LB_GET_TEXT(IDC_VARS_LIST, i, record) ;        

               if(       record[0]==0) { 
                                          LB_ADD_ROW(IDC_VARS_LIST, tmp) ;
                                       } 
          else if(strcmp(record, tmp)) {
                                          LB_DEL_ROW(IDC_VARS_LIST, i) ;
                                          LB_INS_ROW(IDC_VARS_LIST, i, tmp) ;
                                       } 
                                     }

//          Module.mExecIFace.vDecode(iface_signal, iface_data) ;   /* ���������� ������ ���������� ���������� ������ */

              sprintf(tmp, "%10.10s = %s:%s",  "@Buff",  iface_signal, iface_data) ;
               memset(record, 0, sizeof(record)) ;
          LB_GET_TEXT(IDC_EXEC_LIST, 0, record) ;        

        if(!strcmp(record, tmp))   return(FALSE) ;                  /* ���� ������ �� ����������... */

/*
              LB_CLEAR(IDC_EXEC_LIST) ;
               sprintf(tmp, "%10.10s = %s:%s",  "@Buff",  iface_signal, iface_data) ;
            LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
               sprintf(tmp, "%10.10s = %.10lf", "@Value", Module.mExecValue) ;
            LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
               sprintf(tmp, "%10.10s = %d",     "@Error", Module.mExecError) ;
            LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
               sprintf(tmp, "%10.10s = %d",     "@Fail",  Module.mExecFail) ;
            LB_ADD_ROW(IDC_EXEC_LIST, tmp) ;
*/
                                       return(FALSE) ;
                               }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
  			     break ;
  		   }
/*------------------------------------------------ ��������� ������� */

    case WM_COMMAND:    {

	status=HIWORD(wParam) ;
	   elm=LOWORD(wParam) ;
/*- - - - - - - - - - - - - - - - - - - - �������� ������ ���������� */
       if(elm==IDC_VARS_CLEAR) {
                                               VARS_CNT=0 ;

                                  LB_CLEAR(IDC_VARS_LIST) ;

			              return(FALSE) ;
                               }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			  return(FALSE) ;
			     break ;
			}
/*--------------------------------------------------------- �������� */

    case WM_CLOSE:      {
                            Module.mVariables_Window=NULL ;
                                EndDialog(hDlg, 0) ;
                                    return(FALSE) ;
		                           break ;
			}
/*----------------------------------------------------------- ������ */

    default :        {
			  return(FALSE) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

#undef  VAR
#undef  VAR_CNT

    return(TRUE) ;
}


/*********************************************************************/
/*								     */
/*	          ����e ����������� ��������� ListBox                */    

  LRESULT CALLBACK  Task_Battle_Scenario_WndProc(  HWND  hWnd,     UINT  Msg,
	                                         WPARAM  wParam, LPARAM  lParam) 
{
    HMENU  hMenu ;
  LRESULT  result ;
      int  menu_flag ;        /* ���� ����������� ������������ ���� */
     RECT  Rect ;
      int  x ;
      int  y ;

/*------------------------------------------------------- ���������� */

/*----------------------------------- ������� ������ ������ �� ����� */

                              menu_flag= 0 ;

   if(Msg==WM_RBUTTONDOWN)          Msg=WM_LBUTTONDOWN ;
   if(Msg==WM_RBUTTONUP  ) {        Msg=WM_LBUTTONUP ;
                              menu_flag= 1 ;            }
                          
/*-------------------------------- ����� ������� ��������� ��������� */

      result=CallWindowProc(Scenario_WndProc.call,
                                        hWnd, Msg, wParam, lParam) ;
   if(!menu_flag)  return(result) ;

/*------------------------------------- ��������� ���� ������ ������ */

   if(menu_flag) {
                    GetWindowRect(hWnd, &Rect) ;

                         x=LOWORD(lParam) ;
                         y=HIWORD(lParam) ;

                     hMenu=CreatePopupMenu() ;
          AppendMenu(hMenu, MF_STRING, IDC_INSERT_ROW, "�������� ������") ;
          AppendMenu(hMenu, MF_STRING, IDC_DELETE_ROW, "������� ������") ;
          AppendMenu(hMenu, MF_STRING, IDC_SET_COMMENT, "����������������/�����������������") ;

      TrackPopupMenu(hMenu, 0, Rect.left+x, Rect.top+y+4,
                                      0, GetParent(hWnd), NULL) ;

                     DestroyMenu(hMenu) ;
                 }
/*-------------------------------------------------------------------*/

  return(result) ;
}
