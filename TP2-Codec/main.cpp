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


int UtfToUni(char *text) {
    //std::cout << "Utf To Uni" << std::endl;
    return 0;
}

int UniToUtf(char *text) {
    //std::cout << "Uni To Utf" << std::endl;
    return 0;
}

char *reader(char *argv) {
    char *text = NULL;
    int fileLen = 0;
    
    std::fstream file;
    file.open(argv, std::ios::binary | std::ios::in);
    if (file.is_open()) {
        file.seekg(0, file.end);
        fileLen = file.tellg();
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
            std::cout.write(text, std::strlen(text)) << std::endl;
            if (flag1 == "+") {
                res = UtfToUni(text);
            } else if (flag1 == "-") {
                res = UniToUtf(text);
            }
        }
    }
    return res;
}
