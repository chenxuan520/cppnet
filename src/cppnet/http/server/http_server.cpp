#include "http_server.hpp"
#include "../../utils/const.hpp"
#include <functional>
#include <memory>

namespace cppnet {

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
  is_continue_ = false;
  server_.Stop();
  server_.Clean();
}

void HttpServer::EventFunc(TcpServer::Event event, TcpServer &, Socket soc) {
  const std::string kCRLF = "\r\n\r\n";
  switch (event) {
  case TcpServer::kEventRead: {
    // step1:recv and parse http request
    std::string buf;
    bool find_at_least_one = false;
    soc.ReadUntil(buf, kCRLF + kCRLF);
    HttpReq req;
    HttpResp resp;

    auto rc = req.Parse(buf);
    if (rc != kSuccess) {
      if (err_callback_) {
        logger_->Error(err_msg_);
        err_callback_(req.err_msg(), soc);
        soc.Close();
      }
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
    trie_.Search(path, [&](std::shared_ptr<TrieDataType> pdata, bool) -> bool {
      auto &data = *pdata;
      for (auto &node : data) {
        auto route_func = node.first;
        for (auto filter : node.second) {
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
      // set 404 return
      // TODO: finish it //
      resp.body() = "404 no found";
      resp.status_code() = HttpStatusCode::NOT_FOUND;
      resp.header().SetHost("cppnet");
      resp.header().SetContentLength(resp.body().size());
      resp.header().SetContentType(HttpHeader::ContentType::kTextPlain);
    } else {
      resp.header().SetContentLength(resp.body().size());
    }
    std::string resp_buf;
    rc = resp.Build(resp_buf);
    if (rc != kSuccess) {
      if (err_callback_) {
        logger_->Error(err_msg_);
        err_callback_(resp.err_msg(), soc);
        soc.Close();
      }
      return;
    }
    soc.Write(resp_buf);
  } break;

  case TcpServer::kEventAccept:
    break;
  case TcpServer::kEventLeave:
    break;
  case TcpServer::kEventError:
    break;
  }
}

} // namespace cppnet
