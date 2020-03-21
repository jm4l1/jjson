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
        case value::value_type::OBJECT :
            jimpl_->type = type;
            jimpl_->object_value = new jjson_object;
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
    value::value(const value& B)   // copy construtor
    :jimpl_(new impl)
    {
        this->jimpl_->type = B.jimpl_->type;
        switch (B.jimpl_->type)
        {
            case value_type::ARRAY:
                this->jimpl_->array_value = new std::vector<value>;
                *(this->jimpl_->array_value) = *(B.jimpl_->array_value);
                break;
            case value_type::OBJECT:
                this->jimpl_->object_value = new jjson_object;
                *(this->jimpl_->object_value) = *(B.jimpl_->object_value);
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
    };
    value::value(value&& B)        // move construtor
    :jimpl_(new impl){
        jimpl_.release();
        jimpl_ = std::exchange(B.jimpl_ , nullptr);
    };
    void value::Add(const value& value)
    {
        if(this->jimpl_->type != value_type::ARRAY)
        {
            return;
        }
        this->jimpl_->array_value->emplace_back(value);
    }
    value& value::operator=(const value& B)    // copy assignment
    {
        if(this == &B)
        {
            return *this;
        }
        jimpl_.reset(new impl());
        this->jimpl_->type = B.jimpl_->type;
        switch (this->jimpl_->type)
        {
        case value_type::ARRAY:
            this->jimpl_->array_value = new std::vector<value>;
            *(this->jimpl_->array_value) = *(B.jimpl_->array_value);
            break;
        case value_type::OBJECT:
            this->jimpl_->object_value = new jjson_object;
            *(this->jimpl_->object_value) = *(B.jimpl_->object_value);
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
    };
    value& value::operator=(value&& B)            // move assignment
    {
        if(this == &B)
        {
            return *this;
        }
        jimpl_.release();
        jimpl_ = std::exchange(B.jimpl_ , nullptr);
        return *this;
    }
    value Array(std::initializer_list<value> values)
    {
        auto jjson_array = value(value::value_type::ARRAY);
        for(auto itr = values.begin() ; itr != values.end()  ; ++itr)
        {
            jjson_array.Add(*itr);
        }
        return jjson_array;
    }
    value Object()
    {
        auto jjson_object = value(value::value_type::OBJECT);
        return jjson_object;
    }
    value::~value()
    {
    };
    jjson_str_t value::to_string() const
    {
        jjson_str_t exponent_string;
        std::stringstream float_stream;
        jjson_str_t array_string ;
        jjson_str_t object_string ;
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
                array_string +=  ",";
                array_string += itr->to_string();
            }
            array_string += ']';
            return array_string;
        case value_type::OBJECT:
            if(jimpl_->object_value->empty())
            {
                return "{}";
            }
            object_string += "{";
            for(
                auto itr = jimpl_->object_value->begin() ;
                itr != jimpl_->object_value->end() ;
                // ++itr
            )
            {
                object_string +=  JJSON_DQUOTE +  itr->first +  JJSON_DQUOTE + ":" + itr->second.to_string();
                if(++itr != jimpl_->object_value->end())
                {
                    object_string += ",";
                }
            }
            object_string += "}";
            return object_string;
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
                case value::value_type::ARRAY:
                    return *(jimpl_->array_value) == *(B.jimpl_->array_value);
                case value::value_type::OBJECT:
                    return *(jimpl_->object_value) == *(B.jimpl_->object_value);
                default:
                    return true;
            }
        }
    }
    value&& value::operator[](int index) const
    {
        if(this->jimpl_->type != value_type::ARRAY)
        {
            auto v = new value();
            v = nullptr; 
            return std::move(*v);
        }
        if(index < 0)
        {
            auto v = new value();
            v = nullptr; 
            return std::move(*v);
        }
        if(index > this->jimpl_->array_value->size())
        {
            auto v = new value();
            v = nullptr; 
            return std::move(*v);
        }
        return std::move((*this->jimpl_->array_value)[index]);
    }
    value& value::operator[](const jjson_str_t&& key) const
    {
        if(this->jimpl_->type != value_type::OBJECT)
        {
            auto v = new value();
            v = nullptr; 
            return (value&)*v;
        }
        auto member = this->jimpl_->object_value->find(key);
        if(member == this->jimpl_->object_value->end())
        { 
            auto v =  new value();
            this->jimpl_->object_value->emplace(std::make_pair(key , *v));
            member = this->jimpl_->object_value->find(key);
            return (value&)member->second;
        }
        return (value&)member->second;
    }
    std::ostream& operator<<(std::ostream& out , const value& v)
    {
        return out << v.to_string();
    }
    int value::len() const
    {
        if(this->jimpl_->type != value_type::ARRAY)
        {
            return -1;
        }
        return this->jimpl_->array_value->size();
    };
    bool value::is_empty() const
    {
        switch (this->jimpl_->type)
        {
        case value_type::ARRAY:
            return this->jimpl_->array_value->empty();
            break;
        case value_type::OBJECT:
            return this->jimpl_->object_value->empty();
            break;
        default:
            return true;
        }
    };
    value::value_type value::type() const
    {
        return this->jimpl_->type;
    }
    value value::parse_as_int(const jjson_str_t &string_object)
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
    value value::parse_as_float(const jjson_str_t &string_object)
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
            if(string_object == JJSON_DQUOTE)
             return value();
            return value(dequote_string(string_object));
        }
        else if(
            !string_object.empty() &&
            string_object[0] == '[' &&
            string_object[string_object.length() - 1] == ']'
        )
        {

            return parse_as_array(string_object);
        }
        else if(
            !string_object.empty() &&
            string_object[0] == '{' &&
            string_object[string_object.length() - 1] == '}'
        )
        {
            return parse_as_object(string_object);
        }
        else if(
            !string_object.empty() &&
            (
                string_object.find_first_of(".") != jjson_str_t::npos
            )
        )
        {
            return parse_as_float(string_object);
        }
        return parse_as_int(string_object);
    }
    value value::parse_as_array(const jjson_str_t &string_object)
    {
        auto array_string = string_object;
        array_string.erase(array_string.end()-1);
        array_string.erase(array_string.begin());
        trim_string(&array_string , " ");
        auto jjson_array = value(value_type::ARRAY);
        while(!array_string.empty())
        {
            trim_string(&array_string , " ");
            size_t str_pos; 
            if(array_string[0] == '[')
            {
                auto array_end_pos = find_closing_char(array_string , '[' , ']');
                auto nested_array_str = jjson_str_t(array_string , 0 , array_end_pos + 1 );
                trim_string(&nested_array_str , " ");
                jjson_array.Add(parse_as_array(nested_array_str));
                array_string.erase(0 , array_end_pos + 1  );
                str_pos = array_string.find_first_of(',');
                array_string.erase(0 , str_pos + 1  );
                continue;
            }
            str_pos = array_string.find_first_of(',');
            if(str_pos == jjson_str_t::npos)
            {
                auto v = parse_from_string(array_string);
                if(v.jimpl_->type == value_type::INVALID)
                {
                    return value();
                }
                jjson_array.Add(v);
                // jjson_array.Add(parse_from_string(array_string));
                array_string.erase(0 , str_pos );
                trim_string(&array_string , " ");
            }
            else
            {
                auto substr = jjson_str_t(array_string , 0 , str_pos );
                array_string.erase(0 , str_pos + 1  );
                trim_string(&substr , " ");  
                auto v = parse_from_string(substr);
                if(v.jimpl_->type == value_type::INVALID)
                {
                    return value();
                }
                jjson_array.Add(v);
                // jjson_array.Add(parse_from_string(substr));
            }
        }
        return jjson_array;
    }
    std::pair<jjson_str_t , value> parse_as_member(jjson_str_t &string_object)
    {
        auto key_end_pos = find_closing_char(string_object , '"' ,'"');
        auto key = jjson_str_t(string_object , 1 , key_end_pos - 1);
        trim_string(&key , " ");
        auto val_string = jjson_str_t(string_object , key_end_pos  + 1 , string_object.size() - key_end_pos );
        trim_string(&val_string , " ");
        val_string.erase(0,1);
        trim_string(&val_string , " ");
        auto val = jjson::value::parse_from_string(val_string);
        return std::make_pair(key , val);
    }
    value value::parse_as_object(const jjson_str_t &string_object)
    {
        auto object_string = string_object;
        object_string.erase(object_string.end()-1);
        object_string.erase(object_string.begin());
        trim_string(&object_string , " ");
        auto jjson_object = value(value_type::OBJECT);
        while(!object_string.empty())
        {
            trim_string(&object_string , " ");
            size_t str_pos; 
            if(object_string[0] == '{')
            {
                auto object_end_pos = find_closing_char(object_string , '{' , '}');
                auto nested_object_str = jjson_str_t(object_string , 0 , object_end_pos + 1 );
                trim_string(&nested_object_str , " ");
                jjson_object.Add(parse_as_object(nested_object_str));
                object_string.erase(0 , object_end_pos + 1  );
                str_pos = object_string.find_first_of(',');
                object_string.erase(0 , str_pos + 1  );
                continue;
            }
            str_pos = find_end_of_member(object_string);
            if(str_pos == jjson_str_t::npos)
            {
                auto member = parse_as_member(object_string);
                if(member.second.jimpl_->type == value_type::INVALID)
                {
                    return value();
                }
                jjson_object[std::string(member.first)] = member.second;
                object_string.erase(0 , str_pos );
                trim_string(&object_string , " ");
            }
            else
            {
                auto substr = jjson_str_t(object_string , 0 , str_pos);
                object_string.erase(0 , str_pos + 1  );
                trim_string(&substr , " ");
                auto member = parse_as_member(substr);
                if(member.second.jimpl_->type == value_type::INVALID)
                {
                    return value();
                }
                jjson_object[std::string(member.first)] = member.second;
            }
            
        }
        return jjson_object;
    }
}