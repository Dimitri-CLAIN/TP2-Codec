//
//  main.cpp
//  TP2-Codec
//
//  Created by Clain Dimitri on 2022-02-20.
//

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

wchar_t getBytes(wchar_t value, int pos, int nbBytes)
{
    return ((1 << nbBytes) - 1) & (value >> (pos - 1));
}

wchar_t *utf8BytesToUnicodeBytes(wchar_t bytes, int nbBytes)
{
    int n = nbBytes;
    std::vector<wchar_t> res(1, 0);

    while (bytes & (1 << n)) {
        n--;
    }
    n = (nbBytes - n) - 1;
    switch (n) {
        case 1:
            res[0] = (getBytes(bytes, 9, 5) << 6) | (getBytes(bytes, 1, 6));
            res.push_back(0b00000000);
            res.push_back(0b00000000);
            res.push_back(0b00000000);
            break;
        case 2:
            res[0] = (getBytes(bytes, 17, 4) << 4) | (getBytes(bytes, 11, 4));
            res.push_back((getBytes(bytes, 9, 2) << 6) | (getBytes(bytes, 1, 6)));
            res.push_back(0b00000000);
            res.push_back(0b00000000);
            break;
        case 3:
            res[0] = (getBytes(bytes, 25, 3) << 2) | (getBytes(bytes, 21, 2));
            res.push_back((getBytes(bytes, 17, 4) << 4) | (getBytes(bytes, 11, 4)));
            res.push_back((getBytes(bytes, 9, 2) << 6) | (getBytes(bytes, 1, 6)));
            res.push_back(0b00000000);
            break;
    }
    res.push_back('\0');
    return &res[0];
}

int UtfToUni(std::string text)
{
//    wchar_t bytes = 0b1100001010100011;
//    wchar_t bytes = 0b111011110011111100111111;
//    wchar_t bytes = 0b111000001010010010111001;
//    wchar_t bytes = 0b111000101000001010101100;
//    wchar_t bytes = 0b111011011001010110011100;
//    wchar_t bytes = 0b11110000100100001000110110001000;
    wchar_t *data = NULL;

    std::wofstream myfile(L"example.encoded", std::ios_base::binary | std::ios_base::out);
//    data = utf8BytesToUnicodeBytes(bytes, 15);
//    data = utf8BytesToUnicodeBytes(bytes, 23);
//    data = utf8BytesToUnicodeBytes(bytes, 31);
//    myfile << data;

    for (wchar_t nextChar : text) {
        unsigned int len = (unsigned int)nextChar;
        if (len == 0)
            continue;
        if (len < 163) {
            std::vector<wchar_t> res(1, getBytes(nextChar, 1, 7));
            data = &res[0];
        }
        else if (len < 2361)
            data = utf8BytesToUnicodeBytes(nextChar, 15);
        else if (len < 66376)
            data = utf8BytesToUnicodeBytes(nextChar, 23);
        else if (len >= 66376)
            data = utf8BytesToUnicodeBytes(nextChar, 31);
        myfile << data;
    }
    myfile.close();
    return 0;
}

wchar_t convertNbites(wchar_t value, int bits) {
    if (bits <= 0) {
        return ((value & 0b0000111111) & 0b0111111) | 0b10000000;
    }
    return (((value >> bits) & 0b0000111111) & 0b0111111) | 0b10000000;
}

wchar_t *unicodeBytesToUft8bytes(wchar_t value, int nbBytes) {
    static std::vector<wchar_t> resV(1, 0);
    wchar_t firstMask = 0;

    if (nbBytes == 18)
        firstMask = 0b11110000;
    else if (nbBytes == 12)
        firstMask = 0b11100000;
    else if (nbBytes == 6)
        firstMask = 0b11000000;
    else if (nbBytes == 1) {
        resV[0] = (value & 0b01111111);
        resV.push_back('\0');
        return &resV[0];
    } else
        return NULL;
    resV[0] = ((value >> nbBytes) & 0b01111) | firstMask;
    for (int i = nbBytes - 6; i >= 0; i -= 6) {
        resV.push_back(convertNbites(value, i));
    }
    resV.push_back('\0');
    return &resV[0];
}

int UniToUtf(std::string text) {
    wchar_t *charUTF8 = NULL;
    std::wofstream myfile(L"example.encoded", std::ios_base::binary | std::ios_base::out);


    for (wchar_t nextChar : text) {
        if ((unsigned int)nextChar == 0)
            continue;
        if ((unsigned int)nextChar < 163)
            charUTF8 = unicodeBytesToUft8bytes(nextChar, 1);
        else if ((unsigned int)nextChar < 2361)
            charUTF8 =unicodeBytesToUft8bytes(nextChar, 6);
        else if ((unsigned int)nextChar < 66376)
            charUTF8 =unicodeBytesToUft8bytes(nextChar, 12);
        else if ((unsigned int)nextChar >= 66376)
            charUTF8 =unicodeBytesToUft8bytes(nextChar, 18);
        myfile << charUTF8;
    }
    myfile.close();
    return 0;
}

char *reader(char *argv) {
    char *text = NULL;
    int fileLen = 0;

    std::fstream file;
    file.open(argv, std::ios::binary | std::ios::in);
    if (file.is_open()) {
        file.seekg(0, file.end);
        fileLen = (int)file.tellg();
        file.seekg(0, file.beg);
        text = new char[fileLen];
        file.read(text, fileLen);
    } else {
        std::cout << "Error while open file" << std::endl;
    }
    file.close();
    return text;
}

int helper() {
    std::cout << "HELP" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    return 0;
}

int main(int argc, char **argv) {
    int res = 0;
    char *text = NULL;
    std::string flag1 = "";
    std::string flag2 = "";

    if (argc < 2 || argc > 4) {
        res = 84;
    } else if (argc == 2) {
        flag1 = argv[1];
        if (flag1 == "-h") {
            res = helper();
        } else {
            res = 84;
        }
    } else {
        flag1 = argv[1];
        flag2 = argv[2];
        if (flag2 == "-f") {
//            text = NULL;
            text = reader(argv[3]);
        } else if (flag2 == "-s") {
            text = argv[3];
        } else {
            text = NULL;
        }
        if (text == NULL) {
            res = 84;
        } else {
//            std::cout.write(text, std::strlen(text)) << std::endl;
            if (flag1 == "+") {
                res = UtfToUni(text);
            } else if (flag1 == "-") {
                res = UniToUtf(text);
            }
        }
    }
    return res;
}

//int main() {
////    wchar_t data[5] = {0b11110000, 0b10010000, 0b10001101, 0b10001000, '\0'};
////    wchar_t data[3] = {0b11000010, 0b10100011, '\0'};
////    Todo: stock dans un array;
////        wchar_t *data = unicodeBytesToUft8bytes(0b0100100, 1);
//    //    wchar_t *data = unicodeBytesToUft8bytes(0b00010100011, 6);
//    //    wchar_t *data = unicodeBytesToUft8bytes(0b0000100100111001, 12);
//    //    wchar_t *data = unicodeBytesToUft8bytes(0b0010000010101100, 12);
//    //    wchar_t *data = unicodeBytesToUft8bytes(0b1101010101011100, 12);
//    wchar_t *data = unicodeBytesToUft8bytes(0b000010000001101001000, 18);
//    std::wofstream myfile;
//    myfile.open(L"example", std::ios_base::binary | std::ios_base::out);
////    myfile << &data[0];
//    myfile << data;
//    myfile.close();
//    return 0;
//}
