#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace cppapp {

class ArgcDeal {
public:
  struct AppMsg {
    std::string name;
    std::string version;
    std::string usage;
    std::function<int(ArgcDeal &)> pfunc;
  };

public:
  AppMsg app;

public:
  ArgcDeal(int argc, char **argv);
  std::string GetVar(std::string key);
  void SetVar(std::string variName, std::string usage, bool must = false);
  bool GetOption(std::string name);
  void SetOption(std::string option, std::string usage, bool must = false);
  int Run();
  int MsgPrint();

private:
  char **argv_ = nullptr;
  int argc_ = 0;
  std::unordered_map<std::string, std::string> arg_all_;
  std::unordered_map<std::string, std::string> message_;
};

} // namespace cppapp
