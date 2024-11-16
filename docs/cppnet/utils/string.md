# cppnet::StringUtil
## static void Split(const std::string &origin_str, const std::string &delimiter, std::vector\<std::string\> &arr, bool keep_delimiter = false)
- 参数：
    - `origin_str`：原始字符串。
    - `delimiter`：分隔符。
    - `arr`：用于存储分割结果的字符串向量引用。
    - `keep_delimiter`：是否保留分隔符，默认为 `false`。
- 返回值：无
- 作用：将原始字符串按照给定的分隔符进行分割，并将结果存储在 `arr` 中。如果 `keep_delimiter` 为 `true`，则分割结果中会保留分隔符。
## static std::string UrlDecode(const std::string &url)
- 参数：
    - `url`：要进行 URL 解码的字符串。
- 返回值：`std::string`类型，解码后的字符串。
- 作用：对给定的 URL 进行解码操作。
## static std::string UrlEncode(const std::string &url)
- 参数：
    - `url`：要进行 URL 编码的字符串。
- 返回值：`std::string`类型，编码后的字符串。
- 作用：对给定的字符串进行 URL 编码操作。
