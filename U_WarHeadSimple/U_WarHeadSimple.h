
/********************************************************************/
/*								    */
/*              ������ ���������� ����������� "������� ��"          */
/*								    */
/********************************************************************/

#ifdef U_WARHEAD_SIMPLE_EXPORTS
#define U_WARHEAD_SIMPLE_API __declspec(dllexport)
#else
#define U_WARHEAD_SIMPLE_API __declspec(dllimport)
#endif

/*----------------------------- �������� ������ ������� "������� ��" */

  class U_WARHEAD_SIMPLE_API RSS_Unit_WarHeadSimple : public RSS_Unit_WarHead {

    public:

    public:
               virtual void  vFree             (void) ;                             /* ���������� ������� */
               virtual  int  vCalculateStart   (double) ;                           /* ���������� ������� ��������� ��������� */
               virtual  int  vCalculate        (double, double, char *, int) ;      /* ������ ��������� ��������� */
                                                     
               virtual  int  vCalculateShow    (void) ;                             /* ����������� ���������� ������� ��������� ��������� */
               virtual  int  vSpecial          (char *, void *) ;                   /* ����������� �������� */

                virtual int  vSetWarHeadControl(char *) ;                           /* ���������� �� */

	                     RSS_Unit_WarHeadSimple() ;                /* ����������� */
	                    ~RSS_Unit_WarHeadSimple() ;                /* ���������� */
                                                        } ;

/*----------------- �������� ������ ���������� �������� "������� ��" */

  class U_WARHEAD_SIMPLE_API RSS_Module_WarHeadSimple : public RSS_Kernel {

    public:

     static
      struct RSS_Module_WarHeadSimple_instr *mInstrList ;          /* ������ ������ */

    public:
     virtual  RSS_Object *vCreateObject (RSS_Model_data *) ;                 /* �������� ������� */ 
     virtual         int  vGetParameter (char *, char *) ;                   /* �������� �������� */
     virtual         int  vExecuteCmd   (const char *) ;                     /* ��������� ������� */

    public:
                     int  cHelp         (char *) ;                           /* ���������� HELP */ 
                     int  cInfo         (char *) ;                           /* ���������� INFO */ 

                RSS_Unit *FindUnit      (char *) ;                           /* ����� ���������� �� ����� */

    public:
	                  RSS_Module_WarHeadSimple() ;              /* ����������� */
	                 ~RSS_Module_WarHeadSimple() ;              /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct RSS_Module_WarHeadSimple_instr {

           char                            *name_full ;          /* ������ ��� ������� */
           char                            *name_shrt ;          /* �������� ��� ������� */
           char                            *help_row ;           /* HELP - ������ */
           char                            *help_full ;          /* HELP - ������ */
            int (RSS_Module_WarHeadSimple::*process)(char *) ;   /* ��������� ���������� ������� */
                                 }  ;

/*--------------------------------------------- ���������� ��������� */

/* ����  U_WarHeadSimple.cpp */

/* ����  U_WarHeadSimple_dialog.cpp */
    BOOL CALLBACK  Unit_WarHeadSimple_Help_dialog  (HWND, UINT, WPARAM, LPARAM) ;
