
# JJSON

Simple library implementation of Javascript Object Notation as described in [RFC 8259 # The JavaScript Object Notation (JSON) Data Interchange Format.]([https://tools.ietf.org/html/rfc8259](https://tools.ietf.org/html/rfc8259)) This library provides parsing and generating of JSON types. This project idea was started based on video session to work on this project [https://github.com/rhymu8354/Json](https://github.com/rhymu8354/Json), but has diverged largely.
## Tests
A suite of tests are provided in the `test/` directory. A wide range of test are provided for various JSON parsing , generation and parsing functions.
## Building
A simple Make file is provided to build test and library. Operation CMAKE files to be added.
## Usage
JSON values are created by call to `jjson::value`
- null  - `null` value is defined as macro to `nullptr`
```c++
 jjson::value null_value = null;     //null 
 auto null_value = jjson::value(null);  //null
 jjson null_value = jjson::valune(nullptr);  //null
```
-  bool 
```c++
 jjson::value true_value = true;             //true
 auto false_value = jjson::value(false);     //false
```
-  string 
 ```c++
 auto jjson_string = jjson::value("jjson string");  //"jjson string"
 auto jjson_string = R"("jjson string")";           //"jjson string"
 auto jjson_string = jjson_str_t("jjson string");   //"jjson string"
```
- number
```c++
auto jjson_int = jjson::value((int64_t)10);   //10
auto jjson_int_w_exponent = jjson::value((int64_t)10,12); //10e12
auto jjson_float = jjson::value(183.32);    //183.32
auto jjson_float_w_exponent = jjson::value(3.14,-1);      //3.14e-1
```
- Array
```c++
const  auto jjson_array = jjson::Array({ true , null ,false });             //[true , null , false]
const  auto jjson_array = jjson::Array({ "duck" , (int64_t)2 ,"goose" });   //["duck" , 2 , "goose"]
const auto jjson_sub_array = jjson::Array({ "test" , (int64_t)5 });         
auto jjson_nested_array = jjson::Array({ "duck" , (int64_t)2 , jjson_array4 });     //["duck" , 2 , [ "test" , 5 ]]
jjson_nested_array[2]; // ["test",5]
```
- Object
```c++
auto jjson_empty_object = jjson::Object();  //{}
auto jjson_object = jjson::Object();  //{}
auto jjson_array = jjson::Array({ "test" , (int64_t)5 });  //[ "test" , 5 ]
jjson_object["string"] = "string";  //{ "string"  : "string"}
jjson_object["string"] = "new string";    //{ "string" : "new string"}
jjson_object["country"] = "Barbados";     //{ "country" : "Barbados" , "string" : "new string"}
jjson_empty_object["int"] = (int64_t)12;  //{ "int" : 12,}
jjson_object["array"] = jjson_array4;     //{ "array" : [ "test" , 5 ] , "country" : "Barbados" , "string" : "new string"}
jjson_object["object"] = jjson_empty_object;   //{ "array" : [ "test" , 5 ] , "country" : "Barbados" ,"object" : {} , "string" : "new string"}
jjson_object["parish"] = "St. George";     //{ "array" : [ "test" , 5 ] , "country" : "Barbados" ,"object" : {} , "Parish" : "St. George" ,"string" : "new string"}
jjson_object["null"] = null;   //{ "array" : [ "test" , 5 ] , "country" : "Barbados" ,"object" : {} , "null" : null , "Parish" : "St. George" ,"string" : "new string"}
```
- Parsing and printing
```c++
 auto jjson_object_object= jjson::value::parse_from_string(R"({"object1":{},"object2":{"key":"val","array":[{},"string",true]}})");
 jjson_object_object.to_string();   //{"object1":{},"object2":{"key":"val","array":[{},"string",true]}}
```