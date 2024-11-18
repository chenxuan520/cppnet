#include "http/server/http_server.hpp"
#include "json.h"
#include "log/std_logger.hpp"
#include "neural_network_loader.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;
using namespace cppnet;
using namespace cppjson;
using namespace deeplearning;

int GetMnistResult(const vector<vector<int>> &mnist_data) {
  NeuralNetwork network;
  NeuralNetwork::NetworkParam param;
  NeuralNetwork::NetworkOption option;
  const string file_name = "./demo.param";
  auto load_rc =
      NeuralNetworkLoader::ImportParamFromFile(param, option, file_name);
  if (load_rc != NeuralNetworkLoader::SUCCESS) {
    cout << "import param from file error " << load_rc << endl;
    return -1;
  }

  auto rc = network.ImportNetworkParam(param, option);
  if (rc != NeuralNetwork::SUCCESS) {
    cout << "import network param error " << rc << network.err_msg() << endl;
    return -1;
  }

  vector<double> input_data, result_data(10, 0.0);
  for (auto i = 0; i < 28; i++) {
    for (auto j = 0; j < 28; j++) {
      input_data.push_back(mnist_data[i][j]);
    }
  }
  rc = network.Predict(input_data, result_data);
  if (rc != NeuralNetwork::SUCCESS) {
    cout << "predict error " << rc << network.err_msg() << endl;
    return -1;
  }

  int result = 0;
  double max_value = result_data[0];
  for (auto i = 0; i < 10; i++) {
    if (result_data[i] > max_value) {
      max_value = result_data[i];
      result = i;
    }
  }
  return result;
}

int main() {
  HttpServer server;
  auto rc = server.Init({"127.0.0.1", 8080});
  if (rc != 0) {
    cout << "init server error " << server.err_msg() << endl;
    return -1;
  }
  server.set_logger(std::make_shared<StdLogger>());

  server.POST("/mnist", [](HttpContext &ctx) {
    auto raw_data = ctx.req().body();
    Json req_json, resp_json;
    auto is_success = req_json.Parse(raw_data.c_str());
    if (!is_success) {
      resp_json["code"] = 400;
      resp_json["msg"] = string("parse json wrong ") + req_json.err_msg();
      ctx.resp().BadRequest(resp_json());
      return;
    }
    vector<vector<int>> mnist_data(28);
    auto obj = req_json.GetRootObj();
    if (obj["data"].arr.size() != 28) {
      resp_json["code"] = 400;
      resp_json["msg"] =
          "data size must be 28 " + to_string(obj["data"].arr.size());
      ctx.resp().BadRequest(resp_json());
    }
    for (auto i = 0; i < 28; i++) {
      if (obj["data"].arr[i]->arr.size() != 28) {
        resp_json["code"] = 400;
        resp_json["msg"] =
            "data size must be 28 " + to_string(obj["data"].arr[i]->arr.size());
        ctx.resp().BadRequest(resp_json());
        return;
      }
      for (auto j = 0; j < 28; j++) {
        mnist_data[i].push_back(obj["data"].arr[i]->arr[j]->int_val);
      }
    }
    // print mnist_data in stdio
    for (auto i = 0; i < 28; i++) {
      for (auto j = 0; j < 28; j++) {
        cout << mnist_data[i][j];
      }
      cout << endl;
    }

    resp_json["code"] = 200;
    resp_json["msg"] = "ok";
    resp_json["result"] = GetMnistResult(mnist_data);
    ctx.resp().SuccessWithJson(resp_json());
  });

  server.StaticFile("/", "./index.html");

  rc = server.Run();
  if (rc != 0) {
    cout << "run server error " << server.err_msg() << endl;
    return -1;
  }
  return 0;
}
