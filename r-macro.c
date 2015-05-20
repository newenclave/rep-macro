//      r-macro.c
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

#include "r-macro.h"

//  RM_CHAR *_macro( RM_CHAR **, RM_CHAR, REP_MACRO_FUNC,
//                   void *, unsigned int, RM_CHAR **);

/*   MAIN   */
/*************************************/
RM_CHAR *_macro( RM_CHAR        **instr,
                 RM_CHAR          al_sep,
                 REP_MACRO_FUNC   al_func,
                 void            *al_par,
                 unsigned int     flags,
                 RM_CHAR        **parent_macros )
{

#define INCMACI maci += (maci < MACROS_BUFFER_LEN_-1)
#define INCPARI pari += (pari < (MACROS_PARAM_COUNT_-1))
#define INCBACKI (backi += (backi < (MACROS_BACKUP_COUNT_-1)))
#define DECBACKI (backi -= (backi > 0))

    RM_CHAR *in, *mc;

#ifdef MACROS_ALLOW_SUBMACRO
    RM_CHAR *back[MACROS_BACKUP_COUNT_];
    unsigned int backi;
#endif

    RM_CHAR mac_str[MACROS_BUFFER_LEN_];
    RM_CHAR *macros[MACROS_PARAM_COUNT_];

#ifdef   MACROS_ESCAPE_REP_START
    enum escstate { escape_no,
                    escape_slash,
                    escape_rep,
                    escape_cont,
                    escape_cont_rep
                  } escape;
#else
    enum escstate { escape_no,
                    escape_slash,
                    escape_cont,
                  } escape;
#endif

#ifdef   MACROS_SUBSTR_START

    RM_CHAR *stack;
    enum mstate   { macro_no,
                    macro_start,
                    macro_substr,
                    macro_sub_stop
                  } macro, bmacro;

#else
    enum mstate   { macro_no,
                    macro_start,
                  } macro;
#endif

    int  maci, pari;
    int  brak;

    enum pstate {   param_no,
                    param_start,
                    param_proc,
                    param_comma,
                    param_space
                } st;

    unsigned int val_base = 0;
    unsigned int val      = 0;
    unsigned int cur_val  = 0;

    in = *instr;

    st          = param_no;
    escape      = escape_no;
    macros[0]   = (RM_CHAR *)parent_macros;
    macros[1]   = mac_str;
    brak = maci = 0;
    macro       = macro_no;
#ifdef   MACROS_SUBSTR_START
    bmacro      = macro;
    stack       = 0;
#endif

#ifdef MACROS_ALLOW_SUBMACRO

    backi   = 0;
    back[0] = 0;

#endif

    maci = 0;
    pari = 2;

#ifdef MACROS_ALLOW_SUBMACRO

__lets_begin:

#endif

    for( ; *in; in++ ) {  // MAIN FOR
/* proccess \\ */

#ifdef   MACROS_SUBSTR_START
        if(macro != macro_start && macro != macro_substr){
#else
        if(macro == macro_no){
#endif

#ifdef MACROS_ALLOW_SUBMACRO
            if ( (*in == al_sep) && (flags & REP_ENABLE_SUBMACRO) && !brak ) {

#ifdef   MACROS_SUBSTR_START //----+
                bmacro = macro; // |
#endif  // <<----------------------+

                macro = macro_start;
                switch (st){
                    case param_space:
                        mac_str[maci] = MACROS_END_OF_LINE;
                        INCMACI;
                        macros[pari] = &mac_str[maci];
                        INCPARI;
                    case param_comma:
                    case param_start:
                        st = param_proc;
                        break;
                    default:
                        break;
                }
                continue;
            }
#endif
//#endif
            switch (escape){ // SWITCH 001
                case escape_slash:
                    {
                    escape = escape_no;
                    macro = macro_no;
                    switch (*in){
#ifdef MACROS_ESCAPE_EMPTY
                        case MACROS_ESCAPE_EMPTY:
                            escape = escape_no;
                            continue;
#endif
                        case MACROS_ESCAPE_TAB:
                            mac_str[maci] = MACROS_ESCAPE_TAB_VAL;
                            INCMACI;
                            continue;
                        case MACROS_ESCAPE_VTAB:
                            mac_str[maci] = MACROS_ESCAPE_VTAB_VAL;
                            INCMACI;
                            continue;
                        case MACROS_ESCAPE_NEWLINE:
                            mac_str[maci] = MACROS_ESCAPE_NEWLINE_VAL;
                            INCMACI;
                            continue;
                        case MACROS_ESCAPE_RETURN:
                            mac_str[maci] = MACROS_ESCAPE_RETURN_VAL;
                            INCMACI;
                            continue;
                        case MACROS_ESCAPE_BS:
                            mac_str[maci] = MACROS_ESCAPE_BS_VAL;
                            INCMACI;
                            continue;
                        case MACROS_ESCAPE_ALARM:
                            mac_str[maci] = MACROS_ESCAPE_ALARM_VAL;
                            INCMACI;
                            continue;
#ifdef MACROS_ESCAPE_CLN
                        case MACROS_ESCAPE_CLN:
                            mac_str[maci] = MACROS_ESCAPE_CLN_VAL;
                            INCMACI;
                            continue;
#endif

                        case MACROS_ESCAPE_FF:
                            mac_str[maci] = MACROS_ESCAPE_FF_VAL;
                            INCMACI;
                            continue;
/*!
                        case MACROS_ESCAPE_ZERO_:
                            mac_str[maci] = MACROS_ESCAPE_ZERO_VAL;
                            INCMACI;
                            continue;
*/
#ifdef MACROS_SPACE_2
                        case MACROS_SPACE_2:
#endif
#ifdef MACROS_SPACE_3
                        case MACROS_SPACE_3:
#endif
#ifdef MACROS_SPACE_4
                        case MACROS_SPACE_4:
#endif
#ifdef MACROS_SPACE_5
                        case MACROS_SPACE_5:
#endif
#ifdef MACROS_SPACE_6
                        case MACROS_SPACE_6:
#endif
#ifdef MACROS_SPACE_7
                        case MACROS_SPACE_7:
#endif
#ifdef MACROS_SPACE_8
                        case MACROS_SPACE_8:
#endif
#ifdef MACROS_SPACE_1
                        case MACROS_SPACE_1:
                            mac_str[maci] = *in;
                            INCMACI;
                            continue;
#endif
#ifdef   MACROS_SUBSTR_START
                        case MACROS_SUBSTR_START:
                        case MACROS_SUBSTR_STOP:
                            if ( !(flags & REP_ENABLE_SUBSTR)){
                                mac_str[maci] = MACROS_ESCAPE;
                                INCMACI;
                            }
#endif
#ifdef   MACROS_ESCAPE_REP_START
                        case MACROS_ESCAPE_REP_START:
                        case MACROS_ESCAPE_REP_STOP:
                            if ( !(flags & REP_ENABLE_REP)){
                                mac_str[maci] = MACROS_ESCAPE;
                                INCMACI;
                            }
#endif
                        case MACROS_ESCAPE:
                            mac_str[maci] = *in;
                            INCMACI;
                            continue;

                        case MACROS_PARAM_COMMA:
                            mac_str[maci] = MACROS_PARAM_COMMA;
                            INCMACI;
                            continue;

                        case MACROS_PARAM_BRACKET:
                            mac_str[maci] = MACROS_PARAM_BRACKET;
                            INCMACI;
                            continue;

#ifdef MACROS_PARAM_BRACKET_CLOSE                           // if end-symbol
                                                            // defined
#if (MACROS_PARAM_BRACKET_CLOSE != MACROS_PARAM_BRACKET)    // if end-symbol !=
                                                            // begin-sym
                        case MACROS_PARAM_BRACKET_CLOSE:
                            mac_str[maci] = MACROS_PARAM_BRACKET_CLOSE;
                            INCMACI;
                            continue;
#endif
#endif
                        case  MACROS_PARAM_OPEN:
                            mac_str[maci] =  MACROS_PARAM_OPEN;
                            INCMACI;
                            continue;


#if (MACROS_PARAM_OPEN != MACROS_PARAM_CLOSE)
                        case  MACROS_PARAM_CLOSE:
                            mac_str[maci] =  MACROS_PARAM_CLOSE;
                            INCMACI;
                            continue;
#endif

#ifdef MACROS_PARAM_BRACKET_CLOSE
#if (MACROS_PARAM_BRACKET_CLOSE != MACROS_PARAM_BRACKET_CLOSE)
                        case MACROS_PARAM_BRACKET_CLOSE:
                            mac_str[maci] = MACROS_PARAM_BRACKET_CLOSE;
                            INCMACI;
                            continue;
#endif
#endif
                    }
                    }// CASE escape_slash
#ifdef   MACROS_ESCAPE_REP_START
                case escape_rep:
#endif
                    {
                    switch (*in) {
#ifdef MACROS_ESCAPE_BIN
                        case MACROS_ESCAPE_BIN:
                            val_base = MACROS_BIN_BASE;
                            val      = 0;
                            goto __proc_base;
#endif
#ifdef MACROS_ESCAPE_TPL
                        case MACROS_ESCAPE_TPL:
                            val_base = MACROS_TPL_BASE;
                            val      = 0;
                            goto __proc_base;
#endif
#ifdef MACROS_ESCAPE_QDR
                        case MACROS_ESCAPE_QDR:
                            val_base = MACROS_QDR_BASE;
                            val      = 0;
                            goto __proc_base;
#endif
#ifdef MACROS_ESCAPE_OCT
                        case MACROS_ESCAPE_OCT:
                            val_base = MACROS_OCT_BASE;
                            val      = 0;
                            goto __proc_base;
#endif
#ifdef MACROS_ESCAPE_HEX
                        case MACROS_ESCAPE_HEX:
                            val_base = MACROS_HEX_BASE;
                            val      = 0;
                            goto __proc_base;
#endif
                        case RM_T('1'):
                        case RM_T('2'):
                        case RM_T('3'):
                        case RM_T('4'):
                        case RM_T('5'):
                        case RM_T('6'):
                        case RM_T('7'):
                        case RM_T('8'):
                        case RM_T('9'):
#ifdef MACROS_ESCAPE_ZERO_NOTDEF
                        case RM_T('0'):
#endif
                            val_base = MACROS_DEC_BASE;
                            val      = *in - RM_T('0');
__proc_base:
#ifdef   MACROS_ESCAPE_REP_START
                            escape = ( (escape == escape_rep) ?
                                        escape_cont_rep : escape_cont);
#else
                            escape = escape_cont;
#endif
                            continue;
#ifdef   MACROS_SUBSTR_START
                        case MACROS_SUBSTR_START:
                            if(flags & REP_ENABLE_SUBSTR){
                                mac_str[maci] = MACROS_ESCAPE_REP_START;
                                INCMACI;
                                macro = macro_substr;
                                escape = escape_no;
                                continue;
                            }
                            break;
                        case MACROS_SUBSTR_STOP:
                            if(flags & REP_ENABLE_SUBSTR){
                                mac_str[maci] = MACROS_ESCAPE_REP_START;
                                INCMACI;
                                goto __proc_escape_stop;
                            }
                            break;
#endif
#ifdef   MACROS_ESCAPE_REP_START
                        case MACROS_ESCAPE_REP_START:
                                mac_str[maci] = MACROS_ESCAPE_REP_START;
                                INCMACI;
                                escape = escape_rep;
                                continue;
                        case MACROS_ESCAPE:
                                mac_str[maci] = MACROS_ESCAPE_REP_START;
                                escape = escape_slash;
                                INCMACI;
                                continue;
#endif
                        default: // undefined char;
#ifdef   MACROS_ESCAPE_REP_START
                            mac_str[maci] = ( (escape == escape_rep)
                                              ? MACROS_ESCAPE_REP_START
                                              : MACROS_ESCAPE);
#else
                            mac_str[maci] = MACROS_ESCAPE;

#endif
                            escape = escape_no;
                            INCMACI;
                            goto __post_esc;
                    }
                    }   // case escape_rep

                default:
                    break;
            } // END Switch 001

#ifdef   MACROS_ESCAPE_REP_START
            if( (escape == escape_cont) || (escape == escape_cont_rep)) {
#else
            if( escape == escape_cont ) {
#endif
                switch (*in){
                case RM_T('a'):
                case RM_T('b'):
                case RM_T('c'):
                case RM_T('d'):
                case RM_T('e'):
                case RM_T('f'):
                    cur_val = *in - RM_T('a') + 10;
                    goto __proc_escape;
                case RM_T('A'):
                case RM_T('B'):
                case RM_T('C'):
                case RM_T('D'):
                case RM_T('E'):
                case RM_T('F'):
                    cur_val = *in - RM_T('A') + 10;
                    goto __proc_escape;
                case RM_T('1'):
                case RM_T('2'):
                case RM_T('3'):
                case RM_T('4'):
                case RM_T('5'):
                case RM_T('6'):
                case RM_T('7'):
                case RM_T('8'):
                case RM_T('9'):
                case RM_T('0'):
                    cur_val = *in - RM_T('0');
__proc_escape:
                    if (val_base <= cur_val) {
                        goto __char_notdef;
                    }
                    val = val * val_base + (cur_val);
                    continue;
#ifdef   MACROS_ESCAPE_REP_START
                case MACROS_ESCAPE_REP_STOP: // just in case
#endif
__char_notdef:
                default:
#ifdef   MACROS_ESCAPE_REP_START
                    switch (escape){
                        case escape_cont:
                            mac_str[maci] = (RM_CHAR)(val);
                            INCMACI;
                            break;
                        case escape_cont_rep:
                            escape = escape_no;
#ifdef MACROS_SUBSTR_START
                            switch (macro){
                                case macro_no:
                                    if( (&mac_str[maci]) > macros[pari-1] ) {
                                        cur_val = mac_str[maci-1];
                                        for (val = val <= MACROS_MAX_REP
                                                 ? val
                                                 : MACROS_MAX_REP;
                                             val > 1; val--)
                                        {
                                            mac_str[maci] = (RM_CHAR)(cur_val);
                                            INCMACI;
                                        }
                                    }
                                    break; // switch (st)
                                case macro_sub_stop:
                                    macro = macro_no;
                                    for (val = val <= MACROS_MAX_REP
                                             ? val
                                             : MACROS_MAX_REP; val>1; val--)
                                    {
                                        cur_val = rep_macro(stack, al_sep,
                                            &mac_str[maci],
                                            (int)(MACROS_BUFFER_LEN_ - maci),
                                            flags & ~REP_INSTR_IS_POINTER,
                                            al_func, al_par);
                                        maci += cur_val;
                                    }
                                default:
                                    break;
                            }
#else
                            if( (&mac_str[maci]) > macros[pari-1] ){
                                cur_val = mac_str[maci-1];
                                for (val = val <= MACROS_MAX_REP
                                         ? val
                                         : MACROS_MAX_REP; val>1; val--)
                                {
                                    mac_str[maci] = (RM_CHAR)(cur_val);
                                    INCMACI;
                                }
                            }
#endif
                            if (*in == MACROS_ESCAPE_REP_STOP) continue;
                        default:
                            break;
                    }
#else
                    if(escape == escape_cont){
                        mac_str[maci] = (RM_CHAR)(val);
                        INCMACI;
                    }
#endif
                    /*  END SWITCH */
                    escape = escape_no;
                    goto __proc_escape_stop;
                }
                    continue;
            }
__proc_escape_stop:
#ifdef   MACROS_ESCAPE_REP_START
            switch (*in){ // for parameters ->"param{12}\160"<-
                case MACROS_ESCAPE:
                    if( (flags & REP_ENABLE_ESC)){
                        escape = escape_slash;
                        goto __post_switch; //----------------+
                    }                                       //|
                    break;                                  //|
                case MACROS_ESCAPE_REP_START:               //|
                    if( (flags & REP_ENABLE_REP)){          //|
                        escape = escape_rep;                //|
                        goto __post_switch; //----------------+
                    }                                       //|
                    break;                                  //|
#ifdef MACROS_SUBSTR_START                                  //|
                case MACROS_SUBSTR_START:                   //|
                    if( (flags & REP_ENABLE_SUBSTR)){       //|
                        if(macro != macro_substr){          //|
                            macro = macro_substr;           //|
                            goto __post_switch; //------------+
                        }                                   //|
                    }                                       //|
                    break;                                  //|
                case MACROS_SUBSTR_STOP:                    //|
                    if( (flags & REP_ENABLE_SUBSTR)){       //|
                        mac_str[maci] = MACROS_END_OF_LINE; //|
                        goto __post_switch; //----------------+
                    }                                       //|
                    break;                                  //|
#endif  // SUBSTR                                           //|
            }                                               //|
        goto __post_post_switch; //-----------------------+ //|
__post_switch: // speed step    <<------------------------|---+
            switch (st){                                //|
                case param_space:                       //|
                    mac_str[maci] = MACROS_END_OF_LINE; //|
                    INCMACI;                            //|
                    macros[pari] = &mac_str[maci];      //|
                    INCPARI;                            //|
                case param_comma:                       //|
                case param_start:                       //|
                    st = param_proc;                    //|
                    break;                              //|
                default:                                //|
                    break;                              //|
            }                                           //|
            continue;                                   //|
__post_post_switch: // <<---------------------------------+
#else // REPEAT
            if( (*in == MACROS_ESCAPE) && (flags & REP_ENABLE_ESC)){
                escape = escape_slash;
                switch (st){
                    case param_space:
                        mac_str[maci] = MACROS_END_OF_LINE;
                        INCMACI;
                        macros[pari] = &mac_str[maci];
                        INCPARI;
                    case param_comma:
                    case param_start:
                        st = param_proc;
                        break;
                    default:
                        break;
                }
                continue;
            }
#endif  // REPEATE


/* proccess ESCAPE */
    /* BRACKET */
#ifdef MACROS_PARAM_BRACKET_CLOSE
#if (MACROS_PARAM_BRACKET_CLOSE != MACROS_PARAM_BRACKET)
    /*  */
            if (brak && (*in != MACROS_PARAM_BRACKET_CLOSE)){
                macro = macro_no;
/*              if(*in != MACROS_PARAM_BRACKET){
                    mac_str[maci] = *in;
                    INCMACI;
                }*/
                mac_str[maci] = *in;
                INCMACI;
                continue;
            }
#else
            if (brak && (*in != MACROS_PARAM_BRACKET)){
                macro = macro_no;
                mac_str[maci] = *in;
                INCMACI;
                continue;
            }
#endif
#else           // not defined
            if (brak && (*in != MACROS_PARAM_BRACKET)){
                macro = macro_no;
                mac_str[maci] = *in;
                INCMACI;
                continue;
            }
#endif
    /* BRACKET */
        } // if (!macro)
/* Separator found */

/*! MACROS !!! !*/

//#ifdef   MACROS_ESCAPE_REP_START
__post_esc:
//#endif

#ifdef   MACROS_SUBSTR_START
    if ( (*in != al_sep) && (macro != macro_substr)){
#else
    if ( *in != _al_sep ){
#endif
            if (macro == macro_start){
                    macro = macro_no;
/*  CALL _macro (recursive) */
                    mac_str[maci]=MACROS_END_OF_LINE;
                    macros[pari]=0;
                    if( (mc = _macro(&in,
                        al_sep,
                        al_func,
                        al_par,
                        flags,
                        macros)) != 0){ // call SELF
#ifdef MACROS_ALLOW_SUBMACRO
                            if ( (flags & REP_ENABLE_SUBMACRO) &&
                                 (backi < MACROS_BACKUP_COUNT_-1) ) {
                                if (back[backi]) {
                                    INCBACKI;
                                }
                                back[backi] = (in +
                                              (*in != MACROS_END_OF_LINE));
//                              back = in + (*in != MACROS_END_OF_LINE);
                                in = mc;
#ifdef  MACROS_SUBSTR_START
                                macro = bmacro;
//#else
//                              macro = macro_no;
#endif
//                              break;
                                goto __lets_begin;
                            } else {
                                in += (*in!=0);
                                for (; *mc && maci < MACROS_BUFFER_LEN_; mc++) {
                                    mac_str[maci] = *mc;
                                    INCMACI;
                                }
                            }
#else
                            in += (*in!=0);
                            for (; *mc && maci < MACROS_BUFFER_LEN_; mc++) {
                                mac_str[maci] = *mc;
                                INCMACI;
                            }
#endif
/*  Test, only test.
                            if(*in == MACROS_PARAM_COMMA){
                                mac_str[maci] = MACROS_END_OF_LINE;
                                INCMACI;
                                st = param_comma;
                                continue;
                            }
*/
                    } else  in += (*in!=0);
//                  break;
            };
            /* End macro recursion */
#if (MACROS_PARAM_OPEN == MACROS_PARAM_CLOSE)
            if (*in == MACROS_PARAM_CLOSE && st != param_no)
                goto close_def; // GOTO CLOSE_DEF
#endif
            switch (*in){

                case MACROS_END_OF_LINE:  // for some idiots \0
#if (MACROS_PARAM_OPEN != MACROS_PARAM_CLOSE)
                case MACROS_PARAM_CLOSE: // End of the line (c) Pain :)
#endif
                    goto close_def; // GOTO CLOSE_DEF
                case MACROS_PARAM_OPEN:
                    switch (st){
                        case param_no:
                            mac_str[maci] = MACROS_END_OF_LINE;
                            INCMACI;
                            macros[pari] = &mac_str[maci];
                            INCPARI;
                            st = param_start;
                            break;
                        default: // error input ?
                            break;
                    }
                    break;
#ifdef MACROS_SPACE_1
                case MACROS_SPACE_1:
#endif
#ifdef MACROS_SPACE_2
                case MACROS_SPACE_2:
#endif
#ifdef MACROS_SPACE_3
                case MACROS_SPACE_3:
#endif
#ifdef MACROS_SPACE_4
                case MACROS_SPACE_4:
#endif
#ifdef MACROS_SPACE_5
                case MACROS_SPACE_5:
#endif
#ifdef MACROS_SPACE_6
                case MACROS_SPACE_6:
#endif
#ifdef MACROS_SPACE_7
                case MACROS_SPACE_7:
#endif
#ifdef MACROS_SPACE_8
                case MACROS_SPACE_8:
#endif
                    macro = macro_no;
                    switch (st){
                        case param_comma:
//                          st = param_space;
                        case param_start:
                        case param_space:
                            continue;
                        case param_no:
                            goto close_def; //GOTO CLOSE_DEF
                        default:
                            st = param_space;
                            continue;
                    }
                case MACROS_PARAM_COMMA:
                    macro = macro_no;
                    if(st == param_no)
                        goto close_def;
//                  if(st == param_space) continue;
                    mac_str[maci] = MACROS_END_OF_LINE;
                    INCMACI;
                    macros[pari] = &mac_str[maci];
                    INCPARI;
                    st = param_comma;
                    break;

#ifdef  MACROS_PARAM_BRACKET_CLOSE
#if(MACROS_PARAM_BRACKET_CLOSE != MACROS_PARAM_BRACKET)
                case MACROS_PARAM_BRACKET_CLOSE:
                    macro = macro_no;
                    brak = 0;
                    break;
#endif
#endif
                case MACROS_PARAM_BRACKET:
                    macro = macro_no;
                    switch (st){
                        case param_space:
                            mac_str[maci] = MACROS_END_OF_LINE;
                            INCMACI;
                            macros[pari] = &mac_str[maci];
                            INCPARI;
                        case param_start:
                        case param_comma:
                            st = param_proc;

#ifdef  MACROS_PARAM_BRACKET_CLOSE
#if(MACROS_PARAM_BRACKET_CLOSE != MACROS_PARAM_BRACKET)
                        default:
                            brak = 1;
#else
                        default:
                            brak = !brak;
#endif
#else
                        default:
                            brak = !brak;
#endif
                    }
                    break;

#ifdef MACROS_SUBSTR_START
                case MACROS_SUBSTR_START:
                    if( (flags & REP_ENABLE_SUBSTR)){
                        if(macro != macro_substr){
                            macro = macro_substr;
                            continue;
                        }
                    }
                    goto __default_char;
                case MACROS_SUBSTR_STOP:
                    if( (flags & REP_ENABLE_SUBSTR)){
                        mac_str[maci] = MACROS_END_OF_LINE;
                        goto close_def;
                    }
                    goto __default_char;
#endif
                case MACROS_ESCAPE:
                    if( (flags & REP_ENABLE_ESC)){
                        escape = escape_slash;
                        break;
                    }

#ifdef   MACROS_ESCAPE_REP_START
                    goto __default_char;    //--------+
                                                    //|
                case MACROS_ESCAPE_REP_START:       //|
                    if( (flags & REP_ENABLE_REP)){  //|
                        escape = escape_rep;        //|
                        break;                      //|
                    }                               //|
__default_char:                         // <----------+
#endif  // MACROS_ESCAPE_REP_START

                default:
                    if(*in == al_sep){
                        macro = macro_start;
                        break;
                    }
                    switch (st){
                        case param_space:
                            mac_str[maci] = MACROS_END_OF_LINE;
                            INCMACI;
                            macros[pari] = &mac_str[maci];
                            INCPARI;
                        case param_start:
                        case param_comma:   // macros name
                            st = param_proc;
                        default:    // case param_proc: case param_no:
                            // macros name
                            mac_str[maci] = *in;
#ifdef MACROS_SUBSTR_START
                            stack = 0;
#endif
                            INCMACI;
                            break;
                    }
            }

        } else {    // is alias' separatop or substring

#ifdef MACROS_SUBSTR_START
            switch(macro){
                case macro_start:
                    switch(escape){
                        case escape_slash:
                            mac_str[maci] = MACROS_ESCAPE;
                            INCMACI;
                            break;
                        case escape_cont:
                            mac_str[maci] = (RM_CHAR)val;
                            INCMACI;
                            break;
                        case escape_rep:
                            mac_str[maci] = MACROS_ESCAPE_REP_START;
                            INCMACI;
                            break;
                        case escape_cont_rep:
                            if (stack){
                                for (val = val <= MACROS_MAX_REP
                                         ? val
                                         : MACROS_MAX_REP; val>1; val--)
                                {
                                    cur_val = rep_macro(stack, al_sep,
                                            &mac_str[maci],
                                            (int)(MACROS_BUFFER_LEN_ - maci),
                                            flags & ~REP_INSTR_IS_POINTER,
                                            al_func, al_par);
                                    maci += cur_val;
                                }
                            } else {
                                if( (&mac_str[maci]) > macros[pari-1] ) {
                                    cur_val = mac_str[maci-1];
                                    for (val = val <= MACROS_MAX_REP
                                             ? val
                                             : MACROS_MAX_REP; val>1; val--)
                                    {
                                        mac_str[maci] = (RM_CHAR)(cur_val);
                                        INCMACI;
                                    }
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    escape = escape_no;
                    macro = macro_no;   //
                    mac_str[maci] = *in;
                    INCMACI;
                    break;
                case macro_substr:
                        macro = macro_sub_stop;
                        stack = in;
                        cur_val = rep_macro(&in, al_sep,
                                    &mac_str[maci],
                                    (int)(MACROS_BUFFER_LEN_ - maci),
                                    flags | REP_INSTR_IS_POINTER,
                                    al_func, al_par);
                        maci += cur_val;
                        if(!*in){
                            mac_str[maci] = MACROS_END_OF_LINE;
                            goto close_def;
                        }
                        break;
                case macro_sub_stop:
                case macro_no:
                    switch (st){
                        case param_space:
                            mac_str[maci] = MACROS_END_OF_LINE;
                            INCMACI;
                            macros[pari] = &mac_str[maci];
                            INCPARI;
                        case param_start:
                        case param_comma:
                            st=param_proc;
                        default:
                            macro = macro_start;
                            break;
                    }
                default:
                    break;

            }
#else
            if (macro == macro_start){
                mac_str[maci] = _al_sep;
                INCMACI;
                macro = macro_no;
//              continue;
            } else {
                    switch (st){
                        case param_space:
                            mac_str[maci] = MACROS_END_OF_LINE;
                            INCMACI;
                            macros[pari] = &mac_str[maci];
                            INCPARI;
                        case param_start:
                        case param_comma:
                            st=param_proc;
                        default:
                            macro = macro_start;
                            break;
                    }
            }
#endif

            /* end else SEPARATOR */
        }
    }
#ifdef MACROS_ALLOW_SUBMACRO

    if( (flags & REP_ENABLE_SUBMACRO) && back[backi]){
        in = back[backi];
        back[backi] = 0;
        DECBACKI;
        goto __lets_begin;
    }

#endif
    /*    END MAIN FOR    */
close_def:
/* 2006-02-09  */

    switch (escape){
        case escape_cont:
            mac_str[maci] = (RM_CHAR)(val);
            INCMACI;
            break;
        case escape_slash:
            mac_str[maci] = MACROS_ESCAPE;
            INCMACI;
            break;

#ifdef   MACROS_ESCAPE_REP_START
        case escape_rep:
            mac_str[maci] = MACROS_ESCAPE_REP_START;
            INCMACI;
            break;
        case escape_cont_rep:

#ifdef MACROS_SUBSTR_START
            switch (macro){
                case macro_no:
                    if( (&mac_str[maci]) > macros[pari-1] ){
                        cur_val = mac_str[maci-1];
                        for (val = val <= MACROS_MAX_REP
                                 ? val : MACROS_MAX_REP; val>1; val--)
                        {
                            mac_str[maci] = (RM_CHAR)(cur_val);
                            INCMACI;
                        }
                    }
                    break;

                case macro_sub_stop:
                    macro = macro_no;
                    for (val = val <= MACROS_MAX_REP
                             ? val : MACROS_MAX_REP; val>1; val--)
                    {
                        cur_val = rep_macro(stack, al_sep,
                                  &mac_str[maci],
                                  (int)(MACROS_BUFFER_LEN_ - maci),
                                  flags & ~REP_INSTR_IS_POINTER,
                                  al_func, al_par);
                        maci += cur_val;
                    }

                default:
                    break;
            }
#else
            if( (&mac_str[maci]) > macros[pari-1] ){
                cur_val = mac_str[maci-1];
                for (val = val <= MACROS_MAX_REP
                         ? val
                         : MACROS_MAX_REP; val>1; val--)
                {
                    mac_str[maci] = (RM_CHAR)(cur_val);
                    INCMACI;
                }
            }
#endif
            break;
#endif
        default:
            break;
    }
    /*! END SWITCH !*/
/* 2006-02-09  */

    switch (st){    //          dsfsdf sdf sdf %fdg(param, param!)
        case param_start:
        case param_no:
            macros[2] = 0;
        default:
            macros[pari]=0;
            mac_str[maci] = MACROS_END_OF_LINE;
            *instr = in;
    }
    return al_func ? al_func(al_par, macros, flags) : 0;

#undef INCOUT
#undef INCMACI
#undef INCPARI
#undef INCBACKI
#undef DECBACKI
}

/* REP_MACRO */

long rep_macro( void *instr,
                RM_CHAR al_sep,
                RM_CHAR *outstr, long outlen,
                unsigned int flags,
                REP_MACRO_FUNC al_func,
                void *al_par )
{
#define INCOUT (outc += (outc - outstr) < outlen-1)
#define INCBACKI (backi += (backi < (MACROS_BACKUP_COUNT_-1)))
#define DECBACKI (backi -= (backi > 0))

    RM_CHAR *inc;
    RM_CHAR *outc;
    RM_CHAR *mc;

#ifdef MACROS_ALLOW_SUBMACRO

    RM_CHAR *back[MACROS_BACKUP_COUNT_];
    unsigned int backi;

#endif

#ifdef   MACROS_SUBSTR_START
RM_CHAR *stack;

    enum mstate   { macro_no,
                    macro_start,
                    macro_substr,
                    macro_sub_stop
                  } st, bst;
#else
    enum mstate   { macro_no,
                    macro_start,
                  } st;
#endif

#ifdef   MACROS_ESCAPE_REP_START
    enum escstate { escape_no,
                    escape_slash,
                    escape_rep,
                    escape_cont,
                    escape_cont_rep
                  } escape;
#else
    enum escstate { escape_no,
                    escape_slash,
                    escape_cont,
                  } escape;

#endif

    unsigned int val_base = 0;
    unsigned int val      = 0;
    unsigned int cur_val  = 0;

    if ( !instr ) {
        return -1;
    }

 //   n_out = (outstr!=0 && outlen!=0);
    st = macro_no;
    escape = escape_no;

#ifdef MACROS_ALLOW_SUBMACRO
    backi = 0;
    back[0] = 0;
#endif

    inc = (flags & REP_INSTR_IS_POINTER)
        ? *(RM_CHAR **)instr
        : (RM_CHAR *)instr;
#ifdef   MACROS_SUBSTR_START
    bst = macro_no;
    stack = 0;
#endif

    outc=outstr;
/* DO - WHILE*/


    if(outstr!=0 && outlen!=0){

#ifdef MACROS_ALLOW_SUBMACRO

rm_lets_begin:

#endif

       for (; *inc && ((outc - outstr) < outlen-1); inc++){

    /*escape process*/
#ifdef   MACROS_SUBSTR_START
           if(st != macro_start && st != macro_substr){
#ifdef MACROS_ALLOW_SUBMACRO
                if ( (*inc == al_sep) && (flags & REP_ENABLE_SUBMACRO)) {
                    bst = st;
                    st = macro_start;
                    continue;
                }
#endif

#else

           if(st == macro_no){
#ifdef MACROS_ALLOW_SUBMACRO
                if ( (*inc == _al_sep) && (flags & REP_ENABLE_SUBMACRO)) {
                    st = macro_start;
                    continue;
                }
#endif

#endif
                switch (escape){
                    case escape_slash:
                        {
//                      if (*inc == _al_sep){
//                          escape = escape_cont;
//                          st = macro_start;
//                          continue;
//                      }
                        escape = escape_no;
                        st = macro_no;
                        switch (*inc){
#ifdef MACROS_ESCAPE_EMPTY
                            case MACROS_ESCAPE_EMPTY:
                                escape = escape_no;
                                continue;
#endif
                            case MACROS_ESCAPE_RETURN:
                                *outc = MACROS_ESCAPE_RETURN_VAL;
                                INCOUT;
                                continue;
                            case MACROS_ESCAPE_NEWLINE:
                                *outc = MACROS_ESCAPE_NEWLINE_VAL;
                                INCOUT;
                                continue;
                            case MACROS_ESCAPE_TAB:
                                *outc = MACROS_ESCAPE_TAB_VAL;
                                INCOUT;
                                continue;
                            case MACROS_ESCAPE_VTAB:
                                *outc = MACROS_ESCAPE_VTAB_VAL;
                                INCOUT;
                                continue;
                            case MACROS_ESCAPE_BS:
                                *outc = MACROS_ESCAPE_BS_VAL;
                                INCOUT;
                                continue;
                            case MACROS_ESCAPE_ALARM:
                                *outc = MACROS_ESCAPE_ALARM_VAL;
                                INCOUT;
                                continue;
#ifdef MACROS_ESCAPE_CLN
                            case MACROS_ESCAPE_CLN:
                                *outc = MACROS_ESCAPE_CLN_VAL;
                                INCOUT;
                                continue;
#endif
                            case MACROS_ESCAPE_FF:
                                *outc = MACROS_ESCAPE_FF_VAL;
                                INCOUT;
                                continue;

/*!                         case MACROS_ESCAPE_ZERO_:
                                *outc = MACROS_ESCAPE_ZERO_VAL;
                                INCOUT;
                                continue;
*/
#ifdef   MACROS_SUBSTR_START
                            case MACROS_SUBSTR_START:
                            case MACROS_SUBSTR_STOP:
                                if ( !(flags & REP_ENABLE_SUBSTR)){
                                    *outc = MACROS_ESCAPE;
                                    INCOUT;
                                }
#endif
#ifdef   MACROS_ESCAPE_REP_START
                            case MACROS_ESCAPE_REP_START:
                            case MACROS_ESCAPE_REP_STOP:
                                if ( !(flags & REP_ENABLE_REP)){
                                    *outc = MACROS_ESCAPE;
                                    INCOUT;
                                }
#endif
                            case MACROS_ESCAPE:
                                *outc = *inc;
                                INCOUT;
                                continue;

                        }
                        } // case escape_slash:
#ifdef   MACROS_ESCAPE_REP_START
                    case escape_rep:
#endif
                        {
#ifdef MACROS_ALLOW_SUBMACRO  // (@@)
                        if ( !(flags & REP_ENABLE_SUBMACRO) &&
                              (*inc == al_sep) )
                        {
#else  // MACROS_ALLOW_SUBMACRO (@@)
                        if (*inc == _al_sep) {
#endif
#ifdef   MACROS_ESCAPE_REP_START
                            *outc = ( (escape == escape_rep)
                                      ? MACROS_ESCAPE_REP_START
                                      : MACROS_ESCAPE);
#else
                            *outc = MACROS_ESCAPE;
#endif
                            INCOUT;
                            escape = escape_no;
                            st = macro_start;
                            continue;
                        }
    //                  escape = escape_no;
                        switch (*inc){
#ifdef MACROS_ESCAPE_BIN
                            case MACROS_ESCAPE_BIN:
                                val_base = MACROS_BIN_BASE;
                                val = 0;
                                goto rm_proc_base;
#endif
#ifdef MACROS_ESCAPE_TPL
                            case MACROS_ESCAPE_TPL:
                                val_base = MACROS_TPL_BASE;
                                val = 0;
                                goto rm_proc_base;
#endif
#ifdef MACROS_ESCAPE_QDR
                            case MACROS_ESCAPE_QDR:
                                val_base = MACROS_QDR_BASE;
                                val = 0;
                                goto rm_proc_base;
#endif
#ifdef MACROS_ESCAPE_OCT
                            case MACROS_ESCAPE_OCT:
                                val_base = MACROS_OCT_BASE;
                                val = 0;
                                goto rm_proc_base;
#endif
#ifdef MACROS_ESCAPE_HEX
                            case MACROS_ESCAPE_HEX:
                                val_base = MACROS_HEX_BASE;
                                val = 0;
                                goto rm_proc_base;
#endif
                                case RM_T('1'):
                                case RM_T('2'):
                                case RM_T('3'):
                                case RM_T('4'):
                                case RM_T('5'):
                                case RM_T('6'):
                                case RM_T('7'):
                                case RM_T('8'):
                                case RM_T('9'):
#ifdef MACROS_ESCAPE_ZERO_NOTDEF
                                case RM_T('0'):
#endif
                                val_base = MACROS_DEC_BASE;
                                val = *inc - RM_T('0');
rm_proc_base:
#ifdef MACROS_ESCAPE_REP_START
                                escape = (escape == escape_rep
                                                  ? escape_cont_rep
                                                  : escape_cont);
#else
                                escape = escape_cont;
#endif
                                continue;
#ifdef   MACROS_ESCAPE_REP_START
                            case MACROS_ESCAPE:
                                *outc = MACROS_ESCAPE_REP_START;
                                escape = escape_slash;
                                INCOUT;
                                continue;
#endif
#ifdef   MACROS_SUBSTR_START
                            case MACROS_SUBSTR_START:
                                if(flags & REP_ENABLE_SUBSTR){
                                    *outc = MACROS_ESCAPE_REP_START;
                                    INCOUT;
                                    st = macro_substr;
                                    escape = escape_no;
                                    continue;
                                }
                                break;
                            case MACROS_SUBSTR_STOP:
                                if(flags & REP_ENABLE_SUBSTR){
                                    *outc = MACROS_ESCAPE_REP_START;
                                    INCOUT;
    //                              *outc = MACROS_END_OF_LINE;
    //                              INCOUT;
                                    goto rm_proc_escape_stop;
                                }
                                break;
//                              goto exit;
#endif

#ifdef   MACROS_ESCAPE_REP_START
                            case MACROS_ESCAPE_REP_START:
                                *outc = MACROS_ESCAPE_REP_START;
                                INCOUT;
                                st = macro_no;
                                escape = escape_rep;
                                continue;
#endif
                            default:

#ifdef   MACROS_ESCAPE_REP_START
                                *outc = ( (escape == escape_rep)
                                           ? MACROS_ESCAPE_REP_START
                                           : MACROS_ESCAPE );
#else
                                *outc = MACROS_ESCAPE;

#endif
                                escape = escape_no;
                                INCOUT;
                                *outc = *inc;
                                INCOUT;
                                continue; // undefined char
                        }
                        }
                        default:
                            break;
                } // SWITCH(escape)
#ifdef   MACROS_ESCAPE_REP_START
                if( (escape == escape_cont) || (escape == escape_cont_rep)) {
#else
                if( escape == escape_cont ){
#endif
                    switch (*inc){
                        case RM_T('a'):
                        case RM_T('b'):
                        case RM_T('c'):
                        case RM_T('d'):
                        case RM_T('e'):
                        case RM_T('f'):
                            cur_val = *inc - RM_T('a') + 10;
                            goto rm_proc_escape;
                        case RM_T('A'):
                        case RM_T('B'):
                        case RM_T('C'):
                        case RM_T('D'):
                        case RM_T('E'):
                        case RM_T('F'):
                            cur_val = *inc - RM_T('A') + 10;
                            goto rm_proc_escape;
                        case RM_T('1'):
                        case RM_T('2'):
                        case RM_T('3'):
                        case RM_T('4'):
                        case RM_T('5'):
                        case RM_T('6'):
                        case RM_T('7'):
                        case RM_T('8'):
                        case RM_T('9'):
                        case RM_T('0'):
                            cur_val = *inc - RM_T('0');
rm_proc_escape:
                            if (val_base <= cur_val)
                                goto rm_char_notdef;
                            val = val * val_base + (cur_val);
                            continue;

                        default:
rm_char_notdef:
                            if(*inc == al_sep){
                                st = macro_start;
                                continue;
                            }
                            switch (escape){
                                case escape_cont:
                                    *outc = (RM_CHAR)(val);
                                    INCOUT;
                                    break;
#ifdef   MACROS_ESCAPE_REP_START // (1)
                                case escape_cont_rep:
                                    escape = escape_no;
#ifdef   MACROS_SUBSTR_START
                                switch (st){
                                    case macro_no:
                                        if(outc > outstr){
                                            cur_val = *(outc-1);
                                            for (val = val <= MACROS_MAX_REP
                                                     ? val
                                                     : MACROS_MAX_REP;
                                                 val>1; val--)
                                            {
                                                *outc = (RM_CHAR)(cur_val);
                                                INCOUT;
                                            }
                                        }
                                        break; // switch (st)
                                    case macro_sub_stop:
                                        st = macro_no;
                                        for (val = ((val <= MACROS_MAX_REP)
                                                 ? val
                                                 : MACROS_MAX_REP);
                                             val>1; val--)
                                        {
                                            cur_val = rep_macro(stack, al_sep,
                                                outc,
                                                (int)(outlen - (outc - outstr)),
                                                flags & ~REP_INSTR_IS_POINTER,
                                                al_func, al_par );
                                            outc += cur_val;
                                        }
                                        stack = 0;
                                        break;
                                    default:
                                        break;
                                }
#else
                                if(outc > outstr){
                                    cur_val = *(outc-1);
                                    for (val = val <= MACROS_MAX_REP
                                             ? val
                                             : MACROS_MAX_REP;
                                         val>1; val--)
                                    {
                                        *outc = (RM_CHAR)(cur_val);
                                        INCOUT;
                                    }
                                }

#endif
                                if (*inc == MACROS_ESCAPE_REP_STOP) continue;
#endif // MACROS_ESCAPE_REP_START (1)
                                default:
                                    break;
                            }
                            escape = escape_no;
                            goto rm_proc_escape_stop;
                    }
                    continue;
                }
rm_proc_escape_stop:
#ifdef   MACROS_ESCAPE_REP_START  // (3)
                switch (*inc){
                    case MACROS_ESCAPE:
                        if( (flags & REP_ENABLE_ESC)){
                            escape = escape_slash;
                            continue;
                        }
                        st = macro_no;
                        break;
                    case MACROS_ESCAPE_REP_START:
                        if( (flags & REP_ENABLE_REP)){
                            escape = escape_rep;
                            continue;
                        }
                        break;
#ifdef   MACROS_SUBSTR_START // (2)
                    case MACROS_SUBSTR_START:
                        if( (flags & REP_ENABLE_SUBSTR)){
                            if(st != macro_substr){
                                st = macro_substr;
                                continue;
                            }
                        }
                        break;
                    case MACROS_SUBSTR_STOP:
                        if( (flags & REP_ENABLE_SUBSTR)){
                            *outc = MACROS_END_OF_LINE;
                            goto exit;
                        }
                        break;
#endif // MACROS_SUBSTR_START  (2)
                }
#else
            if( (*inc == MACROS_ESCAPE) && (flags & REP_ENABLE_ESC)){
                escape = escape_slash;
                continue;
            }
#endif // MACROS_ESCAPE_REP_START  (3)
        /* escape */
            } // if(st != macro_start)

#ifdef   MACROS_SUBSTR_START
            if ( (*inc != al_sep) && (st != macro_substr)){
#else
            if ( *inc != _al_sep ){

#endif

#ifdef   MACROS_SUBSTR_START
                switch (st){
                    case macro_sub_stop:
                        stack = 0;
                        st = macro_no;
                    case macro_no:  //      proof symbol "!s %test(dfsd .)"
                        *outc = *inc;   //
                        INCOUT;
                        break;
                    case macro_start:   // MACROS found :) sdfsdf %!test(dfsd .)
/*  CALL _macro */
                        if( (mc = _macro(&inc,
                                  al_sep,
                                  al_func,
                                  al_par,
                                  flags | REP_MACROS_WITH_BUFFER,
                                  0)) != 0) {
#ifdef MACROS_ALLOW_SUBMACRO
                            if ( (flags & REP_ENABLE_SUBMACRO) &&
                                 (backi < MACROS_BACKUP_COUNT_-1))
                            {
                                if (back[backi]){
                                    INCBACKI;
                                }
                                back[backi] = (inc +
                                              (*inc != MACROS_END_OF_LINE ) );
                                inc = mc;
    //                          printf("fork!\n");
                                st = bst;
    //                          break;
                                goto rm_lets_begin;
                            } else {
                                for (; *mc; mc++){
                                    *outc = *mc;
                                    INCOUT;
                                }
                            }
#else
                            for (; *mc; mc++){
                                *outc = *mc;
                                INCOUT;
                            }
#endif
                        }
                        if(!*inc){
                            *outc = MACROS_END_OF_LINE;
                            goto exit;
                        }
                        st = macro_no;
                        break;
                    default:
                        break;

                }
#else
                if(st == macro_start) { // MACROS found :) sdfsdf %!test(dfsd )
/*  CALL _macro */
                    if( (mc = _macro(&inc,
                            _al_sep,
                            _al_func,
                            _al_par,
                            flags | REP_MACROS_WITH_BUFFER,
                            0)) != 0){
#ifdef MACROS_ALLOW_SUBMACRO
                        if( (flags & REP_ENABLE_SUBMACRO) &&
                            (backi < MACROS_BACKUP_COUNT_-1))
                        {
                            if (back[backi]){
                                INCBACKI;
                            }
                            back[backi] = (inc + (*inc != MACROS_END_OF_LINE));
//                          back = inc + (*inc != MACROS_END_OF_LINE);
                            inc = mc;
                            st = macro_no;
    //                      printf("fork!\n");
    //                      break;
                            goto rm_lets_begin;
                        } else {
                            for (; *mc; mc++){
                                *outc = *mc;
                                INCOUT;
                            }
                        }
#else
                        for (; *mc; mc++){
                            *outc = *mc;
                            INCOUT;
                        }
#endif
                    }
                    if(!*inc){
                        *outc = MACROS_END_OF_LINE;
                        goto exit;
                    }
                    st = macro_no;
                } else {
                    *outc = *inc;   //
                    INCOUT;

                }
#endif
            } else {

#ifdef   MACROS_SUBSTR_START
                switch (st){
                    case macro_substr:
                        st = macro_sub_stop;
                        stack = inc;
                        cur_val = rep_macro(&inc, al_sep,
                                    outc,
                                    (int)(outlen - (outc - outstr)),
                                    flags | REP_INSTR_IS_POINTER,
                                    al_func, al_par);
                        outc += cur_val;
                        if(!*inc){
                            *outc = MACROS_END_OF_LINE;
                            goto exit;
                        }
                        break;
                    case macro_sub_stop:
                    case macro_no:  // first separator found "sd !%test(dfsd )"
                        st = macro_start; //
                        break;
                    case macro_start:   // separator after separator
                                        // "sf %!%test(dfsd .. dfsd ..)"
                        switch(escape){
                            case escape_slash:
                                *outc = MACROS_ESCAPE;
                                INCOUT;
                                break;
                            case escape_cont:
                                *outc = (RM_CHAR)val;
                                INCOUT;
                                break;
                            case escape_rep:
                                *outc = MACROS_ESCAPE_REP_START;
                                INCOUT;
                                break;
                            case escape_cont_rep:
                                if(stack){
                                    for (val = val <= MACROS_MAX_REP
                                             ? val
                                             : MACROS_MAX_REP; val>1; val--)
                                    {
                                        cur_val = rep_macro(stack, al_sep,
                                              outc,
                                              (int)(outlen - (outc - outstr)),
                                              flags & ~REP_INSTR_IS_POINTER,
                                              al_func, al_par);
                                        outc += cur_val;
                                    }
                                } else {
                                    if(outc > outstr){
                                        cur_val = *(outc-1);
                                        for (val = val <= MACROS_MAX_REP
                                                 ? val
                                                 : MACROS_MAX_REP; val>1; val--)
                                        {
                                            *outc = (RM_CHAR)(cur_val);
                                            INCOUT;
                                        }
                                    }
                                }
                                break;
                            default:
                                break;
                        }
                        escape = escape_no;
                        st = macro_no;  //
                        *outc = *inc;
                        INCOUT;
                        break;
                    default:
                        break;
                }
#else
                if(st == macro_start){
                    switch(escape){
                        case escape_slash:
                            *outc = MACROS_ESCAPE;
                            INCOUT;
                            break;
                        case escape_cont:
                            *outc = (RM_CHAR)val;
                            INCOUT;
                            break;
#ifdef MACROS_ESCAPE_REP_START
                        case escape_rep:
                            *outc = MACROS_ESCAPE_REP_START;
                            INCOUT;
                            break;
                        case escape_cont_rep:
                            if(outc > outstr){
                                cur_val = *(outc-1);
                                for (val = val <= MACROS_MAX_REP ? val
                                             : MACROS_MAX_REP; val>1; val--) {
                                    *outc = (RM_CHAR)(cur_val);
                                    INCOUT;
                                }
                            }
                            break;
                        default:
                            break;
#endif
                    }
                    escape = escape_no;
                    st = macro_no;  //
                    *outc = *inc;
                    INCOUT;
                } else {
                    st = macro_start; //
                }
#endif
            }
//      inc++;
        }
/* FOR */
#ifdef MACROS_ALLOW_SUBMACRO
        if( (flags & REP_ENABLE_SUBMACRO) && back[backi]){

            inc = back[backi];
            back[backi] = 0;
            DECBACKI;
//          bst = macro_no;

            goto rm_lets_begin;
        }
#endif
/* 2006-02-09  */
        switch (escape){
            case escape_cont:
                *outc = (RM_CHAR)(val);
                INCOUT;
                break;
#ifdef   MACROS_ESCAPE_REP_START
            case escape_cont_rep:
#ifdef   MACROS_SUBSTR_START
                switch (st){
                    case macro_no:
                        if(outc > outstr){
                            cur_val = *(outc-1);
                            for (val = val <= MACROS_MAX_REP
                                     ? val
                                     : MACROS_MAX_REP; val>1; val--)
                            {
                                *outc = (RM_CHAR)(cur_val);
                                INCOUT;
                            }
                        }
                        break; // switch (st)
                    case macro_sub_stop:
                        st = macro_no;
                        for (val = val <= MACROS_MAX_REP
                                 ? val
                                 : MACROS_MAX_REP; val>1; val--)
                        {
                            cur_val = rep_macro(stack, al_sep,
                                      outc, (int)(outlen - (outc - outstr)),
                                      flags & ~REP_INSTR_IS_POINTER,
                                      al_func, al_par);
                            outc += cur_val;
                        }
                    default:
                        break;
                }
#else
                if(outc > outstr){
                    cur_val = *(outc-1);
                    for (val = val <= MACROS_MAX_REP
                             ? val
                             : MACROS_MAX_REP; val>1; val--)
                    {
                        *outc = (RM_CHAR)(cur_val);
                        INCOUT;
                    }
                }

#endif
                break;

            case escape_rep:
                *outc = MACROS_ESCAPE_REP_START;
                INCOUT;
                break;

#endif  // MACROS_ESCAPE_REP_START
            case escape_slash:
                *outc = MACROS_ESCAPE;
                INCOUT;
                break;
            default:
                break;
        }
/* 2006-02-09  */
        *outc = MACROS_END_OF_LINE;
        if(*inc && (flags & REP_CONTINUE_ON_BUFFOVERFLOW))
            goto do_without; // not \0 in "instring" ... move forvard.

    } else {
do_without:
        for(; *inc; inc++){
            if ( *inc != al_sep ){
                    if (macro_start == st){
                        _macro( &inc, al_sep, al_func, al_par,
                                 flags & ~REP_MACROS_WITH_BUFFER, 0 );
                        st = macro_no;
                        if(!*inc){
                            goto exit;
                        }
                    } else {
                        switch (*inc){
                            case MACROS_ESCAPE:
                                escape = (escape == escape_slash
                                                  ? escape_no
                                                  : escape );
                                break;
#ifdef MACROS_SUBSTR_START
                            case MACROS_SUBSTR_STOP:
                                if( ( flags & REP_ENABLE_REP ) &&
                                    ( escape != escape_slash ) )
                                {
                                    goto exit;
                                } else {
                                    escape = escape_no;
                                }
                                break;
#endif
                            default:
                                break;
                        }
                    }
            } else {
                st = macro_start;
            }
        }
    }
exit:
    if (flags & REP_INSTR_IS_POINTER) *(RM_CHAR **)instr = inc;
    return (int)(outc - outstr);

#undef INCOUT
#undef INCBACKI
#undef DECBACKI
}
/****************************************************/
