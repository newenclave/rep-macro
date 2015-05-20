#ifndef HEADER_RMACRO_HPP
#define HEADER_RMACRO_HPP

#include "r-macro.h"
#include <string>

namespace repmacro {

    class rmacro {

        static RM_CHAR *cb_translate( void *this_inst,
                                      void *header,
                                      unsigned int flags )
        {
            return static_cast<rmacro *>(this_inst)->traslate (
                            static_cast<rep_header *>(header),
                            flags );
        }

        typedef std::basic_string<RM_CHAR> string_type;

        string_type src_;
        unsigned int flags_;

    public:

        typedef RM_CHAR value_type;

        rmacro( const string_type &input, unsigned int flags )
            :src_(input)
            ,flags_(flags)
        { }

        const string_type &src( ) const
        {
            return src_;
        }

        string_type run( value_type separator, size_t maxoutput = 0 )
        {
            if( !src_.empty( ) ) {
                string_type out( maxoutput, 0 );
                long res = ::rep_macro( &src_[0], separator,
                             out.empty( ) ? NULL : &out[0], out.size( ),
                             -1, &rmacro::cb_translate, this );
                if( -1 != res ) {
                    out.resize( res );
                    return out;
                }
            }
            return string_type( );
        }

    private:

        virtual value_type *traslate( rep_header   */*head*/,
                                      unsigned int  /*flags*/ )
        {
            return NULL;
        }

    };

}

#endif // HEADER_RMACRO_HPP
