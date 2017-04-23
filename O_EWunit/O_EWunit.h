
/********************************************************************/
/*								    */
/*		������ ���������� �������� "������� ���"            */
/*								    */
/********************************************************************/

#ifdef O_EWUNIT_EXPORTS
#define O_EWUNIT_API __declspec(dllexport)
#else
#define O_EWUNIT_API __declspec(dllimport)
#endif

#include "..\T_Battle\T_Battle.h"

/*---------------------------------------------- ��������� ��������� */

#define  _EWUNIT_THREATS_MAX  50

/*------------------ �������� ������ ��������� ������� "������� ���" */

  class O_EWUNIT_API RSS_Transit_EWunit : public RSS_Transit {

    public:
             virtual   int  vExecute(void) ;             /* ���������� �������� */
                                             
    public:
                            RSS_Transit_EWunit() ;        /* ����������� */
                           ~RSS_Transit_EWunit() ;        /* ���������� */

                                                               } ;

/*---------------------------- �������� ������ ������� "������� ���" */

  class O_EWUNIT_API RSS_Object_EWunit : public RSS_Object {

    public:
                      char  model_path[FILENAME_MAX] ;        /* ���� ������ */

                    double  range_min ;                       /* �������� ���������� ����������� */
                    double  range_max ;
                    double  velocity ;                        /* ����������� �������� ��������� */

                      char  owner[128] ;                      /* ������-�������� */
                RSS_Object *o_owner ;

                      char  event_name[128] ;                 /* ��� ������������ ������� */
                    double  event_time ;                      /* ����� ������� */
                       int  event_send ;                      /* ���� �������� ������� */

         RSS_Module_Battle *battle ;                          /* ������ �� BATTLE-������ */

                RSS_Object *threats[_EWUNIT_THREATS_MAX] ;    /* ������ ����� */
                       int  threats_cnt ;

                      HWND  hWnd ;                            /* ���� ���������� */ 

    public:
               virtual void  vFree          (void) ;            /* ���������� ������� */
               virtual void  vWriteSave     (std::string *) ;   /* �������� ������ � ������ */
               virtual  int  vCalculateStart(void) ;            /* ���������� ������� ��������� ��������� */
               virtual  int  vCalculate     (double, double,    /* ������ ��������� ��������� */
                                                     char *, int) ;
               virtual  int  vCalculateShow (void) ;            /* ����������� ���������� ������� ��������� ��������� */
               virtual  int  vSpecial       (char *, void *) ;  /* ����������� �������� */

	                     RSS_Object_EWunit() ;              /* ����������� */
	                    ~RSS_Object_EWunit() ;              /* ���������� */
                                                        } ;

/*---------------- �������� ������ ���������� �������� "������� ���" */

  class O_EWUNIT_API RSS_Module_EWunit : public RSS_Kernel {

    public:

     static
      struct RSS_Module_EWunit_instr *mInstrList ;          /* ������ ������ */

    public:
     virtual         int  vGetParameter (char *, char *) ;  /* �������� �������� */
     virtual         int  vExecuteCmd   (const char *) ;    /* ��������� ������� */
     virtual        void  vReadSave     (std::string *) ;   /* ������ ������ �� ������ */
     virtual        void  vWriteSave    (std::string *) ;   /* �������� ������ � ������ */

    public:
                     int  cHelp         (char *) ;                     /* ���������� HELP */ 
                     int  cCreate       (char *) ;                     /* ���������� CREATE */ 
                     int  cInfo         (char *) ;                     /* ���������� INFO */ 
                     int  cOwner        (char *) ;                     /* ���������� OWNER */ 
                     int  cRange        (char *) ;                     /* ���������� RANGE */
                     int  cVelocity     (char *) ;                     /* ���������� VELOCITY */
                     int  cEvent        (char *) ;                     /* ���������� EVENT */ 
                     int  cShow         (char *) ;                     /* ���������� SHOW */ 

              RSS_Object *FindObject    (char *, int) ;                /* ����� ������� �� ����� */
                     int  CreateObject  (RSS_Model_data *) ;           /* �������� ������� */ 

    public:
	                  RSS_Module_EWunit() ;              /* ����������� */
	                 ~RSS_Module_EWunit() ;              /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct RSS_Module_EWunit_instr {

           char                     *name_full ;          /* ������ ��� ������� */
           char                     *name_shrt ;          /* �������� ��� ������� */
           char                     *help_row ;           /* HELP - ������ */
           char                     *help_full ;          /* HELP - ������ */
            int (RSS_Module_EWunit::*process)(char *) ;   /* ��������� ���������� ������� */
                                 }  ;

/*--------------------------------------------- ���������� ��������� */

/* ����  O_EWunit.cpp */

/* ����  O_EWunit_dialog.cpp */
    BOOL CALLBACK  Object_EWunit_Help_dialog  (HWND, UINT, WPARAM, LPARAM) ;
    BOOL CALLBACK  Object_EWunit_Show_dialog  (HWND, UINT, WPARAM, LPARAM) ;
 LRESULT CALLBACK  Object_EWunit_Indicator_prc(HWND, UINT, WPARAM, LPARAM) ;
