# cppnet::TcpServer
## Event
- 这是一个枚举类，定义了不同的事件类型，包括 `kEventAccept`（接受连接事件）、`kEventRead`（读取事件）、`kEventLeave`（离开事件）和 `kEventError`（错误事件）。
## Mode
- 这是一个枚举类，定义了服务器的运行模式，包括 `kIOMultiplexing`（I/O 多路复用模式）、`kMultiThread`（多线程模式）和 `kMixed`（混合模式）。
## TcpServer()
- 参数：无
- 返回值：无
- 作用：构造一个默认的 TcpServer 对象。
## TcpServer(const std::string &ip, uint16_t port)
- 参数：
    - `ip`：服务器的 IP 地址字符串。
    - `port`：服务器的端口号。
- 返回值：无
- 作用：构造一个指定 IP 地址和端口号的 TcpServer 对象。
## TcpServer(Address &addr)
- 参数：
    - `addr`：服务器的地址对象。
- 返回值：无
- 作用：构造一个使用给定地址对象的 TcpServer 对象。
## virtual ~TcpServer()
- 参数：无
- 返回值：无
- 作用：TcpServer 类的析构函数。
## int Init()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 TCP 服务器，包括创建文件描述符、绑定地址、设置非阻塞等操作。
## void Stop()
- 参数：无
- 返回值：无
- 作用：停止 TCP 服务器，关闭文件描述符等资源。
## void Clean()
- 参数：无
- 返回值：无
- 作用：清理所有资源。
## void Register(EventCallBack cb)
- 参数：
    - `cb`：事件回调函数。
- 返回值：无
- 作用：注册事件回调函数，当特定事件发生时调用。
## int EventLoop()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：进入事件循环，等待并处理各种事件。
## int RemoveSoc(const Socket &fd)
- 参数：
    - `fd`：要移除的套接字文件描述符。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：从事件监听中移除指定的套接字，但不关闭它。
## int AddSoc(const Socket &fd)
- 参数：
    - `fd`：要添加的套接字文件描述符。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：将指定的套接字添加到事件监听中。
## int WakeUp()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：唤醒服务器，在停止后可选地用于退出。
## inline void set_mode(Mode mode)
- 参数：
    - `mode`：服务器的运行模式枚举值。
- 返回值：无
- 作用：设置服务器的运行模式。
## inline void set_max_connect_queue(int max_connect_queue)
- 参数：
    - `max_connect_queue`：最大连接队列长度。
- 返回值：无
- 作用：设置服务器的最大连接队列长度。
## inline void set_addr(const Address &addr)
- 参数：
    - `addr`：服务器的地址对象。
- 返回值：无
- 作用：设置服务器的地址。
## inline Mode mode() const
- 参数：无
- 返回值：`Mode`类型，服务器的运行模式枚举值。
- 作用：获取服务器的运行模式。
## inline Address addr() const
- 参数：无
- 返回值：`Address`类型，服务器的地址对象。
- 作用：获取服务器的地址。
## inline std::string err_msg() const
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
## inline std::shared_ptr\<IOMultiplexingBase\> io_multiplexing()
- 参数：无
- 返回值：指向 `IOMultiplexingBase` 的智能指针，I/O 多路复用对象。
- 作用：获取服务器的 I/O 多路复用对象。
## inline std::shared_ptr\<ThreadPool\> thread_pool()
- 参数：无
- 返回值：指向 `ThreadPool` 的智能指针，线程池对象。
- 作用：获取服务器的线程池对象。
## Socket CreateSocket()
- 参数：无
- 返回值：`Socket`类型，创建的套接字文件描述符。
- 作用：创建服务器套接字。
## int InitMode()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：根据服务器的运行模式进行初始化。
## void HandleAccept()
- 参数：无
- 返回值：无
- 作用：处理接受连接事件。
## void HandleRead(int fd)
- 参数：
    - `fd`：套接字文件描述符。
- 返回值：无
- 作用：处理读取事件。
## void HandleLeave(int fd)
- 参数：
    - `fd`：套接字文件描述符。
- 返回值：无
- 作用：处理离开事件。
## void HandleError(int fd)
- 参数：
    - `fd`：套接字文件描述符。
- 返回值：无
- 作用：处理错误事件。
