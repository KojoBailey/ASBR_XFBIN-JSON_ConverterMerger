#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::ordered_json;

void XFBIN(std::istream& input, std::ostream& output);

json UNPACK_MainModeParam(std::istream& input);
int REPACK_MainModeParam(std::istream& input);

json UNPACK_messageInfo(std::istream& input);
int REPACK_messageInfo(std::istream& input);