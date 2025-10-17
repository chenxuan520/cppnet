#include "http_server.hpp"
#include "../../utils/const.hpp"
#include "../../utils/file.hpp"
#include "filter/http_method_filter.hpp"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace cppnet {

void HttpContext::Run(int run_func_pos) {
  if (run_func_pos >= route_funcs_.size()) {
    return;
  }
  for (int i = run_func_pos; i < route_funcs_.size(); i++) {
    func_pos_ = i;
    route_funcs_[i](*this);
    if (!this->is_continue_) {
      break;
    }
  }
}

int HttpGroup::GET(const std::string &path, HttpCallback callback,
                   const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  auto new_filters = filters;
  auto method_filter = std::make_shared<HttpMethodFilter>();
  method_filter->Init(HttpMethod::GET);
  new_filters.push_back(method_filter);
  return RegisterHandler(path, callback, filters);
}

int HttpGroup::GET(const std::string &path,
                   std::initializer_list<HttpCallback> callbacks,
                   const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  for (auto callback : callbacks) {
    auto rc = GET(path, callback, filters);
    if (rc != kSuccess) {
      return rc;
    }
  }
  return kSuccess;
}

int HttpGroup::POST(const std::string &path, HttpCallback callback,
                    const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  auto new_filters = filters;
  auto method_filter = std::make_shared<HttpMethodFilter>();
  method_filter->Init(HttpMethod::POST);
  new_filters.push_back(method_filter);
  return RegisterHandler(path, callback, filters);
}

int HttpGroup::POST(const std::string &path,
                    std::initializer_list<HttpCallback> callbacks,
                    const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  for (auto callback : callbacks) {
    auto rc = POST(path, callback, filters);
    if (rc != kSuccess) {
      return rc;
    }
  }
  return kSuccess;
}

int HttpGroup::RegisterHandler(
    const std::string &path, HttpCallback callback,
    const std::vector<std::shared_ptr<HttpFilter>> &filters,
    bool is_exact_match) {
  auto pdata = trie_.Get<TrieDataType>(path);
  if (pdata == nullptr) {
    auto set_data = std::make_shared<TrieDataType>();
    set_data->push_back(HttpTrieData{callback, is_exact_match, filters});
    trie_.Set(route_ + path, set_data);
  } else {
    pdata->push_back(HttpTrieData{callback, is_exact_match, filters});
  }
  return kSuccess;
}

int HttpGroup::RegisterHandler(
    const std::string &path, std::initializer_list<HttpCallback> callbacks,
    const std::vector<std::shared_ptr<HttpFilter>> &filters,
    bool is_exact_match) {
  for (auto callback : callbacks) {
    auto rc = RegisterHandler(path, callback, filters, is_exact_match);
    if (rc != kSuccess) {
      return rc;
    }
  }
  return kSuccess;
}

HttpGroup HttpGroup::Group(const std::string &route) {
  return HttpGroup(route_ + route, trie_, logger_);
}

int HttpGroup::Use(HttpCallback callback,
                   const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  return RegisterHandler("", callback, filters, false);
}

int HttpGroup::StaticDir(
    const std::string &path, const std::string &dir_path,
    const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  auto method_filter = std::make_shared<HttpMethodFilter>();
  method_filter->Init(HttpMethod::GET);
  auto new_filters = filters;
  new_filters.push_back(method_filter);
  auto rc = RegisterHandler(
      path,
      [=](HttpContext &ctx) {
        auto route = ctx.req().route().GetPath();
        auto pos = route.find(path);
        if (pos == std::string::npos) {
          logger_->Warn("[logicerr]:static dir path error " +
                        ctx.req().route().GetPath() + " " + path);
          ctx.resp().NotFound();
          return;
        }
        auto aim_path = route.substr(pos + path.size());
        // avoid path like /static/../xxx
        if (aim_path.find("..") != std::string::npos) {
          logger_->Warn("[logicerr]:static dir path error " + aim_path);
          ctx.resp().NotFound();
          return;
        }
        auto file_path = dir_path + "/" + aim_path;
        auto exist = File::Exist(file_path);
        if (!exist) {
          logger_->Warn("[logicerr]:static dir not exist " + file_path);
          ctx.Next();
          return;
        }
        auto rc = File::Read(file_path, ctx.resp().body());
        if (rc != kSuccess) {
          logger_->Error(file_path + " read error");
          ctx.resp().NotFound();
          return;
        }
        logger_->Info("static_dir: return static dir success " + file_path);
        ctx.resp().header().SetLongConnection(true);
        ctx.resp().Success(
            HttpHeader::ConvertFileType(File::Suffix(file_path)));
      },
      new_filters, false);
  if (rc != kSuccess) {
    logger_->Error("[logicerr]:register static dir error");
    return rc;
  }
  return kSuccess;
}

