# cppnet::HttpFilter
- 这是一个抽象基类，用于定义 HTTP 过滤器的接口。
- 目前实现的过滤器包括：`htto_host_filter`,`http_method_filter`
    - `http_host_filter`：用于检查 HTTP 请求的 Host 头部是否匹配指定的主机名。
    - `http_method_filter`：用于检查 HTTP 请求的方法是否匹配指定的方法。
## HttpFilter()
- 参数：无
- 返回值：无
- 作用：构造一个 HttpFilter 对象。
## ~HttpFilter()
- 参数：无
- 返回值：无
- 作用：HttpFilter 类的虚析构函数。
## bool IsMatchFilter(HttpReq &req)
- 参数：
    - `req`：Http 请求对象的引用。
- 返回值：`bool`类型，true 表示匹配过滤器，false 表示不匹配过滤器。
- 作用：检查给定的 Http 请求是否匹配该过滤器。
