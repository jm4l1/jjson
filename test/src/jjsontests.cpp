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
    auto jjson_null = jjson::value();
    REQUIRE(jjson_null.to_string()== JJSON_INVALD);
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
    REQUIRE(jjson_string.to_string() == "\"jjson string\"");
}
TEST_CASE("jjson value string to string")
{
    const auto jjson = jjson::value::parse_from_string("\"jjson string\"");
    REQUIRE(jjson == std::string("jjson string"));
}