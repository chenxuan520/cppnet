# cppnet::Socket
## Status
- 这是一个枚举类，定义了套接字的不同状态，包括 `kUninit`（未初始化）、`kInit`（已初始化）和 `kClosed`（已关闭）。
## Socket()
- 参数：无
- 返回值：无
- 作用：构造一个默认的 Socket 对象。
## Socket(int fd)
- 参数：
    - `fd`：套接字文件描述符。
- 返回值：无
- 作用：构造一个使用给定文件描述符的 Socket 对象。
## int Init()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化套接字。
## int InitUdp()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 UDP 套接字。
## int Connect(Address &addr)
- 参数：
    - `addr`：服务器地址对象。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：尝试连接到服务器。
## int Bind(Address &addr)
- 参数：
    - `addr`：服务器地址对象。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：绑定套接字到指定地址。
## Socket Accept(Address &addr)
- 参数：
    - `addr`：服务器地址对象。
- 返回值：`Socket`类型，新接受的套接字对象。
- 作用：接受连接并返回新的套接字对象。
## int Listen(int max_connect_queue)
- 参数：
    - `max_connect_queue`：最大连接队列长度。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：监听套接字上的连接请求。
## int Close()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：关闭套接字。
## int Read(std::string &buf, size_t len, bool complete = false)
- 参数：
    - `buf`：用于存储读取数据的字符串引用。
    - `len`：要读取的字节长度。
    - `complete`：如果为真，则必须读取 `len` 个字节的数据。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：从套接字读取数据到字符串中。
## int Read(void \*buf, size_t len, bool complete = false)
- 参数：
    - `buf`：用于存储读取数据的缓冲区指针。
    - `len`：要读取的字节长度。
    - `complete`：如果为真，则必须读取 `len` 个字节的数据。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：从套接字读取数据到缓冲区中。
## int ReadUntil(std::string &buf, const std::string &delim)
- 参数：
    - `buf`：用于存储读取数据的字符串引用。
    - `delim`：分隔符字符串。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：从套接字读取数据直到遇到分隔符（包括分隔符）。
## int ReadPeek(std::string &buf, size_t len)
- 参数：
    - `buf`：用于存储读取数据的字符串引用。
    - `len`：要读取的字节长度。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：从套接字读取数据但不从套接字中移除数据。
## int ReadUdp(std::string &buf, size_t len, Address &addr)
- 参数：
    - `buf`：用于存储读取数据的字符串引用。
    - `len`：要读取的字节长度。
    - `addr`：用于存储 UDP 来源地址的对象引用。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：从 UDP 套接字读取数据并获取来源地址。
## int ReadUdp(void \*buf, size_t len, Address &addr)
- 参数：
    - `buf`：用于存储读取数据的缓冲区指针。
    - `len`：要读取的字节长度。
    - `addr`：用于存储 UDP 来源地址的对象引用。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：从 UDP 套接字读取数据并获取来源地址。
## int Write(const std::string &buf)
- 参数：
    - `buf`：要写入的数据字符串。
- 返回值：`int`类型，实际写入的字节长度。
- 作用：将字符串数据写入套接字。
## int Write(const void \*buf, size_t len)
- 参数：
    - `buf`：要写入的数据缓冲区指针。
    - `len`：要写入的字节长度。
- 返回值：`int`类型，实际写入的字节长度。
- 作用：将缓冲区中的数据写入套接字。
## int WriteUdp(const std::string &buf, Address &addr)
- 参数：
    - `buf`：要写入的数据字符串。
    - `addr`：UDP 目标地址对象。
- 返回值：`int`类型，实际写入的字节长度。
- 作用：将字符串数据写入 UDP 套接字并指定目标地址。
## int WriteUdp(const void \*buf, size_t len, Address &addr)
- 参数：
    - `buf`：要写入的数据缓冲区指针。
    - `len`：要写入的字节长度。
    - `addr`：UDP 目标地址对象。
- 返回值：`int`类型，实际写入的字节长度。
- 作用：将缓冲区中的数据写入 UDP 套接字并指定目标地址。
## int SetNoBlock() const
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：设置套接字为非阻塞模式。
## int SetBlock() const
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：设置套接字为阻塞模式。
## int SetReuseAddr() const
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：设置套接字可重用地址。
## int SetReadTimeout(int timeout_sec, int timeout_usec) const
- 参数：
    - `timeout_sec`：超时时间的秒数。
    - `timeout_usec`：超时时间的微秒数。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：设置套接字的读取超时时间。
## int SetWriteTimeout(int timeout_sec, int timeout_usec) const
- 参数：
    - `timeout_sec`：超时时间的秒数。
    - `timeout_usec`：超时时间的微秒数。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：设置套接字的写入超时时间。
## int SetSockOpt(int level, int optname, const void \*optval, size_t optlen) const
- 参数：
    - `level`：套接字选项级别。
    - `optname`：套接字选项名称。
    - `optval`：套接字选项值的指针。
    - `optlen`：套接字选项值的长度。
- 返回值：`int`类型，0 表示成功，其他表示失败。
- 作用：设置套接字选项。
## int GetAddr(Address &addr) const
- 参数：
    - `addr`：用于存储套接字地址的对象引用。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：获取套接字的地址并存储在给定的地址对象中。
## Address GetAddr() const
- 参数：无
- 返回值：`Address`类型，套接字的地址对象。
- 作用：获取套接字的地址对象。
## static inline std::string err_msg()
- 参数：无
- 返回值：`std::string`类型，系统错误消息。
- 作用：获取系统错误消息字符串。
## static inline int err_no()
- 参数：无
- 返回值：`int`类型，系统错误编号。
- 作用：获取系统错误编号。
## inline int fd() const
- 参数：无
- 返回值：`int`类型，套接字文件描述符。
- 作用：获取套接字的文件描述符。
## inline Status status() const
- 参数：无
- 返回值：`Status`类型，套接字的状态枚举值。
- 作用：获取套接字的状态。
## bool operator==(const Socket &rhs) const
- 参数：
    - `rhs`：另一个套接字对象。
- 返回值：`bool`类型，判断两个套接字是否相等。
- 作用：比较两个套接字对象是否相等。
## protected inline int IORead(void \*buf, size_t len, int flags = 0)
- 参数：
    - `buf`：用于存储读取数据的缓冲区指针。
    - `len`：要读取的字节长度。
    - `flags`：读取标志，默认为 0。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：内部使用的读取函数，默认使用 `::recv` 进行读取。
## protected inline int IOWrite(const void \*buf, size_t len, int flags = 0)
- 参数：
    - `buf`：要写入的数据缓冲区指针。
    - `len`：要写入的字节长度。
    - `flags`：写入标志，默认为 0。
- 返回值：`int`类型，实际写入的字节长度。
- 作用：内部使用的写入函数，默认使用 `::send` 进行写入。
