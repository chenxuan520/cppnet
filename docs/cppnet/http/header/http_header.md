# cppnet::HttpHeader
## ContentType
- 这是一个枚举类，定义了多种HTTP头部的内容类型，包括常见的文本类型（如 `kTextPlain`、`kTextHtml`等）、应用类型（如 `kApplicationJson`、`kApplicationOctetStream`等）、图像类型（如 `kImagePng`、`kImageJpeg`等），还有 `kCustom` 用于自定义类型以及 `kUnknown` 表示未知类型。
## static ContentType ConvertToContentType(const std::string &content_type)
- 参数：
    - `content_type`：字符串类型的内容类型表示。
- 返回值：`ContentType`类型，对应的内容类型枚举值。
- 作用：将给定的字符串形式的内容类型转换为枚举值。
## static ContentType ConvertFileType(const std::string &file_type)
- 参数：
    - `file_type`：字符串类型的文件类型表示。
- 返回值：`ContentType`类型，对应的内容类型枚举值。
- 作用：将给定的字符串形式的文件类型转换为内容类型枚举值。
## static std::string ConvertToStr(const ContentType &content_type, const std::string &file_type = "")
- 参数：
    - `content_type`：`ContentType`类型的内容类型枚举值。
    - `file_type`：可选的字符串类型的文件类型表示，默认为空字符串。
- 返回值：`std::string`类型，对应的内容类型字符串表示，可能会结合文件类型进行转换。
- 作用：将给定的内容类型枚举值转换为字符串表示，会根据文件类型做进一步处理。
## static void SetCustomContentType(const std::string &file_type, const std::string &content_type)
- 参数：
    - `file_type`：字符串类型的文件类型。
    - `content_type`：字符串类型的自定义内容类型。
- 返回值：无
- 作用：设置自定义的内容类型，将文件类型与对应的自定义内容类型进行关联。
## HttpHeader()
- 参数：无
- 返回值：无
- 作用：构造一个 HttpHeader 对象。
## int Parse(const std::string &origin_header)
- 参数：
    - `origin_header`：原始的HTTP头部字符串。
- 返回值：`int`类型，0表示成功，-1表示无效的头部。
- 作用：解析给定的原始HTTP头部。
## void Add(const std::string &key, const std::string &value)
- 参数：
    - `key`：头部键。
    - `value`：头部值。
- 返回值：无
- 作用：向HTTP头部添加一个键值对。
## std::string Get(const std::string &key) const
- 参数：
    - `key`：头部键。
- 返回值：`std::string`类型，头部值。
- 作用：根据头部键获取头部值。
## std::string ToString() const
- 参数：无
- 返回值：`std::string`类型，HTTP头部的字符串表示。
- 作用：将HTTP头部转换为字符串。
## std::string &operator[](const std::string &key)
- 参数：
    - `key`：头部键。
- 返回值：`std::string&`类型，头部值的引用。
- 作用：通过头部键获取头部值的引用。
## inline std::string err_msg()
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
## void Clear()
- 参数：无
- 返回值：无
- 作用：清空HTTP头部。
## int GetContentLength() const
- 参数：无
- 返回值：`int`类型，内容长度。
- 作用：获取HTTP头部中的内容长度。
## void SetContentLength(int content_length)
- 参数：
    - `content_length`：内容长度。
- 返回值：无
- 作用：设置HTTP头部中的内容长度。
## ContentType GetContentType() const
- 参数：无
- 返回值：`ContentType`类型，内容类型枚举值。
- 作用：获取HTTP头部中的内容类型。
## void SetContentType(ContentType content_type)
- 参数：
    - `content_type`：内容类型枚举值。
- 返回值：无
- 作用：设置HTTP头部中的内容类型为枚举值形式。
## void SetContentType(const std::string &content_type)
- 参数：
    - `content_type`：字符串形式的内容类型。
- 返回值：无
- 作用：设置HTTP头部中的内容类型为字符串形式。
## std::string GetHost() const
- 参数：无
- 返回值：`std::string`类型，主机名。
- 作用：获取HTTP头部中的主机名。
## void SetHost(const std::string &host)
- 参数：
    - `host`：主机名。
- 返回值：无
- 作用：设置HTTP头部中的主机名。
## void SetLongConnection(bool is_long_connect = true)
- 参数：
    - `is_long_connect`：布尔类型，指示是否为长连接，默认为true。
- 返回值：无
- 作用：设置HTTP头部中的连接类型为长连接或短连接。
## std::string GetCookieVal(const std::string &key) const
- 参数：
    - `key`：Cookie的键。
- 返回值：`std::string`类型，Cookie的值。
- 作用：根据Cookie的键获取其值。
## void SetCookie(const Cookie &cookie)
- 参数：
    - `cookie`：`Cookie`结构体，包含Cookie的各种属性。
- 返回值：无
- 作用：设置HTTP头部中的Cookie。
## private static std::unordered_map\<std::string, std::string\> custom_content_type_
- 这是一个静态的无序映射，用于存储自定义的文件类型与对应的内容类型的关联关系，以便在处理自定义内容类型时使用。
