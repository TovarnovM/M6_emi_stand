/*********************************************************************/
/*                                                                   */
/*               ������� ������������ ����������                     */
/*                                                                   */
/*********************************************************************/

#include <windows.h>

#include "UserDlg.h"

#pragma warning(disable : 4996)

/*********************************************************************/
/*                                                                   */
/*                 ���� ����������� ��������                         */

 LRESULT CALLBACK  UD_show_panel_prc(  HWND  hWnd,     UINT  Msg,
 			             WPARAM  wParam, LPARAM  lParam)
{
            HDC  hDC ;
    PAINTSTRUCT  PaintCfg ;
     TEXTMETRIC  TextMetric ;
         HBRUSH  BkBrush ;
           RECT  Rect ;
  UD_show_panel *data ;                /* �������� �������� */
           char  data_ptr[32] ;        /* ����� ��������, ������������ */
           char *beg ;
           char *end ;
            int  i ;

/*--------------------------------------------------- ����� �������� */

  switch(Msg) {

/*----------------------------------------------- �������� ��������� */

    case WM_CREATE:   break ;

    case WM_COMMAND:  break ;

/*-------------------------------------------------------- ��������� */

    case WM_PAINT:   {
/*- - - - - - - - - - - - - - - - - - - - - - - -  ���������� ������ */
     InvalidateRect(hWnd, NULL, false) ;
        SendMessage(hWnd, WM_GETTEXT, (WPARAM)sizeof(data_ptr),
                                      (LPARAM)       data_ptr  ) ;

           data=(UD_show_panel *)UD_ptr_decode(data_ptr) ;
        if(data==NULL) {
                          ValidateRect(hWnd,  NULL) ;
                              break ;
                       }
/*- - - - - - - - - - - - - - - - - - - - - - -  ����� ������ � ���� */
        if(data->text==NULL) {   
                                ValidateRect(hWnd,  NULL) ;
                                    break ;
                             }

             hDC=BeginPaint(hWnd, &PaintCfg) ;

             GetTextMetrics(hDC, &TextMetric) ;
               SetTextColor(hDC, data->fore_color) ;
                 SetBkColor(hDC, data->back_color) ;

                        GetWindowRect(hWnd, &Rect) ;
             BkBrush=CreateSolidBrush(data->back_color) ;
                                            Rect.right -=Rect.left ;
                                            Rect.bottom-=Rect.top ;
                                            Rect.left   =  0 ;
                                            Rect.top    =  0 ;
                             FillRect(hDC, &Rect, BkBrush) ;
                         DeleteObject(BkBrush) ;

        for(beg=data->text, i=0 ; ; beg=end+1, i++) {

              end=strchr(beg, '\n') ;
           if(end!=NULL)  *end=0 ;

                    TextOut(hDC, 0, i*TextMetric.tmHeight,
                                           beg, strlen(beg)) ;

           if(end!=NULL)  *end='\n' ;
           if(end==NULL)     break ;
                                                    }

               ValidateRect(hWnd,  NULL) ;
                   EndPaint(hWnd, &PaintCfg) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			     break ;
		     }
/*------------------------------------------------- ������ ��������� */

    default :        {
		return( DefWindowProc(hWnd, Msg, wParam, lParam) ) ;
			    break ;
		     }
/*-------------------------------------------------------------------*/
	      }
/*-------------------------------------------------------------------*/

    return(0) ;
}

