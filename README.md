# CppNet
- A simple and lightweight C++ network library.
- 一个简单轻量的C++网络库(cppweb 2.0 重构版)
## Author
- [chenxuan](https://github.com/chenxuan520)
## 相关文档以及详细demo
- [cppdocs](https://github.com/chenxuan520/cppdocs)
## Quick Start
1. 下载 release 库 https://github.com/chenxuan520/cppnet/releases
    - 需要根据自己的操作系统以及是否需要ssl下载对应包(需要ssl的话需要安装 openssl 的库)
2. 解压文件
3. 将 cppnet/include 加入到 头文件目录中 , cppnet/lib 加入到系统文件目录中
4. 编译时候加上动态链接 `-lcppnet`
## 本地编译
1. clone 仓库 `git clone --recurse-submodules https://github.com/chenxuan520/cppnet.git`
2. 编译
```bash
cd src
./build.sh
```
## TODO
- [ ] 补充完整 readme
- [x] 支持设置LT和ET模式下的事件触发方式
- [x] 支持设置连接的超时时间
- [ ] 补全select
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
- [ ] 添加压力测试
- [x] 抽象出epoll层
- [x] 支持SSL
- [x] accept 改造
- [x] 完善多线程
- [ ] 提升trie
