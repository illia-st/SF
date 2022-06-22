#include "SF.h"


std::u8string SF::ScanRootDirectory(const std::u8string &file_name) {
    //Scan the root directory to find all the subdirectories
    for (auto &it: iterator(root)) {
        if (is_directory(it.path())) {
            root_subdirs.push_back(it.path());
        }
        //If we find the answer here, we don't have to initialize the threads
        else if (it.path().filename() == file_name) {
            return it.path().u8string();
        }
    }
    return {};
}
//just initialize root_deirectory, which then we are going to analise
SF::SF(const std::u8string &root_directory):root{root_directory} {}

std::u8string SF::ScanDirectory(const_path &path, const std::u8string &file_name) {
    return u8"";
}

std::u8string SF::FindPath(const std::u8string &file_name) {
    auto result = ScanRootDirectory(file_name);
    if(!result.empty()) return result;

    ThreadPool pool(static_cast<int>(root_subdirs.size()));
    pool.init();
//    for(const_path& p: root_subdirs){
//        auto ft = pool.submit(ScanRootDirectory, this, p, file_name);
//    }
}