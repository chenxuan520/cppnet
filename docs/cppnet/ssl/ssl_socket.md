# cppnet::SSLSocket
## SSLSocket(SSL \*ssl, const Socket &soc, Mode mode = Mode::kQuickly)
- 参数：
    - `ssl`：OpenSSL 的 SSL 指针。
    - `soc`：普通套接字对象。
    - `mode`: socket 模式
        - kQuickly: 快捷模式, 默认此模式, 多线程并发操作同一个 soc 可能段错误。
        - kSafely:  安全模式, 此时对这个 socket 的操作都会加锁, 并发安全。
- 返回值：无
- 作用：构造一个 SSLSocket 对象，关联给定的 SSL 指针和套接字。
## int Connect(Address &addr) override
- 参数：
    - `addr`：服务器地址对象。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：重写父类的连接方法，实现通过 SSL 连接服务器。
## int Close() override
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：重写父类的关闭方法，关闭 SSL 和套接字。
## int CloseSSL()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：关闭 SSL。
## inline std::string err_msg() const
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
## protected inline int IORead(void \*buf, size_t len, int flag = 0) override
- 参数：
    - `buf`：用于存储读取数据的缓冲区指针。
    - `len`：要读取的字节长度。
    - `flag`：读取标志，默认为 0。
- 返回值：`int`类型，实际读取的字节长度。
- 作用：重写父类的读取方法，使用 SSL 进行读取。
## protected inline int IOWrite(const void \*buf, size_t len, int flag = 0) override
- 参数：
    - `buf`：要写入的数据缓冲区指针。
    - `len`：要写入的字节长度。
    - `flag`：写入标志，默认为 0。
- 返回值：`int`类型，实际写入的字节长度。
- 作用：重写父类的写入方法，使用 SSL 进行写入。
