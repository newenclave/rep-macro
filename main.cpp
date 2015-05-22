#include <iostream>
#include <map>
#include "r-macro.hpp"



namespace ph = std::placeholders;
typedef repmacro::rmacro::value_type char_type;

typedef std::function<
    void ( const rep_header *, std::string & )
> translator_type;

typedef std::map<std::string, translator_type> translator_map;

const char_type *translate( const rep_header *head,
                            uint32_t    /*flags*/,
                            std::string &buff,
                            const translator_map &calls )
{
    auto f = calls.find( head->first_name );
    if ( f == calls.end( ) ) {
        return nullptr;
    } else {
        f->second( head, buff );
        return buff.c_str( );
    }
}

void test_call( const rep_header */*head*/, std::string &result )
{
    result = "This is a test!";
}

void time_call( const rep_header *head, std::string &result )
{
    const char_type *format = head->params[0] ? head->params[0] : "%F %D";

    time_t      rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer, 80, format, timeinfo);
    result = buffer;
}

translator_map make_translators( )
{
    translator_map res = {
         {"test", test_call}
        ,{"time", time_call}
    };

    return res;
}

int main( )
{
    translator_map translators = make_translators( );

    repmacro::rmacro fsm( "$time(\"%H:%M:%S\")\\n" );
    std::string buff;
    fsm.set_translator( std::bind( translate, ph::_1, ph::_2,
                                   std::ref( buff ),
                                   std::ref( translators ) ) );
    std::cout << fsm.run( '$', 1024 );

    return 0;
}

