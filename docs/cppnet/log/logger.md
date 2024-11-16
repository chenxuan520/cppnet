# cppnet::Logger
## Level
- 这是一个枚举类，定义了不同的日志级别，包括 `DEBUG`、`INFO`、`WARN`、`ERROR` 和 `FATAL`。
## Logger()
- 参数：无
- 返回值：无
- 作用：构造一个 Logger 对象。
## virtual void Debug(const std::string &msg)
- 参数：
    - `msg`：要记录的调试信息字符串。
- 返回值：无
- 作用：记录调试级别的日志信息，默认实现为空。
## virtual void Info(const std::string &msg)
- 参数：
    - `msg`：要记录的信息字符串。
- 返回值：无
- 作用：记录信息级别的日志信息，默认实现为空。
## virtual void Warn(const std::string &msg)
- 参数：
    - `msg`：要记录的警告信息字符串。
- 返回值：无
- 作用：记录警告级别的日志信息，默认实现为空。
## virtual void Error(const std::string &msg)
- 参数：
    - `msg`：要记录的错误信息字符串。
- 返回值：无
- 作用：记录错误级别的日志信息，默认实现为空。
## virtual void Fatal(const std::string &msg)
- 参数：
    - `msg`：要记录的致命错误信息字符串。
- 返回值：无
- 作用：记录致命错误级别的日志信息，默认实现为空。
## void set_level(Level level)
- 参数：
    - `level`：日志级别枚举值。
- 返回值：无
- 作用：设置日志记录的级别
# 子类
### StdLogger
- 信息打印到标准输入输出
### FileLogger
- 信息打印到文件中, 通过 `Init` 函数制定文件位置
### MultiLogger
- 做个logger 作为init, 同时打听到多个logger中
