#ifndef _JJSON_HPP_
#define _JJSON_HPP_

#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>

/*
    Class to represents JSON (Javascript Object Notation) data format
    specified in RFC 7159 (https://tools.ietf.org/html/rfc7159)
                 RFC 8259 (https://tools.ietf.org/html/rfc8259)
*/


#define JJSON_NULL "null"
#define JJSON_TRUE "true"
#define JJSON_FALSE "false"
#define JJSON_INVALD "???"
#define JJSON_DQUOTE "\""
#define JJSON_BEGIN_ARRAY '['
#define JJSON_END_ARRAY ']'
#define JJSON_BEGIN_OBJECT '{'
#define JJSON_END_OBJECT '}'
#define JJSON_NAME_SEPARATOR ':'
#define JJSON_VALUE_SEPARATOR ','
#define JJSON_SPACE ' '
#define JJSON_HORIZONTAL_TAB '\t'
#define JJSON_NEW_LINE '\n'
#define JJSON_CARRIAGE_RETURN '\r'
#define JJSON_ESCAPE_CHARACTERS "\\\"/\b\f\n\t\r"
#define JJSON_MIN_INT  ( pow( 2 , 53 ) * -1) + 1
#define JJSON_MAX_INT  ( pow( 2 , 53 )  - 1 )
// auto white_space = jjson_str_t("") + JJSON_HORIZONTAL_TAB + JJSON_NEW_LINE + JJSON_SPACE + JJSON_CARRIAGE_RETURN ;
namespace {
    typedef std::string jjson_str_t;
    static void trim_string(jjson_str_t *string , const jjson_str_t chars )
    {
        auto first_nws = string->find_first_not_of(chars);
        if(first_nws != jjson_str_t::npos)
        {
            string->erase(string->begin(), string->begin() + (first_nws));
        }
        auto last_nws = string->find_last_not_of(chars);
        if(last_nws != jjson_str_t::npos)
        {
            string->erase(string->begin() + last_nws + 1  , string->end());
        }
    }
    jjson_str_t dequote_string(const jjson_str_t quoted_string){
        auto dequoted_string = quoted_string;
        trim_string(&dequoted_string , JJSON_DQUOTE );
        return dequoted_string;
    }
    jjson_str_t to_escaped_string(const jjson_str_t unescaped_string , const jjson_str_t char_to_escape)
    {
        auto escaped_string = unescaped_string;
        auto str_pos = escaped_string.find_first_of(char_to_escape);
        while(str_pos != jjson_str_t::npos)
        {
            switch (escaped_string[str_pos])
            {
            case '\\':
                escaped_string.replace(str_pos , 1 , "\\\\");
                break;
            case '\"':
                escaped_string.replace(str_pos , 1 , "\\\"");
                break;
            case '/':
                escaped_string.replace(str_pos , 1 , "\\/");
                break;
            case '\b':
                escaped_string.replace(str_pos , 1 , "\\b");
                break;
            case '\f':
                escaped_string.replace(str_pos , 1 , "\\f");
                break;
            case '\n':
                escaped_string.replace(str_pos , 1 , "\\n");
                break;
            case '\t':
                escaped_string.replace(str_pos , 1 , "\\t");
                break;
            case '\r':
                escaped_string.replace(str_pos , 1 , "\\r");
                break;
            }
            str_pos = escaped_string.find_first_of(char_to_escape , str_pos + 2);
        }
        return escaped_string;
    }
}
namespace jjson{

    class value
    {
        private:
            struct impl
            {
                // value = false / null / true / object / array / number / string
                // false = %x66.61.6c.73.65   ; false
                // null  = %x6e.75.6c.6c      ; null
                // true  = %x74.72.75.65      ; true
                // string = quotation-mark *char quotation-mark
                enum class value_type
                {
                    INVALID,
                    Null,
                    BOOLEAN,
                    STRING,
                    INT,
                    FLOAT
                } ;
                value_type type = value_type::INVALID;
                union 
                {
                    bool boolean_value;
                    jjson_str_t *string_value;
                    int64_t int_value;
                    double float_value;
                };
                int exponent = 1;
                impl(){
                };
                ~impl(){
                    switch (this->type)
                    {
                    case impl::value_type::STRING:
                        delete this->string_value;
                        break;
                    
                    default:
                        break;
                    }
                };
                impl(const impl&) = delete;
                impl(impl&&) = delete;
                impl& operator=(impl&&) = delete;
                impl& operator=(const impl&) = delete;
            }; // PIMPL implementation
            std::unique_ptr<impl> jimpl_;
        public:
            value();
            value(std::nullptr_t);
            value(bool bool_value);
            value(const char* cstr_value);
            value(const jjson_str_t& string_value);
            value(const int64_t int_value , const int exponent = 0);
            value(const double float_value , const int exponent = 0);
            
            bool operator==(const value &B)const;

            ~value(); // destructor
            value(const value&) = delete;   // copy construtor
            value(value&&) = default;        // move construtor
            value& operator=(const value&) = delete;    // copy assignment
            value& operator=(value&&) = default;         // move assignment

            jjson_str_t to_string() const;
            static value parse_from_string(const jjson_str_t &string_object);
            static value decode_as_int(const jjson_str_t &string_object);
            static value decode_as_float(const jjson_str_t &string_object);
    };
}

#endif // _JJSON_HPP_