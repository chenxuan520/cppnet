# cppnet::Address
## Address()
- 参数：无
- 返回值：无
- 作用：构造一个默认的 Address 对象。
## Address(const std::string &ip, uint16_t port)
- 参数：
    - `ip`：IP 地址字符串。
    - `port`：端口号。
- 返回值：无
- 作用：构造一个指定 IP 地址和端口号的 Address 对象。
## int Init(const std::string &ip, uint16_t port)
- 参数：
    - `ip`：IP 地址字符串。
    - `port`：端口号。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：使用给定的 IP 地址和端口号初始化 Address 对象。
## int InitWithDomain(const std::string &domain, uint16_t port)
- 参数：
    - `domain`：域名字符串。
    - `port`：端口号。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：使用给定的域名和端口号初始化 Address 对象。
## void GetIPAndPort(std::string &ip, uint16_t &port) const
- 参数：
    - `ip`：用于存储 IP 地址的字符串引用。
    - `port`：用于存储端口号的变量引用。
- 返回值：无
- 作用：从 Address 对象中获取 IP 地址和端口号。
## std::string ToString() const
- 参数：无
- 返回值：`std::string`类型，地址的字符串表示。
- 作用：将 Address 对象转换为字符串表示。
## sockaddr_in *GetAddr()
- 参数：无
- 返回值：指向`sockaddr_in`结构体的指针。
- 作用：获取底层的`sockaddr_in`结构体指针。
## sockaddr *GetSockAddr()
- 参数：无
- 返回值：指向`sockaddr`结构体的指针。
- 作用：获取底层的`sockaddr`结构体指针。
## static socklen_t *GetAddrLen()
- 参数：无
- 返回值：指向`socklen_t`类型的指针。
- 作用：获取地址长度的指针，可能用于某些系统调用。
## static int GetPeerAddr(int fd, Address &addr)
- 参数：
    - `fd`：套接字文件描述符。
    - `addr`：用于存储对等方地址的 Address 对象引用。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：获取指定套接字的对等方地址，并存储在`addr`中。
