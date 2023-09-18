#include "functions.h"
#include "template.h"

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

void MainModeParam(std::istream& input, std::ostream& output) {
    BigEndian();
    uint32_t Size = parse(input, Size);
    LittleEndian();
    uint32_t Version = parse(input, Version);
    uint32_t Count = parse(input, Count);
    output << "Size: " << Size << std::endl;
    output << "Version: " << Version << std::endl;
    output << "Count: " << Count << std::endl;

    uint64_t unk = parse(input, unk);
    output << "unk: " << unk << std::endl;

    int Pos;

    struct {
        struct {
            struct {
                uint64_t Part;

                uint64_t Pointer;
                //continue
            } BasicInfo;
            struct {

            } Player1Info;
            struct {

            } Player2Info;
            struct {

            } SpecialRules;
            struct {

            } Missions;
            struct {

            } UNKNOWNS;
        } Panel;
    } Page;
    
    Page.Panel.BasicInfo.Part = parse(input, Page.Panel.BasicInfo.Part);
    output << "~ Page ~" << std::endl;
    output << "\t~ Panel ~" << std::endl;
    output << "\t\t~ BasicInfo ~" << std::endl;
    output << "\t\t\tPart: " << Page.Panel.BasicInfo.Part << std::endl;
    for (int page = 0; page < Count/8; page++) {

    }
}

