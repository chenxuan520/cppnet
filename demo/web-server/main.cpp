#include "argc_deal.hpp"
#include "config/config.hpp"
#include "http/server/http_server.hpp"
#include "log/file_logger.hpp"
#include "log/multi_logger.hpp"
#include "log/std_logger.hpp"
#include "process_ctrl.hpp"
#include "utils/file.hpp"
#include <csignal>
#include <ctime>
#include <iostream>
#include <memory>
#include <unistd.h>
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
    ProcessCtrl::Guard();
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
    server.GET(item.route,
               [=](HttpContext &ctx) { ctx.resp().Redirect(item.redirect); });
  }
  for (auto &item : config.statics) {
    if (File::IsDir(item.path)) {
      server.StaticDir(item.route, item.path);
      cout << "statics dir: " << item.route << " -> " << item.path << endl;
    } else if (File::IsFile(item.path)) {
      server.StaticFile(item.route, item.path);
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
      ProcessCtrl::Kill(pid);
      if (args.GetOption("stop")) {
        cout << "kill " << pid << " stop ok" << endl;
        exit(0);
      } else {
        printf("wait for the port unbound...\n");
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

  Json json;
  auto re = json.Parse(config_str.c_str());
  if (!re) {
    cout << "parse json err " << json.err_msg() << endl;
  }
  if (json.err_msg() != nullptr) {
    cout << "parse json err " << json.err_msg() << endl;
    exit(0);
  }

  Config config;
  config.Parse(json.GetRootObj());

  RunWithConfig(config);
  return 0;
}

int main(int argc, char *argv[]) {
  ArgcDeal args(argc, argv);
  args.app.name = "chenxuanweb server";
  args.app.pfunc = _main;
  args.app.usage =
      "thank using chenxuanweb,if you have any question\n"
      "\tsend email to 1607772321@qq.com to deal problem,thank you!\n"
      "\t! only in linux the argv is accepted\n";
  args.app.version = "v2.0.1";
  args.SetOption("reload", "restart the server");
  args.SetOption("stop", "stop the server");
  args.SetVar("config", "choose the file to be config file");
  return args.Run();
}
