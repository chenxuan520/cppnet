#pragma once
#include "json.h"
#include <string>
#include <vector>
using namespace cppjson;

struct RedirectConfig {
  std::string route;
  std::string redirect;
  void Parse(Json::Object &json) {
    route = json["route"].str_val;
    redirect = json["redirect"].str_val;
  }
};

struct StaticConfig {
  std::string route;
  std::string path;
  void Parse(Json::Object &json) {
    route = json["route"].str_val;
    path = json["path"].str_val;
  }
};

struct SSLConfig {
  std::string cert_path;
  std::string key_path;
  std::string password;
  void Parse(Json::Object &json) {
    cert_path = json["cert_path"].str_val;
    key_path = json["key_path"].str_val;
    password = json["password"].str_val;
  }
};

struct Config {
  int port = 0;
  std::string host = "";
  bool is_background = false;
  bool is_guard = false;
  std::string log_path = "";
  std::vector<RedirectConfig> redirects;
  std::vector<StaticConfig> statics;
  SSLConfig ssl;
  void Parse(Json::Object &json) {
    port = json["port"].int_val;
    host = json["host"].str_val;
    is_background = json["is_background"].bool_val;
    is_guard = json["is_guard"].bool_val;
    log_path = json["log_path"].str_val;
    for (int i = 0; i < json["redirects"].arr.size(); i++) {
      RedirectConfig redirect;
      redirect.Parse(json["redirects"][i]);
      redirects.push_back(redirect);
    }
    for (int i = 0; i < json["statics"].arr.size(); i++) {
      StaticConfig sta;
      sta.Parse(json["statics"][i]);
      statics.push_back(sta);
    }
    if (json["ssl"] != Json::npos()) {
      ssl.Parse(json["ssl"]);
    }
  }
};
