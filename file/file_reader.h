#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <vector>

class FileReader {
public:
    static std::vector<int> readAccessTrace(const std::string& filePath);
};

#endif