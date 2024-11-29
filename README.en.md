# cppnet
- A lightweight C++ network framework, an upgraded version of cppweb 2.0
- Avoids the bloatiness of the previous cppweb single header file, and uses a more friendly way of using
![](https://chenxuanweb.top/cppnet/cppnet.png)
## Author
- **chenxuan**
> ![](http://cdn.androidftp.top/pic/chenxuanweb/dog.png)
## Repository Address
- [github](https://github.com/chenxuan520/cppnet)
- [gitee](https://gitee.com/chenxuan520/cppnet)
## Official Website
- [chenxuanweb.top/cppnet/index.html](https://chenxuanweb.top/cppnet/index.html)
## Docs
- [cppnet online docs](https://chenxuan520.github.io/cppnet/)
## Advantages
1. Easy to use, low invasiveness, **full platform support**, does not require forcible installation into the system's include directory, recommended to be directly referenced as a submodule or used directly with static libraries
2. Built with modern C++, similar in usage and functions to Go's gin framework, small learning and entry cost, suitable for beginners to learn source code
3. Lightweight framework, very small, with less than 5000 lines of source code, avoiding the bloat of large network frameworks
## Quick Start
### Using the Release Package
1. Download the release library ([Releases · chenxuan520/cppnet](https://github.com/chenxuan520/cppnet/releases)), and extract it (you can choose any location, you can put it in the system's include directory or not. In the following, it is assumed that the extraction is to the current directory). A C++17-compatible compiler is required.
    - If you need SSL (you need to install OpenSSL), download the SSL version. The default option is fine.
2. Write code. You can write code according to your needs. The following are two simple demos, one without SSL and the other with SSL. For specific function descriptions, you can refer to the documentation.
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
3. Compiling the build file
    1. If you are using CMake to compile, you need to add the following content to the CMakeLists.txt file. In fact, you just add the link library
    ```cmake
    # Whether to use SSL or not, you need to add this
    link_directories(./cppnet/lib)
    link_libraries(-lcppnet)
    
    # **If you are using the SSL version and need to use the SSL function, you need to add the following**
    add_definitions(-DCPPNET_OPENSSL)
    link_libraries(-lssl -lcrypto)
    ```
    2. If you are using Makefile to compile, you need to add `-L./cppnet/lib` and `-lcppnet` to the compilation options. Here is a Makefile compilation demo. Of course, if it is an SSL compilation, you also need to add the lib library and **add the PPNET_OPENSSL macro**.
    ```Makefile
    all: libserver-makefile
    
    libserver-makefile:
        g++ -O2 -Wall -std=c++17./main.cpp -o libserver-makefile -lcppnet -L./cppnet/lib
        # SSL version
        g++ -O2 -Wall -std=c++17./main.cpp -o libserver-makefile -DCPPNET_OPENSSL -lcppnet -lssl -lcrypto -L./cppnet/lib
    ```
4. Execute the compilation to obtain the compiled file successfully.
### Compiling from source
1. Use `git clone https://github.com/chenxuan520/cppnet --recurse-submodules` to pull the source code
    1. By default, the ssh version is compiled. If you need an ssl-free version, you need to modify the cmake file manually
2. Run `cd src;./build.sh` to generate the static library in lib
    - Requires cmake
    - Requires a compiler that supports C++17
## Performance Testing
- Automated testing is performed using ubuntu-latest from github action, with machine configuration [refer to](https://docs.github.com/zh/actions/writing-workflows/choosing-where-your-workflow-runs/choosing-the-runner-for-a-job#%E7%94%A8%E4%BA%8E%E5%85%AC%E5%85%B1%E5%AD%98%E5%82%A8%E5%BA%93%E7%9A%84-github-%E6%89%98%E7%AE%A1%E7%9A%84%E6%A0%87%E5%87%86%E8%BF%90%E8%A1%8C%E5%99%A8)
    - 4 cores, 16GB of memory, and 14GB of hard disk
- Automatically run updated test results based on each commit's code, referring to the scripts in the bench folder, and the hash value will also be in the picture.
- Use [vegeta](https://github.com/tsenart/vegeta) for stress testing, testing at different QPS respectively
    - Average response time
    - p99 response time
    - Accuracy
    - Wait time
- Compare cppnet framework and go gin framework, and implement the same function with both

## More Demo
- More demo references can be found in the "demo" folder and the "test" folder.
### Create a TcpServer
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
### Connect to the server using socket
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
### Create HttpServer
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
### Create HttpsServer ssl
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
### Create HttpClient
```cpp
  HttpClient client;
  Address addr;
  addr.InitWithDomain("www.androidftp.top", 80);
  auto rc = client.Init(addr);
  // If it is the https 443 port ssl
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
### Timer trigger server
- To use AddSoc to listen for the timesocket and wait for a trigger.
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
## Source code structure
- The repository structure is as follows
    - cppnet stores all source code and build files
    - lib stores the generated static library and the CMakeLists file for static library construction
    - test is the test case
    - third_party is a third-party repository, currently only depends on cpptest for unit testing in the test folder
    - build.sh generates the lib static library and the unit test binary file
    - docs is the interface documentation
    - demo is the reference example
    - bench is the benchmark
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
- The source code is mainly placed in `src/cppnet`.
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
### Class Diagram
![](http://cdn.androidftp.top/test/2024111122224337cppnet.drawio.png)
