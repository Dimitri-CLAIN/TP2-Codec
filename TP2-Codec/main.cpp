//
//  main.cpp
//  TP2-Codec
//
//  Created by Clain Dimitri on 2022-02-20.
//

#include <string>
#include <iostream>
#include <fstream>



int UtfToUni() {
    std::cout << "Utf To Uni" << std::endl;
    return 0;
}

int UniToUtf() {
    std::cout << "Uni To Utf" << std::endl;
    return 0;
}

int main(int argc, char **argv) {
    int res = 0;
    if (argc < 2) {
        res = 84;
    } else if (argc == 2) {
        std::string flag(argv[1]);
        if (flag == "-h") {
            std::cout << "HELP" << std::endl;
            std::cout << std::endl;
            std::cout << "Usage:" << std::endl;
            res = 0;
        } else {
            res = 84;
        }
    } else if (argc > 3) {
        res = 84;
    } else {
        std::fstream file;
        file.open(argv[2], std::ios::binary | std::ios::in);
        
        if (!file.is_open()) {
            std::cout << "Error while open file" << std::endl;
            res = 84;
        } else {
            std::string flag(argv[1]);
            if (flag == "+") {
                res = UtfToUni();
            } else if (flag == "-") {
                res = UniToUtf();
            }
            file.close();
        }
    }
    return res;
}
