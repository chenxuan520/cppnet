# cppnet::HttpStatusCode
- 这是一个枚举类，定义了各种 HTTP 状态码，包括常见的如 `OK`（200）、`BAD_REQUEST`（400）、`INTERNAL_SERVER_ERROR`（500）等以及一个表示未知状态的 `UNKNOWN`（-1）。
# cppnet::HttpStatusCodeUtil
## static std::string ConvertToStr(const HttpStatusCode &code)
- 参数：
    - `code`：`HttpStatusCode`类型的 HTTP 状态码。
- 返回值：`std::string`类型，对应的 HTTP 状态码字符串。
- 作用：将给定的 HTTP 状态码转换为字符串表示。
## static HttpStatusCode ConvertToCode(const std::string &code)
- 参数：
    - `code`：字符串类型的 HTTP 状态码表示。
- 返回值：`HttpStatusCode`类型，对应的 HTTP 状态码枚举值。
- 作用：将给定的字符串形式的 HTTP 状态码转换为枚举值。
## static HttpStatusCode ConvertToCode(int code)
- 参数：
    - `code`：整数类型的 HTTP 状态码。
- 返回值：`HttpStatusCode`类型，对应的 HTTP 状态码枚举值。
- 作用：将给定的整数形式的 HTTP 状态码转换为枚举值。
