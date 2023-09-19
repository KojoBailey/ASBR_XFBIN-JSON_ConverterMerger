#include "functions.h"

void sendError (std::string message) {
    std::cerr << "\033[31m" << "> ERROR: " << message << "\033[0m";
}

void EndProgram() {
    std::cout << "\n\n";
    system("pause");
}

void LittleEndian() {
    littleEndian = true;
}
void BigEndian() {
    littleEndian = false;
}

std::string swapBytes(std::string input) {
    int size = input.size() / 2;
    std::string output;
    for (int i = size; i > 0; i--) {
        output += input.at(i * 2 - 2);
        output += input.at(i * 2 - 1);
    }
    return output;
}

std::string parseStr(std::istream& file) {
    char buffer;
    std::string output;
    while (file.peek() != 0) {
        file.read((char*)&buffer, 1);
        output += buffer;
    }
    return output;
}

void XFBIN(std::istream& input, std::ostream& output) {
    char xfbinByte;
    input.seekg(0, std::ios::end);
    int size = input.tellg();
    input.seekg(0, std::ios::beg);
    for (int i = 0; i < size; i++) {
        input.read((&xfbinByte), 1);
        output.put(xfbinByte);
    }
}