# cppnet
> English version is [here](./README.en.md)
- 一个轻量级 C++ 网络框架, cppweb 2.0 升级版
- 避免了之前 cppweb 单个头文件的臃肿的问题, 并且使用更加友善的使用方式
![](https://chenxuanweb.top/cppnet/cppnet.png)
## Author
- **chenxuan**
> ![](http://cdn.androidftp.top/test/2025101017224732pasteboard.paste)
## 仓库地址
- [github](https://github.com/chenxuan520/cppnet)
- [gitee](https://gitee.com/chenxuan520/cppnet)
## 官网
- [chenxuanweb.top/cppnet](https://www.chenxuanweb.top/cppnet)
## Docs
- [cppnet 在线文档地址](https://chenxuan520.github.io/cppnet/)
## 优势
1. 使用简单,入侵性小,**全平台支持**, 不要求强行安装到系统的include目录中, 推荐直接作为一个submodule 引用或者直接使用静态库
2. 使用现代的C++构建, 使用方式和函数和 Go 的 gin 框架类似 , 学习和入门成本小, 可适合作为初学者进行源码学习
3. 轻量化框架, 非常小, 源码不到5000行, 避免了大型网络框架的臃肿
4. 网站的文档体系和测试体系,保障易用性和稳定性
## Quick Start
- 需要编译器支持 C++17
### 使用 Release 包
1. 下载 release 库([Releases · chenxuan520/cppnet](https://github.com/chenxuan520/cppnet/releases)) , 并且解压(地点可以随意, 可以放到系统的 include 也可以不放 下文假设是解压到当前目录), 需要编译器支持C++17
	- 这里如果是需要ssl (需要安装 openssl ),就下载 ssl 的版本, 否则下载默认即可
2. 编写代码, 这一步可以根据需要编写代码, 下面是两个简单的demo , 分别是 没有ssl 和有ssl的两个demo, 具体的函数介绍可以参考文档内容
```cpp
#include "./cppnet/include/cppnet/http/server/http_server.hpp"
#include "./cppnet/include/cppnet/utils/const.hpp"
#include <iostream>

using namespace cppnet;

int main() {
  HttpServer server;
  auto rc = server.Init({"127.0.0.1", 8080});
  if (rc != kSuccess) {
    std::cout << "init server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  server.set_logger(std::make_shared<StdLogger>());
  server.GET("/", [](HttpContext &ctx) {
    ctx.resp().Success(HttpHeader::ContentType::kTextHtml,
                       "<h1>Hello, World!</h1>");
  });
  rc = server.Run();
  if (rc != kSuccess) {
    std::cout << "run server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  return 0;
}
```
- 下面是 SSL 版本, 需要准备好 pem 文件
```cpp
// ssl 版本

#include "./cppnet/include/cppnet/http/server/http_server.hpp"
#include "./cppnet/include/cppnet/utils/const.hpp"
#include <iostream>
#include <memory>

using namespace cppnet;

int main() {
  HttpServer server;
  std::shared_ptr<SSLContext> ssl_ctx = std::make_shared<SSLContext>();
  auto rc = ssl_ctx->InitSvrFile("./ssl/cacert.pem", "./ssl/privkey.pem");
  if (rc != kSuccess) {
    std::cout << "init ssl context wrong " << ssl_ctx->err_msg() << std::endl;
    return rc;
  }
  rc = server.InitSSL({"127.0.0.1", 8080}, ssl_ctx);
  if (rc != kSuccess) {
    std::cout << "init server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  server.set_logger(std::make_shared<StdLogger>());
  server.GET("/", [](HttpContext &ctx) {
    ctx.resp().Success(HttpHeader::ContentType::kTextHtml,
                       "<h1>Hello, World!</h1>");
  });
  rc = server.Run();
  if (rc != kSuccess) {
    std::cout << "run server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  return 0;
}

```
3. 编写编译文件
	1. 如果是使用 CMake 编译, 需要在CMakeLists 添加如下内容 , 实际上就是加上了链接库
	```cmake
	# 是否使用ssl都需要添加
	link_directories(./cppnet/lib)
	link_libraries(-lcppnet)
	
	# **如果使用的是 ssl版本并且需要使用ssl的功能,才需要添加下面的**
	add_definitions(-DCPPNET_OPENSSL)
	link_libraries(-lssl -lcrypto)
	```
	2. 如果是使用 Makefile 编译, 需要在编译选项中添加 `-L./cppnet/lib` 和 `-lcppnet`, 下面是一个 Makefile 编译 demo, 当然如果是ssl的编译也是同理添加 lib 库 和 **添加 CPPNET_OPENSSL 宏**
	```Makefile
	all: libserver-makefile
	
	libserver-makefile:
		g++ -O2 -Wall -std=c++17 ./main.cpp -o libserver-makefile -lcppnet -L./cppnet/lib
		# ssl 版本的是
		g++ -O2 -Wall -std=c++17 ./main.cpp -o libserver-makefile -DCPPNET_OPENSSL -lcppnet -lssl -lcrypto -L./cppnet/lib
	```
4. 执行编译, 获得编译的文件就成功了
### 从源码编译
1. 使用 `git clone https://github.com/chenxuan520/cppnet --recurse-submodules` 拉取源代码
	1. 默认是编译出 ssh版本的, 如果需要无ssl版本的需要手动修改cmake文件
2. 运行 `cd src;./build.sh` 生成的静态库在lib中
    - 需要 cmake
    - 需要编译器支持 C++17
## 性能测试
- 使用 github action的ubuntu-latest进行自动化测试, 机器配置[参考](https://docs.github.com/zh/actions/writing-workflows/choosing-where-your-workflow-runs/choosing-the-runner-for-a-job#%E7%94%A8%E4%BA%8E%E5%85%AC%E5%85%B1%E5%AD%98%E5%82%A8%E5%BA%93%E7%9A%84-github-%E6%89%98%E7%AE%A1%E7%9A%84%E6%A0%87%E5%87%86%E8%BF%90%E8%A1%8C%E5%99%A8)
    - 4核 16G 内存 14G 硬盘
- 根据每次 commit 的代码自动运行更新测试结果, 参考 bench 文件夹下脚本, 图片中也会有 hash 值
- 使用 [vegeta](https://github.com/tsenart/vegeta) 进行压力测试, 分别测试在不同QPS下
    - 平均响应时间
    - p99 响应时间
    - 正确率
    - 等待时间
- 使用 cppnet 框架 和 go gin 框架进行对比, 使用两者实现同样的功能
![](https://chenxuan520.github.io/cppnet/img/result.png)
## More Demo
- 更多demo参考 demo 文件夹和 test 文件夹
### 创建TcpServer
```cpp
  Address addr{"127.0.0.1", 8080};
  TcpServer server{addr};

  // init server
  auto rc = server.Init();
  if (rc != kSuccess) {
    std::cout << "init server wrong " << server.err_msg() << std::endl;
    return rc;
  }

  // init event function
  auto event_func = [&](TcpServer::Event event, TcpServer &server, Socket fd) {
    if (event == TcpServer::kEventAccept) {
      // accept
      std::cout << "accept " << fd.fd() << std::endl;

    } else if (event == TcpServer::kEventRead) {
      // read
      string buf;
      auto ser_rc = fd.Read(buf, msg.size());
      std::cout << "read " << buf << std::endl;

    } else if (event == TcpServer::kEventLeave) {
      // leave
      std::cout << "leave " << fd.fd() << std::endl;
    } else {
      // error event
      std::cout << "dismiss " << fd.fd() << std::endl;
    }
  };
  // register event function
  server.Register(event_func);
```
### socket连接服务器
```cpp
  Socket sock;
  auto rc = sock.Init();
  if (rc != kSuccess) {
    std::cout << "init socket wrong " << sock.err_msg() << std::endl;
    return rc;
  }
  rc = sock.Connect(addr);
  if (rc != kSuccess) {
    std::cout << "connect wrong " << sock.err_msg() << std::endl;
    return rc;
  }
  // write
  string msg = "hello world";
  rc = sock.Write(msg);
  if (rc != kSuccess) {
    std::cout << "write wrong " << sock.err_msg() << std::endl;
    return rc;
  }
  // read
  string buf;
  rc = sock.Read(buf, msg.size());
  if (rc <= 0) {
    std::cout << "read wrong " << sock.err_msg() << std::endl;
    return rc;
  }
  std::cout << "read " << buf << std::endl;
  // close
  rc = sock.Close();
  if (rc != kSuccess) {
    std::cout << "close wrong " << sock.err_msg() << std::endl;
    return rc;
  }
```
### 创建httpserver
```cpp
  HttpServer server;
  auto rc = server.Init({"127.0.0.1", 8080});
  if (rc != kSuccess) {
    std::cout << "init server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  server.set_logger(std::make_shared<StdLogger>());
  server.GET("/", [](HttpContext &ctx) {
    ctx.resp().Success(HttpHeader::ContentType::kTextHtml,
                       "<h1>Hello, World!</h1>");
  });
  rc = server.Run();
  if (rc != kSuccess) {
    std::cout << "run server wrong " << server.err_msg() << std::endl;
    return rc;
  }
```
### 创建httpsserver ssl
```cpp
  HttpServer server;
  std::shared_ptr<SSLContext> ssl_ctx = std::make_shared<SSLContext>();
  auto rc = ssl_ctx->InitSvrFile("./ssl/cacert.pem", "./ssl/privkey.pem");
  if (rc != kSuccess) {
    std::cout << "init ssl context wrong " << ssl_ctx->err_msg() << std::endl;
    return rc;
  }
  rc = server.InitSSL({"127.0.0.1", 8080}, ssl_ctx);
  if (rc != kSuccess) {
    std::cout << "init server wrong " << server.err_msg() << std::endl;
    return rc;
  }
  server.set_logger(std::make_shared<StdLogger>());
  server.GET("/", [](HttpContext &ctx) {
    ctx.resp().Success(HttpHeader::ContentType::kTextHtml,
                       "<h1>Hello, World!</h1>");
  });
  rc = server.Run();
  if (rc != kSuccess) {
    std::cout << "run server wrong " << server.err_msg() << std::endl;
    return rc;
  }
```
### 创建httpclient
```cpp
  HttpClient client;
  Address addr;
  addr.InitWithDomain("www.androidftp.top", 80);
  auto rc = client.Init(addr);
  // 如果是https 443 端口 ssl
  // auto rc = client.InitSSL(addr);
  if (rc != kSuccess) {
    std::cout << "init client wrong " << client.err_msg() << std::endl;
    return rc;
  }
  HttpReq req;
  req.GET("/");
  HttpResp resp;
  rc = client.Send(req, resp);
  if (rc != kSuccess) {
    std::cout << "send request wrong " << client.err_msg() << std::endl;
    return rc;
  }
  std::string resp_str;
  rc = resp.Build(resp_str);
  if (rc != kSuccess) {
    std::cout << "build response wrong " << resp.err_msg() << std::endl;
    return rc;
  }
```
### 定时触发器server
- 使用 AddSoc 监听timesocket, 等待触发即可
```cpp
  TcpServer server{addr};
  auto rc = server.Init();
  if (rc != kSuccess) {
    std::cout << "init server wrong " << server.err_msg() << std::endl;
    return rc;
  }

  TimerSocket timerfd(0, 1e5);
  if (rc != kSuccess) {
    std::cout << "init timerfd wrong " << timerfd.err_msg() << std::endl;
    return rc;
  }
  rc = server.AddSoc(timerfd);
```
## 源码结构
- 仓库结构如下
	- cppnet 放置所以的源码以及编译文件
	- lib 放置生成的静态库以及静态库构建的 CMakeLists 文件
	- test 是测试用例
	- third_party 是第三方仓库, 目前只依赖 cpptest 用于test 文件夹的单元测试
	- build.sh 生成 lib 静态库以及 单元测试的二进制文件
    - docs 是接口文档
    - demo 是参考的事例
    - bench 是性能测试
```tree
.
├── LICENSE
├── README.en.md
├── README.md
├── bench
│   ├── build.sh
│   └── init.sh
├── demo
│   ├── asset
│   ├── build.sh
│   ├── mnist
│   ├── pack
│   ├── simple-http-server
│   ├── use-lib-server
│   ├── use-lib-server-ssl
│   ├── util
│   └── web-server
├── docs
│   ├── cppnet
│   └── update.sh
└── src
    ├── CMakeLists.txt
    ├── build.sh
    ├── cppnet
    ├── lib
    ├── release.sh
    ├── test
    ├── third_party
    └── win_release.sh

```
- 源码主要放置在 `src/cppnet`
```txt
cppnet
├── http
│   ├── client
│   │   ├── http_client.cpp
│   │   └── http_client.hpp
│   ├── header
│   │   ├── http_header.cpp
│   │   └── http_header.hpp
│   ├── req
│   │   ├── http_req.cpp
│   │   ├── http_req.hpp
│   │   ├── method
│   │   │   ├── http_method.cpp
│   │   │   └── http_method.hpp
│   │   └── route
│   │       ├── http_route.cpp
│   │       └── http_route.hpp
│   ├── resp
│   │   ├── http_resp.cpp
│   │   ├── http_resp.hpp
│   │   └── status_code
│   │       ├── http_status_code.cpp
│   │       └── http_status_code.hpp
│   ├── server
│   │   ├── filter
│   │   │   ├── http_filter.hpp
│   │   │   ├── http_host_filter.hpp
│   │   │   └── http_method_filter.hpp
│   │   ├── http_server.cpp
│   │   └── http_server.hpp
│   └── version
│       ├── http_version.cpp
│       └── http_version.hpp
├── log
│   ├── file_logger.cpp
│   ├── file_logger.hpp
│   ├── logger.hpp
│   ├── multi_logger.cpp
│   ├── multi_logger.hpp
│   └── std_logger.hpp
├── server
│   ├── io_multiplexing
│   │   ├── epoll.cpp
│   │   ├── epoll.hpp
│   │   ├── io_multiplexing_base.hpp
│   │   ├── io_multiplexing_factory.cpp
│   │   ├── io_multiplexing_factory.hpp
│   │   ├── kqueue.cpp
│   │   ├── kqueue.hpp
│   │   ├── select.cpp
│   │   └── select.hpp
│   ├── tcp_server.cpp
│   └── tcp_server.hpp
├── socket
│   ├── address.cpp
│   ├── address.hpp
│   ├── socket.cpp
│   └── socket.hpp
├── ssl
│   ├── ssl_context.cpp
│   ├── ssl_context.hpp
│   ├── ssl_socket.cpp
│   └── ssl_socket.hpp
├── timer
│   ├── timer.cpp
│   └── timer.hpp
└── utils
    ├── const.hpp
    ├── file.cpp
    ├── file.hpp
    ├── host.cpp
    ├── host.hpp
    ├── string.cpp
    ├── string.hpp
    ├── threadpool.cpp
    ├── threadpool.hpp
    ├── trie.cpp
    ├── trie.hpp
    ├── version.cpp
    └── version.hpp

19 directories, 61 files
```
### 类关系图
![](http://cdn.androidftp.top/test/2024111122224337cppnet.drawio.png)
## TODO
- [x] 补充完整 readme
- [x] 支持设置LT和ET模式下的事件触发方式
- [x] 支持设置连接的超时时间
- [x] 补全select
- [x] 支持UDP协议(添加测试)
- [x] 支持Http协议(彻底迁移 cppweb -> cppnet)
- [x] 支持 cicd 生成 lib 包
- [x] 添加 release pkg的demo 和cmakelist
- [x] httpclient 完成
- [x] httpserver test
- [x] 添加更多log
- [x] route 宽泛匹配
- [x] post 参数设置
- [x] 触发模式控制修改
- [x] 抽象出epoll层
- [x] 支持SSL
- [x] accept 改造
- [x] 完善多线程
- [x] 添加压力测试
- [x] 提升trie
- [ ] 添加常用中间件(限流,统计),完善日志
