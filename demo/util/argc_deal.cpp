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
  std::string temp = ",no must";
  if (must)
    temp = ",must";
  message_.insert({option, usage + temp});
  if (must && arg_all_.find(option) == arg_all_.end()) {
    app.pfunc = NULL;
  }
}

int ArgcDeal::Run() {
  if (app.pfunc == NULL || arg_all_.find("h") != arg_all_.end() ||
      arg_all_.find("help") != arg_all_.end()) {
    return MsgPrint();
  } else if (arg_all_.find("v") != arg_all_.end() ||
             arg_all_.find("version") != arg_all_.end()) {
    printf("app version:\t%s\n", app.version.c_str());
    return 0;
  } else {
    return app.pfunc(*this);
  }
}

int ArgcDeal::MsgPrint() {
  printf("app name:\n");
  printf("\t%s  %s\n", app.name.c_str(), app.version.c_str());
  printf("usage:\n");
  for (auto &iter : app.usage) {
    printf("\t%s\n", iter.c_str());
  }
  printf("options:\n");
  for (auto iter : message_) {
    printf("\t--%-32s %-32s\n", iter.first.c_str(), iter.second.c_str());
  }
  printf("\t--%-32s %-32s\n", "help,-h", "get help of app,no must");
  printf("\t--%-32s %-32s\n", "version,-v", "get version of app,no must");
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
  std::string temp = ",no must";
  if (must)
    temp = ",must";
  message_.insert({variName + "=(variable)", usage + temp});
  if (must && arg_all_.find(variName) == arg_all_.end()) {
    app.pfunc = NULL;
  }
}

} // namespace cppapp
