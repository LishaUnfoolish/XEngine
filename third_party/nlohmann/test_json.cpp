#include <iostream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main() {
  // 创建一个JSON对象
  json j = {{"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {{"everything", 42}}},
            {"list", {1, 0, 2}},
            {"object", {{"currency", "USD"}, {"value", 42.99}}}};
  // 转换成字符串
  std::string j_str = j.dump();
  std::cout << j_str << std::endl;
  // 从字符串中解析
  json j2 = json::parse(j_str);
  std::cout << j2.dump() << std::endl;
  return 0;
}
