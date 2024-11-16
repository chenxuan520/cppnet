# cppnet::HttpResp
## HttpResp()
- 参数：无
- 返回值：无
- 作用：构造一个 HttpResp 对象。
## int Parse(const std::string &origin_resp)
- 参数：
    - `origin_resp`：原始的 HTTP 响应字符串。
- 返回值：`int`类型，可能表示解析是否成功，具体含义需结合实际情况判断。
- 作用：解析给定的原始 HTTP 响应。
## HttpVersion &version()
- 参数：无
- 返回值：`HttpVersion &`，HTTP 版本对象的引用。
- 作用：获取 HTTP 响应的版本对象。
## HttpHeader &header()
- 参数：无
- 返回值：`HttpHeader &`，HTTP 头部对象的引用。
- 作用：获取 HTTP 响应的头部对象。
## std::string &body()
- 参数：无
- 返回值：`std::string &`，HTTP 响应体的引用。
- 作用：获取 HTTP 响应体。
## HttpStatusCode &status_code()
- 参数：无
- 返回值：`HttpStatusCode &`，HTTP 状态码对象的引用。
- 作用：获取 HTTP 响应的状态码对象。
## int Build(std::string &resp_str)
- 参数：
    - `resp_str`：用于存储构建后的 HTTP 响应字符串的引用。
- 返回值：`int`类型，可能表示构建是否成功，具体含义需结合实际情况判断。
- 作用：将 HTTP 响应构建为字符串。
## void Clear()
- 参数：无
- 返回值：无
- 作用：清空 HTTP 响应的内容。
## void NotFound()
- 参数：无
- 返回值：无
- 作用：设置 HTTP 响应为未找到的状态。
## void Redirect(const std::string &location, bool forever = false)
- 参数：
    - `location`：重定向的目标位置。
    - `forever`：布尔类型，指示是否是永久重定向。
- 返回值：无
- 作用：设置 HTTP 响应为重定向状态。
## void SuccessWithBinary(const std::string &body)
- 参数：
    - `body`：二进制形式的成功响应体内容。
- 返回值：无
- 作用：设置 HTTP 响应为成功状态，并带有二进制响应体。
## void SuccessWithJson(const std::string &body)
- 参数：
    - `body`：JSON 格式的成功响应体内容。
- 返回值：无
- 作用：设置 HTTP 响应为成功状态，并带有 JSON 格式的响应体。
## void Success(HttpHeader::ContentType content_type, const std::string &body = "")
- 参数：
    - `content_type`：HTTP 头部的内容类型。
    - `body`：成功响应体内容，默认为空字符串。
- 返回值：无
- 作用：设置 HTTP 响应为成功状态，并指定内容类型和响应体。
## void BadRequest(const std::string &body = "")
- 参数：
    - `body`：错误请求的响应体内容，默认为空字符串。
- 返回值：无
- 作用：设置 HTTP 响应为错误请求状态，并带有响应体。
## void InternalServerError(const std::string &body = "")
- 参数：
    - `body`：内部服务器错误的响应体内容，默认为空字符串。
- 返回值：无
- 作用：设置 HTTP 响应为内部服务器错误状态，并带有响应体。
## void Json(HttpStatusCode status_code, const std::string &body)
- 参数：
    - `status_code`：HTTP 状态码。
    - `body`：JSON 格式的响应体内容。
- 返回值：无
- 作用：设置 HTTP 响应为带有特定状态码和 JSON 格式响应体的状态。
## void Text(HttpStatusCode status_code, const std::string &body)
- 参数：
    - `status_code`：HTTP 状态码。
    - `body`：文本格式的响应体内容。
- 返回值：无
- 作用：设置 HTTP 响应为带有特定状态码和文本格式响应体的状态。
## std::string err_msg()
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
在 `HttpResp` 类中添加一个方法，用于设置 HTTP 响应的内容类型
如何在 `HttpResp` 类中处理错误情况？
提供一个使用 `HttpResp` 类的示例代码
