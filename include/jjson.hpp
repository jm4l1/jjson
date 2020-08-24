#ifndef _JJSON_HPP_
#define _JJSON_HPP_

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <memory>
#include <string>
#include <algorithm>
#include <cmath>
#include <initializer_list>
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
#define null nullptr
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
    static jjson_str_t dequote_string(const jjson_str_t quoted_string){
        auto dequoted_string = quoted_string;
        dequoted_string.erase(dequoted_string.end()-1);
        dequoted_string.erase(dequoted_string.begin());
        return dequoted_string;
    }
    static jjson_str_t to_escaped_string(const jjson_str_t unescaped_string , const jjson_str_t char_to_escape)
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
    static size_t find_closing_char(const jjson_str_t string , const char opening , const char closing ){
        size_t pos = 0 ;
        int open_count = 0;
        bool in_quotes = false;
        bool escaped = false;
        for(
            auto itr = string.begin() ;
            itr != string.end() ;
            ++itr
        )
        {
            if(*itr == '\\')
            {
                escaped = !escaped;
                ++pos;
                continue;
            }
            if((*itr == '"') && escaped)
            {
                ++pos;
                continue;
            }
            if(*itr == '"' && (closing != '"'))
            {
                in_quotes = !in_quotes;
                ++pos;
                continue;
            }
            if(
                (*itr == opening) && 
                !in_quotes && 
                ( 
                    ((closing == '"') && 
                     (open_count == 0)
                    ) ||
                    (closing != '"'
                ))
            )
            {
                ++open_count;
                ++pos;
                continue;
            }
             if((*itr == closing) && in_quotes && (closing != '"'))
            {
                ++pos;
                continue;
            }
            if((*itr == closing) && (open_count == 1))
            {
                return pos;
            }
            if((*itr == closing) && !in_quotes)
            {
                --open_count;
                ++pos;
                continue;
            }
            ++pos;
        }
        return jjson_str_t::npos;
    }
    static size_t find_end_of_member(const jjson_str_t string ){
        size_t pos = 0 ;
        char end_char = ',';
        bool in_quotes = false;
        size_t open_brace = 0;
        size_t open_curly = 0;
        for(
            auto itr = string.begin() ;
            itr != string.end() ;
            ++itr
        ){
            if(*itr == '"' && ( open_brace  == 0 ) && ( open_curly % 2 == 0 ))
            {
                in_quotes = !in_quotes;
                ++pos;
                continue;
            }
            if(*itr == '{' && (!in_quotes))
            {
                ++open_curly;
                ++pos;
                continue;
            }
            if(*itr == '}' && (!in_quotes))
            {
                --open_curly;
                ++pos;
                continue;
            }
            if(*itr == '[' && (!in_quotes))
            {
                ++open_brace;
                ++pos;
                continue;
            }
            if(*itr == ']' && (!in_quotes))
            {
                --open_brace;
                ++pos;
                continue;
            }
            if((*itr == end_char) && !in_quotes && ( open_brace  == 0) && ( open_curly  == 0))
            {
                return pos;
            }
            ++pos;
        }

        return jjson_str_t::npos;
    }
}
namespace jjson{
    class value;
    typedef std::map<jjson_str_t , value> jjson_object;
    class value
    {
        public:
            enum class value_type
                    {
                        INVALID,
                        Null,
                        BOOLEAN,
                        STRING,
                        INT,
                        FLOAT,
                        ARRAY,
                        OBJECT
                    } ;
        private:
            struct impl
            {
                // value = false / null / true / object / array / number / string
                // false = %x66.61.6c.73.65   ; false
                // null  = %x6e.75.6c.6c      ; null
                // true  = %x74.72.75.65      ; true
                // string = quotation-mark *char quotation-mark
                
                value_type type = value_type::INVALID;
                union 
                {
                    bool boolean_value;
                    jjson_str_t *string_value;
                    int64_t int_value;
                    double float_value;
                    std::vector<value> *array_value;
                    jjson_object *object_value;
                };
                int exponent = 1;
                impl(){
                };
                ~impl(){
                    switch (this->type)
                    {
                    case value_type::STRING:
                        delete this->string_value;
                        break;
                    case value_type::ARRAY:
                        delete this->array_value;
                        break;
                    case value_type::OBJECT:
                        delete this->object_value;
                        break;
                    default:
                        break;
                    }
                };
                impl(const impl&) = default;
                impl(impl&&) = default;
                impl& operator=(impl&&) = delete;
                impl& operator=(const impl&) = delete;
            }; // PIMPL implementation
            std::unique_ptr<impl> jimpl_;
        public:
            value(std::nullptr_t);
            value(bool bool_value);
            value(const char* cstr_value);
            value(const jjson_str_t& string_value);
            value(const int int_value , const int exponent=0);
            value(const double float_value , const int exponent = 0);
            value(value_type type = value_type::INVALID);
            bool operator==(const value &B)const;
            value&& operator[](int index)const;
            value& operator[](const jjson_str_t&& key) const;
            value& operator[](const char* key) const;
            int len() const;
            value_type type() const;
            bool is_valid() const;
            bool is_empty() const;
            bool HasKey(const jjson_str_t key) const;
            operator int() const;
            operator std::string() const;
            operator float() const;
            operator bool() const;

            ~value(); // destructor
            value(const value&);   // copy construtor
            value(value&&);        // move construtor
            value& operator=(const value&);    // copy assignment
            value& operator=(value&&);         // move assignment

            friend std::ostream& operator<<(std::ostream& out , const value&);
            
            jjson_str_t to_string(bool pretty_format = false , int indent_level =  0) const;
            static value parse_from_string(const jjson_str_t &string_object);
            static value parse_as_int(const jjson_str_t &string_object);
            static value parse_as_float(const jjson_str_t &string_object);
            static value parse_as_array(const jjson_str_t &string_object);
            static value parse_as_object(const jjson_str_t &string_object);

            void Add(const value& value);
    };
    value Array(std::initializer_list<value> values={});
    value Object();
}

#endif // _JJSON_HPP_