#ifndef HEADER_RMACRO_HPP
#define HEADER_RMACRO_HPP

#include "r-macro.h"
#include <string>

#include <functional>

namespace repmacro {

    enum flags {
        flag_enable_src        = REP_ENABLE_ESC
       ,flag_enable_repeate    = REP_ENABLE_REP
       ,flag_enable_substrings = REP_ENABLE_SUBSTR
       ,flag_enable_submacro   = REP_ENABLE_SUBMACRO
       ,flag_enable_overflow   = REP_CONTINUE_ON_BUFFOVERFLOW
       ,flag_all               = flag_enable_src
                               | flag_enable_repeate
                               | flag_enable_substrings
                               | flag_enable_submacro
                               | flag_enable_overflow
    };

    class rmacro {

        typedef rmacro this_type;

    public:

        typedef RM_CHAR                       value_type;
        typedef std::basic_string<value_type> string_type;
        typedef std::function<
                const value_type * (const rep_header *, unsigned int)
        > translate_function;

    private:

        static const value_type *cb_translate( void *this_inst,
                                               void *header,
                                               unsigned int flags )
        {
            return static_cast<rmacro *>(this_inst)->translate (
                            static_cast<const rep_header *>(header),
                            flags );
        }

        static const value_type *def_translator( const rep_header */*head*/,
                                                 unsigned int      /*flags*/ )
        {
            return nullptr;
        }

        value_type          separator_;
        unsigned int        flags_;
        translate_function  default_;

    public:

        rmacro( value_type sep,
                unsigned int flags = flag_all )
            :separator_(sep)
            ,flags_(flags)
            ,default_(&this_type::def_translator)
        { }

        unsigned int flags( ) const
        {
            return flags_;
        }

        void set_flags( unsigned int value )
        {
            flags_ = value;
        }

        value_type separator( ) const
        {
            return separator_;
        }

        void set_separator( value_type value )
        {
            separator_ = value;
        }

        void set_translator( translate_function value )
        {
            if( value ) {
                default_ = value;
            } else {
                default_ = &this_type::def_translator;
            }
        }

        string_type run( const string_type &input, size_t maxoutput = 0 )
        {
            if( !input.empty( ) ) {
                string_type out( maxoutput, 0 );
                long res = ::rep_macro( &input[0],
                                         separator_,
                                         out.empty( ) ? NULL : &out[0],
                                         out.size( ),
                                         flags_,
                                         &rmacro::cb_translate, this );
                if( -1 != res ) {
                    out.resize( res );
                    return out;
                }
            }
            return string_type( );
        }

    private:

        virtual const value_type *translate( const rep_header *head,
                                             unsigned int      flags )
        {
            return default_( head, flags );
        }

    };

}

#endif // HEADER_RMACRO_HPP
