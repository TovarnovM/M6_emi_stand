/*********************************************************************/
/*                                                                   */
/*               ������� ������������ ����������                     */
/*                                                                   */
/*********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <search.h>
#include <windows.h>

#include "UserDlg.h"


   void  UDi_grid_2D_init      (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_scale     (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_sizing    (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_clear     (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_frame     (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_rare      (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_ceil      (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_obstacle  (HDC, UD_area_data *, UD_metrics *) ;
   void  UDi_grid_2D_grid      (HDC, UD_area_data *, UD_metrics *) ;

   void  UDi_grid_2D_arrow     (HDC, double, double, double, double) ;
    int  UDi_grid_2D_links     (UD_area_2Dpoint *, int, int, int *) ;
    int  UDi_grid_2D_spot      (UD_area_2Dpoint *, int, int, int *) ;
   void  UDi_grid_2D_p_area    (UD_data_2Dpoint *, int *, int, int, UD_point_w *) ;
    int  UDi_grid_2D_graphloops(int, UD_area_2Dpoint *, int *) ;

   void  UDi_g2Dscan_position  (UD_metrics *, int, int *, int *, double *, double *) ;


/*********************************************************************/
/*                                                                   */
/*                 ���� 2-������ �����                               */

 LRESULT CALLBACK  UD_grid_2D_prc(  HWND  hWnd,     UINT  Msg,
 			          WPARAM  wParam, LPARAM  lParam)
{
             HDC  hDC_wnd ;
             HDC  hDC_tmp ;
             HDC *hDC ;
         HBITMAP *hBM ;
     PAINTSTRUCT  PaintCfg ;
            RECT  Rect ;
      UD_metrics *metrics ;
             int  zoom_flag ;           /* ���� ����������� ���� */
             int  zoom_num ;            /* ����� ����������� ���� */
    UD_area_data *data ;                /* �������� �������� */
            char  data_ptr[32] ;        /* ����� ��������, ������������ */     
             int  scanner_oper ;        /* �������� ������ �� �������� */ 
      static int  scanner_flag ;        /* ���� ������ �� �������� */ 
      static int  x_scanner ;           /* ��������� ������� */ 
      static int  y_scanner ; 
      static int  zone_flag ;           /* ���� ������ �� ����-����� */ 
      static int  x_zone_beg ;          /* ���������� ����-���� */ 
      static int  y_zone_beg ;
      static int  x_zone_end ;
      static int  y_zone_end ;
      static int  smooth_zoom_flag ;    /* ���� ������ � ������� ZOOM-�� */ 
             int  x_mouse ;             /* ���������� ���� ��� �������� */ 
             int  y_mouse ;
      static int  x_mouse_prv ;         /* ���������� ��������� ���� ��� �������� */ 
      static int  y_mouse_prv ;
          double  x_zoom_min ;          /* ���������� ������ �� �������� ���� */ 
          double  y_zoom_min ;
          double  x_zoom_max ;
          double  y_zoom_max ;
          double  x_zoom_size ;
          double  y_zoom_size ;
          double  delta ;
          double  base ;
          double  base_n ;
          double  zoom ;
          double  shift ;
             int  tmp ;
             int  i ;

/*------------------------------------------------------- ���������� */

                                  data=NULL ; 

   if(Msg==WM_PAINT       ||
      Msg==WM_LBUTTONUP   ||
      Msg==WM_LBUTTONDOWN ||
      Msg==WM_RBUTTONUP   ||
      Msg==WM_RBUTTONDOWN ||
      Msg==WM_MOUSEMOVE     ) {

        SendMessage(hWnd, WM_GETTEXT, (WPARAM)sizeof(data_ptr),
                                      (LPARAM)       data_ptr  ) ;

           data=(UD_area_data *)UD_ptr_decode(data_ptr) ;

                              }
/*--------------- ��������� ������������ ������������� ������ ������ */

     if(data             !=NULL && 
        data->parent_data==NULL   )  data=NULL ;

/*---------------------------------------------- ����������� ������� */

                                              zoom_flag= 0 ;
                                              zoom_num =-1 ;  
     if(data!=NULL) {

         for(i=0 ; i<data->zooms_cnt ; i++)                         /* ���������, ��� �� ���� ����� ������ */
                if(hWnd==data->zooms[i].hWnd) {  zoom_num =i ;      /*   ���������� �����������            */
                                                 zoom_flag=1 ;   }

         if(zoom_flag)  metrics=&data->zooms[zoom_num].metrics ;
         else           metrics=&data->metrics ;

                        metrics->zoom_flag=zoom_flag ;
                        metrics->zoom_num =zoom_num ;
                    }
/*------------------------------------------ ��������� ����-�������� */

   if(data!=NULL) {
/*- - - - - - - - - - - - - "����������� �������"/"�������� �������" */
     if(data->spec_oper==_UD_SCANNER_MOVE ||
        data->spec_oper==_UD_SCANNER_CLEAR  ) {

                             InvalidateRect(hWnd, NULL, false) ;
                         hDC_wnd=BeginPaint(hWnd, &PaintCfg) ;

        if(zoom_flag) {  hDC=&data->zooms[zoom_num].hDC ;
                         hBM=&data->zooms[zoom_num].hBitMap ;  }
        else          {  hDC=&data->hDC ;
                         hBM=&data->hBitMap ;                  }

              BitBlt(hDC_wnd,                                       /* �������������� ���� */
                     metrics->x_base, metrics->y_base, 
                     metrics->x_size, metrics->y_size, 
                    *hDC, 
                     metrics->x_base, metrics->y_base, 
                     SRCCOPY                           ) ;

      if(data->spec_oper==_UD_SCANNER_MOVE) {

        UDi_g2Dscan_position(metrics, 0, &x_mouse, &y_mouse,        /* ������� �������� ������� */
                                           &data->x_scanner,
                                           &data->y_scanner ) ;
            UDi_cross_invert(hDC_wnd, metrics, x_mouse, y_mouse) ;  /* ��������� ������������ ������ */
                                            }             

                ValidateRect(hWnd, NULL) ;
                    EndPaint(hWnd, &PaintCfg) ;

                        return(0) ;
                                              }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
                  }
/*--------------------------------------------------- ����� �������� */

  switch(Msg) {

/*----------------------------------------------- �������� ��������� */

    case WM_CREATE:   break ;

    case WM_COMMAND:  break ;

/*-------------------------------------------------------- ��������� */

    case WM_PAINT:   {
/*- - - - - - - - - - - - - - - - - - - - - - - -  ���������� ������ */
                         InvalidateRect(hWnd, NULL, false) ;
        if(data==NULL) { 
                           ValidateRect(hWnd,  NULL) ;
                              break ;
                       }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - ����� ������ */
        if(data->points    ==NULL ||
           data->points_cnt==  0    ) {   
                                ValidateRect(hWnd,  NULL) ;
                                        break ;
                                      }

                         hDC_wnd=BeginPaint(hWnd, &PaintCfg) ;

        if(zoom_flag) {  hDC=&data->zooms[zoom_num].hDC ;
                         hBM=&data->zooms[zoom_num].hBitMap ;  }
        else          {  hDC=&data->hDC ;
                         hBM=&data->hBitMap ;                  }

       if(*hBM!=NULL)  DeleteObject(*hBM) ;
       if(*hDC!=NULL)  DeleteDC    (*hDC) ;

          *hDC=CreateCompatibleDC    ( hDC_wnd) ; 
          *hBM=CreateCompatibleBitmap( hDC_wnd, GetDeviceCaps(hDC_wnd, HORZRES), 
                                                GetDeviceCaps(hDC_wnd, VERTRES) ) ; 
                         SelectObject(*hDC, *hBM) ;

//                         data->hDC    =hDC ;
//                         data->hBitMap=hBM ;

               GetClientRect(hWnd, &Rect) ;
                     metrics->x_base=Rect.left ;
                     metrics->y_base=Rect.top ;
                     metrics->x_size=Rect.right -Rect.left ;
                     metrics->y_size=Rect.bottom-Rect.top ;
                        
                    SetBkColor(*hDC, data->back_color) ;

          UDi_grid_2D_init    (*hDC, data, metrics) ;               /* ��������� ��������� ���������� */
          UDi_grid_2D_sizing  (*hDC, data, metrics) ;               /* ������������� ��� ��������� � ���� ��������� */
          UDi_grid_2D_scale   (*hDC, data, metrics) ;               /* ������������ ���� ������ */
          UDi_grid_2D_clear   (*hDC, data, metrics) ;               /* ������� ���������������� ��� */
          UDi_grid_2D_frame   (*hDC, data, metrics) ;               /* ��������� ����� */

          UDi_grid_2D_rare    (*hDC, data, metrics) ;               /* ������������ ����� ������ */
          UDi_grid_2D_grid    (*hDC, data, metrics) ;               /* ��������� ���� � ��������� */
          UDi_grid_2D_ceil    (*hDC, data, metrics) ;               /* ��������� ��������� ����� */

            BitBlt(hDC_wnd, 
                   metrics->x_base, metrics->y_base, 
                   metrics->x_size, metrics->y_size, 
                  *hDC,
                   metrics->x_base, metrics->y_base, 
                   SRCCOPY                           ) ;

                 ValidateRect(hWnd,  NULL) ;
                     EndPaint(hWnd, &PaintCfg) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			     break ;
		     }
/*--------------------------------------------- ������� ����� ������ */

    case WM_LBUTTONDOWN: {
                            if(data==NULL)  break ; 
/*- - - - - - - - - - - - - - - - - - - -  ��������� ��������� ����� */
          if((wParam & MK_SHIFT  )==MK_SHIFT  ||
             (wParam & MK_CONTROL)==MK_CONTROL  ) {
         
                             x_mouse_prv=LOWORD(lParam) ; 
                             y_mouse_prv=HIWORD(lParam) ;
                                
               if(zoom_flag)  smooth_zoom_flag=1 ;
                                                  }
/*- - - - - - - - - - - - - - - - - - - ������ ���������������� ���� */
     else                  {

                                    x_zone_beg=LOWORD(lParam) ; 
                                    y_zone_beg=HIWORD(lParam) ; 
                                    x_zone_end= -1 ; 
                                    y_zone_end= -1 ; 

                                     zone_flag=1 ;

                                    SetCapture(hWnd) ;              /* ����������� ���� */
                           }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
     		                      break ;
                         }
/*--------------------------------- �������/���������� ������ ������ */

    case WM_RBUTTONUP:   {
                         }   
    case WM_RBUTTONDOWN: {
                              if(data==NULL)  break ;
/*- - - - - - - - - - - - - - - - - - - - - ������/��������� ������� */
               x_scanner=LOWORD(lParam) ;                           /* ����� ��������� ������� */
               y_scanner=HIWORD(lParam) ;          

            if(x_scanner< metrics->draw_x_base ||                   /* ��������� ������ �� ������� */
               x_scanner>=metrics->draw_x_base+
                          metrics->draw_x_size ||
               y_scanner< metrics->draw_y_base ||
               y_scanner>=metrics->draw_y_base+
                          metrics->draw_y_size   )  break ;

            if(Msg==WM_RBUTTONUP) {
                                         ReleaseCapture() ;         /* ������������� */
                                    scanner_flag= 0 ;
                                    scanner_oper=_UD_SCANNER_CLEAR ; 
                                  }
            else                  {
                                             SetCapture(hWnd) ;     /* ����������� ���� */
                                    scanner_flag= 1 ;
                                    scanner_oper=_UD_SCANNER_MOVE ; 
                                  }
/*- - - - - - - - - - - - - - - - - - - - ��������� ������-��������� */
             UDi_g2Dscan_position(metrics, 1, &x_scanner, &y_scanner, 
                                               &data->x_scanner, 
                                               &data->y_scanner ) ;

         if(data->ctrl_proc)
                  data->ctrl_proc(scanner_oper, zoom_num, data) ;   /*  �������� ��������� ��������� */   
/*- - - - - - - - - - - - - - - - - - - ��������� ���������� ������� */
            if(Msg==WM_RBUTTONUP) {
                                       x_scanner=-1 ;               /* ���������� ��������� ������� */
                                       y_scanner=-1 ;
                                  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
     		                      break ;
                         }
/*---------------------------------------------------- �������� ���� */

    case WM_MOUSEMOVE:   {

#define  ZOOM  data->zooms[zoom_num]

                             x_mouse=LOWORD(lParam) ; 
                             y_mouse=HIWORD(lParam) ; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ������ */
          if((wParam & MK_RBUTTON)==MK_RBUTTON) {

            if(data==NULL)  break ; 

            if(scanner_flag==0) {
                                    SetCapture(hWnd) ;
                                  x_scanner=-1 ; 
                                  y_scanner=-1 ;
                                }

            if(x_mouse< metrics->draw_x_base ||                   /* ��������� ������ �� ������� */
               x_mouse>=metrics->draw_x_base+
                        metrics->draw_x_size ||
               y_mouse< metrics->draw_y_base ||
               y_mouse>=metrics->draw_y_base+
                        metrics->draw_y_size   ) {  scanner_flag= 0 ;
                                                    scanner_oper=_UD_SCANNER_CLEAR ;  }
            else                                 {  scanner_flag= 1 ;
                                                    scanner_oper=_UD_SCANNER_MOVE ;  }

                if(scanner_flag==0) {
                                       ReleaseCapture() ;
                                          x_scanner=-1 ; 
                                          y_scanner=-1 ;
                                              break ;
                                    }

                                          x_scanner=x_mouse ; 
                                          y_scanner=y_mouse ;

               UDi_g2Dscan_position(metrics, 1, &x_scanner, &y_scanner,
                                                 &data->x_scanner, 
                                                 &data->y_scanner ) ;

              if(data->ctrl_proc)                                   /* �������� ��������� ��������� */   
                    data->ctrl_proc(scanner_oper, zoom_num, data) ;

                                    break ;
                                                }
/*- - - - - - - - - - - - - - - - - - - - - -  ���������������� ���� */
     else if(zone_flag) {
                           InvalidateRect(hWnd, NULL, false) ;
                       hDC_tmp=BeginPaint(hWnd, &PaintCfg) ;
              
        for(i=0 ; i<2 ; i++) {
                                Rect.left  =x_zone_beg ;
                                Rect.right =x_zone_end ;
                                Rect.top   =y_zone_beg ;
                                Rect.bottom=y_zone_end ;

                     UDi_zone_invert(hDC_tmp, metrics, &Rect) ;

                               x_zone_end=LOWORD(lParam) ; 
                               y_zone_end=HIWORD(lParam) ; 
                             }

                        ValidateRect(hWnd, NULL) ;
                            EndPaint(hWnd, &PaintCfg) ;

           if(x_zone_beg>metrics->draw_x_base   &&                  /* ���� ��� ������ � ���������������� */
              x_zone_beg<metrics->draw_x_base+                      /*  ����� �� ������� �� �������       */
                         metrics->draw_x_size-1 &&                  /*  ���� ������� - ������������       */
              x_zone_end>metrics->draw_x_base   &&                  /*  ���������� ����� ������           */ 
              x_zone_end<metrics->draw_x_base+
                         metrics->draw_x_size-1 &&
              y_zone_beg>metrics->draw_y_base   &&
              y_zone_beg<metrics->draw_y_base+
                         metrics->draw_y_size-1 &&
              y_zone_end>metrics->draw_y_base   &&
              y_zone_end<metrics->draw_y_base+
                          metrics->draw_y_size-1  )  break ;
                        }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - ZOOM - ����� */
     else  if( smooth_zoom_flag             && 
              (wParam & MK_SHIFT)==MK_SHIFT   ) {

                             delta=ZOOM.x_max-ZOOM.x_min ;
                             shift=delta*(x_mouse-x_mouse_prv)/     /* ������� ������ */
                                     metrics->draw_x_size    ;
                       ZOOM.x_min-=shift ;                          /* �������� ������ �� X */
                       ZOOM.x_max-=shift ;

                if(ZOOM.x_min<0.) {  ZOOM.x_min=  0. ;              /* �������� ������ �� X */
                                     ZOOM.x_max=delta ;  }
                if(ZOOM.x_max>1.) {  ZOOM.x_min=  1.-delta ;
                                     ZOOM.x_max=  1. ;  }

                             delta=ZOOM.y_max-ZOOM.y_min ;
                             shift=delta*(y_mouse-y_mouse_prv)/     /* ������� ������ */
                                     metrics->draw_y_size    ;
                       ZOOM.y_min+=shift ;                          /* �������� ������ �� X */
                       ZOOM.y_max+=shift ;

                if(ZOOM.y_min<0.) {  ZOOM.y_min=  0. ;              /* �������� ������ �� Y */
                                     ZOOM.y_max=delta ;  }
                if(ZOOM.y_max>1.) {  ZOOM.y_min=  1.-delta ;
                                     ZOOM.y_max=  1. ;  }

             SendMessage(hWnd, WM_PAINT, (WPARAM)0, (LPARAM)0) ;

                        x_mouse_prv=x_mouse ;
                        y_mouse_prv=y_mouse ;

                                break ;
                                                }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - ZOOM - ������� */
     else  if( smooth_zoom_flag                && 
              (wParam & MK_CONTROL)==MK_CONTROL   ) {

                            delta=x_mouse-x_mouse_prv ;
                             base=metrics->draw_x_size ;

         if(delta!=0) {
                 if(delta>0)  zoom=1+10.*delta /base ;              /* ������� ��������� �������� �� X */
                 else         zoom=1/(1+10.*(-delta)/base) ;

                           base   = ZOOM.x_max-ZOOM.x_min ;         /* ������� ������ ��������� �� ��� X */
                           base_n =base*zoom ;
            if(base_n>1.)  base_n =1. ;

                       ZOOM.x_min+=(base-base_n)/2. ;               /* �������� ������ �� X */
                       ZOOM.x_max-=(base-base_n)/2. ;
                      }

                if(ZOOM.x_min<0.) {  ZOOM.x_min=  0. ;              /* �������� ������ �� X */
                                     ZOOM.x_max=base_n ;      }
                if(ZOOM.x_max>1.) {  ZOOM.x_min=  1.-base_n ;
                                     ZOOM.x_max=  1. ;        }

                            delta=y_mouse-y_mouse_prv ;
                             base=metrics->draw_y_size ;

         if(delta!=0) {
                 if(delta>0)  zoom=1+10.*delta /base ;              /* ������� ��������� �������� �� Y */
                 else         zoom=1/(1+10.*(-delta)/base) ;

                           base   =ZOOM.y_max-ZOOM.y_min ;          /* ������� ������ ��������� �� ��� Y */
                           base_n =base*zoom ;
            if(base_n>1.)  base_n =1. ;

                       ZOOM.y_min+=(base-base_n)/2. ;               /* �������� ������ �� Y */
                       ZOOM.y_max-=(base-base_n)/2. ;
                      }

                if(ZOOM.y_min<0.) {  ZOOM.y_min=  0. ;              /* �������� ������ �� Y */
                                     ZOOM.y_max=base_n ;      }
                if(ZOOM.y_max>1.) {  ZOOM.y_min=  1.-base_n ;
                                     ZOOM.y_max=  1. ;        }

             SendMessage(hWnd, WM_PAINT, (WPARAM)0, (LPARAM)0) ;

                        x_mouse_prv=x_mouse ;
                        y_mouse_prv=y_mouse ;

                                break ;
                                                    }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

#undef  ZOOM
     		                      break ;
                         }
/*------------------------------------------ ���������� ����� ������ */

    case WM_LBUTTONUP:   {
                               smooth_zoom_flag=0 ;
/*- - - - - - - - - - - - - - - - - - - - - -  ���������������� ���� */
                             if(!zone_flag)  break ;
                                 zone_flag=0 ;

                          ReleaseCapture() ;                        /* ����������� ���� */

                             if(x_zone_end<0)  break ;
/*- - - - - - - - - - - - - - - - - - �������� ���������������� ���� */
                    InvalidateRect(hWnd, NULL, false) ;
                hDC_tmp=BeginPaint(hWnd, &PaintCfg) ;

                               Rect.left  =x_zone_beg ;
                               Rect.right =x_zone_end ;
                               Rect.top   =y_zone_beg ;
                               Rect.bottom=y_zone_end ;

                   UDi_zone_invert(hDC_tmp, metrics, &Rect) ;

                      ValidateRect(hWnd, NULL) ;
                          EndPaint(hWnd, &PaintCfg) ;
/*- - - - - - - - - - - - - - - - - - - - �������������� ������ ���� */
         if(x_zone_beg > x_zone_end) {         tmp=x_zone_end ;
                                        x_zone_end=x_zone_beg ;
                                        x_zone_beg=  tmp ;       }
         if(y_zone_beg > y_zone_end) {         tmp=y_zone_end ;
                                        y_zone_end=y_zone_beg ;
                                        y_zone_beg=  tmp ;       }
/*- - - - - - - - - - - - - -  ���������� ������ � ������������ ���� */
         if(abs(x_zone_beg-x_zone_end)<4 ||                         /* ���������� ���� �� ������� */    
            abs(y_zone_beg-y_zone_end)<4   )  break ;               /*    ��������� ��������      */

                x_zoom_min=(x_zone_beg-metrics->draw_x_base)        /* ����������� "���������" ������ */
                          / (double)metrics->draw_x_size ;
                x_zoom_max=(x_zone_end-metrics->draw_x_base)
                          / (double)metrics->draw_x_size ;
                y_zoom_min=(metrics->draw_y_base+
                            metrics->draw_y_size-y_zone_end)
                          / (double)metrics->draw_y_size ;
                y_zoom_max=(metrics->draw_y_size-y_zone_beg)
                          / (double)metrics->draw_y_size ;
/*- - - - - - - - - - - - - - - - - -  ������������ ��������� ������ */
                 if(x_zoom_min<0.)  x_zoom_min=0. ; 
                 if(x_zoom_max>1.)  x_zoom_max=1. ; 
                 if(y_zoom_min<0.)  y_zoom_min=0. ; 
                 if(y_zoom_max>1.)  y_zoom_max=1. ; 
/*- - - - - - - - - - - - - - - - - - -  ���������� ������ ZOOM-���� */
     if(data           ==NULL ||                                    /* ���� ��� ZOOM-��������� */
        data->ctrl_proc==NULL   )  break ;

#define  ZOOMS  data->zooms
#define  Z_CNT  data->zooms_cnt

          ZOOMS=(struct UD_zoom_scale *)                            /* ��������� ����� �������� */    
                     realloc(ZOOMS, (Z_CNT+1)*sizeof(*ZOOMS)) ;
/*- - - - - - - - - - - - - - - - - ���������� ������ � ������� ���� */
      if(zoom_flag) {                                               /* ���� ������������ ���� - */
                              x_zoom_size=ZOOMS[zoom_num].x_max-    /*   ZOOM-����              */
                                          ZOOMS[zoom_num].x_min ;
                              y_zoom_size=ZOOMS[zoom_num].y_max-
                                          ZOOMS[zoom_num].y_min ;
                       ZOOMS[Z_CNT].x_min=ZOOMS[zoom_num].x_min+
                                          x_zoom_size*x_zoom_min ;
                       ZOOMS[Z_CNT].x_max=ZOOMS[zoom_num].x_min+
                                          x_zoom_size*x_zoom_max ;
                       ZOOMS[Z_CNT].y_min=ZOOMS[zoom_num].y_min+
                                          y_zoom_size*y_zoom_min ;
                       ZOOMS[Z_CNT].y_max=ZOOMS[zoom_num].y_min+
                                          y_zoom_size*y_zoom_max ;
                    }
      else          {                                               /* ���� ������������ ���� - */
                       ZOOMS[Z_CNT].x_min=x_zoom_min ;              /*   ������� ����           */
                       ZOOMS[Z_CNT].x_max=x_zoom_max ;
                       ZOOMS[Z_CNT].y_min=y_zoom_min ;
                       ZOOMS[Z_CNT].y_max=y_zoom_max ;
                    }
/*- - - - - - - - - - - - - - - - ����� ��������� �������� ZOOM-���� */
                             Z_CNT++ ;

                data->ctrl_proc(_UD_ZOOM, Z_CNT-1, data) ;          /*  �������� ��������� ��������� */                             
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
     		                      break ;

#undef  ZOOMS
#undef  Z_CNT

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


/*********************************************************************/
/*                                                                   */
/*                  ��������� ��������� ����������                   */

   void  UDi_grid_2D_init(HDC  hDC, UD_area_data *view_decl, 
                                      UD_metrics *metrics)

{
      GetTextMetrics(hDC, &metrics->font) ;
}


/*********************************************************************/
/*                                                                   */
/*          ������������� ��� ��������� � ���� ���������             */

   void  UDi_grid_2D_sizing(HDC  hDC, UD_area_data *view_decl, 
                                           UD_metrics *metrics)
{
              int  size ;

/*----------------------------- ���������� '������' ���� X-��������� */

         metrics->x_scale_h=metrics->font.tmHeight ;

/*------------------------------ ����������� ������ ���� Y-��������� */

   if(metrics->zoom_flag)  size= 9 ;
   else                    size= 7 ;

         metrics->y_scale_w=size*metrics->font.tmAveCharWidth ;     /* ������������� � �������� ������� */

/*----------------------------- ����������� ���������� ���� �������� */

	metrics->draw_x_base=metrics->y_scale_w ;
	metrics->draw_y_base= 0 ;

	metrics->draw_x_size=metrics->x_size-metrics->y_scale_w ;
	metrics->draw_y_size=metrics->y_size-metrics->x_scale_h ;

/*-------------------------------------------------------------------*/
}


/*********************************************************************/
/*                                                                   */
/*                  ������������ ���� ������                         */

   void  UDi_grid_2D_scale(HDC  hDC, UD_area_data *view_decl, 
                                       UD_metrics *metrics)

{
  UD_area_2Dpoint *points ;
              int  points_cnt ;
    UD_zoom_scale *zoom ;           /* ��������� ��������������� */
           double  range ;
           double  x_step ;
           double  y_step ;
           double  v_len ;
           double  v_len_max ;
               int  i ;

#define   X_MIN      view_decl->x_min
#define   X_MAX      view_decl->x_max
#define   Y_MIN      view_decl->y_min
#define   Y_MAX      view_decl->y_max

/*------------------------------------------- ����������� ���������� */

                points    =view_decl->points ;
                points_cnt=view_decl->points_cnt ;

				          X_MIN=points[0].x ;       /* ����.���������� */
				          X_MAX=points[0].x ;
				          Y_MIN=points[0].y ;
				          Y_MAX=points[0].y ;

	  for(i=1 ; i<points_cnt ; i++) {                           /* ���.��������� */
		   if(points[i].x<X_MIN)  X_MIN=points[i].x ;
		   if(points[i].x>X_MAX)  X_MAX=points[i].x ;
		   if(points[i].y<Y_MIN)  Y_MIN=points[i].y ;
		   if(points[i].y>Y_MAX)  Y_MAX=points[i].y ;
				        }

                    view_decl->x_data_min =X_MIN ;
                    view_decl->x_data_max =X_MAX ;
                    view_decl->y_data_min =Y_MIN ;
                    view_decl->y_data_max =Y_MAX ;

                    view_decl->x_grid_0   =X_MIN ;
                    view_decl->y_grid_0   =Y_MIN ;
                
/*------------------------------------- ��������� ������������������ */

   if(view_decl->prop_scale) {

           x_step=(X_MAX-X_MIN)/((double)metrics->draw_x_size) ;
           y_step=(Y_MAX-Y_MIN)/((double)metrics->draw_y_size) ;

        if(x_step<y_step)  X_MAX=X_MIN+metrics->draw_x_size*y_step ;
        if(x_step>y_step)  Y_MAX=Y_MIN+metrics->draw_y_size*x_step ;

                             }
/*-------------------------------------------- ���������� ���������� */

                range =X_MAX-X_MIN ;
                X_MIN-=range/10. ;  
                X_MAX+=range/10. ;

                range =Y_MAX-Y_MIN ;
                Y_MIN-=range/10. ;  
                Y_MAX+=range/10. ;

/*--------------------------------------- ��������������� ���������� */

   if(metrics->zoom_flag) {

                       zoom =&view_decl->zooms[metrics->zoom_num] ;

                         range=X_MAX-X_MIN ;
                      X_MIN  +=range*    zoom->x_min ;
                      X_MAX  -=range*(1.-zoom->x_max) ;

                         range=Y_MAX-Y_MIN ;
                      Y_MIN  +=range*    zoom->y_min ;
                      Y_MAX  -=range*(1.-zoom->y_max) ;
                          }
/*-------------------------------------- ����������� ����� ��������� */

           view_decl->x_grid_step=UDi_range_step(X_MAX-X_MIN) ;

   if(view_decl->prop_scale)
           view_decl->y_grid_step=view_decl->x_grid_step/2. ;
   else    view_decl->y_grid_step=UDi_range_step(Y_MAX-Y_MIN)/2. ;
   
/*--------------------------- ����������� ������������ ����� ������� */

  if(view_decl->type & _UD_VECTOR_DATA) {

                v_len_max=-1 ;

      for(i=1 ; i<points_cnt ; i++) {                               /* ���.��������� */

        if(points[i].x<X_MIN || 
           points[i].x>X_MAX ||  
           points[i].y<Y_MIN || 
           points[i].y>Y_MAX   )  continue ;

             v_len=sqrt(points[i].x_vector*points[i].x_vector+
                        points[i].y_vector*points[i].y_vector ) ;
          if(v_len>v_len_max)  v_len_max=v_len ;
                                    }

             if(v_len_max>0)  view_decl->v_len_max=v_len_max ;
             else             view_decl->v_len_max=   1. ;
                                        }
/*----------------------------------------------- �������� � ������� */

                      metrics->x_base_min =X_MIN ;
                      metrics->x_base_max =X_MAX ;
                      metrics->y_base_min =Y_MIN ;
                      metrics->y_base_max =Y_MAX ;

/*-------------------------------------------------------------------*/
}


/*********************************************************************/
/*                                                                   */
/*                  ������� ���������������� ���                     */

   void  UDi_grid_2D_clear(HDC  hDC, UD_area_data *view_decl, 
                                       UD_metrics *metrics)

{
    RECT  Rect ;
  HBRUSH  hBrush ;

/*------------------------------------------------------- ���������� */

       hBrush=CreateSolidBrush(view_decl->back_color) ;

/*------------------------------------------ ������ ������� �������� */

                    Rect.left  =metrics->x_base ;
                    Rect.top   =metrics->y_base ;
                    Rect.right =metrics->x_base+metrics->x_size ;
                    Rect.bottom=metrics->y_base+metrics->y_size ;

	  FillRect(hDC, &Rect, hBrush) ;

/*------------------------------------------------------- ���������� */

                DeleteObject(hBrush) ;

/*-------------------------------------------------------------------*/
}


/*********************************************************************/
/*                                                                   */
/*                         ��������� �����                           */

   void  UDi_grid_2D_frame(HDC  hDC, UD_area_data *view_decl,
                                       UD_metrics *metrics)
{
}


/*********************************************************************/
/*                                                                   */
/*                  ��������� ���� � ���������                       */

   void  UDi_grid_2D_grid(HDC  hDC, UD_area_data *view_decl, 
                                      UD_metrics *metrics)
{
             HPEN  hPen ;                    /* ���� */
             HPEN  hPen_prev ;               /* ����, ���������� */
           double  value ;
           double  step ;
    UD_value_zone  zone ;
              int  i ;

#define                    _VALUES_MAX   30
    UD_value_zone  x_value[_VALUES_MAX] ;    /* �������� ��������� X-���� */
              int  x_value_cnt ;
    UD_value_zone  y_value[_VALUES_MAX] ;    /* �������� ��������� Y-���� */
              int  y_value_cnt ;

/*--------------------------------- ���������� ����������� ��������� */

     hPen     =         CreatePen(PS_DASH, 0, view_decl->fore_color) ;
     hPen_prev=(HPEN)SelectObject(hDC, hPen) ;

/*--------------------------------------- ��������� ����� ��� ������ */

        UDg_setviewport(hDC, metrics->draw_x_base,                  /* ���.������� ���� */
                             metrics->draw_y_base, 
                             metrics->draw_x_size,
                             metrics->draw_y_size ) ; 

		    x_value_cnt=0 ;
		    y_value_cnt=0 ;

	 UDg_setwindow(hDC, 1, view_decl->x_min,                    /* ������������ ������� ���� */
                               view_decl->y_min,       
                               view_decl->x_max, 
                               view_decl->y_max ) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - ��������� �� X */
	       step=view_decl->x_grid_step ;                        /* ���������� ��� �� ��� X */

       for(value=view_decl->x_grid_0 ;
           value<view_decl->x_data_max+0.01*step ; value+=step) {   /* CIRCLE.2 - ���� �� ����� ����� */

          if(value<view_decl->x_min)  continue ;

		UDg_moveto_w(hDC, value, view_decl->y_min) ;        /* ������ ����� ����� */
		UDg_lineto_w(hDC, value, view_decl->y_data_max) ;

	   if(x_value_cnt<_VALUES_MAX) {
		 x_value[x_value_cnt].value=                value ;
		 x_value[x_value_cnt].pos  =UDg_xpos_w(hDC, value) ;
		         x_value_cnt++ ;
				       }
							        }   /* CONTINUE.2 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - ��������� �� Y */
	       step=view_decl->y_grid_step ;                        /* ���������� ��� �� ��� X */

       for(value=view_decl->y_grid_0 ;
           value<view_decl->y_data_max+0.01*step ; value+=step) {   /* CIRCLE.3 - ���� �� ����� ����� */

          if(value<view_decl->y_min)  continue ;

		UDg_moveto_w(hDC, view_decl->x_min, value) ;        /* ������ ����� ����� */
		UDg_lineto_w(hDC, view_decl->x_data_max, value) ;

	   if(y_value_cnt<_VALUES_MAX) {
		 y_value[y_value_cnt].value=                value ;
		 y_value[y_value_cnt].pos  =UDg_ypos_w(hDC, value) ;
		         y_value_cnt++ ;
				       }
							        }   /* CONTINUE.3 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

    UDg_setviewport(hDC, 0, 0, metrics->x_size, metrics->y_size) ;  /* �������������� �������� ���� */

/*--------------------------------------------------- ��������� ���� */

                 SetTextColor(hDC, view_decl->grid_color) ;
                   SetBkColor(hDC, view_decl->back_color) ;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - ��������� �� X */
               UDi_value_zone(view_decl->x_min,
                              view_decl->x_max, 
                              view_decl->x_grid_step, &zone) ;

   for(i=0 ; i<x_value_cnt ; i+=2) {
	  UDi_value_cvt(x_value[i].value, &zone, 0) ;
	        TextOut(hDC, metrics->draw_x_base+x_value[i].pos
                            -metrics->font.tmAveCharWidth*zone.digits/2,
                             metrics->draw_y_base+metrics->draw_y_size,
                                      zone.buff, strlen(zone.buff)) ;
				  }
/*- - - - - - - - - - - - - - - - - - - - - - - - - - ��������� �� Y */
               UDi_value_zone(view_decl->y_min,
                              view_decl->y_max,
                              view_decl->y_grid_step, &zone) ;

   for(i=0 ; i<y_value_cnt ; i++) {
	  UDi_value_cvt(y_value[i].value, &zone, 0) ;
	        TextOut(hDC,   0,
                             metrics->draw_y_base-
                              metrics->font.tmHeight/2+
                                y_value[i].pos,
                                      zone.buff, strlen(zone.buff)) ;
				  }
/*----------------------------- �������������� ����������� ��������� */

             SelectObject(hDC, hPen_prev) ;
             DeleteObject(hPen) ;

/*-------------------------------------------------------------------*/

}


/*********************************************************************/
/*                                                                   */
/*                   ������������ ����� ������                       */

   void  UDi_grid_2D_rare(HDC  hDC, UD_area_data *view_decl, 
                                      UD_metrics *metrics)

{
  UD_area_2Dpoint *points ;
              int  points_cnt ;
              int  i ;


                points    =view_decl->points ;
                points_cnt=view_decl->points_cnt ;

   for(i=0 ; i<points_cnt ; i++) {
                                        points[i].rare_active=1 ;
                                 }
}


/*********************************************************************/
/*                                                                   */
/*                    ��������� ��������� �����                      */

   void  UDi_grid_2D_ceil(HDC  hDC, UD_area_data *view_decl, 
                                      UD_metrics *metrics)

{
  UD_area_2Dpoint *points ;
              int  points_cnt ;
             HPEN  hPen ;           /* ���� */
             HPEN  hPen_prev ;      /* ����, ���������� */
              int  draw[100] ;      /* ������ ��������� �������� ����� */
              int  draw_cnt ;
              int  i ;
              int  j ;

/*-------------------------------------------------- �������� ������ */

      if(!(view_decl->type & _UD_GRID_DATA))  return ;

/*-------------------------------------------------- ���������� ���� */

        UDg_setviewport(hDC, metrics->draw_x_base,                  /* ���.������� ���� */
                             metrics->draw_y_base, 
                             metrics->draw_x_size,
                             metrics->draw_y_size ) ; 

	  UDg_setwindow(hDC, 1, view_decl->x_min,                   /* ������������ ������� ���� */
                                view_decl->y_min,       
                                view_decl->x_max, 
                                view_decl->y_max ) ;              

     hPen     =         CreatePen(PS_SOLID, 0,                      /* ���������� ���� */    
                                      view_decl->fore_color) ;      
     hPen_prev=(HPEN)SelectObject(hDC, hPen) ;
            
/*-------------------------------------------- ��������� ����� ����� */

                points    =view_decl->points ;
                points_cnt=view_decl->points_cnt ;

   for(i=0 ; i<points_cnt ; i++) {                                  /* CIRCLE.1 - ���������� ������ */
        
       if(!points[i].rare_active)  continue ;                       /* ��������� ����� ���������� */

         draw_cnt=UDi_grid_2D_links(points, points_cnt, i, draw) ;  /* ���������� ������ ��������� �������� ����� */

    for(j=0 ; j<draw_cnt ; j++) {                                   /* ������������ ����� */    
       UDg_moveto_w(hDC, points[     i ].x, points[     i ].y) ; 
       UDg_lineto_w(hDC, points[draw[j]].x, points[draw[j]].y) ;
                                }
                                 }                                  /* CONTINUE.1 */

/*----------------------------- �������������� ����������� ��������� */

       SelectObject(hDC, hPen_prev) ;
       DeleteObject(hPen) ;

    UDg_setviewport(hDC, 0, 0, metrics->x_size, metrics->y_size) ;  /* �������������� �������� ���� */

/*-------------------------------------------------------------------*/
}


/*********************************************************************/
/*                                                                   */
/*              ���������� ������ ��������� �������� �����           */

    int  UDi_grid_2D_links(UD_area_2Dpoint *points,
                                       int  points_cnt, 
                                       int  point_num,
                                       int *draw       )              
{
    struct {  int  idx ;
              int  ptr ;  } scan[100] ;
              int           n_scan ;
              int           n ;
              int           draw_cnt ;

#define  S    scan[n_scan]


                         n_scan= 0 ;
                          S.idx=point_num ;
                          S.ptr= 0 ;

                       draw_cnt= 0 ;
                
        while(1) {

           if(S.ptr>=points[S.idx].link_cnt) {
                                if(n_scan==0)  break ;

                                              n_scan-- ;
                                               S.ptr++ ;     
                                               continue ;
                                             }

                     n=points[S.idx].link_list[S.ptr] ;
                                    
           if(points[n].rare_active) {
                      if(n>point_num) {  draw[draw_cnt]=n ;
                                              draw_cnt++ ;  }
                                                 S.ptr++ ;
                                                 continue ;
                                     }
           else                      {
                                                n_scan++ ;
                                                 S.idx=n ;
                                                 S.ptr=0 ;
                                                 continue ;
                                     }
                 }
#undef   S

   return(draw_cnt) ;
}


/*********************************************************************/
/*                                                                   */
/*                  ������� ��������� ������� � ����                 */

   void  UDi_g2Dscan_position(UD_metrics *metrics,
                                     int  flag,  
                                     int *x, 
                                     int *y,
                                  double *x_value, 
                                  double *y_value )

{

/*-------------------------------------------------- ���������� ���� */

        UDg_setviewport(NULL, metrics->draw_x_base,                 /* ���.������� ���� */
                              metrics->draw_y_base, 
                              metrics->draw_x_size,
                              metrics->draw_y_size ) ; 

	  UDg_setwindow(NULL, 1, metrics->x_base_min,               /* ������������ ������� ���� */
                                 metrics->y_base_min,       
                                 metrics->x_base_max, 
                                 metrics->y_base_max ) ;              
            
/*-------------------------- ���������� �������/��������� ���������� */

   if(flag) {

      *x_value=UDg_xcoord_w(NULL, *x-metrics->draw_x_base, NULL, NULL) ;
      *y_value=UDg_ycoord_w(NULL, *y-metrics->draw_y_base, NULL, NULL) ;
            }
   else     {
              *x=UDg_xpos_w(NULL, *x_value)+metrics->draw_x_base ;
              *y=UDg_ypos_w(NULL, *y_value)+metrics->draw_y_base ;
            }
/*-------------------------------------------- �������������� ������ */

    UDg_setviewport(NULL, 0, 0, metrics->x_size, metrics->y_size) ; /* �������������� �������� ���� */

/*-------------------------------------------------------------------*/

}

