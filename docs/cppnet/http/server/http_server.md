# cppnet::HttpContext
## HttpContext(HttpReq &req, HttpResp &resp, std::shared_ptr\<Socket\> &soc)
- 参数：
    - `req`：Http 请求对象的引用。
    - `resp`：Http 响应对象的引用。
    - `soc`：指向套接字的智能指针的引用。
- 返回值：无
- 作用：构造一个 HttpContext 对象，用于存储 Http 请求、响应和套接字信息。
## Next()
- 参数：无
- 返回值：无
- 作用：设置继续执行的标志，在中间件中使用。
## Abort()
- 参数：无
- 返回值：无
- 作用：设置停止执行的标志，在中间件中使用。
## is_continue()
- 参数：无
- 返回值：`bool`类型，表示是否继续执行。
- 作用：获取当前的执行状态标志。
## T Get(const std::string &key)
- 参数：
    - `key`：字符串类型，用于查找内联数据的键。
- 返回值：模板类型`T`的值，如果类型转换失败则返回默认值。
- 作用：根据给定的键获取内联数据。
## void Set(const std::string &key, const T &value)
- 参数：
    - `key`：字符串类型，内联数据的键。
    - `value`：模板类型`T`的值，要设置的内联数据。
- 返回值：无
- 作用：设置内联数据。
## req()
- 参数：无
- 返回值：`HttpReq &`，Http 请求对象的引用。
- 作用：获取 Http 请求对象。
## resp()
- 参数：无
- 返回值：`HttpResp &`，Http 响应对象的引用。
- 作用：获取 Http 响应对象。
## soc()
- 参数：无
- 返回值：`Socket &`，套接字对象的引用。
- 作用：获取套接字对象。
# cppnet::HttpGroup
## int GET(const std::string &path, HttpCallback callback, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {})
- 参数：
    - `path`：字符串类型，请求路径。
    - `callback`：函数指针类型，请求回调函数。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：注册 GET 请求的处理函数和过滤器。
## int GET(const std::string &path, std::initializer_list\<HttpCallback\> callbacks, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {})
- 参数：
    - `path`：字符串类型，请求路径。
    - `callbacks`：初始化列表类型，包含多个请求回调函数。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：注册多个 GET 请求的处理函数和过滤器。
## int POST(const std::string &path, HttpCallback callback, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {})
- 参数：
    - `path`：字符串类型，请求路径。
    - `callback`：函数指针类型，请求回调函数。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：注册 POST 请求的处理函数和过滤器。
## int POST(const std::string &path, std::initializer_list\<HttpCallback\> callbacks, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {})
- 参数：
    - `path`：字符串类型，请求路径。
    - `callbacks`：初始化列表类型，包含多个请求回调函数。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：注册多个 POST 请求的处理函数和过滤器。
## int RegisterHandler(const std::string &path, HttpCallback callback, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {}, bool is_exact_match = true)
- 参数：
    - `path`：字符串类型，请求路径。
    - `callback`：函数指针类型，请求回调函数。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
    - `is_exact_match`：布尔类型，是否精确匹配路径。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：注册通用请求的处理函数和过滤器，并指定匹配方式。
## int RegisterHandler(const std::string &path, std::initializer_list\<HttpCallback\> callbacks, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {}, bool is_exact_match = true)
- 参数：
    - `path`：字符串类型，请求路径。
    - `callbacks`：初始化列表类型，包含多个请求回调函数。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
    - `is_exact_match`：布尔类型，是否精确匹配路径。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：注册多个通用请求的处理函数和过滤器，并指定匹配方式。
## int Use(HttpCallback callback, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {})
- 参数：
    - `callback`：函数指针类型，中间件回调函数。
    - `filters`：包含`HttpFilter`智能指针的向量，中间件过滤器。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：注册中间件。
## int StaticDir(const std::string &path, const std::string &dir_path, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {})
- 参数：
    - `path`：字符串类型，请求路径。
    - `dir_path`：字符串类型，静态目录路径。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：添加静态目录。
## int StaticFile(const std::string &path, const std::string &file_path, const std::vector\<std::shared_ptr\<HttpFilter\>\> &filters = {})
- 参数：
    - `path`：字符串类型，请求路径。
    - `file_path`：字符串类型，静态文件路径。
    - `filters`：包含`HttpFilter`智能指针的向量，请求过滤器。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：添加静态文件。
## HttpGroup Group(const std::string &route)
- 参数：
    - `route`：字符串类型，子路由路径。
- 返回值：`HttpGroup`类型，新的 HttpGroup 对象。
- 作用：创建一个新的 HttpGroup 对象，用于分组路由。
## std::string err_msg()
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
## std::shared_ptr\<Logger\> logger()
- 参数：无
- 返回值：指向日志对象的智能指针。
- 作用：获取日志对象。
## void set_logger(std::shared_ptr\<Logger\> logger)
- 参数：
    - `logger`：指向日志对象的智能指针。
- 返回值：无
- 作用：设置日志对象。
# cppnet::HttpServer
- 主要使用类,构建http 服务器
## HttpServer()
- 参数：无
- 返回值：无
- 作用：构造一个 HttpServer 对象，继承自 HttpGroup。
## int Init(const Address &addr)
- 参数：
    - `addr`：地址对象。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 Http 服务器。
## int Run()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：运行 Http 服务器。
## void Stop()
- 参数：无
- 返回值：无
- 作用：停止 Http 服务器。
## TcpServer &server()
- 参数：无
- 返回值：`TcpServer &`，Tcp 服务器对象的引用。
- 作用：获取 Tcp 服务器对象。
## void SetReadTimeout(unsigned timeout_sec, unsigned timeout_usec)
- 参数：
    - `timeout_sec`：秒数。
    - `timeout_usec`：微秒数。
- 返回值：无
- 作用：设置读取超时时间。
## void SetWriteTimeout(unsigned timeout_sec, unsigned timeout_usec)
- 参数：
    - `timeout_sec`：秒数。
    - `timeout_usec`：微秒数。
- 返回值：无
- 作用：设置写入超时时间。
## int InitSSL(const Address &addr, std::shared_ptr\<SSLContext\> ssl_context)
- 参数：
    - `addr`：地址对象。
    - `ssl_context`：指向 SSL 上下文的智能指针。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 HTTPS 服务器。
- 注意: 这个部分只有在 开启ssl才可用
