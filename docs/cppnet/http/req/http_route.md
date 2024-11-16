# cppnet::HttpRoute
## HttpRoute()
- 参数：无
- 返回值：无
- 作用：构造一个 HttpRoute 对象。
## int Parse(const std::string &origin_path)
- 参数：
    - `origin_path`：原始路径字符串。
- 返回值：`int`类型，可能表示解析是否成功，具体含义需结合实际情况判断。
- 作用：解析给定的原始路径。
## void AddParam(const std::string &key, const std::string &value)
- 参数：
    - `key`：参数键。
    - `value`：参数值。
- 返回值：无
- 作用：向路由中添加参数。
## std::string GetParam(const std::string &key) const
- 参数：
    - `key`：参数键。
- 返回值：`std::string`类型，参数值。
- 作用：根据参数键获取参数值。
## inline void SetPath(const std::string &path)
- 参数：
    - `path`：路径字符串。
- 返回值：无
- 作用：设置路由的路径。
## inline std::string GetPath() const
- 参数：无
- 返回值：`std::string`类型，路径字符串。
- 作用：获取路由的路径。
## std::string ToString() const
- 参数：无
- 返回值：`std::string`类型，路由的字符串表示。
- 作用：将路由转换为字符串。
## inline std::string &operator[](const std::string &key)
- 参数：
    - `key`：参数键。
- 返回值：`std::string&`类型，参数值的引用。
- 作用：通过参数键获取参数值的引用。
## inline std::string err_msg()
- 参数：无
- 返回值：`std::string`类型，错误消息。
- 作用：获取错误消息。
## void Clear()
- 参数：无
- 返回值：无
- 作用：清空路由。
