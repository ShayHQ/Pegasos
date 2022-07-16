#include "../include/PegasosCommon/common.hpp"

std::vector<char> readBinaryFile(std::string path){
    std::vector<char> buffer;
    std::ifstream targetFile(path.c_str(), std::ios::ate | std::ios::binary);

    if (!targetFile.is_open()){
        throw std::runtime_error(std::string("Could not open: " + path));
    }

    buffer.resize(static_cast<std::size_t>(targetFile.tellg()));
    targetFile.seekg(0);
    targetFile.read(buffer.data(), buffer.size());

    return buffer;
}