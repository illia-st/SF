#pragma once

#include <string>
#include <filesystem>
#include <algorithm>
#include "../ThreadPool/BS_thread_pool.hpp"
namespace Operator{

    using iterator = std::filesystem::directory_iterator;

    using path = std::filesystem::path;

    using const_path = const std::filesystem::path;

    class SF{
    private:
        // m_root directory
        path m_root;
        // m_root's subdirectories
        std::vector<path> m_root_subdirs;
        // function which scat the m_root directory
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

        std::u8string tempFindPath(const std::u8string& file_name);
    };
}

