#include "http_server.hpp"
#include "../../utils/const.hpp"
#include "../../utils/file.hpp"
#include "filter/http_method_filter.hpp"
#include <functional>
#include <memory>
#include <vector>

namespace cppnet {

int HttpGroup::GET(const std::string &path, HttpCallback callback,
                   const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  auto new_filters = filters;
  auto method_filter = std::make_shared<HttpMethodFilter>();
  method_filter->Init(HttpMethod::GET);
  new_filters.push_back(method_filter);
  return RegisterHandler(path, callback, filters);
}

int HttpGroup::POST(const std::string &path, HttpCallback callback,
                    const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  auto new_filters = filters;
  auto method_filter = std::make_shared<HttpMethodFilter>();
  method_filter->Init(HttpMethod::POST);
  new_filters.push_back(method_filter);
  return RegisterHandler(path, callback, filters);
}

int HttpGroup::RegisterHandler(
    const std::string &path, HttpCallback callback,
    const std::vector<std::shared_ptr<HttpFilter>> &filters,
    bool is_exact_match) {
  auto pdata = trie_.Get(path);
  if (pdata == nullptr) {
    auto set_data = std::make_shared<TrieDataType>();
    set_data->push_back(HttpTrieData{callback, is_exact_match, filters});
    trie_.Set(route_ + path, set_data);
  } else {
    pdata->push_back(HttpTrieData{callback, is_exact_match, filters});
  }
  return kSuccess;
}

HttpGroup HttpGroup::Group(const std::string &route) {
  return HttpGroup(route_ + route, trie_);
}

int HttpGroup::Use(HttpCallback callback,
                   const std::vector<std::shared_ptr<HttpFilter>> &filters) {
  return RegisterHandler("", callback, filters, false);
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

int HttpServer::StaticDir(
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
          logger_->Warn("static dir path error " + ctx.req().route().GetPath() +
                        " " + path);
          ctx.resp().NotFound();
          return;
        }
        auto aim_path = route.substr(pos + path.size());
        auto file_path = dir_path + "/" + aim_path;
        auto exist = File::Exist(file_path);
        if (!exist) {
          logger_->Warn("static dir not exist " + file_path);
          ctx.resp().NotFound();
          return;
        }
        auto rc = File::Read(file_path, ctx.resp().body());
        if (rc != kSuccess) {
          logger_->Error(file_path + " read error");
          ctx.resp().NotFound();
          return;
        }
        ctx.resp().Success(HttpHeader::ContentType::kApplicationOctetStream);
      },
      new_filters, false);
  if (rc != kSuccess) {
    logger_->Error("register static dir error");
    return rc;
  }
  return kSuccess;
}

int HttpServer::StaticFile(
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
          logger_->Warn("static file not exist " + file_path);
          ctx.resp().NotFound();
          return;
        }
        auto rc = File::Read(file_path, ctx.resp().body());
        if (rc != kSuccess) {
          logger_->Error(file_path + " read error");
          ctx.resp().NotFound();
          return;
        }
        ctx.resp().Success(HttpHeader::ContentType::kApplicationOctetStream);
      },
      new_filters);
  if (rc != kSuccess) {
    logger_->Error("register static file error");
    return rc;
  }
  return kSuccess;
}

void HttpServer::EventFunc(TcpServer::Event event, TcpServer &, Socket soc) {
  const std::string kCRLF = "\r\n";
  switch (event) {
  case TcpServer::kEventRead: {

#ifdef CPPNET_OPENSSL

#endif
    // step1:recv and parse http request
    std::string buf;
    bool find_at_least_one = false;
    auto len = soc.ReadUntil(buf, kCRLF + kCRLF);
    if (len <= 0) {
      logger_->Error(soc.err_msg());
      err_callback_(soc.err_msg(), soc);
      soc.Close();
      return;
    }

    HttpReq req;
    HttpResp resp;

    auto rc = req.Parse(buf);
    if (rc != kSuccess) {
      logger_->Error(req.err_msg());
      err_callback_(req.err_msg(), soc);
      soc.Close();
      return;
    }
    auto content_size = req.header().GetContentLength();
    std::string body;
    if (content_size > 0) {
      soc.Read(body, content_size, true);
      req.body() = body;
    }
    auto path = req.route().GetPath();
    auto method = req.method();
    logger_->Info(HttpMethodUtil::ConvertToStr(method) + " " + path);

    // step2:find route and run func
    trie_.Search(path,
                 [&](std::shared_ptr<TrieDataType> pdata, bool is_end) -> bool {
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
                       find_at_least_one = true;
                       HttpContext ctx(req, resp, soc);
                       route_func(ctx);
                       return ctx.is_continue();
                     }
                   }
                   return true;
                 });

    // step3:build resp and send
    if (!find_at_least_one) {
      resp.NotFound();
    } else {
      resp.header().SetContentLength(resp.body().size());
      resp.header().SetHost("cppnet");
    }
    std::string resp_buf;
    rc = resp.Build(resp_buf);
    if (rc != kSuccess) {
      logger_->Error(resp.err_msg());
      err_callback_(resp.err_msg(), soc);
      soc.Close();
      return;
    }
    rc = soc.Write(resp_buf);
    if (rc < 0) {
      logger_->Error(soc.err_msg());
      err_callback_(soc.err_msg(), soc);
      soc.Close();
    }
  } break;

  case TcpServer::kEventAccept: {
    Address recv_addr;
    auto rc = soc.GetAddr(recv_addr);
    if (rc != RC::kSuccess) {
      logger_->Error(soc.err_msg());
      err_callback_(soc.err_msg(), soc);
      soc.Close();
      return;
    }
    logger_->Info("accept from " + recv_addr.ToString());
  } break;

  case TcpServer::kEventLeave: {
    Address recv_addr;
    soc.GetAddr(recv_addr);
    logger_->Info("leave from " + recv_addr.ToString());
  } break;

  case TcpServer::kEventError:
    logger_->Error(server_.err_msg());
    break;
  }
}

#ifdef CPPNET_OPENSSL
int HttpServer::InitSSL(const Address &addr,
                        std::shared_ptr<SSLContext> ssl_context) {
  if (ssl_context == nullptr) {
    err_msg_ = "ssl context is nullptr";
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
