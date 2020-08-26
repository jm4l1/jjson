#include "../include/jjson.hpp"
#include <stdexcept>
namespace jjson{
    value::value(value::value_type type)
    :jimpl_(std::move(std::make_unique<impl>()))
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
    :jimpl_(std::move(std::make_unique<impl>()))
    {
        jimpl_->type= value::value_type::Null;
    };
    value::value(bool bool_value)
    :jimpl_(std::move(std::make_unique<impl>()))
    {
        jimpl_->type = value::value_type::BOOLEAN;
        jimpl_->boolean_value = bool_value;
    }
    value::value(const char* cstr_value)
    :jimpl_(std::move(std::make_unique<impl>()))
    {
        jimpl_->type = value::value_type::STRING;
        jimpl_->string_value = new jjson_str_t(cstr_value);
    }
    value::value(const jjson_str_t &string_value)
    :jimpl_(std::move(std::make_unique<impl>()))
    {
        jimpl_->type = value::value_type::STRING;
        jimpl_->string_value = new jjson_str_t(string_value);
    }
    value::value(const int int_value , const int exponent)
    :jimpl_(std::move(std::make_unique<impl>()))
    {
        jimpl_->type = value::value_type::INT;
        jimpl_->int_value = int_value;
        jimpl_->exponent = exponent;
    }
    value::value(const double float_value , const int exponent)
    :jimpl_(std::move(std::make_unique<impl>()))
    {
        jimpl_->type = value::value_type::FLOAT;
        jimpl_->float_value = float_value;
        jimpl_->exponent = exponent;
    }
    value::value(const value& B)   // copy construtor
    :jimpl_(std::move(std::make_unique<impl>()))
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
    :jimpl_(std::move(std::make_unique<impl>())){
        jimpl_.release();
        jimpl_ = std::exchange(B.jimpl_ , null);
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
        jimpl_.release();
        jimpl_ = std::move(std::make_unique<impl>());
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
        jimpl_ = std::exchange(B.jimpl_ , null);
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
    jjson_str_t value::to_string(bool pretty_format , int indent_level) const
    {
        indent_level++;
        jjson_str_t exponent_string;
        std::stringstream float_stream;
        jjson_str_t array_string ;
        jjson_str_t object_string ;
        jjson_str_t bool_string;
        jjson_str_t string_string;
        jjson_str_t null_string = JJSON_NULL;
        auto string_quote = "\"";
        auto color_reset = pretty_format ? "\033[0m" : "";
        auto null_color = pretty_format ? "\033[1;30m" : color_reset ;
        auto bool_color = pretty_format ? "\033[1;35m" : color_reset ;
        auto number_color = pretty_format ? "\033[0;32m" : color_reset ;
        auto string_color = pretty_format ? "\033[0;31m" : color_reset ;
        auto brace_color = pretty_format ? "\033[1;37m" : color_reset ;
        jjson_str_t open_curly = brace_color + jjson_str_t("{")  + color_reset;
        jjson_str_t close_curly = brace_color + jjson_str_t("}")  + color_reset;
        jjson_str_t open_square_brace = brace_color + jjson_str_t("[")  + (pretty_format ? " " : "") + color_reset;
        jjson_str_t close_square_brace = brace_color + jjson_str_t(pretty_format ? " " : "") + jjson_str_t("]")  + color_reset;
        switch (jimpl_->type)
        {
        case value_type::Null:
            return null_color + null_string + color_reset;
        case value_type::BOOLEAN:
            bool_string = (jimpl_->boolean_value ? JJSON_TRUE : JJSON_FALSE);
            return  bool_color + bool_string + color_reset;
        case value_type::STRING:
            string_string += string_quote ;
            return  string_color +  string_string + to_escaped_string(*jimpl_->string_value , JJSON_ESCAPE_CHARACTERS) + string_quote + color_reset ;    
        case value_type::INT:
            exponent_string = ( jimpl_->exponent == 0 ? "" : "e" + std::to_string(jimpl_->exponent));
            return number_color + std::to_string(jimpl_->int_value) + exponent_string + color_reset ;    
        case value_type::FLOAT:
            float_stream << jimpl_->float_value ;
            exponent_string = ( jimpl_->exponent == 0 ? "" : "e" + std::to_string(jimpl_->exponent));
            return number_color +  float_stream.str() + exponent_string + color_reset ;
        case value_type::ARRAY:
            if(jimpl_->array_value->empty())
            {
                return open_square_brace + close_square_brace;
            }
            array_string += open_square_brace;
            array_string += jimpl_->array_value->begin()->to_string(pretty_format , indent_level);
            for(
                auto itr = jimpl_->array_value->begin() + 1;
                itr != jimpl_->array_value->end();
                ++itr
            )
            {
                array_string +=  ",";
                array_string += itr->to_string(pretty_format , indent_level);
            }
            array_string += close_square_brace;
            return array_string;
        case value_type::OBJECT:
            if(jimpl_->object_value->empty())
            {
                return open_curly + close_curly;
            }
            object_string += open_curly;
            for(
                auto itr = jimpl_->object_value->begin() ;
                itr != jimpl_->object_value->end() ;
                // ++itr
            )
            {
                if(pretty_format) object_string += '\n';
                if(pretty_format)
                {
                    for(auto i = 0 ; i < indent_level ; i++)
                        object_string += "  ";
                }
                object_string +=  JJSON_DQUOTE +  itr->first +  JJSON_DQUOTE + ( pretty_format ? " : " : ":") + itr->second.to_string(pretty_format , indent_level);
                if(++itr != jimpl_->object_value->end())
                {
                    object_string += ",";
                }
            }
                if(pretty_format) object_string += '\n';
                if(pretty_format)
                {
                    for(auto i = 0 ; i < indent_level - 1 ; i++)
                        object_string += "  ";
                }
            object_string += close_curly;
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
            v = null; 
            return std::move(*v);
        }
        if(index < 0)
        {
            auto v = new value();
            v = null; 
            return std::move(*v);
        }
        if(index > this->jimpl_->array_value->size())
        {
            auto v = new value();
            v = null; 
            return std::move(*v);
        }
        return std::move((*this->jimpl_->array_value)[index]);
    }
    value& value::operator[](const jjson_str_t&& key) const
    {
        if(this->jimpl_->type != value_type::OBJECT)
        {
            auto v = new value();
            v = null; 
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
    value& value::operator[](const char* key) const
    {
        if(key == nullptr){
            auto v = new value();
            v = null; 
            return (value&)*v;
        }
        return (*this)[jjson_str_t(key)];
    }
    std::ostream& operator<<(std::ostream& out , const value& v)
    {
        return out << v.to_string();
    }
    
    value::operator int() const{
        assert((this->jimpl_->type == value_type::INT) || (this->jimpl_->type == value_type::FLOAT));
        if(this->jimpl_->type == value_type::INT){
            return this->jimpl_->int_value;
        }
            return this->jimpl_->float_value;
    };
    value::operator std::string() const{
        assert((this->jimpl_->type == value_type::STRING));
        return *(this->jimpl_->string_value);
    };
    value::operator float() const{
        assert((this->jimpl_->type == value_type::INT) || (this->jimpl_->type == value_type::FLOAT));
        if(this->jimpl_->type == value_type::INT){
            return this->jimpl_->int_value;
        }
        return this->jimpl_->float_value;
        
    };
    value::operator bool() const{
        assert(this->jimpl_->type == value_type::BOOLEAN);
         return this->jimpl_->boolean_value;
    };
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
    bool value::is_valid() const
    {
        return this->type()  != value_type::INVALID;
    };
    value::value_type value::type() const
    {
        return this->jimpl_->type;
    }
    bool value::HasKey(const jjson_str_t key) const
    {
        if(this->jimpl_->type != value_type::OBJECT )
        {
            return false;
        }
        return (this->jimpl_->object_value->find(key) != this->jimpl_->object_value->end());
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
        return value(int(int_value) , exponent);
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
            return value(null);
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
        trim_string(&array_string , " \n\r\t");
        auto jjson_array = value(value_type::ARRAY);
        while(!array_string.empty())
        {
            trim_string(&array_string , " \n\r\t");
            size_t str_pos; 
            if(array_string[0] == '[')
            {
                auto array_end_pos = find_closing_char(array_string , '[' , ']');
                auto nested_array_str = jjson_str_t(array_string , 0 , array_end_pos + 1 );
                trim_string(&nested_array_str , " \n\r\t");
                jjson_array.Add(parse_as_array(nested_array_str));
                array_string.erase(0 , array_end_pos + 1  );
                str_pos = array_string.find_first_of(',');
                array_string.erase(0 , str_pos + 1  );
                continue;
            }
            str_pos =  find_end_of_member(array_string);
            if(str_pos == jjson_str_t::npos)
            {
                auto v = parse_from_string(array_string);
                if(v.jimpl_->type == value_type::INVALID)
                {
                    return value();
                }
                jjson_array.Add(v);
                array_string.erase(0 , str_pos );
                trim_string(&array_string , " \n\r\t");
            }
            else
            {
                auto substr = jjson_str_t(array_string , 0 , str_pos );
                array_string.erase(0 , str_pos + 1  );
                trim_string(&substr , " \n\r\t");  
                auto v = parse_from_string(substr);
                if(v.jimpl_->type == value_type::INVALID)
                {
                    return value();
                }
                jjson_array.Add(v);
            }
        }
        return jjson_array;
    }
    std::pair<jjson_str_t , value> parse_as_member(jjson_str_t &string_object)
    {
        auto key_end_pos = find_closing_char(string_object , '"' ,'"');
        auto key = jjson_str_t(string_object , 1 , key_end_pos - 1);
        trim_string(&key , " \n\r\t");
        auto val_string = jjson_str_t(string_object , key_end_pos  + 1 , string_object.size() - key_end_pos );
        trim_string(&val_string , " \n\r\t");
        val_string.erase(0,1);
        trim_string(&val_string , " \n\r\t");
        auto val = jjson::value::parse_from_string(val_string);
        return std::make_pair(key , val);
    }
    value value::parse_as_object(const jjson_str_t &string_object)
    {
        auto object_string = string_object;
        object_string.erase(object_string.end()-1);
        object_string.erase(object_string.begin());
        trim_string(&object_string , " \n\r\t");
        auto jjson_object = value(value_type::OBJECT);
        while(!object_string.empty())
        {
            trim_string(&object_string , " \n\r\t");
            size_t str_pos; 
            if(object_string[0] == '{')
            {
                auto object_end_pos = find_closing_char(object_string , '{' , '}');
                auto nested_object_str = jjson_str_t(object_string , 0 , object_end_pos + 1 );
                trim_string(&nested_object_str , " \n\r\t");
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
                trim_string(&object_string , " \n\r\t");
            }
            else
            {
                auto substr = jjson_str_t(object_string , 0 , str_pos);
                object_string.erase(0 , str_pos + 1  );
                trim_string(&substr , " \n\r\t");
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