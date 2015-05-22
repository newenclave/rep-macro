#include <iostream>
#include "r-macro.hpp"

namespace ph = std::placeholders;

const repmacro::rmacro::value_type *translate(
                                         const rep_header *head,
                                         uint32_t    /*flags*/,
                                         std::string &buff )
{
    std::cout << "name: " << head->first_name << "\n";
    return buff.c_str( );
}

int main( )
{
    repmacro::rmacro fsm( "1234567$test()" );
    std::string buff;
    fsm.set_translator( std::bind( translate, ph::_1, ph::_2,
                                   std::ref(buff) ) );
    std::cout << fsm.run( '$', 4 );

    return 0;
}

