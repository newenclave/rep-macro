#include <iostream>
#include <map>
#include <sstream>

#include "r-macro.hpp"

namespace ph = std::placeholders;
typedef repmacro::rmacro::value_type char_type;

struct state {
    std::string                        buf_;
    std::map<std::string, std::string> aliases_;
};

typedef std::function<void ( const rep_header *, state & )> translator_type;

typedef std::map<std::string, translator_type> translator_map;

const char_type *translate( const rep_header *head,
                            uint32_t    /*flags*/,
                            state       &buff,
                            const translator_map &calls )
{
    auto f = calls.find( head->first_name );
    return  f == calls.end( )
               ?   nullptr
               : ( f->second( head, buff ), buff.buf_.c_str( ) );
}

void set_macro( const rep_header *head, state &result )
{
    const char_type *name  = head->params[0];
    const char_type *value = name ? head->params[1] : "";
    if( name ) {
        std::ostringstream oss;
        result.aliases_[name] = value ? value : "";
    }
    result.buf_.clear( );
}

void get_macro( const rep_header *head, state &result )
{
    const char_type *name  = head->params[0];
    if( name ) {
        auto f = result.aliases_.find( name );
        std::ostringstream oss;
        if( head->parent ) {
            oss << MACROS_PARAM_BRACKET
                << (f == result.aliases_.end( ) ? "" : f->second)
                << MACROS_PARAM_BRACKET;
        } else {
            oss << (f == result.aliases_.end( ) ? "" : f->second);
        }
        result.buf_ = oss.str( );
    } else {
        result.buf_.clear( );
    }
}

void len_macro( const rep_header *head, state &result )
{
    const char_type *name  = head->params[0];
    if( name ) {
        std::string sname(name);
        std::ostringstream oss;
        oss << sname.size( );
        result.buf_ = oss.str( );
    } else {
        result.buf_ = "0";
    }
}

void time_macro( const rep_header *head, state &result )
{
    const char_type *format = head->params[0] ? head->params[0] : "%H:%M:%S";

    time_t      rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime( buffer, 80, format, timeinfo );
    result.buf_ = buffer;
}

translator_map make_translators( )
{
    translator_map res;

    res["len" ] = len_macro;
    res["time"] = time_macro;
    res["set" ] = set_macro;
    res["get" ] = get_macro;

    return res;
}

int main( )
{
    translator_map translators = make_translators( );

/* format for
+--------------------------+
| Current time is 10:00:00 |
+--------------------------+
  set - set alias string
  get - get string by alias
  len - get string length
*/
    const char *format = "$set( timestr, 'Current time is '"
                               "$time( '%H:%M:%S' ) )"
                         "+---{$len($get(timestr))}+\n"
                         "| $get(timestr) |\n"
                         "+---{$len($get(timestr))}+\n";
    state st;

    repmacro::rmacro macro( '$' );
    macro.set_translator( std::bind( translate, ph::_1, ph::_2,
                                     std::ref( st ),
                                     std::ref( translators ) ) );

    std::cout << macro.run( format, 1024 );

    return 0;
}

