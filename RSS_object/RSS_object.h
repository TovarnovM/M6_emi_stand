
#ifndef  RSS_OBJECT_H 

#define  RSS_OBJECT_H 

#include "..\RSS_Feature\RSS_Feature.h"

#include <string>

#ifdef RSS_OBJECT_EXPORTS
#define RSS_OBJECT_API __declspec(dllexport)
#else
#define RSS_OBJECT_API __declspec(dllimport)
#endif

/*---------------------------------------------------- ���� �������� */

#define   _RSS_OBJECT_UNKNOWN_ERROR      -999    /* ����������� ������ */

/*------------------------------------- ���� ����� ������� ��������� */

#define   _RSS_ABSOLUTE_TARGET  0   /* �������� ����������� ������������ */ 

/*----------------------------------- ��������� �������������� ����� */

#define   _GRD_TO_RAD   0.017453
#define   _RAD_TO_GRD  57.296
#define   _PI           3.1415926

/*---------------------------------------- �������� �������� "�����" */

   typedef struct {
                     double  x ;
                     double  y ;
                     double  z ;

                     double  azim ;
                     double  elev ;
                     double  roll ;

                        int  mark ;

                  } RSS_Point ;

/*--------------------------------------- �������� �������� "������" */

   typedef struct {
                     double  x ;
                     double  y ;
                     double  z ;

                        int  mark ;

                  } RSS_Vector ;

/*------------------------- �������� �������� "����������� ��������" */

   typedef struct {
                     char  link[32] ;            /* �����-������ �� �������� */
                     char  section_name[32] ;    /* �������� ������ ���������� */
                     char  parameter_name[32] ;  /* �������� ��������� */
                     char  type[8] ;             /* ��� ��������� */  
                     void *value ;               /* ��������������� �������� */
                  } RSS_ControlPar ;

/*------------------------------------ ��������� �������� ���������� */

  class RSS_OBJECT_API RSS_Transit {

    public:
                      char  action[1024] ;
                RSS_Object *object ;

    public:
             virtual   int  vExecute(void) ;             /* ���������� �������� */
                                             
    public:
                            RSS_Transit() ;              /* ����������� */
                           ~RSS_Transit() ;              /* ���������� */

                                   } ; 

/*-------------------------------- �������� ������ "������ ��������" */

  struct RSS_Objects_List_Elem {
                                 class RSS_Object *object ;
                                             char  relation[128] ;
                               }  ;

  class RSS_OBJECT_API RSS_Objects_List {

       public:
                RSS_Objects_List_Elem *List ;
                                  int   List_cnt ;
                                  int   List_max ;

       public:

                        int  Add  (class RSS_Object *, char *) ;    /* ���������� � ������ */
                       void  Clear(void) ;                          /* ������� ������ */

			     RSS_Objects_List() ;                   /* ����������� */
			    ~RSS_Objects_List() ;                   /* ���������� */

                                  } ;
/*----------------------------------------- �������� ������ "������" */

  class RSS_OBJECT_API RSS_Object {

       public:
                       char   Name[128] ;      /* ��� ������� */
                       char   Type[128] ;      /* ��� ������� */
                       char   Decl[1024] ;     /* �������� ������� */

                     double  x_base ;          /* ���������� ������� ����� */
                     double  y_base ;
                     double  z_base ;

                     double  a_azim ;          /* ���� ���������� */
                     double  a_elev ;
                     double  a_roll ;

                     double  x_velocity ;      /* ������ �������� */
                     double  y_velocity ;
                     double  z_velocity ;

       struct RSS_Parameter  *Parameters ;     /* ������ ���������� */
                        int   Parameters_cnt ;

                RSS_Feature **Features ;       /* ������ ������� */
                        int   Features_cnt ;

     class RSS_Objects_List   Units ;          /* ������ ��������-��������� ������ */

                RSS_Transit  *Context ;        /* ��������� �������� ���������� */
          class  RSS_Kernel  *Module ;         /* ����������� ������ ������� */

                        int   ErrorEnable ;    /* ���� ������ ��������� �� ������� */

       public:

   virtual class RSS_Object *vCopy           (char *) ;             /* ���������� ������ */

   virtual             void  vErrorMessage   (int) ;                /* ���./����. ��������� �� ������� */

   virtual             void  vReadSave       (std::string *) ;      /* ������� ������ �� ������ */
   virtual             void  vWriteSave      (std::string *) ;      /* �������� ������ � ������ */
   virtual             void  vFree           (void) ;               /* ���������� ������� */

   virtual             void  vFormDecl       (void) ;               /* ����������� �������� */

   virtual              int  vListControlPars(RSS_ControlPar *) ;   /* �������� ������ ���������� ���������� */
   virtual              int  vSetControlPar  (RSS_ControlPar *) ;   /* ���������� �������� ��������� ���������� */

   virtual              int  vGetPosition    (RSS_Point *) ;        /* ������ � ���������� ������� */
   virtual             void  vSetPosition    (RSS_Point *) ;
   virtual              int  vGetVelocity    (RSS_Vector *) ;       /* ��������� ������� �������� */

   virtual              int  vSpecial        (char *, void *) ;     /* ����������� �������� */

   virtual              int  vCalculateStart (double) ;             /* ���������� ������� ��������� ��������� */
   virtual              int  vCalculate      (double, double,       /* ������ ��������� ��������� */
                                                      char *, int) ;
   virtual              int  vCalculateShow  (void) ;               /* ����������� ���������� ������� ��������� ��������� */
   virtual              int  vEvent          (char *, double) ;     /* ��������� ������� */

   virtual              int  vResetFeatures  (void *) ;             /* ����� ��������� �������� ������� */
   virtual              int  vPrepareFeatures(void *) ;             /* ����������� �������� � �������� ������������ */
   virtual              int  vCheckFeatures  (void *,               /* ��������� ������������ ������� */
                                              RSS_Objects_List *) ;
   virtual             void  vSetFeature     (RSS_Feature *) ;      /* ������ �� ���������� */
   virtual             void  vGetFeature     (RSS_Feature *) ;

                        int  iAngleInCheck   (double,               /* �������� ��������� ���� � �������� */
                                              double, double) ;
                        int  iReplaceText    (char *, char *,       /* ������ ���������� ��������� � ������ */
                                                      char *, int) ;

			     RSS_Object      () ;                   /* ����������� */
			    ~RSS_Object      () ;                   /* ���������� */

                                  } ;
/*-------------------------------------------------------------------*/

#endif  // RSS_OBJECT_H
