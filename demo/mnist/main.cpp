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

struct MnistReq {
  vector<vector<int>> data;
  int ParseFromJson(Json::Object &obj) {
    auto &data_obj = obj["data"];
    for (int i = 0; i < data_obj.arr.size(); i++) {
      data.push_back(vector<int>());
      for (int j = 0; j < data_obj[i].arr.size(); j++) {
        data[i].push_back(data_obj[i][j].int_val);
      }
    }
    return 0;
  }
};

struct MnistResp {
  int code;
  string msg;
  int result;
  void DumpToJson(Json::Node &node) {
    node["code"] = code;
    node["msg"] = msg;
    node["result"] = result;
  }
};

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
    const int arr_size = 28;
    auto raw_data = ctx.req().body();
    MnistReq req;
    MnistResp resp;
    auto rc = Json::Parse(raw_data, req);
    if (rc != 0) {
      resp.code = int(HttpStatusCode::BAD_REQUEST);
      resp.msg = string("parse json wrong ");
      ctx.resp().BadRequest(Json::Dump(resp));
      return;
    }
    if (req.data.size() != arr_size) {
      resp.code = int(HttpStatusCode::BAD_REQUEST);
      resp.msg = string("data size must be 28 ");
      ctx.resp().BadRequest(Json::Dump(resp));
      return;
    }
    for (auto i = 0; i < arr_size; i++) {
      if (req.data[i].size() != arr_size) {
        resp.code = int(HttpStatusCode::BAD_REQUEST);
        resp.msg = string("data size must be 28 ");
        ctx.resp().BadRequest(Json::Dump(resp));
        return;
      }
    }
    // print mnist_data in stdio
    for (auto i = 0; i < arr_size; i++) {
      for (auto j = 0; j < arr_size; j++) {
        cout << req.data[i][j];
      }
      cout << endl;
    }

    resp.result = GetMnistResult(req.data);
    resp.code = int(HttpStatusCode::OK);
    resp.msg = string("ok");
    ctx.resp().SuccessWithJson(Json::Dump(resp));
  });

  server.StaticFile("/", "./index.html");

  rc = server.Run();
  if (rc != 0) {
    cout << "run server error " << server.err_msg() << endl;
    return -1;
  }
  return 0;
}
