# cppnet::HttpVersion
- 这是一个枚举类，定义了不同的 HTTP 版本，包括 `HTTP_1_0`、`HTTP_1_1`、`HTTP_2_0`以及表示未知版本的 `UNKNOWN`。
# cppnet::HttpVersionUtil
## static std::string ConvertToStr(const HttpVersion &version)
- 参数：
    - `version`：`HttpVersion`类型的 HTTP 版本枚举值。
- 返回值：`std::string`类型，对应的 HTTP 版本字符串表示。
- 作用：将给定的 HTTP 版本枚举值转换为字符串表示。
## static HttpVersion ConvertToVersion(const std::string &version)
- 参数：
    - `version`：字符串类型的 HTTP 版本表示。
- 返回值：`HttpVersion`类型，对应的 HTTP 版本枚举值。
- 作用：将给定的字符串形式的 HTTP 版本转换为枚举值。
