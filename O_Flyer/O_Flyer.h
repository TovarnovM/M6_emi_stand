
/********************************************************************/
/*								    */
/*		������ ���������� �������� "�����"  		    */
/*								    */
/********************************************************************/

#ifdef O_FLYER_EXPORTS
#define O_FLYER_API __declspec(dllexport)
#else
#define O_FLYER_API __declspec(dllimport)
#endif


/*---------------------------------------------- ��������� ��������� */

/*------------------------ �������� ������ ��������� ������� "�����" */

  class O_FLYER_API RSS_Transit_Flyer : public RSS_Transit {

    public:
             virtual   int  vExecute(void) ;             /* ���������� �������� */
                                             
    public:
                            RSS_Transit_Flyer() ;        /* ����������� */
                           ~RSS_Transit_Flyer() ;        /* ���������� */

                                                           } ;

/*---------------------------------- �������� ������ ������� "�����" */

 struct RSS_Object_FlyerTrace {
                                  double  x_base ;
                                  double  y_base ;
                                  double  z_base ;

                                  double  a_azim ;
                                  double  a_elev ;
                                  double  a_roll ;

                                  double  x_velocity ;
                                  double  y_velocity ;
                                  double  z_velocity ;
                              } ;

  class O_FLYER_API RSS_Object_Flyer : public RSS_Object {

    public:
                      char  model_path[FILENAME_MAX] ;        /* ���� ������ */

     RSS_Object_FlyerTrace *mTrace ;                          /* ���������� */
                       int  mTrace_cnt ;  
                       int  mTrace_max ;
                  COLORREF  mTrace_color ;
                       int  mTrace_dlist ;

    public:
               virtual void  vFree          (void) ;            /* ���������� ������� */
               virtual void  vWriteSave     (std::string *) ;   /* �������� ������ � ������ */
               virtual  int  vCalculate     (double, double) ;  /* ������ ��������� ��������� */
                        int  iSaveTracePoint(void) ;            /* ���������� ����� ���������� */
                       void  iShowTrace_    (void) ;            /* ����������� ���������� � ��������� ��������� */
                       void  iShowTrace     (void) ;            /* ����������� ���������� */

	                     RSS_Object_Flyer() ;               /* ����������� */
	                    ~RSS_Object_Flyer() ;               /* ���������� */
                                                        } ;

/*---------------------- �������� ������ ���������� �������� "�����" */

  class O_FLYER_API RSS_Module_Flyer : public RSS_Kernel {

    public:

     static
      struct RSS_Module_Flyer_instr *mInstrList ;          /* ������ ������ */
				     
    public:
     virtual         int  vExecuteCmd   (const char *) ;    /* ��������� ������� */
     virtual        void  vReadSave     (std::string *) ;   /* ������ ������ �� ������ */
     virtual        void  vWriteSave    (std::string *) ;   /* �������� ������ � ������ */

    public:
                     int  cHelp         (char *) ;          /* ���������� HELP */ 
                     int  cCreate       (char *) ;          /* ���������� CREATE */ 
                     int  cInfo         (char *) ;          /* ���������� INFO */ 
                     int  cBase         (char *) ;          /* ���������� BASE */ 
                     int  cAngle        (char *) ;          /* ���������� ANGLE */ 
                     int  cVelocity     (char *) ;          /* ���������� VELOCITY */
                     int  cTrace        (char *) ;          /* ���������� TRACE */

        RSS_Object_Flyer *FindObject    (char *) ;          /* ����� ������� ���� BODY �� ����� */
                     int  CreateObject  (RSS_Model_data *); /* �������� ������� */ 

    public:
	                  RSS_Module_Flyer() ;              /* ����������� */
	                 ~RSS_Module_Flyer() ;              /* ���������� */
                                                       } ;

/*-------------------------------------------- ���������� ���������� */

 struct RSS_Module_Flyer_instr {

           char                   *name_full ;          /* ������ ��� ������� */
           char                   *name_shrt ;          /* �������� ��� ������� */
           char                   *help_row ;           /* HELP - ������ */
           char                   *help_full ;          /* HELP - ������ */
            int (RSS_Module_Flyer::*process)(char *) ;  /* ��������� ���������� ������� */
                               }  ;

/*--------------------------------------------- ���������� ��������� */

/* ����  O_Flyer.cpp */

/* ����  O_Flyer_dialog.cpp */
  BOOL CALLBACK  Object_Flyer_Help_dialog  (HWND, UINT, WPARAM, LPARAM) ;
  BOOL CALLBACK  Object_Flyer_Create_dialog(HWND, UINT, WPARAM, LPARAM) ;