int HttpGroup::StaticFile(
    const std::string &path, const std::string &file_path,
    const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  auto method_filter = std::make_shared<HttpMethodFilter>();
  method_filter->Init(HttpMethod::GET);
  auto new_filters = filters;
  new_filters.push_back(method_filter);
  auto rc = RegisterHandler(
      path,
      [=](HttpContext &ctx) {
        auto exist = File::Exist(file_path);
        if (!exist) {
          logger_->Warn("[logicerr]:static file not exist " + file_path);
          ctx.resp().NotFound();
          ctx.Next();
          return;
        }
        auto rc = File::Read(file_path, ctx.resp().body());
        if (rc != kSuccess) {
          logger_->Error("[logicerr]:" + file_path + " read error");
          ctx.resp().NotFound();
          return;
        }
        logger_->Info("static_file: return static file success " + file_path);
        ctx.resp().header().SetLongConnection(true);

        auto suffix = File::Suffix(file_path);
        auto type_str = HttpHeader::ConvertToStr(
            HttpHeader::ConvertFileType(suffix), suffix);
        ctx.resp().header().SetContentType(type_str);
        ctx.resp().Success();
      },
      new_filters);
  if (rc != kSuccess) {
    logger_->Error("[logicerr]:register static file error");
    return rc;
  }
  return kSuccess;
}

int HttpServer::Init(const Address &addr) {
  server_.set_addr(addr);
  is_continue_ = true;
  auto rc = server_.Init();
  if (rc != RC::kSuccess) {
    err_msg_ = server_.err_msg();
    return rc;
  }
  server_.Register(std::bind(&HttpServer::EventFunc, this,
                             std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3));
  return kSuccess;
}

int HttpServer::Run() {
  if (!is_continue_) {
    err_msg_ = "[logicerr]:server status stop,not init";
    return kLogicErr;
  }
#ifdef CPPNET_OPENSSL
  if (ssl_context_) {
    logger_->Info("https server run in https://" + server_.addr().ToString());
  } else {
    logger_->Info("http server run in http://" + server_.addr().ToString());
  }
#else
  logger_->Info("http server run in http://" + server_.addr().ToString());
#endif
  auto rc = server_.EventLoop();
  if (rc != RC::kSuccess) {
    err_msg_ = server_.err_msg();
    return rc;
  }
  return kSuccess;
}

void HttpServer::Stop() {
  if (!is_continue_) {
    return;
  }
  is_continue_ = false;
  server_.Stop();
  server_.WakeUp();
  server_.Clean();
}

void HttpServer::HandleError(TcpServer &server, Socket &event_soc) {
  logger_->Error(server.err_msg());
}

void HttpServer::HandleAccept(TcpServer &server, Socket &event_soc) {
  Address recv_addr;
  auto rc = event_soc.GetAddr(recv_addr);
  if (rc != RC::kSuccess) {
    logger_->Error("[event_soc.GetAddr]:" + event_soc.err_msg());
    server.RemoveSoc(event_soc);
    event_soc.Close();
    return;
  }
  logger_->Info("accept from " + recv_addr.ToString() +
                " soc: " + std::to_string(event_soc.fd()));

  if (read_timeout_.first != 0 || read_timeout_.second != 0) {
    event_soc.SetReadTimeout(read_timeout_.first, read_timeout_.second);
  }
  if (write_timeout_.first != 0 || write_timeout_.second != 0) {
    event_soc.SetWriteTimeout(write_timeout_.first, write_timeout_.second);
  }

#ifdef CPPNET_OPENSSL
  if (ssl_context_) {
    auto ssl_socket = ssl_context_->AcceptSSL(event_soc);
    if (ssl_socket == nullptr) {
      logger_->Error("[ssl_context.AcceptSSL]:" + ssl_context_->err_msg());
      server.RemoveSoc(event_soc);
      event_soc.Close();
      return;
    }
    ssl_sockets_map_[event_soc.fd()] = ssl_socket;
  }
#endif
}

void HttpServer::HandleLeave(TcpServer &server, Socket &event_soc) {
  Address recv_addr;
  event_soc.GetAddr(recv_addr);
  logger_->Info("leave from " + recv_addr.ToString() +
                " soc: " + std::to_string(event_soc.fd()));
#ifdef CPPNET_OPENSSL
  if (ssl_context_ &&
      ssl_sockets_map_.find(event_soc.fd()) != ssl_sockets_map_.end()) {
    ssl_sockets_map_[event_soc.fd()]->CloseSSL();
    ssl_sockets_map_.erase(event_soc.fd());
  }
#endif
}

