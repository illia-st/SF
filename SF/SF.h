#pragma once

#include <string>
#include <filesystem>
#include <algorithm>
#include "../ThreadPool/Headers/ThreadPool.h"
namespace Operator{

    using iterator = std::filesystem::directory_iterator;

    using path = std::filesystem::path;

    using const_path = const std::filesystem::path;
    namespace {
        std::mutex res_mutex;
        std::u8string path_function_result{};
        void ScanDirectory(const_path &path, const std::u8string &file_name);
        bool OperateDirectory(const_path& path, const std::u8string& file_name);
    }

    class SF{
    private:
        // root directory
        path root;
        // root's subdirectories
        std::vector<path> root_subdirs;
        // function which scat the root directory
        std::u8string ScanRootDirectory(const std::u8string &file_name);
    public:
        //the threads number will be default
        explicit SF(const std::u8string& root_directory);
        //we don't need a default constructor
        SF() = delete;
        // also a copy constructors
        SF(const SF& copy) = delete;
        SF(SF&& copy) = delete;
        //the function which will do the job
        std::u8string FindPath(const std::u8string& file_name);
    };
}

