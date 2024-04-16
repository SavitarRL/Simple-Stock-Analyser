#include <iostream>
#include <cstring> 
#include "analyser.cpp"


int main(int argc, char* argv[])
{   
    if (argc != 2) {
        std::cerr << "Usage: ./<execution file name> <input text file>" << std::endl;
        return 1;
    }

    if (!std::strcmp("version", argv[1])) {
        std::cerr << "StockAnalyser version 0.1" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    StockAnalyser analyser(filename);

    analyser.PrintOutput();

    return 0;
}