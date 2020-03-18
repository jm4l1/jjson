#include "../include/jjson.hpp"
#include <stdexcept>
namespace jjson{
    value::value()
    :jimpl_(new impl)
    {
    };
    value::value(std::nullptr_t)
    :jimpl_(new impl)
    {
        jimpl_->type= impl::value_type::Null;
    };
    value::value(bool bool_value)
    :jimpl_(new impl)
    {
        jimpl_->type = impl::value_type::BOOLEAN;
        jimpl_->boolean_value = bool_value;
    }
    value::value(const char* cstr_value)
    :jimpl_(new impl)
    {
        jimpl_->type = impl::value_type::STRING;
        jimpl_->string_value = new jjson_str_t(cstr_value);
    }
    value::value(const jjson_str_t &string_value)
    :jimpl_(new impl)
    {
        jimpl_->type = impl::value_type::STRING;
        jimpl_->string_value = new jjson_str_t(string_value);
    }
    value::value(const int64_t int_value , const int exponent)
    :jimpl_(new impl)
    {
        jimpl_->type = impl::value_type::INT;
        jimpl_->int_value = int_value;
        jimpl_->exponent = exponent;
    }
    value::value(const double float_value , const int exponent)
    :jimpl_(new impl)
    {
        jimpl_->type = impl::value_type::FLOAT;
        jimpl_->float_value = float_value;
        jimpl_->exponent = exponent;
    }
    value::~value()
    {
    };
    jjson_str_t value::to_string() const
    {
        jjson_str_t exponent_string;
        std::stringstream float_stream;

        switch (jimpl_->type)
        {
        case impl::value_type::Null:
            return JJSON_NULL;
        case impl::value_type::BOOLEAN:
            return jimpl_->boolean_value ? JJSON_TRUE : JJSON_FALSE;
        case impl::value_type::STRING:
            return '\"'+  to_escaped_string(*jimpl_->string_value , JJSON_ESCAPE_CHARACTERS) + '\"';    
        case impl::value_type::INT:
            exponent_string = ( jimpl_->exponent == 0 ? "" : "e" + std::to_string(jimpl_->exponent));
            return std::to_string(jimpl_->int_value) + exponent_string ;    
        case impl::value_type::FLOAT:
            float_stream << jimpl_->float_value ;
            exponent_string = ( jimpl_->exponent == 0 ? "" : "e" + std::to_string(jimpl_->exponent));
            return float_stream.str() + exponent_string ;
        default:
            return JJSON_INVALD;
        }
    }
    bool value::operator==(const value &B)const
    {
        if(jimpl_->type != B.jimpl_->type)
        {
            return false;
        }
        else 
        {
            switch (jimpl_->type)
            {
                case impl::value_type::Null: 
                    return true;
                case impl::value_type::BOOLEAN:
                    return jimpl_->boolean_value == B.jimpl_->boolean_value;
                case impl::value_type::STRING:
                    return *(jimpl_->string_value) == *(B.jimpl_->string_value);
                case impl::value_type::INT:
                    return (jimpl_->int_value == B.jimpl_->int_value) && (jimpl_->exponent == B.jimpl_->exponent);
                case impl::value_type::FLOAT:
                    return (jimpl_->float_value == B.jimpl_->float_value) && (jimpl_->exponent == B.jimpl_->exponent);
                default:
                    return true;
            }
        }
    }
    value value::decode_as_int(const jjson_str_t &string_object)
    {
        if(
            string_object[0] != '-' &&
            std::isdigit(string_object[0]) == 0
            )
        {
            return value();
        }
        if(
            string_object[0] == '0' &&
            string_object.length() != 1
        )
        {
            return value();
        }
        size_t str_pos = string_object.find_first_of("Ee");
        int64_t int_value = 0;
        int exponent = 0;
        if(
            str_pos != jjson_str_t::npos
        )
        {
            try
            {
                exponent = std::stoi(jjson_str_t(string_object ,str_pos + 1 , string_object.length() - 1));
                int_value = std::stoi(jjson_str_t(string_object , 0 , str_pos));
            }
            catch(const std::exception& e ){
                return value();
            }
        }
        else
        { 
            try
            {
                int_value = std::stoi(string_object);
            }
            catch(const std::exception& e ){
                return value();
            }
        }
        return value((int64_t)int_value , exponent);
    }
    value value::decode_as_float(const jjson_str_t &string_object)
    {
        if(
            string_object[0] != '-' &&
            std::isdigit(string_object[0]) == 0
            )
        {
            return value();
        }
        if(
            string_object[0] == '0' &&
            string_object.length() != 1
        )
        {
            return value();
        }
        if(
            std::count(string_object.begin(), string_object.end(),'.') > 1
        )
        {
            return value();
        }
        size_t str_pos = string_object.find_first_of("Ee");
        double float_value = 0.0;
        int exponent = 0;
        if(
            str_pos != jjson_str_t::npos
        )
        {
            try
            {
                exponent = std::stoi(jjson_str_t(string_object ,str_pos + 1 , string_object.length() - 1));
                float_value = std::stod(jjson_str_t(string_object , 0 , str_pos -1 ));
            }
            catch(const std::exception& e){
                return value();
            }
        }
        else
        {
            try
            {
                float_value = std::stod(string_object);
            }
            catch(const std::exception& e)
            {
                return value();
            }
            
        }
        
        return value((double)float_value , exponent);
    }
    value value::parse_from_string(const jjson_str_t &string_object)
    {
        if(string_object == JJSON_NULL)
        {
            return value(nullptr);
        }
        else if (string_object == JJSON_TRUE)
        {
            return value(true);
        }
        else if (string_object == JJSON_FALSE)
        {
            return value(false);
        }
        else if (
            !string_object.empty() &&
            string_object[0] == '"' &&
            string_object[string_object.length() - 1] == '"'
            )
        {
            return value(dequote_string(string_object));
        }
        else if(
            !string_object.empty() &&
            string_object[0] == '[' &&
            string_object[string_object.length() - 1] == ']'
        )
        {
        }
        else if(
            !string_object.empty() &&
            string_object[0] == '{' &&
            string_object[string_object.length() - 1] == '}'
        )
        {
        }
        else if(
            !string_object.empty() &&
            (
                string_object.find_first_of(".") != jjson_str_t::npos
            )
        )
        {
            return decode_as_float(string_object);
        }
        return decode_as_int(string_object);
        // return value();
    }
}