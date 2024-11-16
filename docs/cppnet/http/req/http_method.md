# cppnet::HttpMethod
- 这是一个枚举类，定义了各种 HTTP 方法，包括 `GET`、`POST`、`PUT`、`DELETE` 等常见方法以及一个表示未知方法的 `UNKNOWN`。
# cppnet::HttpMethodUtil
## static std::string ConvertToStr(const HttpMethod &method)
- 参数：
    - `method`：`HttpMethod`类型的 HTTP 方法枚举值。
- 返回值：`std::string`类型，对应的 HTTP 方法字符串表示。
- 作用：将给定的 HTTP 方法枚举值转换为字符串表示。
## static HttpMethod ConvertToMethod(const std::string &method)
- 参数：
    - `method`：字符串类型的 HTTP 方法表示。
- 返回值：`HttpMethod`类型，对应的 HTTP 方法枚举值。
- 作用：将给定的字符串形式的 HTTP 方法转换为枚举值。
