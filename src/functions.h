#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>
#include <string>
#include <vector>
#include <cstdint>
#include <bit>
#include <algorithm>

inline bool littleEndian;

#define IS_BIG_ENDIAN (std::endian::native == std::endian::big)

void sendError (std::string message);

void EndProgram();

template <typename T>
T toBigEndian(T value) {
    if (!IS_BIG_ENDIAN) return _byteswap_ulong(value); else return value;
}
template <typename T>
T toLittleEndian(T value) {
    if (IS_BIG_ENDIAN) return _byteswap_ulong(value); else return value;
}

void LittleEndian();
void BigEndian();

template <typename T>
T parse(std::istream& file, T param) {
    T buffer;
    file.read((char*)&buffer, sizeof(buffer));
    if (typeid(buffer).name() == typeid(int).name()) {
        if (littleEndian == false) {
            buffer = toBigEndian(buffer);
        } else {
            buffer = toLittleEndian(buffer);
        }
    }
    return buffer;
}

std::string parseStr(std::istream& file);

std::string swapBytes(std::string input);

void XFBIN(std::istream& input, std::ostream& output);