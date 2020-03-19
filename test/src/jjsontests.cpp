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
    const auto jjson_nested_array = jjson::Array({ "duck" , (int64_t)2 , jjson_array4 });
    const auto jjson_empty_array =jjson::Array({});
    const auto jjson_nested_empty_array =jjson::Array({jjson_empty_array,jjson_empty_array,jjson_empty_array});
    REQUIRE(jjson_array.to_string() == R"([1 , 2 , 3])");
    REQUIRE(jjson_array2.to_string() == R"([true , null , false])");
    REQUIRE(jjson_array3.to_string() == R"(["duck" , 2 , "goose"])");
    REQUIRE(jjson_nested_array.to_string() == R"(["duck" , 2 , ["test" , 5]])");
    REQUIRE(jjson_empty_array.to_string() == R"([])");
    REQUIRE(jjson_nested_empty_array.to_string() == R"([[] , [] , []])");

    REQUIRE(jjson_array[0] == (int64_t)1 );
    REQUIRE(jjson_array.len() == 3 );
    REQUIRE(jjson_nested_array[2] == jjson_array4 );
    REQUIRE(jjson_nested_empty_array.type() == jjson::value::value_type::ARRAY );
}
// TEST_CASE("jjson value parse array")
// {
//     const auto jjson_array = jjson::value::parse_from_string(R"([ 1 , 2 , 3])");
//     const auto jjson_array2 = jjson::value::parse_from_string(R"([ "duck" , false , "goose"])");
//     const auto jjson_array3 = jjson::value::parse_from_string(R"([ true , "test" , null])");
//     REQUIRE(jjson_array == {(int64_t)1 , (int64_t)2 , (int64_t)3});
//     REQUIRE(jjson_array2 == { "duck" , false , "goose"});
//     REQUIRE(jjson_array3 == { true , "test" , nullptr});
    
// }