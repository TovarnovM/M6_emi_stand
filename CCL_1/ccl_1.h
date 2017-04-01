/*----------------------------------------------- '�������' �������� */


#ifdef  _CCL_1_MAIN
#define  _EXTERNAL
#else
extern "C" {
#define  _EXTERNAL  extern
#endif

/*---------------------------------------------- ��������� ��������� */

   struct L_entry {                          /* ���� ���.���� */
		   char  name[_NAME_MAX+1] ;  /* ��� ����� */
		    int  start ;              /* ����� ���������� ����� */
		    int  pars[50] ;           /* ������ ����������: ������� �����.���������� */
		    int  pars_cnt ;           /* ����� ���������� */
		  } ;

/*-------------------------------------- �������� ������� ���������� */

#ifndef  _NAME_MAX
#define  _NAME_MAX  15
#endif

_EXTERNAL struct L_variable *lng_vars ;         /* �������� ������� ���������� */
_EXTERNAL               int  lng_vars_cnt ;     /* ������� ������� ���������� */
_EXTERNAL struct L_function *lng_func ;         /* �������� ������� */
_EXTERNAL               int  lng_func_cnt ;     /* ������� ������� */

/*--------------------------------- '�����������' ������� ���������� */

 struct L_table {
		     int  type ;      /* ��� ������ */
		    void *objects ;   /* ������ �������� */
		} ;

_EXTERNAL struct L_table *lng_table ;  /* ������� ������� ���������� */

/* ��������! ������� ������� ������������� NULL-����������,       */
/*             ������ LANG ��������� - ��������� � ������ ������, */
/*             � CCL � DCL ���������� - ��������� � NULL ������.  */

/* ��������! ����� ������� � ��������� CCL � DCL �� �������������� */

/*----------------------------------- ������� ������������ ��������� */

_EXTERNAL void *(*lng_ext_lib)(int, char *) ; /* ������� ������� ���������� */

/* ������ ���������� ���������������� ������� lng_ext_lib             */
/* �������� ��� ������������� �������, � ������ - ��� ����������      */
/* ���. ���� ������ ������ - ������������ ��������� �� ��� ��������,  */
/* � ���� ��� - NULL.                                                 */

 _EXTERNAL     int   lng_ext_load_flag ;                      /* ���� ���������� ������� ��������� */

 _EXTERNAL  double (*lng_ext_exec)(char *, double *, int) ;   /* ������� ������� ��������� */

/* ������ ���������� ���������������� ������� lng_ext_exec             */
/* �������� ���������� ��� ��������� ���������, ������ - ����� ������� */
/* ����������, ������� - ����� ����������. ������������ ������         */
/* �������� ��������.                                                  */

 _EXTERNAL    void  *lng_ext_addr ;                           /* ����� ������� ��������� */

/*-------------------------------------------------- ������� ������� */

 _EXTERNAL    int  lng_debug ;              /* ���� ������� */

 _EXTERNAL  void (*lng_user_debug)(int) ;   /* ���������������� ���� ������� */

/* ������ ���������� ������� lng_user_debug ���������� ����� ����� */
/* ������������ ����.                                              */

/*---------------------------------------------- ���������� �������� */

   struct L_module {                         /* �������� ������������ ������ */
		     void   *ctrl_buff ;         /* ����� ������������ ���� */
		      int    ctrl_size ;         /* ������ ������ ������������ ���� */
		      int    ctrl_cnt ;          /* ������ ������������ ����, ������ */
		      int    ctrl_used ;         /* ������.������ ������, ���� */
		      int    ctrl_frame ;        /* ����� ����� ������������ ���� */

		     void   *culc_buff ;         /* ����� ��������������� ���� */
		      int    culc_size ;         /* ������ ������ ��������������� ���� */
		      int    culc_cnt ;          /* ������ ��������������� ����, ������ */
		      int    culc_used ;         /* ������.������ ������, ���� */
		      int    culc_frame ;        /* ����� ����� ��������������� ���� */

		     char *(*get_text)(void) ;   /* ������ �� �/� ������ ������ */
		      int  (*test_name)(int,     /* ������ �� �/� �������� ���� */
					char *) ;

	struct L_variable   *ext_vars ;          /* �������� ������� ���������� */
		      int    ext_vars_cnt ;      /* ������� ������� ���������� */
	struct L_function   *ext_func ;          /* �������� ������� */
		      int    ext_func_cnt ;      /* ������� ������� */
	   struct L_table   *ext_table ;         /* ������� ������� ���������� */
		     void *(*ext_lib)(int,       /* ������� ������� ���������� */
				      char *) ;
		   double  (*ext_exec)(char *,   /* ������� ������� ��������� */
				       double *, int) ;

	struct L_variable   *int_vars ;          /* ������ ���������� ���������� */
		      int    int_vars_max ;      /* ����.����� ���������� ���������� */
		      int    int_vars_cnt ;      /* ������� ���������� ���������� */
		   double   *work_vars ;         /* ������ ������� ����� */
		      int    work_vars_max ;     /* ����.����� ������� ����� */
		      int    work_vars_cnt ;     /* ������� ������� ����� */

	   struct L_entry   *entry_list ;
		      int    entry_max ;
		      int    entry_cnt ;
		   } ;
   
/*    ������� int_vars � work_vars �������� ���������������!        */
/*    ������������ ������ �������� int_vars � work_vars �� �����    */
/*  ��������� 255 ����� ��� ������� � ������������� �� ������������ */
/*  �������� ������ ���������� 6120 � 2040 ��������������.          */

/*------------------------------------- �������� ��������� ��������� */

 struct L_warning {
		     int  code ;  /* ��� �������������� */
		     int  row ;   /* ������ ������ � ��������������� */
		  } ;

_EXTERNAL              int  lng_error ;      /* ��������� ������ */
_EXTERNAL             char *lng_bad ;        /* ��������� ���������� ����� */
_EXTERNAL              int  lng_row  ;       /* ��������� ������ ������ */

#define       _LNG_WARN_MAX     100
_EXTERNAL struct L_warning  lng_warn[_LNG_WARN_MAX] ;  /* ������ ����������� �������������� */
_EXTERNAL              int  lng_warn_cnt ;             /* ������� �������������� */


#define   _EL_GET_TEXT      999    /* �� ������ ��������� ������ ������ */
#define   _EL_CTRL_BUFF       1    /* ������������ ������ ������.���� */
#define   _EL_CULC_BUFF       2    /* ������������ ������ �������� ���� */
#define   _EL_CYR_CHAR        3    /* "�������" ������ � ������ */
#define   _EL_MARK_BAD        4    /* ������������ ��� ������� ������� ����� */
#define   _EL_MARK_DBL        5    /* ��������� ����� */
#define   _EL_MARK_UNK        6    /* ����������� ����� */
#define   _EL_END_MARK        7    /* �������� END �������� ����� */
#define   _EL_END_FILE        8    /* ����������� �������� END */
#define   _EL_NAME_LEN        9    /* ������� ������� ��� */
#define   _EL_NAME_EMPTY     10    /* ������ ��� */
#define   _EL_RET_OUT        11    /* � ������ ����������� RETURN */
#define   _EL_RET_FMT        12    /* ������������ ������ ��������� RETURN */
#define   _EL_RET_NAME       13    /* �����. ����. RETURN - ��������� ��� ������.��� */
#define   _EL_RET_UNK        14    /* ��� RETURN-���������� ���������� */
#define   _EL_IF_FMT         15    /* ������������ ������ ��������� IF */
#define   _EL_ELSE_MARK      16    /* �������� ELSE �������� ����� */
#define   _EL_ELSE_UNK       17    /* ELSE ��� IF */
#define   _EL_BLOCK_UNK      18    /* ������� ������� ���������� ���� */
#define   _EL_UNCLOSE        19    /* "����������" ��������� IF ��� DO */
#define   _EL_DO_FMT         20    /* ������������ ������ ��������� DO */
#define   _EL_EXT_CRC        21    /* BREAK ��� CONTINUE ��� ����� */
#define   _EL_TEXT_OPEN      24    /* �������� ��������� ��������� */
#define   _EL_INV_CHAR       25    /* ������������ Escape-������������������ */
#define   _EL_DECL_FMT       26    /* ������������ ������ �������� */
#define   _EL_DECL_SIZE      27    /* ������������ ������ ���������� */
#define   _EL_NAME_USED      28    /* ��������� ������������� ����� */
#define   _EL_ENTRY_UNK      29    /* ����������� ����� ENTRY-����� */

#define   _EL_FUNC_BEFORE    31   /* ����� �������� ���������� ������ */
#define   _EL_FUNC_FMT       32   /* ����������� ������ ������ ������� */
#define   _EL_FUNC_SINTAX    34   /* ����������� ��� ������������ �������� */
#define   _EL_FUNC_UNK       35   /* ����������� ��� ������� */
#define   _EL_OPEN           36   /* ������ ����������� ������ */
#define   _EL_CLOSE          37   /* ������ ����������� ������ */
#define   _EL_EMPTY          38   /* ������ ��������� (������������ � �������) */
#define   _EL_NAME_UNK       39   /* ����������� ��� ���������� */
#define   _EL_NAME_SINTAX    40   /* ����������� ��� ���������� */
#define   _EL_INDEX          41   /* ���������� ��������� ��������� */
#define   _EL_SINTAX1        51   /* ������ ���������� */
#define   _EL_SINTAX2        52   /* ������ ���������� */
#define   _EL_SINTAX3        53   /* ������ ���������� */
#define   _EL_SINTAX4        54   /* ������ ���������� */
#define   _EL_SINTAX5        55   /* ������ ���������� */

#define   _EL_STD_OUT        61   /* ����� ������� ��������� CCL ��� DCL */
#define   _EL_VARS_OUT       62   /* ����� ����������, �������� �����   */
				  /*   lng_vars �� ����� ���� ����� 255 */
#define   _EL_FUNC_OUT       63   /* ����� �������, �������� �����      */
				  /*   lng_func �� ����� ���� ����� 255 */

#define   _EL_LEN_MAX        91   /* ������� ������� �������� ������ */
#define   _EL_IFDO_MAX       93   /* ������� �������� ����������� ���������� IF � DO */
#define   _EL_INT_VAR_MAX    94   /* ������� ����� ���������� ���������� */
#define   _EL_WRK_VAR_MAX    95   /* ������� ����� ������� ����� � ������ */
#define   _EL_PARS_MAX       96   /* ������� ����� ���������� */
#define   _EL_TEXT_MAX       97   /* ������� ����� �������� �������� */
#define   _EL_INDEX_MAX      98   /* ������� ����� �������� */
#define   _EL_AREA_MAX       99   /* ������������ ������ ���������� �������� */
#define   _EL_ENTRY_MAX     100   /* ������������ ������ ������ */
#define   _EL_GOTO_OVR       22   /* ������� ����� ����� ��� GOTO-��������� */
#define   _EL_IFDO_OVR       23   /* ������������ IFDO-����� */

#define   _EL_UNK_ENTRY     201   /* ����������� ���� ������ */

#define   _WL_EXT_CHNG        1   /* ������� ��������� ������� ���������� */
#define   _WL_FUNC_PARS      33   /* �������������� ����� ���������� ������� */

/*--------------------------------------------- ��������� ���������� */

_EXTERNAL    char *(*L_get_text )(void) ;         /* ������ �� �/� ������ ������ */
_EXTERNAL     int  (*L_test_name)(int, char *) ;  /* ������ �� �/� �������� ����� */
#define                  _L_POINTER_NAME   0

_EXTERNAL     int    lng_ctrl_abort ;             /* ���� ���������� ���������� ���� ��������� */
_EXTERNAL     int    lng_culc_abort ;             /* ���� ������ ���������� ��������������� ��������� */

_EXTERNAL     int    lng_pars_cnt ;               /* ����� ���������� ���������� */

_EXTERNAL    long    lng_ctrl_used ;              /* ������ ������������ ���� */
_EXTERNAL    long    lng_culc_used ;              /* ������ ��������������� ���� */

/*---------------------------------------------- �������� ���������� */

    int  L_compile      (struct L_module *) ;    /* ���������� ������ */
 double  L_execute      (struct L_module *,      /* ���������� ������ */
			  char *, double *, int) ;

   void  L_debug_set    (int) ;                  /* ���. ������ ������� */
 double  L_set_pointer  (void *) ;               /* ����������� ������ */
   void *L_get_pointer  (double) ;               /* ������������� ������ */

    int  L_ctrl_buff    (char *, int) ;          /* ������� ������ ������������ ���� */
    int  L_culc_buff    (char *, int) ;          /* ������� ������ �������� ���� */
    int  L_area_buff    (char *, int) ;          /* ������� ������ ���������� �������� */
   void *L_int_vars     (int *) ;                /* ������ ����� ������� ���������� ���������� */
    int  L_ctrl_maker   (void) ;                 /* ���������� ������������ ���������� */

    int  L_decl_maker   (char *) ;               /* ��������� �������� */
    int  L_ctrl_code    (int, double *, int) ;   /* ������������� ������������ ���������� */
   void  L_ied_close    (int) ;                  /* �������� IF, ELSE � DO ���������� */
   char *L_equ_maker    (char *) ;               /* ��������� ��������� ���������� */
    int  L_alloc_var    (char *) ;               /* ���������� ���������� ���������� */

   char *L_culc_maker   (char *) ;               /* ���������� ��������������� ���������� */
   char *L_simple_string(char *) ;               /* ��������� ������� ����� */
   char *L_simple_unit  (char *) ;               /* ��������� ��������� ��������� ��� ���������� */
    int  L_table_objects(int, char *) ;          /* ������ � ��������� ������� ������� LNG_TABLE */
#define  _FIND_VAR   1
#define  _FIND_FUNC  2
    int  L_culc_code    (int, char *, char *) ;  /* ������������� ��������������� ���������� */
    int  L_escape_dcd   (char *, char *) ;       /* ��������� Escape-������������������� */

    int  L_decl_entry   (char *, double *, int); /* '������' ����� */
 double  L_ctrl_exec    (void) ;                 /* ���������� ������������ ���������� �� 0-����� */
 double  L_culc_exec    (int) ;                  /* ����������� ������ ��������������� ���������� */

#ifdef  _CCL_1_MAIN
#else
}
#endif
