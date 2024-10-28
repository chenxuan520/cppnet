#pragma once

#include "../../log/logger.hpp"
#include "../../server/tcp_server.hpp"
#include "../../utils/trie.hpp"
#include "../resp/http_resp.hpp"
#include "../server/filter/http_filter.hpp"
#include <memory>
#include <string>
#include <vector>

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
using HttpErrCallback =
    std::function<void(const std::string &err_msg, Socket soc)>;
using TrieDataSingleType =
    std::pair<HttpCallback, std::vector<std::shared_ptr<HttpFilter>>>;
using TrieDataType = std::vector<TrieDataSingleType>;

class HttpGroup {
protected:
  HttpGroup(const std::string &route, Trie<TrieDataType> &trie)
      : route_(route), trie_(trie) {};

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
  /**
   * @brief POST request
   * @param path: request path
   * @param callback: request callback
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int POST(const std::string &path, HttpCallback callback,
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
      const std::vector<std::shared_ptr<HttpFilter>> &filters = {});
  /**
   * @brief: middware
   * @param callback: request callback
   * @param filters: request filters
   * @return 0 if success, -1 if failed
   */
  int Use(HttpCallback callback,
          const std::vector<std::shared_ptr<HttpFilter>> &filters);
  /**
   * @brief: group
   * @param route: group route
   * @return HttpGroup
   */
  HttpGroup Group(const std::string &route);

protected:
  std::string route_;
  Trie<TrieDataType> &trie_;
};

class HttpServer : public HttpGroup {
public:
  HttpServer() : HttpGroup("", trie_) {};
  ~HttpServer() { Stop(); }
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
   * @brief: register error callback
   * @param callback: error callback
   */
  void RegisterErrorCallback(HttpErrCallback callback) {
    err_callback_ = callback;
  }
  /**
   * @brief: get error message
   * @return error message
   */
  std::string err_msg() { return err_msg_; }
  /**
   * @brief: set logger
   * @param logger: logger
   */
  void set_logger(std::shared_ptr<Logger> logger) { logger_ = logger; }

private:
  // event callback
  void EventFunc(TcpServer::Event, TcpServer &, Socket);

private:
  TcpServer server_;
  Trie<TrieDataType> trie_;
  std::string err_msg_;
  HttpErrCallback err_callback_ = [](const std::string &, Socket) {};
  bool is_continue_ = false;
  std::shared_ptr<Logger> logger_ = std::make_shared<Logger>();
};

} // namespace cppnet
