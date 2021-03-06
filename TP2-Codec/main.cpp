//
//  main.cpp
//  TP2-Codec
//
//  Created by Clain Dimitri & Vandenbossche Louis on 2022-02-15.
//

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

long long fileLen = 0;

wchar_t getBytes(wchar_t value, int pos, int nbBytes)
{
    return ((1 << nbBytes) - 1) & (value >> (pos - 1));
}

wchar_t *utf8BytesToUnicodeBytes(wchar_t bytes, int nbBytes)
{
    int n = nbBytes;
    static std::vector<wchar_t> res(4, 0);

    while (bytes & (1 << n)) {
        n--;
    }
    n = (nbBytes - n) - 1;
    switch (n) {
        case 1:
            res[0] = (getBytes(bytes, 9, 5) << 6) | (getBytes(bytes, 1, 6));
            break;
        case 2:
            res[0] = (getBytes(bytes, 17, 4) << 4) | (getBytes(bytes, 11, 4));
            res[1] = (getBytes(bytes, 9, 2) << 6) | (getBytes(bytes, 1, 6));
            break;
        case 3:
            res[0] = (getBytes(bytes, 25, 3) << 2) | (getBytes(bytes, 21, 2));
            res[1] = (getBytes(bytes, 17, 4) << 4) | (getBytes(bytes, 11, 4));
            res[2] = (getBytes(bytes, 9, 2) << 6) | (getBytes(bytes, 1, 6));
            break;
    }
    res.push_back('\0');
    return &res[0];
}

int UtfToUni(std::string text)
{
    wchar_t *data = NULL;
    std::wofstream myfile("example.uni", std::ios_base::binary | std::ios_base::out);

    for (wchar_t nextChar : text) {
        unsigned int len = (unsigned int)nextChar;
        if (len == 0)
            continue;
        if (len < 163) {
            std::vector<wchar_t> res(4, 0);
            res[0] = getBytes(nextChar, 1, 7);
            res.push_back('\0');
            data = &res[0];
        }// Le vector est set avec {0, 0, 0, 0}, dans la mémoire le résultat de 'L' est bien "4C 00 00 00" mais dans le fichier j'ai pas les 00 correspondant ...
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

int UniToUtf(char *text) {
    wchar_t *charUTF8 = NULL;
    std::ofstream myfile("example.encoded", std::ios_base::binary | std::ios_base::out);

    for (int i = 0; i < fileLen; i += 4) {
        unsigned int size = (unsigned int)text[i];

        if (size == 0)
            continue;
        if (size < 163)
            charUTF8 = unicodeBytesToUft8bytes(text[i], 1);
        else if (size < 2361)
            charUTF8 =unicodeBytesToUft8bytes(text[i], 6);
        else if (size <= 54620)
            charUTF8 =unicodeBytesToUft8bytes(text[i], 12);
        else if (size <= 66376)
            charUTF8 =unicodeBytesToUft8bytes(text[i], 18);
        for(int j = 0; charUTF8[j]; j++) {
            myfile.put(charUTF8[j]);
        }
    }
    myfile.close();
    return 0;
}

char *reader(char *argv) {
    char *text = NULL;

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
            text = reader(argv[3]);
        } else if (flag2 == "-s") {
            text = argv[3];
        } else {
            text = NULL;
        }
        if (text == NULL) {
            res = 84;
        } else {
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
