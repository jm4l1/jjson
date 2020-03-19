#include "../include/jjson.hpp"
#include <stdexcept>
namespace jjson{
    value::value(value::value_type type)
    :jimpl_(new impl)
    {
        switch (type)
        {
        case value::value_type::ARRAY :
            jimpl_->type = type;
            jimpl_->array_value = new std::vector<value>;
            break;
        default:
            break;
        }
    };
    value::value(std::nullptr_t)
    :jimpl_(new impl)
    {
        jimpl_->type= value::value_type::Null;
    };
    value::value(bool bool_value)
    :jimpl_(new impl)
    {
        jimpl_->type = value::value_type::BOOLEAN;
        jimpl_->boolean_value = bool_value;
    }
    value::value(const char* cstr_value)
    :jimpl_(new impl)
    {
        jimpl_->type = value::value_type::STRING;
        jimpl_->string_value = new jjson_str_t(cstr_value);
    }
    value::value(const jjson_str_t &string_value)
    :jimpl_(new impl)
    {
        jimpl_->type = value::value_type::STRING;
        jimpl_->string_value = new jjson_str_t(string_value);
    }
    value::value(const int64_t int_value , const int exponent)
    :jimpl_(new impl)
    {
        jimpl_->type = value::value_type::INT;
        jimpl_->int_value = int_value;
        jimpl_->exponent = exponent;
    }
    value::value(const double float_value , const int exponent)
    :jimpl_(new impl)
    {
        jimpl_->type = value::value_type::FLOAT;
        jimpl_->float_value = float_value;
        jimpl_->exponent = exponent;
    }
    value::value(const value& B)
    :jimpl_(new impl)
    {
        this->jimpl_->type = B.jimpl_->type;
        switch (B.jimpl_->type)
        {
        case value_type::ARRAY:
            this->jimpl_->array_value = new std::vector<value>;
            *(this->jimpl_->array_value) = *(B.jimpl_->array_value);
            break;
        case value_type::BOOLEAN:
            this->jimpl_->boolean_value = B.jimpl_->boolean_value;
            break;
        case value_type::FLOAT:
            this->jimpl_->float_value = B.jimpl_->float_value;
            this->jimpl_->exponent = B.jimpl_->exponent;
            break;
        case value_type::INT:
            this->jimpl_->int_value = B.jimpl_->int_value;
            this->jimpl_->exponent = B.jimpl_->exponent;
            break;
        case value_type::STRING:
            this->jimpl_->string_value = new jjson_str_t;
            *(this->jimpl_->string_value) = *(B.jimpl_->string_value);
            break;
        
        default:
            break;
        }
    };   // copy construtor
    value::value(value&& B)
    :jimpl_(new impl){
    };        // move construtor
    void value::Add(const value& value)
    {
        if(this->jimpl_->type != value_type::ARRAY )
        {
            return;
        }
        this->jimpl_->array_value->emplace_back(value);
    }
    value& value::operator=(const value& B){
        jimpl_.reset(new impl());
        this->jimpl_->type = B.jimpl_->type;
        switch (this->jimpl_->type)
        {
        case value_type::ARRAY:
            this->jimpl_->array_value = new std::vector<value>;
            *(this->jimpl_->array_value) = *(B.jimpl_->array_value);
            break;
        case value_type::STRING:
            this->jimpl_->string_value = new jjson_str_t;
            *(this->jimpl_->string_value) = *(B.jimpl_->string_value);
            break;
        case value_type::BOOLEAN:
            this->jimpl_->boolean_value = B.jimpl_->boolean_value;
            break;
        case value_type::FLOAT:
            this->jimpl_->float_value = B.jimpl_->float_value;
            this->jimpl_->exponent = B.jimpl_->exponent;
            break;
        case value_type::INT:
            this->jimpl_->int_value = B.jimpl_->int_value;
            this->jimpl_->exponent = B.jimpl_->exponent;
            break;
        default:
            break;
        }
        return *this;
    };    // copy assignment
    value Array(std::initializer_list<value> values)
    {
        auto jjson_array = value(value::value_type::ARRAY);
        for(auto itr = values.begin() ; itr != values.end()  ; ++itr)
        {
            jjson_array.Add(*itr);
        }
        return jjson_array;
    }
    value::~value()
    {
    };
    jjson_str_t value::to_string() const
    {
        jjson_str_t exponent_string;
        std::stringstream float_stream;
        jjson_str_t array_string ;

        switch (jimpl_->type)
        {
        case value_type::Null:
            return JJSON_NULL;
        case value_type::BOOLEAN:
            return jimpl_->boolean_value ? JJSON_TRUE : JJSON_FALSE;
        case value_type::STRING:
            return '\"'+  to_escaped_string(*jimpl_->string_value , JJSON_ESCAPE_CHARACTERS) + '\"';    
        case value_type::INT:
            exponent_string = ( jimpl_->exponent == 0 ? "" : "e" + std::to_string(jimpl_->exponent));
            return std::to_string(jimpl_->int_value) + exponent_string ;    
        case value_type::FLOAT:
            float_stream << jimpl_->float_value ;
            exponent_string = ( jimpl_->exponent == 0 ? "" : "e" + std::to_string(jimpl_->exponent));
            return float_stream.str() + exponent_string ;
        case value_type::ARRAY:
            if(jimpl_->array_value->empty())
            {
                return "[]";
            }
            array_string += '[';
            array_string += jimpl_->array_value->begin()->to_string();
            for(
                auto itr = jimpl_->array_value->begin() + 1;
                itr != jimpl_->array_value->end();
                ++itr
            )
            {
                array_string +=  " , ";
                array_string += itr->to_string();
            }
            array_string += ']';
            return array_string;
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
                case value::value_type::Null: 
                    return true;
                case value::value_type::BOOLEAN:
                    return jimpl_->boolean_value == B.jimpl_->boolean_value;
                case value::value_type::STRING:
                    return *(jimpl_->string_value) == *(B.jimpl_->string_value);
                case value::value_type::INT:
                    return (jimpl_->int_value == B.jimpl_->int_value) && (jimpl_->exponent == B.jimpl_->exponent);
                case value::value_type::FLOAT:
                    return (jimpl_->float_value == B.jimpl_->float_value) && (jimpl_->exponent == B.jimpl_->exponent);
                default:
                    return true;
            }
        }
    }

    value&& value::operator[](int index) const
    {
        if(this->jimpl_->type != value_type::ARRAY)
        {
            return nullptr;
        }
        if(index < 0)
        {
            return nullptr;
        }
        if(index > this->jimpl_->array_value->size())
        {
            return nullptr;
        }
        return std::move((*this->jimpl_->array_value)[index]);
    }

    size_t value::len() const
    {
        if(this->jimpl_->type != value_type::ARRAY)
        {
            return -1;
        }
        return this->jimpl_->array_value->size();
    };
    value::value_type value::type() const
    {
        return this->jimpl_->type;
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