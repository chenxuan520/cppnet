# cppnet::HttpReq
## HttpReq()
- 参数：无
- 返回值：无
- 作用：构造一个 HttpReq 对象。
## int Parse(const std::string &origin_req)
- 参数：
    - `origin_req`：原始的 HTTP 请求字符串。
- 返回值：`int`类型，0 表示成功，-1 表示无效的 HTTP 请求。
- 作用：解析给定的原始 HTTP 请求。
## int Build(std::string &req_str)
- 参数：
    - `req_str`：用于存储构建后的 HTTP 请求字符串的引用。
- 返回值：`int`类型，0 表示成功，-1 表示无效的 HTTP 请求。
- 作用：构建 HTTP 请求并存储为字符串。
## void Clear()
- 参数：无
- 返回值：无
- 作用：清空 HTTP 请求的内容。
## void GET(const std::string &url, const KVMappings &params = KVMappings(), const KVMappings &extra_header = KVMappings())
- 参数：
    - `url`：请求的 URL。
    - `params`：键值对映射，表示请求参数，默认为空映射。
    - `extra_header`：键值对映射，表示额外的请求头部，默认为空映射。
- 返回值：无
- 作用：构建一个 GET 请求。
## void POST(const std::string &url, const std::string &body, const KVMappings &extra_header = KVMappings())
- 参数：
    - `url`：请求的 URL。
    - `body`：请求体内容。
    - `extra_header`：键值对映射，表示额外的请求头部，默认为空映射。
- 返回值：无
- 作用：构建一个带有请求体的 POST 请求。
## void POST(const std::string &url, const KVMappings & params, const KVMappings &extra_header = KVMappings())
- KVMappings含义 `KVMappings = std::unordered_map<std::string, std::string>`
- 参数：
    - `url`：请求的 URL。
    - `params`：键值对映射，表示请求参数。
    - `extra_header`：键值对映射，表示额外的请求头部，默认为空映射。
- 返回值：无
- 作用：构建一个带有请求参数的 POST 请求。
## HttpHeader &header()
- 参数：无
- 返回值：`HttpHeader &`，HTTP 请求头部对象的引用。
- 作用：获取 HTTP 请求的头部对象。
## HttpRoute &route()
- 参数：无
- 返回值：`HttpRoute &`，HTTP 请求路由对象的引用。
- 作用：获取 HTTP 请求的路由对象。
## HttpMethod &method()
- 参数：无
- 返回值：`HttpMethod &`，HTTP 请求方法对象的引用。
- 作用：获取 HTTP 请求的方法对象。
## HttpVersion &version()
- 参数：无
- 返回值：`HttpVersion &`，HTTP 请求版本对象的引用。
- 作用：获取 HTTP 请求的版本对象。
## std::string &body()
- 参数：无
- 返回值：`std::string &`，HTTP 请求体的引用。
- 作用：获取 HTTP 请求体。
## std::string err_msg()
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
