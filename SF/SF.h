#pragma once

#include <string>
#include <filesystem>
#include "../ThreadPool/Headers/ThreadPool.h"

class SF{
private:
    using iterator = std::filesystem::directory_iterator;
    using path = std::filesystem::path;
    using const_path = const std::filesystem::path;

    path root;
    std::vector<path> root_subdirs;
    std::u8string ScanDirectory(const_path& path, const std::u8string& file_name);
    std::u8string ScanRootDirectory(const std::u8string &file_name);
public:
    //the threads number will be default

    SF(const std::u8string& root_directory);
    SF() = delete;
    SF(const SF& copy) = delete;
    SF(SF&& copy) = delete;
    std::u8string FindPath(const std::u8string& file_name);
};
