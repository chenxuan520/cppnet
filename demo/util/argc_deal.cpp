#include "argc_deal.hpp"

namespace cppapp {

ArgcDeal::ArgcDeal(int argc, char **argv) : argv_(argv), argc_(argc) {
  for (int i = 1; i < argc; i++) {
    char *temp = argv[i];
    while (*temp == '-')
      temp++;
    std::string now = temp;
    if (now.find("=") == now.npos) {
      arg_all_.insert({temp, ""});
    } else {
      std::string key = now.substr(0, now.find("="));
      std::string value = now.substr(now.find("=") + 1);
      arg_all_.insert({key, value});
    }
  }
}

bool ArgcDeal::GetOption(std::string name) {
  return arg_all_.find(name) != arg_all_.end();
}

void ArgcDeal::SetOption(std::string option, std::string usage, bool must) {
  std::string temp = "\tno must";
  if (must)
    temp = "\tmust";
  message_.insert({option, usage + temp});
  if (must && arg_all_.find(option) == arg_all_.end()) {
    app.pfunc = NULL;
  }
}

int ArgcDeal::Run() {
  if (app.pfunc == NULL || arg_all_.find("h") != arg_all_.end() ||
      arg_all_.find("help") != arg_all_.end()) {
    return MsgPrint();
  } else {
    return app.pfunc(*this);
  }
}

int ArgcDeal::MsgPrint() {
  printf("app name:\n");
  printf("\t%s  %s\n", app.name.c_str(), app.version.c_str());
  printf("usage:\n");
  printf("\t%s\n", app.usage.c_str());
  printf("options:\n");
  for (auto iter : message_) {
    printf("\t%s\t\t%s\n", iter.first.c_str(), iter.second.c_str());
  }
  printf("\t-h,--help\t\tget help of app\tno must\n");
  return 0;
}

std::string ArgcDeal::GetVar(std::string key) {
  if (arg_all_.find(key) == arg_all_.end()) {
    return "";
  } else {
    return arg_all_[key];
  }
}

void ArgcDeal::SetVar(std::string variName, std::string usage, bool must) {
  std::string temp = "\tno must";
  if (must)
    temp = "\tmust";
  message_.insert({"--" + variName + "=(variable)", usage + temp});
  if (must && arg_all_.find(variName) == arg_all_.end()) {
    app.pfunc = NULL;
  }
}

} // namespace cppapp
