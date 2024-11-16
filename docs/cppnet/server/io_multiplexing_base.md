# cppnet::IOMultiplexingType
- 这是一个枚举类，定义了不同的 I/O 多路复用类型，包括 `kEpoll`（适用于 Linux）、`kSelect`（适用于 Windows）和 `kQueue`（适用于 Mac Unix）。
# cppnet::IOMultiplexingBase
## IOEvent
- 这是一个枚举类，定义了不同的 I/O 事件类型，包括 `kIOEventRead`（可读事件）、`kIOEventLeave`（离开事件）和 `kIOEventError`（错误事件）。
## TriggerType
- 这是一个枚举类，定义了不同的触发类型，包括 `kEdgeTrigger`（边缘触发）和 `kLevelTrigger`（水平触发）。
## virtual int Init()
- 参数：无
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：初始化 I/O 多路复用对象。
## virtual int MonitorSoc(const Socket &fd)
- 参数：
    - `fd`：要监视的套接字文件描述符。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：将套接字添加到监视列表中。
## virtual int RemoveSoc(const Socket &fd)
- 参数：
    - `fd`：要移除监视的套接字文件描述符。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：从监视列表中移除套接字。
## virtual int Loop(NotifyCallBack callback)
- 参数：
    - `callback`：当事件发生时的回调函数。
- 返回值：`int`类型，0 表示成功，-1 表示失败。
- 作用：进入事件循环，等待并处理事件。
## virtual void Close()
- 参数：无
- 返回值：无
- 作用：关闭 I/O 多路复用对象。
## virtual void Stop()
- 参数：无
- 返回值：无
- 作用：停止事件循环。
## inline std::string err_msg() const
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
## inline void set_max_event_num(int num)
- 参数：
    - `num`：最大事件数量。
- 返回值：无
- 作用：设置最大事件数量。
## inline void set_wait_timeout(int timeout)
- 参数：
    - `timeout`：等待超时时间。
- 返回值：无
- 作用：设置等待超时时间。
## inline void set_trigger_type(TriggerType type)
- 参数：
    - `type`：触发类型枚举值。
- 返回值：无
- 作用：设置触发类型。