json UNPACK_messageInfo(std::istream& input) {
    // move past XFBIN fluff
    input.seekg(284);

    json JsonData; // Where the JSON data will be stored
    JsonData["Filetype"] = "messageInfo"; // So the parser can know what filetype it's dealing with automatically

    BigEndian();
    uint32_t Size = toBigEndian(parse(input, Size)); // Hidden: modified automatically
    LittleEndian();
    uint32_t Version = parse(input, Version);
    uint32_t Count = parse(input, Count); // Hidden: modified automatically
    uint64_t FirstPointer = parse(input, FirstPointer); // Hidden: unimportant

    JsonData["Version"] = Version;

    struct entries {
        std::string crc32_id;

        uint32_t unk1; // Hidden: unimportant
        uint32_t unk2; // Hidden: unimportant
        uint32_t unk3; // Hidden: unimportant

        std::string message;

        std::string ref_crc32;
        int16_t is_ref;

        int16_t char_id;
        int16_t cue_id;

        int16_t unk6; // Hidden: unimportant
        uint32_t unk7; // Hidden: unimportant

    };
    std::vector<entries> entry;

    int Pos;
    std::string hex_output;
    uint32_t buffer;

    for (int i = 0; i < Count; i++) {
        // { crc32_id, unk, unk, unk, message, ref_crc32, is_ref, char_id, cue_id, unk, unk }
        entry.push_back({"", 0, 0, 0, "", "", 0, 0, 0, 0, 0});

        hex_output = std::format("{:x}", parse(input, buffer)); // fsr this is extremely flawed!!! fml
        while (hex_output.size() < 8) {
            hex_output = "0" + hex_output;
        }
        entry[i].crc32_id = swapBytes(hex_output);

        entry[i].unk1 = parse(input, entry[i].unk1);
        entry[i].unk2 = parse(input, entry[i].unk2);
        entry[i].unk3 = parse(input, entry[i].unk3);
        
        uint64_t Pointer = parse(input, Pointer);
        Pos = input.tellg();
        input.seekg(Pos - 8 + Pointer); // Jumps forward by Pointer bytes
        entry[i].message = parseStr(input);
        input.seekg(Pos); // Go back to before jump

        hex_output = std::format("{:x}", parse(input, buffer));
        if (hex_output.size() == 7) {
            entry[i].ref_crc32 = swapBytes("0" + hex_output); // Prevents goofy missing 0 shenanigans
        } else {
            entry[i].ref_crc32 = swapBytes(hex_output);
        }
        entry[i].is_ref = parse(input, entry[i].is_ref);
        
        entry[i].char_id = parse(input, entry[i].char_id);
        entry[i].cue_id = parse(input, entry[i].cue_id);

        entry[i].unk6 = parse(input, entry[i].unk6);
        entry[i].unk7 = parse(input, entry[i].unk7);

        JsonData[entry[i].crc32_id] = {
            {"message", entry[i].message},
            {"is_ref", entry[i].is_ref},
            {"ref_crc32", entry[i].ref_crc32},
            {"char_id", entry[i].char_id},
            {"cue_id", entry[i].cue_id}
        }; // JSON output
    }
    return JsonData;
}
int REPACK_messageInfo (std::istream& input) {
    json json = json::parse(input);

    struct entries {
        std::string crc32_id;
        std::string message;
        std::string ref_crc32;
        int16_t is_ref;
        int16_t char_id;
        int16_t cue_id;
    };
    std::vector<entries> entry;

    std::ofstream binary("messageInfo.bin.xfbin", std::ios::binary);

    // get and write XFBIN structure data
    std::ifstream xfbinData("data/messageInfo.xfbin.dat", std::ios::binary);
    XFBIN(xfbinData, binary);
    xfbinData.close();

    int i = 0;
    std::string key;
    for (auto& x : json.items()) {
        key = x.key();
        auto& entry_obj = json[key];

        if (key != "Filetype" && key != "Version") {
            entry.push_back({"", "", "", 0, 0, 0});

            if (key.size() != 8) {
                sendError("Detected a CRC32 ID without 8 characters.\n");
                system("pause");
                return 0;
            }
            entry[i].crc32_id = key;
            entry[i].message = entry_obj["message"];
            entry[i].ref_crc32 = entry_obj["ref_crc32"];
            entry[i].is_ref = entry_obj["is_ref"];
            entry[i].char_id = entry_obj["char_id"];
            entry[i].cue_id = entry_obj["cue_id"];
            
            i++;
        }
    }
    uint16_t b16; // buffer16
    uint32_t b32; // buffer32
    uint64_t b64; // buffer64
    uint64_t bp; // buffer pointer

    uint32_t Size = 20; // size of pre-entries stuff
    uint32_t Version = json["Version"];
    uint32_t Count = i; // # of entries
    uint64_t Pointer = 8;

    // calculate final filesize
    Size += Count * 40;
    for (int m = 0; m < Count; m++) {
        Size += entry[m].message.size() + 1;
    }
    int saveSize = Size; // to be used later
    while (Size % 4 != 0) {
        Size++;
    }
    Size = toBigEndian(Size - 4);

    // more XFBIN writing
    uint32_t TotalSize = toBigEndian(toBigEndian(Size) + 4);
    binary.write((char*)&TotalSize, sizeof(TotalSize));
    b32 = toBigEndian(1);
    binary.write((char*)&b32, sizeof(b32));
    b32 = toBigEndian(7956736);
    binary.write((char*)&b32, sizeof(b32));

    // write start-of-data stuff
    binary.write((char*)&Size, sizeof(Size));
    binary.write((char*)&Version, sizeof(Version));
    binary.write((char*)&Count, sizeof(Count));
    binary.write((char*)&Pointer, sizeof(Pointer));

    for (int j = 0; j < Count; j++) {
        // crc32_id
        b32 = std::stoul(entry[j].crc32_id.c_str(), nullptr, 16);
        b32 = toBigEndian(b32);
        binary.write((char*)&b32, sizeof(b32));

        // blank bytes
        b32 = 0x0000;
        b64 = 0x0000;
        binary.write((char*)&b64, sizeof(b64));
        binary.write((char*)&b32, sizeof(b32));

        // message pointer
        if (j > 0) {
            bp += entry[j-1].message.size() + 1 - 40;
        } else {
            bp = (40 * Count) - 16;
        }
        binary.write((char*)&bp, sizeof(bp));

        // ref_crc32
        if (entry[j].ref_crc32 == "") {
            b32 = 0x0000;
        } else {
            b32 = std::stoul(entry[j].ref_crc32.c_str(), nullptr, 16);
            b32 = toBigEndian(b32);
        }
        binary.write((char*)&b32, sizeof(b32));
        binary.write((char*)&entry[j].is_ref, sizeof(entry[j].is_ref));

        // char_id
        binary.write((char*)&entry[j].char_id, sizeof(entry[j].char_id));
        // cue_id
        binary.write((char*)&entry[j].cue_id, sizeof(entry[j].cue_id));

        // blank bytes
        b16 = 0xFFFF;
        b32 = 0x0000;
        binary.write((char*)&b16, sizeof(b16));
        binary.write((char*)&b32, sizeof(b32));
    }

    // write strings
    for (int j = 0; j < Count; j++) {
        const char* buffer = entry[j].message.c_str();
        binary.write(buffer, entry[j].message.size());
        binary.put(0x00);
    }

    // make file size a multiple of 4
    while (saveSize % 4 != 0) {
        binary.put(0x00);
        saveSize++;
    }
    
    // add end of XFBIN stuff
    std::ifstream xfbinEnd("data/end.xfbin.dat", std::ios::binary);
    XFBIN(xfbinEnd, binary);
    xfbinEnd.close();

    binary.close();
    return 1;
}