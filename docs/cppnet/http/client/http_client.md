# cppnet::HttpClient
## HttpClient()
- 参数：无
- 返回值：无
- 作用：构造一个 HttpClient 对象。
## ~HttpClient()
- 参数：无
- 返回值：无
- 作用：HttpClient 类的析构函数，用于关闭客户端连接。
## int Init(Address &addr)
- 参数：
    - `addr`：服务器地址对象。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化客户端，连接到指定的服务器地址。
## int Send(HttpReq &req, HttpResp &resp)
- 参数：
    - `req`：Http 请求对象的引用。
    - `resp`：Http 响应对象的引用。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：发送 Http 请求并接收 Http 响应。
## void Close()
- 参数：无
- 返回值：无
- 作用：关闭客户端连接。
## int InitSSL(Address &addr)
- 参数：
    - `addr`：服务器地址对象。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 SSL 客户端，连接到指定的服务器地址。
## Socket &socket()
- 参数：无
- 返回值：`Socket &`，套接字对象的引用。
- 作用：获取客户端的套接字对象。
## std::string err_msg()
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
