# cppnet::SSLContext
## SSLContext()
- 参数：无
- 返回值：无
- 作用：构造一个默认的 SSLContext 对象。
## ~SSLContext()
- 参数：无
- 返回值：无
- 作用：SSLContext 类的析构函数。
## int Init(SSL_CTX *ctx)
- 参数：
    - `ctx`：OpenSSL 的 SSL 上下文指针。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 SSL 上下文。
## int InitCli()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 SSL 客户端上下文。
## int InitSvr(const std::string &cert_data, const std::string &key_data, const std::string &password = "")
- 参数：
    - `cert_data`：证书数据。
    - `key_data`：密钥数据。
    - `password`：密码，默认为空字符串。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：使用证书数据和密钥数据初始化 SSL 服务器上下文。
## int InitSvrFile(const std::string &cert_path, const std::string &key_path, const std::string &password = "")
- 参数：
    - `cert_path`：证书文件路径。
    - `key_path`：密钥文件路径。
    - `password`：密码，默认为空字符串。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：使用证书文件路径和密钥文件路径初始化 SSL 服务器上下文。
## std::shared_ptr\<SSLSocket\> CreateSSLSocket(const Socket &soc)
- 参数：
    - `soc`：普通套接字对象。
- 返回值：指向 `SSLSocket` 的智能指针。
- 作用：根据给定的普通套接字创建一个 SSL 套接字。
## std::shared_ptr\<SSLSocket\> CreateSSLSocket()
- 参数：无
- 返回值：指向 `SSLSocket` 的智能指针。
- 作用：创建一个新的 SSL 套接字。
## std::shared_ptr\<SSLSocket\> AcceptSSL(const Socket &soc)
- 参数：
    - `soc`：普通套接字对象。
- 返回值：指向 `SSLSocket` 的智能指针。
- 作用：接受一个连接并创建一个 SSL 套接字。
## int Close()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：关闭 SSL 上下文。
## const std::string &err_msg() const
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
