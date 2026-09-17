#include "file_reader.h"

#include <fstream>
#include <stdexcept>

std::vector<int> FileReader::readAccessTrace(const std::string& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::vector<int> trace;
    int page;

    while (file >> page) {
        trace.push_back(page);
    }

    return trace;
}