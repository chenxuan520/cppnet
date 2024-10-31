#pragma once

#include "../../log/logger.hpp"
#include "../../server/tcp_server.hpp"
#include "../../utils/trie.hpp"
#include "../resp/http_resp.hpp"
#include "../server/filter/http_filter.hpp"
#include <memory>
#include <string>
#include <vector>

#ifdef CPPNET_OPENSSL
#include "../../ssl/ssl_context.hpp"
#include <unordered_map>
#endif

namespace cppnet {

class HttpContext {
public:
  HttpContext(HttpReq &req, HttpResp &resp, Socket &soc)
      : req_(req), resp_(resp), soc_(soc) {}

public:
  /*
   * @brief: whether continue, use in middware
   * */
  inline void Next() { is_continue_ = true; }
  inline void Abort() { is_continue_ = false; }
  inline bool is_continue() { return is_continue_; }
  /*
   * @brief: get inline data
   */
  HttpReq &req() { return req_; }
  HttpResp &resp() { return resp_; }
  Socket &soc() { return soc_; }

private:
  HttpReq &req_;
  HttpResp &resp_;
  Socket &soc_;
  bool is_continue_ = false;
};

using HttpCallback = std::function<void(HttpContext &)>;

struct HttpTrieData {
  HttpCallback callback = nullptr;
  bool is_exact_match = true;
  std::vector<std::shared_ptr<HttpFilter>> filters;
};
using TrieDataType = std::vector<HttpTrieData>;

class HttpGroup {
protected:
  HttpGroup(const std::string &route, Trie<TrieDataType> &trie,
            std::shared_ptr<Logger> logger)
      : route_(route), trie_(trie), logger_(logger) {};

public:
  /**
   * @brief GET request
   * @param path: request path
   * @param callback: request callback
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int GET(const std::string &path, HttpCallback callback,
          const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  int GET(const std::string &path,
          std::initializer_list<HttpCallback> callbacks,
          const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  /**
   * @brief POST request
   * @param path: request path
   * @param callback: request callback
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int POST(const std::string &path, HttpCallback callback,
           const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  int POST(const std::string &path,
           std::initializer_list<HttpCallback> callbacks,
           const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  /**
   * @brief all type request
   * @param path: request path
   * @param callback: request callback
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int RegisterHandler(
      const std::string &path, HttpCallback callback,
      const std::vector<std::shared_ptr<HttpFilter>> &filters = {},
      bool is_exact_match = true);
  int RegisterHandler(
      const std::string &path, std::initializer_list<HttpCallback> callbacks,
      const std::vector<std::shared_ptr<HttpFilter>> &filters = {},
      bool is_exact_match = true);
  /**
   * @brief: middware
   * @param callback: request callback
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int Use(HttpCallback callback,
          const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  /**
   * @brief: add static dir
   * @param path: request path
   * @param dir_path: dir path
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int StaticDir(const std::string &path, const std::string &dir_path,
                const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  /**
   * @brief: add static file
   * @param path: request path
   * @param dir_path: file path
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int StaticFile(const std::string &path, const std::string &file_path,
                 const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  /**
   * @brief: group
   * @param route: group route
   * @return HttpGroup
   */
  HttpGroup Group(const std::string &route);
  /**
   * @brief: get error message
   * @return error message
   */
  std::string err_msg() { return err_msg_; }
  /**
   * @brief: set logger
   * @param logger: logger
   */
  void set_logger(std::shared_ptr<Logger> logger) {
    if (logger != nullptr) {
      logger_ = logger;
    }
  }

protected:
  std::string route_;
  Trie<TrieDataType> &trie_;
  std::string err_msg_;
  std::shared_ptr<Logger> logger_ = std::make_shared<Logger>();
};

class HttpServer : public HttpGroup {
public:
  HttpServer() : HttpGroup("", trie_, std::make_shared<Logger>()) {};
  ~HttpServer() { Stop(); }
  // forbiden copy
  HttpServer(const HttpServer &) = delete;
  HttpServer &operator=(const HttpServer &) = delete;
  /**
   * @brief: init http server
   * @param addr: server address
   * @return 0 if success, -1 if failed
   */
  int Init(const Address &addr);
  /**
   * @brief: run http server
   * @return 0 if success, -1 if failed
   */
  int Run();
  /**
   * @brief: stop http server
   */
  void Stop();

#ifdef CPPNET_OPENSSL
public:
  // https
  /**
   * @brief: init https server
   * @param addr: server address
   * @param ssl_context: ssl context
   * @return 0 if success, -1 if failed
   */
  int InitSSL(const Address &addr, std::shared_ptr<SSLContext> ssl_context);

private:
  std::shared_ptr<SSLContext> ssl_context_ = nullptr;
  std::unordered_map<int, std::shared_ptr<SSLSocket>> ssl_sockets_map_;
#endif

private:
  // event callback
  void EventFunc(TcpServer::Event, TcpServer &, Socket);
  void HandleRead(TcpServer &, Socket &soc);
  void HandleAccept(TcpServer &, Socket &soc);
  void HandleLeave(TcpServer &, Socket &soc);
  void HandleError(TcpServer &, Socket &soc);

private:
  TcpServer server_;
  Trie<TrieDataType> trie_;
  bool is_continue_ = false;
};

} // namespace cppnet
