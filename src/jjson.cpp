#include "../include/jjson.hpp"

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
    value::~value()
    {
    };
    jjson_str_t value::to_string() const
    {
        switch (jimpl_->type)
        {
        case impl::value_type::Null:
            return JJSON_NULL;
        case impl::value_type::BOOLEAN:
            return jimpl_->boolean_value ? JJSON_TRUE : JJSON_FALSE;
        case impl::value_type::STRING:
            return '\"'+  to_escaped_string(*jimpl_->string_value , JJSON_ESCAPE_CHARACTERS) + '\"';    
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
            default:
                return true;
            }
        }
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
        
        return value();
    }
}