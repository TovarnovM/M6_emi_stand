
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

/*------------------------------------ �������� ��������� ���������� */

 struct RSS_Object_FlyerPFrame {
                                     char  used[128] ;
                                   double  t ;
                                   double  x, y, z ;
                                   double  a, d_a, t_a ;
                                   double  e, d_e, t_e ;
                                   double  r, d_r, t_r ;
                                   double  g, d_g, t_g ;
                                   double  v, d_v, t_v ;
                               } ;

#define  _PFRAMES_MAX   100
 class RSS_Object_FlyerProgram {

    public:
                                  char  name[128] ;
                RSS_Object_FlyerPFrame  frames[_PFRAMES_MAX] ;
                                   int  frames_cnt ;

    public:
                            RSS_Object_FlyerProgram() ;        /* ����������� */
                           ~RSS_Object_FlyerProgram() ;        /* ���������� */
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

                    double  v_abs ;                           /* ���������� �������� */
                    double  g_ctrl ;                          /* ���������� ����������� ���������� */

#define   _PROGRAMS_MAX  10
   RSS_Object_FlyerProgram *programs[_PROGRAMS_MAX] ;         /* ������ �������� ���������� */
   RSS_Object_FlyerProgram *program ;                         /* ������� ��������� ���������� */
                       int  p_frame ;

    RSS_Object_FlyerPFrame  p_controls ;                      /* ����������� ����������� ��������� */

   private:
                    double  r_ctrl ;                          /* ��������� ��������� �������� */
            class Matrix2d *m_ctrl ;
                    double  a_ctrl ;

     RSS_Object_FlyerTrace *mTrace ;                          /* ���������� */
                       int  mTrace_cnt ;  
                       int  mTrace_max ;
                  COLORREF  mTrace_color ;
                       int  mTrace_dlist ;

    public:
               virtual void  vFree          (void) ;            /* ���������� ������� */
               virtual void  vWriteSave     (std::string *) ;   /* �������� ������ � ������ */
               virtual  int  vCalculate     (double, double) ;  /* ������ ��������� ��������� */
                        int  iExecuteProgram(double, double) ;  /* ��������� ������������ ���������� */
                        int  iSaveTracePoint(char *) ;          /* ���������� ����� ���������� */
                       void  iShowTrace_    (void) ;            /* ����������� ���������� � ��������� ��������� */
                       void  iShowTrace     (void) ;            /* ����������� ���������� */

	                     RSS_Object_Flyer() ;               /* ����������� */
	                    ~RSS_Object_Flyer() ;               /* ���������� */
                                                        } ;

/*---------------------- �������� ������ ���������� �������� "�����" */

  class O_FLYER_API RSS_Module_Flyer : public RSS_Kernel {

    public:

     static
      struct RSS_Module_Flyer_instr *mInstrList ;           /* ������ ������ */

                  double  g_step ;                          /* ��� ��������� ����������� ���������� */
                  double  a_step ;                          /* ��� ��������� ����� */
				     
    public:
     virtual         int  vExecuteCmd   (const char *) ;    /* ��������� ������� */
     virtual        void  vReadSave     (std::string *) ;   /* ������ ������ �� ������ */
     virtual        void  vWriteSave    (std::string *) ;   /* �������� ������ � ������ */

    public:
                     int  cHelp         (char *) ;                     /* ���������� HELP */ 
                     int  cCreate       (char *) ;                     /* ���������� CREATE */ 
                     int  cInfo         (char *) ;                     /* ���������� INFO */ 
                     int  cBase         (char *) ;                     /* ���������� BASE */ 
                     int  cAngle        (char *) ;                     /* ���������� ANGLE */ 
                     int  cVelocity     (char *) ;                     /* ���������� VELOCITY */
                     int  cControl      (char *) ;                     /* ���������� CONTROL */
                     int  cProgram      (char *) ;                     /* ���������� PROGRAM */
                     int  cTrace        (char *) ;                     /* ���������� TRACE */

        RSS_Object_Flyer *FindObject    (char *) ;                     /* ����� ������� ���� BODY �� ����� */
                     int  CreateObject  (RSS_Model_data *) ;           /* �������� ������� */ 
                     int  iReadProgram  (RSS_Object_Flyer *, char *) ; /* ���������� ����� �������� ��������� */

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
