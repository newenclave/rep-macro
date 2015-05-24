//      r-macro.h
//
//      Copyright 2006 newenclave
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.
//
//

#ifndef R_MACRO_HEADER
#define R_MACRO_HEADER

#define  YES    1
#define  NO     0

//#define RM_UNICODE

#ifdef  RM_UNICODE

typedef  unsigned short     RM_CHAR; // wchar_t
#define  RM_T(x)            L ## x

#else

typedef  char           RM_CHAR;
#define  RM_T(x)        x

#endif

#define  REP_MACRO_ENABLE_REP_SUPPORT           YES // Enabling "{...}" sequence
#define  REP_MACRO_ENABLE_SUBSTRINGS_SUPPORT    YES // Enabling "[...]" sequence
#define  REP_MACRO_ENABLE_SUBMACRO_SUPPORT      YES // Enabling "submacros"

#define  MACROS_PARAM_OPEN              RM_T('(')
#define  MACROS_PARAM_CLOSE             RM_T(')')
#define  MACROS_PARAM_BRACKET           RM_T('\"')
//#define  MACROS_PARAM_BRACKET           RM_T('<')
//#define  MACROS_PARAM_BRACKET_CLOSE     RM_T('>')
#define  MACROS_PARAM_COMMA             RM_T(',')

#define  MACROS_ESCAPE                  RM_T('\\')
#define  MACROS_ESCAPE_TAB              RM_T('t')
#define  MACROS_ESCAPE_VTAB             RM_T('v')
#define  MACROS_ESCAPE_BS               RM_T('b')
#define  MACROS_ESCAPE_ALARM            RM_T('a')
//#define  MACROS_ESCAPE_CLN            RM_T('c')
#define  MACROS_ESCAPE_FF               RM_T('f')
#define  MACROS_ESCAPE_NEWLINE          RM_T('n')
#define  MACROS_ESCAPE_RETURN           RM_T('r')
//#define  MACROS_ESCAPE_ZERO_          RM_T('z')
#define  MACROS_ESCAPE_EMPTY            RM_T('e')

#define  MACROS_ESCAPE_TAB_VAL          RM_T('\t')
#define  MACROS_ESCAPE_VTAB_VAL         RM_T('\v')
#define  MACROS_ESCAPE_BS_VAL           RM_T('\b')
#define  MACROS_ESCAPE_ALARM_VAL        RM_T('\a')
//#define  MACROS_ESCAPE_CLN_VAL        RM_T('\c')
#define  MACROS_ESCAPE_FF_VAL           RM_T('\f')
#define  MACROS_ESCAPE_NEWLINE_VAL      RM_T('\n')
#define  MACROS_ESCAPE_RETURN_VAL       RM_T('\r')
//#define  MACROS_ESCAPE_ZERO_VAL       RM_T('\0')

/* !!!!!!!!!!!! */
#define  MACROS_END_OF_LINE             RM_T('\0')
/* !!!!!!!!!!!! */

#define  MACROS_ESCAPE_OCT              RM_T('0')
#define  MACROS_ESCAPE_HEX              RM_T('x')
#define  MACROS_ESCAPE_BIN              RM_T('B')
#define  MACROS_ESCAPE_QDR              RM_T('q')
#define  MACROS_ESCAPE_TPL              RM_T('T')


/**********//*! REPEAT SUPPORT !*//**********/
#if (REP_MACRO_ENABLE_REP_SUPPORT == YES)

#define  MACROS_ESCAPE_REP_START        RM_T('{')
#define  MACROS_ESCAPE_REP_STOP         RM_T('}')

#endif
/**********//*! END REPEAT SUPPORT !*//**********/


/**************//*! SUBSTRINGS SUPPORT !*//****************/
/*substrings does't work without MACROS_ESCAPE_REP_START  */

#if   ( REP_MACRO_ENABLE_REP_SUPPORT == YES && \
        REP_MACRO_ENABLE_SUBSTRINGS_SUPPORT == YES)

#define  MACROS_SUBSTR_START            RM_T('[')
#define  MACROS_SUBSTR_STOP             RM_T(']')

#endif
/**********//*! END SUBSTRINGS SUPPORT !*//**********/

/**********//*! SUBMACRO SUPPORT !*//**********/
#if (REP_MACRO_ENABLE_SUBMACRO_SUPPORT == YES)

#define  MACROS_ALLOW_SUBMACRO

#endif
/**********//*! END SUBMACRO SUPPORT !*//**********/


#define  MACROS_MAX_REP                 4096

#define  MACROS_BIN_BASE                2
#define  MACROS_TPL_BASE                3
#define  MACROS_QDR_BASE                4
#define  MACROS_OCT_BASE                8
#define  MACROS_DEC_BASE                10
#define  MACROS_HEX_BASE                16

//#define  MACROS_ESCAPE_ZERO_NOTDEF

#define  MACROS_SPACE_1                 RM_T(' ')
#define  MACROS_SPACE_2                 RM_T('\t')
#define  MACROS_SPACE_3                 RM_T('\n')
#define  MACROS_SPACE_4                 RM_T('\r')

/*
#define  MACROS_SPACE_5                 ''
#define  MACROS_SPACE_6                 ''
#define  MACROS_SPACE_7                 ''
#define  MACROS_SPACE_8                 ''
*///Do you need more? :)

#define  MACROS_PARAM_COUNT_            1+1+10+1 // parent + macro name
                                                 //  + 10 params
                                                 //  + zero pointer = 13
#define  MACROS_BUFFER_LEN_             4096     // for buffer in function
#define  MACROS_BACKUP_COUNT_           10       // for backcalls

#define  REP_DEFAULT                    0
#define  REP_CONTINUE_ON_BUFFOVERFLOW   1
#define  REP_MACROS_WITH_BUFFER        (1 << 1)
#define  REP_ENABLE_ESC                (1 << 2)
#define  REP_ENABLE_REP                (1 << 3)
#define  REP_ENABLE_SUBSTR             (1 << 4)
#define  REP_ENABLE_SUBMACRO           (1 << 5)

#define  REP_INSTR_IS_POINTER          (1 << 6) // for internal usage

#define  REP_ENABLE_ALL                 REP_ENABLE_ESC | \
                                        REP_ENABLE_REP | \
                                        REP_ENABLE_SUBSTR | \
                                        REP_ENABLE_SUBMACRO

/* FUNCTION for _macro*/

struct rep_header {
    struct  rep_header *parent;
    RM_CHAR *first_name;
    RM_CHAR *params[1];
};

typedef const RM_CHAR *(*REP_MACRO_FUNC) ( void *,        /* user data */
                                           void *,        /* header */
                                           unsigned int   /* flags */
                                         );

#ifdef __cplusplus
extern "C" {
#endif
    long rep_macro( const void *input,
                    RM_CHAR separator, RM_CHAR *output,
                    long outmax,
                    unsigned int flags,
                    REP_MACRO_FUNC cb_translate,
                    void *user_data );
#ifdef __cplusplus
}
#endif

#endif /*! R_MACRO_HEADER !*/


