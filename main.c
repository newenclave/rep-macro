#include "r-macro.h"

#include <stdio.h>

#define  REP_MY_FLAGS   REP_ENABLE_ESC | \
                        REP_ENABLE_REP | \
                        REP_ENABLE_SUBSTR | \
                        REP_ENABLE_SUBMACRO | \
                        REP_CONTINUE_ON_BUFFOVERFLOW

int cmp(RM_CHAR *v1, RM_CHAR *v2) {
    char *p1, *p2;
    if(!v1 || !v2) return v1 == v2;
    for( p1=v1, p2=v2; *p1 && *p2; p1++, p2++ ) {
        if(*p1 != *p2) return 0;
    }
    return ( *p1 == 0 && *p2 == 0 );
}

int simple_atoi(char *input) {
    int res = 0;
    for(; *input; input++ ) {
        int tmp = *input - '0';
        if (tmp > 9 || tmp < 0) return 0;
        res = res * 10 + tmp;
    }
    return res;
}

//#define REP_DEBUG

struct user_state {
    int     space;
    int     height;
    int     rows;
    int     call_counter;
    char    tmp_buf[100];
    int     call_back_count;
    int     max;
    int     min;
};

RM_CHAR * cb_variable(void *user_param, struct rep_header *input, unsigned int flags)
{

    char **p = &input->params[0];
    struct rep_header *parent = input;
    struct user_state *state = user_param;

    state->call_back_count++;

    char *line_height[4] = {"1", "2", "3", "4"};
    char *spaces[4] = {"+", "-", "=", "*"};
    char *rows[4] = {"9", "9", "9", "9"};

#ifdef REP_DEBUG
    printf("===============\nfunc name: %s\n", input->first_name);
    while( *p ) {
        printf("\tparam: %s\n", *p);
        p++;
    }
    printf("call stack: \n");
    while( parent ) {
        printf("%s::", parent->first_name);
        parent = parent->parent;
    }
    printf("()\nend callstack\n\n");
#endif

    if( 0 != cmp( input->first_name, "set") ) {
        if( 0 != cmp( input->params[0], "max") )
            state->max = simple_atoi(input->params[1]);
        else if( 0 != cmp( input->params[0], "min"))
            state->min = simple_atoi(input->params[1]);
       return 0;
    }

    if( 0 != cmp( input->params[0], "width") )
        return "3";
    else if( 0 != cmp( input->params[0], "height")) {
        return line_height[state->height++ % 4];
    }
    else if( 0 != cmp( input->params[0], "rows"))
        return rows[state->rows++ % 4];
    else if( 0 != cmp( input->params[0], "lines"))
        return "10";
    else if( 0 != cmp( input->params[0], "space")) {
        return spaces[state->space++ % 4];
    } else if( 0 != cmp(input->params[0], "multiply")) {
        state->call_counter++;
        state->call_counter += (state->call_counter % (state->max + 1) == 0);
        int res = (state->call_counter % (state->max + 1));
        sprintf(state->tmp_buf, "%5u", res * (state->call_counter / (state->max + 1) + 1));
        return state->tmp_buf;
    } else if(0 != cmp(input->params[0], "cal")) {
        state->call_counter++;
        if(state->call_counter <=0  || state->call_counter > 31) return "   ";
        sprintf(state->tmp_buf, "%3u", state->call_counter);
        return state->tmp_buf;
    } else if (0 != cmp(input->params[0], "max")) {
        sprintf(state->tmp_buf, "%u", state->max);
        return state->tmp_buf;
    } else if (0 != cmp(input->params[0], "min")) {
        sprintf(state->tmp_buf, "%u", state->min);
        return state->tmp_buf;
    }

    return "unknown :)";
}

int main(int argc, char **argv)
{
    struct user_state user_param = {0};
    char test[] = "\\n[[+-{$(width)}]{$(rows)}+"
                  "[\\n|[[$(space)]{$(width)}|]"
                  "{$(rows)}]{$(height)}\\n]"
                  "{$(lines)}[+-{$(width)}]{$(rows)}+\\n";

    char mul_table[] = "$set(max, 11)\\n[[$(multiply) ]{$(max)}\\n]{$(max)}\\n";
    char calendar[] = "\\n[[$(cal)]{7}\\n]{5}";

    char out[4096];
    int i;
    for(i=0; i<10000; i++)
    {
        user_param.call_counter = -1;
        rep_macro(mul_table, '$', out, 4096,
              REP_MY_FLAGS,
             (REP_MACRO_FUNC)cb_variable, &user_param);
    }
    printf("callbacks count: %u\nresult = %s\n", user_param.call_back_count, out);
    return 0;
}

