#include <iostream>
#include <fstream>
#include <format>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::ordered_json;

void XFBIN(std::istream& input, std::ostream& output);

void MainModeParam(std::istream& input, std::ostream& output);
json UNPACK_messageInfo(std::istream& input);
int REPACK_messageInfo(std::istream& input);