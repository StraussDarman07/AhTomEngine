//
// Created by stach on 03/12/2023.
//

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <format>
namespace Core::Utils {

    static std::vector<char> readFile(const std::string &filename) {
        std::ifstream fileStream(filename, std::ios::ate | std::ios::binary);

        if (!fileStream.is_open())
            throw std::runtime_error(std::format("failed to open file {}", filename));

        size_t end = static_cast<size_t>(fileStream.tellg());
        fileStream.seekg(0, std::ios::beg);

        size_t fileSize = std::size_t(end - fileStream.tellg());

        std::vector<char> buffer(end);

        if (!fileStream.read(buffer.data(), buffer.size()))
            throw std::runtime_error(std::format("could not read file {}", filename));

        fileStream.close();

        return buffer;
    }
}