void HttpServer::HandleRead(TcpServer &server, Socket &event_soc) {
  const std::string kCRLF = "\r\n";
  const std::string kEndl = "\n";
  std::shared_ptr<Socket> soc = nullptr;
#ifdef CPPNET_OPENSSL
  if (ssl_context_) {
    if (ssl_sockets_map_.find(event_soc.fd()) != ssl_sockets_map_.end()) {
      soc = ssl_sockets_map_[event_soc.fd()];
    } else {
      logger_->Error("[logicerr]:ssl socket not found");
      server.RemoveSoc(event_soc);
      event_soc.Close();
      return;
    }
  } else {
    soc = std::make_shared<Socket>(event_soc);
  }
#else
  soc = std::make_shared<Socket>(event_soc);
#endif

  // step1:recv and parse http request
  std::string buf;
  auto len = soc->ReadUntil(buf, kCRLF + kCRLF);
  if (len == 0) {
    logger_->Error("[soc.ReadUntil]: peer close before read complete");
    server.RemoveSoc(event_soc);
    soc->Close();
    return;
  }
  if (len < 0) {
    if (soc->err_no() == EAGAIN || soc->err_no() == EWOULDBLOCK) {
      logger_->Error("[soc.ReadUntil]: read timeout");
    } else {
      logger_->Error("[soc.ReadUntil]:" + soc->err_msg());
    }
    server.RemoveSoc(event_soc);
    soc->Close();
    return;
  }

  HttpReq req;
  HttpResp resp;
  std::string resp_buf;

  auto rc = req.Parse(buf);
  if (rc != kSuccess) {
    logger_->Error("[req.Parse]:" + req.err_msg());
    resp.BadRequest(req.err_msg());
    resp.Build(resp_buf);
    soc->Write(resp_buf);
    logger_->Info(
        "resp: " + HttpStatusCodeUtil::ConvertToStr(resp.status_code()) +
        " soc:" + std::to_string(event_soc.fd()) + kEndl);
    server.RemoveSoc(event_soc);
    soc->Close();
    return;
  }
  auto content_size = req.header().GetContentLength();
  std::string body;
  if (content_size > 0) {
    rc = soc->Read(body, content_size, true);
    if (rc == 0) {
      logger_->Error("[soc.Read]: peer close before read complete");
      server.RemoveSoc(event_soc);
      soc->Close();
      return;
    }
    if (rc < 0) {
      if (soc->err_no() == EAGAIN || soc->err_no() == EWOULDBLOCK) {
        logger_->Error("[soc.Read]: read body timeout");
      } else {
        logger_->Error("[soc.Read]: " + soc->err_msg());
      }
      server.RemoveSoc(event_soc);
      soc->Close();
      return;
    }
    req.body() = body;
  }
  auto path = req.route().GetPath();
  auto method = req.method();
  logger_->Debug("req: " + HttpMethodUtil::ConvertToStr(method) + " " + path +
                 " soc:" + std::to_string(event_soc.fd()));

  // step2:find route and run func
  HttpContext ctx(req, resp, soc);
  trie_.Search<TrieDataType>(
      path, [&](std::shared_ptr<TrieDataType> pdata, bool is_end) -> bool {
        auto &data = *pdata;
        for (auto &node : data) {
          auto route_func = node.callback;
          if (!is_end && node.is_exact_match) {
            continue;
          }
          for (auto filter : node.filters) {
            auto is_match = filter->IsMatchFilter(req);
            if (!is_match) {
              route_func = nullptr;
              break;
            }
          }
          if (route_func) {
            ctx.route_funcs_.push_back(route_func);
          }
        }
        return true;
      });
  ctx.Run(0);

  // step3:build resp and send
  if (resp.status_code() == HttpStatusCode::UNKNOWN) {
    resp.NotFound();
  }
  rc = resp.Build(resp_buf);
  if (rc != kSuccess) {
    logger_->Error("[resp.Build]:" + resp.err_msg());
    resp.InternalServerError(resp.err_msg());
    resp.Build(resp_buf);
  }
  rc = soc->Write(resp_buf);
  if (rc < 0) {
    logger_->Error("[soc.Write]" + soc->err_msg());
    server.RemoveSoc(event_soc);
    soc->Close();
    return;
  }
  logger_->Debug(
      "resp: " + HttpStatusCodeUtil::ConvertToStr(resp.status_code()) +
      " soc:" + std::to_string(event_soc.fd()) + kEndl);
}

void HttpServer::EventFunc(TcpServer::Event event, TcpServer &,
                           Socket event_soc) {
  switch (event) {
  case TcpServer::kEventRead: {
    HandleRead(server_, event_soc);
  } break;
  case TcpServer::kEventAccept: {
    HandleAccept(server_, event_soc);
  } break;
  case TcpServer::kEventLeave: {
    HandleLeave(server_, event_soc);
  } break;
  case TcpServer::kEventError:
    HandleError(server_, event_soc);
    break;
  }
}

void HttpServer::SetReadTimeout(unsigned timeout_sec, unsigned timeout_usec) {
  read_timeout_ = std::make_pair(timeout_sec, timeout_usec);
}

void HttpServer::SetWriteTimeout(unsigned timeout_sec, unsigned timeout_usec) {
  write_timeout_ = std::make_pair(timeout_sec, timeout_usec);
}

#ifdef CPPNET_OPENSSL
int HttpServer::InitSSL(const Address &addr,
                        std::shared_ptr<SSLContext> ssl_context) {
  if (server_.mode() == TcpServer::kMixed) {
    err_msg_ = "[logicerr]:mixed mode not support ssl";
    return kNotSupport;
  }
  if (ssl_context == nullptr) {
    err_msg_ = "[logicerr]:ssl context is nullptr";
    return kLogicErr;
  }
  auto rc = Init(addr);
  if (rc != kSuccess) {
    return rc;
  }
  ssl_context_ = ssl_context;
  return kSuccess;
}
#endif

} // namespace cppnet
