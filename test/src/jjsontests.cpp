#define CATCH_CONFIG_MAIN

#include "../catch.hpp"
#include "../../include/jjson.hpp"

TEST_CASE("escape unescaped string")
{
    auto unescaped_string = jjson_str_t("this is a tab char \t and this is backspace \b, this is quotes \" and this is formfeed \f this is newline \n and carriage return \r , forward slash / test this should be unescaped.");
    auto escaped_string = to_escaped_string(unescaped_string , JJSON_ESCAPE_CHARACTERS);
    REQUIRE(escaped_string == R"(this is a tab char \t and this is backspace \b, this is quotes \" and this is formfeed \f this is newline \n and carriage return \r , forward slash \/ test this should be unescaped.)");
}
TEST_CASE("Remove quotes from string remove whitespace")
{
    auto quoted_string = jjson_str_t("\"This is quoted string in quotes\"");
    auto dequoted_string = dequote_string(quoted_string);
    REQUIRE(dequoted_string == "This is quoted string in quotes");
}
TEST_CASE("trim_string remove whitespace")
{
    auto untrimmed_string = jjson_str_t("\t\n\r This is untrimmed string in spaces \t \n \r");
    auto white_space = jjson_str_t("") + JJSON_HORIZONTAL_TAB + JJSON_NEW_LINE + JJSON_SPACE + JJSON_CARRIAGE_RETURN ;
    trim_string(&untrimmed_string , white_space );
    REQUIRE(untrimmed_string == "This is untrimmed string in spaces");
}
TEST_CASE("jjson value default constructor default")
{
    auto jjson_invalid = jjson::value();
    auto jjson_invalid2 = jjson::value();
    REQUIRE(jjson_invalid.to_string()== JJSON_INVALD);
    REQUIRE(jjson_invalid == jjson_invalid2);
}
TEST_CASE("jjson value null constructor")
{
    auto jjson_null = jjson::value(nullptr);
    REQUIRE(jjson_null.to_string()== JJSON_NULL);
}
TEST_CASE("jjson value null to string")
{
    const auto jjson_null = jjson::value::parse_from_string(JJSON_NULL);
    REQUIRE(jjson_null == nullptr);
}
TEST_CASE("jjson value bool constructor")
{
    auto jjson_true= jjson::value(true);
    REQUIRE(jjson_true.to_string()== JJSON_TRUE);
    auto jjson_false= jjson::value(false);
    REQUIRE(jjson_false.to_string()== JJSON_FALSE);
}
TEST_CASE("jjson value bool to string")
{
    auto jjson_true= jjson::value::parse_from_string(JJSON_TRUE);
    auto jjson_false= jjson::value::parse_from_string(JJSON_FALSE);
    REQUIRE(jjson_true == true);
    REQUIRE(jjson_false == false);
}
TEST_CASE("jjson value c_string constructor")
{
    auto jjson_string = jjson::value("jjson \n \t string");
    REQUIRE(jjson_string.to_string() == R"("jjson \n \t string")");
}
TEST_CASE("jjson value c_string to string")
{
    const auto jjson = jjson::value::parse_from_string("\"jjson string\"");
    REQUIRE(jjson == "jjson string");
}
TEST_CASE("jjson value string constructor")
{
    auto str = std::string("jjson string");
    auto jjson_string = jjson::value(str);
    REQUIRE(jjson_string.to_string() == R"("jjson string")");
}
TEST_CASE("jjson value string to string")
{
    const auto jjson = jjson::value::parse_from_string(R"("jjson string")");
    REQUIRE(jjson == std::string("jjson string"));
}
TEST_CASE("jjson value number constructor")
{
    auto jjson_int = jjson::value((int64_t)10);
    auto jjson_int2 = jjson::value((int64_t)10, 1);
    auto jjson_negative_int = jjson::value((int64_t)-890);
    auto jjson_int_w_exponent = jjson::value((int64_t)10,12);
    auto jjson_float = jjson::value(183.32);
    auto jjson_negative_float = jjson::value(-0.0032);
    auto jjson_float_w_exponent = jjson::value(3.14,-1);
    REQUIRE(jjson_int.to_string() == "10");
    REQUIRE_FALSE(jjson_int == jjson_int2);
    REQUIRE(jjson_negative_int.to_string() == "-890");
    REQUIRE(jjson_int_w_exponent.to_string() == "10e12");
    REQUIRE(jjson_float.to_string() == "183.32");
    REQUIRE(jjson_negative_float.to_string() == "-0.0032");
    REQUIRE(jjson_float_w_exponent.to_string() == "3.14e-1");
}
TEST_CASE("jjson value parse number from string")
{
    const auto jjson_int = jjson::value::parse_from_string("10");
    const auto jjson_negative_int = jjson::value::parse_from_string("-3340");
    const auto jjson_float = jjson::value::parse_from_string("340.2343");
    const auto jjson_negative_float = jjson::value::parse_from_string("-0.0032");
    REQUIRE(jjson_int == (int64_t)10);
    REQUIRE(jjson_negative_int == (int64_t)(-3340));
    REQUIRE(jjson_float == 340.2343);
    REQUIRE(jjson_negative_float == -0.0032);
}
TEST_CASE("jjson value parse number invalid number")
{
    auto jjson_int_invalid1 = jjson::value::parse_from_string("+1");
    auto jjson_int_invalid2 = jjson::value::parse_from_string("?1");
    auto jjson_int_invalid3 = jjson::value::parse_from_string("09");
    auto jjson_int_invalid4 = jjson::value::parse_from_string("9etest");
    auto jjson_float_invalid1 = jjson::value::parse_from_string("+1.2");
    auto jjson_float_invalid2 = jjson::value::parse_from_string("1..1");
    auto jjson_float_invalid3 = jjson::value::parse_from_string("1.2.1");
    auto jjson_float_invalid4 = jjson::value::parse_from_string("01.23");
    REQUIRE(jjson_int_invalid1.to_string() == JJSON_INVALD);
    REQUIRE(jjson_int_invalid2.to_string() == JJSON_INVALD);
    REQUIRE(jjson_int_invalid3.to_string() == JJSON_INVALD);
    REQUIRE(jjson_int_invalid4.to_string() == JJSON_INVALD);
    REQUIRE(jjson_float_invalid1.to_string() == JJSON_INVALD);
    REQUIRE(jjson_float_invalid2.to_string() == JJSON_INVALD);
    REQUIRE(jjson_float_invalid3.to_string() == JJSON_INVALD);
    REQUIRE(jjson_float_invalid4.to_string() == JJSON_INVALD);
}
TEST_CASE("jjson value array constructor")
{
    const auto jjson_array = jjson::Array({ (int64_t)1 , (int64_t)2 ,(double)3.00 });
    const auto jjson_array2 = jjson::Array({ true , nullptr ,false });
    const auto jjson_array3 = jjson::Array({ "duck" , (int64_t)2 ,"goose" });
    const auto jjson_array4 = jjson::Array({ "test" , (int64_t)5 });
    auto jjson_nested_array = jjson::Array({ "duck" , (int64_t)2 , jjson_array4 });
    const auto jjson_empty_array =jjson::Array({});
    const auto jjson_nested_empty_array =jjson::Array({jjson_empty_array,jjson_empty_array,jjson_empty_array});
    const auto jjson_object_object= jjson::value::parse_from_string(R"({"object1":{},"object2":{"key":"val","array":[{},"string",true]}})");
    REQUIRE(jjson_array.to_string() == R"([1,2,3])");
    REQUIRE(jjson_array2.to_string() == R"([true,null,false])");
    REQUIRE(jjson_array3.to_string() == R"(["duck",2,"goose"])");
    REQUIRE(jjson_nested_array.to_string() == R"(["duck",2,["test",5]])");
    REQUIRE(jjson_empty_array.to_string() == R"([])");
    REQUIRE(jjson_nested_empty_array.to_string() == R"([[],[],[]])");
    REQUIRE(jjson_array[0] == (int64_t)1 );
    REQUIRE(jjson_array.len() == 3 );
    REQUIRE(jjson_nested_array[2] == jjson_array4 );
    REQUIRE(jjson_nested_empty_array.type() == jjson::value::value_type::ARRAY );
    REQUIRE(jjson::value::parse_from_string( R"([1,2,3])").to_string() == R"([1,2,3])");
    REQUIRE(jjson::value::parse_from_string( R"([[1,[2]],[3,[4]],[],2,12.2,"test"])").to_string() == R"([[1,[2]],[3,[4]],[],2,12.2,"test"])");
    REQUIRE(jjson::value::parse_from_string( R"([[[[[1,[2,[[[4]]]]]]]]])").to_string() == R"([[[[[1,[2,[[[4]]]]]]]]])");
    REQUIRE(jjson::value::parse_from_string( R"([[[[[1,[2,[[[4],"[t{e}r]"]]]]]]],{}])").to_string() == R"([[[[[1,[2,[[[4],"[t{e}r]"]]]]]]],{}])");
    jjson_nested_array.Add(jjson_object_object);
    REQUIRE(jjson_nested_array.to_string() == R"(["duck",2,["test",5],{"object1":{},"object2":{"array":[{},"string",true],"key":"val"}}])");
}
TEST_CASE("jjson value parse array")
{
    const auto jjson_array =  jjson::Array({ (int64_t)1 , (int64_t)2 ,(int64_t)3 });
    const auto jjson_array_parsed = jjson::value::parse_from_string(R"([1,2,3])");
    const auto jjson_array2 = jjson::value::parse_from_string(R"(["duck",false,"goose"])");
    const auto jjson_array3 = jjson::value::parse_from_string(R"([true,"test",null])");
    const auto jjson_array4 = jjson::Array({ "test" , (int64_t)5 });
    const auto jjson_nested_array = jjson::Array({ "duck" , (int64_t)2 , jjson_array4 });
    REQUIRE(jjson_array_parsed.type() == jjson::value::value_type::ARRAY);
    REQUIRE(jjson_array_parsed.len() == 3);
    REQUIRE(jjson_array == jjson_array_parsed);
    REQUIRE(jjson_array2.type() == jjson::value::value_type::ARRAY);
    REQUIRE(jjson_array2[1] == false);
    REQUIRE(jjson_array3.type() == jjson::value::value_type::ARRAY);
    REQUIRE(jjson_nested_array ==  jjson::value::parse_from_string(R"(["duck",2,["test",5]])"));
    REQUIRE_FALSE(jjson_nested_array ==  jjson::value::parse_from_string(R"(["duck",2,["test",5],[]])"));
}
TEST_CASE("jjson value object constructor")
{
    const auto jjson_empty_object = jjson::Object();
    const auto jjson_empty_array = jjson::Array();
    const auto jjson_object = jjson::Object();
    const auto jjson_array4 = jjson::Array({ "test" , (int64_t)5 });
    REQUIRE(jjson_empty_object.to_string() == R"({})");
    jjson_object["test"] = "string";
    REQUIRE(jjson_object.to_string() == R"({"test":"string"})");
    jjson_object["test"] = "new string";
    REQUIRE(jjson_object.to_string() == R"({"test":"new string"})");
    jjson_object["country"] = "Barbados";
    REQUIRE(jjson_object.to_string() == R"({"country":"Barbados","test":"new string"})");
    jjson_empty_object["int"] = (int64_t)12;
    jjson_empty_object["bool"] = true;
    jjson_object["array"] = jjson_array4;
    jjson_object["object"] = jjson_empty_object;
    jjson_object["parish"] = "St. George";
    jjson_object["null"] = nullptr;
    REQUIRE(jjson_object.to_string() == R"({"array":["test",5],"country":"Barbados","null":null,"object":{"bool":true,"int":12},"parish":"St. George","test":"new string"})");
}
TEST_CASE("jjson object parse from string")
{
    auto jjson_empty_object = jjson::value::parse_from_string(R"({})");
    auto jjson_object_int =   jjson::value::parse_from_string(R"({"int":6})");
    auto jjson_object_float = jjson::value::parse_from_string(R"({"float":6.00})");
    auto jjson_object_null =  jjson::value::parse_from_string(R"({"null":null})");
    auto jjson_object_bool =  jjson::value::parse_from_string(R"({"bool":false})");
    auto jjson_object_string= jjson::value::parse_from_string(R"({"string":"string"})");
    auto jjson_object_array = jjson::value::parse_from_string(R"({"array":[1,2,3]})");
    auto jjson_object_object= jjson::value::parse_from_string(R"({"object1":{},"object2":{"key":"val","array":[{},"string",true]}})");
    auto jjson_object_mixes = jjson::value::parse_from_string(R"({"string":"string","int" : 10})");
    REQUIRE(jjson_empty_object.to_string() == R"({})");
    REQUIRE(jjson_object_int.to_string() == R"({"int":6})");
    REQUIRE(jjson_object_float.to_string() == R"({"float":6})");
    REQUIRE(jjson_object_null.to_string() == R"({"null":null})");
    REQUIRE(jjson_object_bool.to_string() == R"({"bool":false})");
    REQUIRE(jjson_object_string.to_string() == R"({"string":"string"})");
    REQUIRE(jjson_object_array.to_string() == R"({"array":[1,2,3]})");
    REQUIRE(jjson_object_object.to_string() == R"({"object1":{},"object2":{"array":[{},"string",true],"key":"val"}})");
    REQUIRE(jjson_object_mixes.to_string() == R"({"int":10,"string":"string"})");
}
TEST_CASE("jjson value object / array methods")
{
    const auto jjson_empty_object = jjson::Object();
    const auto jjson_empty_array = jjson::Array();
    const auto jjson_object = jjson::Object();
    const auto jjson_array = jjson::Array({nullptr , false , true});
    jjson_object["value"] = "test value";
    REQUIRE(jjson_empty_array.len() == 0);
    REQUIRE(jjson_empty_object.len() < 0);
    REQUIRE(jjson_empty_array.is_empty() == true);
    REQUIRE(jjson_empty_object.is_empty() == true);
    REQUIRE(jjson_object.is_empty() == false);
    REQUIRE(jjson_array.is_empty() == false);
}
TEST_CASE("jjson value invalid formats")
{
    REQUIRE(jjson::value::parse_from_string( R"(+)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(-)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(*)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(+0)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(00)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(00.123)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(00.12.3)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(")").to_string()== JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"("unclosed string)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"(["unclosed array")").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"({"unclosed obect")").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"({"unclosed obect":6)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"({"unclosed obect":"123)").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"({"unclosed obect": {123 , array : "" })").to_string() == JJSON_INVALD );
    REQUIRE(jjson::value::parse_from_string( R"({"unclosed obect": 123 , "array" : [1,2 ,{}] , "te" : })").to_string() == JJSON_INVALD );
}
TEST_CASE("jjson RFC8259 examples")
{
    jjson_str_t rfc8259_json_object = R"({
        "Image": {
            "Width":  800,
            "Height": 600,
            "Title":  "View from 15th Floor",
            "Thumbnail": {
                "Url":    "http://www.example.com/image/481989943",
                "Height": 125,
                "Width":  100
            },
            "Animated" : false,
            "IDs": [116, 943, 234, 38793]
          }
      })";
    jjson_str_t rfc8259_json_array = R"([
        {
           "precision": "zip",
           "Latitude":  37.7668,
           "Longitude": -122.3959,
           "Address":   "",
           "City":      "SAN FRANCISCO",
           "State":     "CA",
           "Zip":       "94107",
           "Country":   "US"
        },
        {
           "precision": "zip",
           "Latitude":  37.371991,
           "Longitude": -122.026020,
           "Address":   "",
           "City":      "SUNNYVALE",
           "State":     "CA",
           "Zip":       "94085",
           "Country":   "US"
        }
      ])";
      REQUIRE(jjson::value::parse_from_string(rfc8259_json_object).is_valid());
      REQUIRE(jjson::value::parse_from_string(rfc8259_json_array).is_valid());
      REQUIRE(jjson::value::parse_from_string(rfc8259_json_object).type() == jjson::value::value_type::OBJECT);
      REQUIRE(jjson::value::parse_from_string(rfc8259_json_array).type() == jjson::value::value_type::ARRAY);
      REQUIRE(jjson::value::parse_from_string(rfc8259_json_object)["Image"]["IDs"].type() == jjson::value::value_type::ARRAY);
      REQUIRE(jjson::value::parse_from_string(rfc8259_json_array)[0].type() == jjson::value::value_type::OBJECT);
}