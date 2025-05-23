#include "argc_deal.hpp"
#include "config/config.hpp"
#include "http/server/filter/http_host_filter.hpp"
#include "http/server/http_server.hpp"
#include "log/file_logger.hpp"
#include "log/multi_logger.hpp"
#include "log/std_logger.hpp"
#include "process_ctrl.hpp"
#include "utils/file.hpp"
#include "utils/version.hpp"
#include <csignal>
#include <ctime>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>
using namespace std;
using namespace cppnet;
using namespace cppapp;

void RunWithConfig(Config &config) {
  auto pid = 0;
  if (config.is_background) {
    ProcessCtrl::BackGround();
    pid = getpid();
  }

  if (config.is_guard) {
    ProcessCtrl::Guard([](int exit_code, bool is_normal_exit) -> bool {
      if (!is_normal_exit) {
        cout << "child exit code:" << exit_code
             << " not normal exit:" << is_normal_exit << endl;
        return true;
      } else {
        cout << "child exit code:" << exit_code
             << " normal exit:" << is_normal_exit << endl;
        return false;
      }
    });
  }

  // get now time str
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  char time_str[100];
  strftime(time_str, 100, "%Y-%m-%d %H:%M:%S", ltm);
  cout << "Start Server " << time_str << endl;

  HttpServer server;
  Address addr{config.host, (uint16_t)config.port};

  if (config.ssl.cert_path != "") {
    shared_ptr<SSLContext> ctx = make_shared<SSLContext>();
    auto rc = ctx->InitSvrFile(config.ssl.cert_path, config.ssl.key_path,
                               config.ssl.password);
    if (rc != 0) {
      cout << "init ssl error " << ctx->err_msg() << endl;
      return;
    }

    rc = server.InitSSL(addr, ctx);
    if (rc != 0) {
      cout << "init ssl server error " << server.err_msg() << endl;
      return;
    }
  } else {
    auto rc = server.Init(addr);
    if (rc != 0) {
      cout << "init server error " << server.err_msg() << endl;
      return;
    }
  }

  for (auto &item : config.redirects) {
    cout << "redirect: " << item.route << " -> " << item.redirect << endl;
    vector<shared_ptr<HttpFilter>> filter_arr;
    if (item.host != "") {
      auto host_filter = std::make_shared<HttpHostFilter>();
      host_filter->InitWildCard(item.host);
      filter_arr.push_back(host_filter);
    }
    server.GET(
        item.route,
        [=](HttpContext &ctx) { ctx.resp().Redirect(item.redirect); },
        filter_arr);
  }

  for (auto &item : config.statics) {
    vector<shared_ptr<HttpFilter>> filter_arr;
    if (item.host != "") {
      auto host_filter = std::make_shared<HttpHostFilter>();
      host_filter->InitWildCard(item.host);
      filter_arr.push_back(host_filter);
    }

    if (File::IsDir(item.path)) {
      server.StaticDir(item.route, item.path, filter_arr);
      cout << "statics dir: " << item.route << " -> " << item.path << endl;
    } else if (File::IsFile(item.path)) {
      server.StaticFile(item.route, item.path, filter_arr);
      cout << "statics file: " << item.route << " -> " << item.path << endl;
    } else {
      cout << "static file not found: " << item.path << endl;
      return;
    }
  }

  if (config.log_path != "") {
    cout << "init logger: " << config.log_path << endl;
    std::shared_ptr<MultiLogger> logger = std::make_shared<MultiLogger>();
    auto std_logger = make_shared<StdLogger>();
    auto file_logger = make_shared<FileLogger>();
    file_logger->Init(config.log_path);
    logger->Init({std_logger, file_logger});

    server.set_logger(logger);
  }

  // add header server for sign cppnet server
  server.Use([](HttpContext &ctx) {
    ctx.resp().header().Add("Server", "cppnet/" + Version::GetStr());
  });

  //  write pid into file
  File::Write("./server.pid", to_string(pid));

  // set time out avoid some case
  server.SetReadTimeout(2, 0);
  server.SetWriteTimeout(5, 0);

  server.Run();
}

int _main(ArgcDeal &args) {
  if (args.GetOption("reload") || args.GetOption("stop")) {
    string pid_str = "";
    auto rc = File::Read("./server.pid", pid_str);
    if (rc != 0) {
      cout << "read pid file error" << endl;
    } else {
      int pid = 0;
      try {
        pid = stoi(pid_str);
      } catch (exception &e) {
        cout << "pid file error" << endl;
        exit(0);
      }
      cout << "dealing pid:" << pid << endl;
      if (pid <= 66) {
        cout << "pid too small, check if error";
        exit(0);
      }
      auto kill_result = ProcessCtrl::Kill(pid);
      if (kill_result != 0) {
        cout << "kill " << pid << " error" << endl;
        exit(-1);
      }
      cout << "kill " << pid << " stop ok" << endl;
      if (args.GetOption("stop")) {
        // delete file
        File::Remove("./server.pid");
        cout << "delete pid file" << endl;
        exit(0);

      } else {
        cout << "wait for the port unbound..." << endl;
        sleep(1);
      }
    }
  }

  std::string config_file = "./config.json";
  if (args.GetOption("config")) {
    config_file = args.GetVar("config");
    cout << "config file:" << config_file << endl;
  }

  string config_str;
  auto rc = File::Read(config_file, config_str);
  if (rc != 0) {
    cout << " read config file error " << config_file << endl;
    exit(0);
  }

  Config config;
  rc = Json::Parse(config_str, config);
  if (rc != 0) {
    cout << "parse config error" << endl;
    exit(-1);
  }

  RunWithConfig(config);
  return 0;
}

int main(int argc, char *argv[]) {
  ArgcDeal args(argc, argv);
  args.app.name = "chenxuanweb server";
  args.app.pfunc = _main;
  args.app.usage = {
      "thank using chenxuanweb,if you have any question",
      "send email to chenxuanweb@qq.com to deal problem,thank you!",
      "only in linux the argv is accepted"};
  args.app.version = Version::GetStr() + "  " + Version::GetDate();
  args.SetOption("reload", "restart the server");
  args.SetOption("stop", "stop the server");
  args.SetVar("config", "choose config file");
  return args.Run();
}
