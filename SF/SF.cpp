#include "SF.h"
namespace Operator {
    namespace {
        std::mutex res_mutex;
        std::u8string path_function_result{};

        bool OperateDirectory(const_path &path_, const std::u8string &file_name) {
            std::unique_lock<std::mutex> lock;
            lock.lock();
            if(!path_function_result.empty()) return true;
            lock.unlock();
            std::vector<path> subdirectories;
            for(const_path& _path: iterator(path_)){
                if(is_directory(_path)){
                    subdirectories.push_back(_path);
                }
                if(_path.filename().u8string() == file_name){
                    lock.lock();
                    path_function_result = _path.u8string();
                    return true;
                }
            }
            if(std::ranges::any_of(subdirectories.begin(), subdirectories.end()
                    ,[&file_name](const_path & _path) { return OperateDirectory(_path, file_name);})){
                return true;
            }
            return false;
        }
        // function which scans directory and searches for the file name
        void ScanDirectory(const_path &path, const std::u8string &file_name) {
            //implementation
            for (auto &it: iterator(path)) {
                if(OperateDirectory(path, file_name)) break;
            }
        }
    }

// we needn't use threads here, main thread is enough
    std::u8string SF::ScanRootDirectory(const std::u8string &file_name) {
        //Scan the m_root directory to find all the subdirectories
        for (auto &it: iterator(m_root)) {
            if (is_directory(it.path())) {
                m_root_subdirs.push_back(it.path());
            }
                //If we find the answer here, we don't have to initialize the threads
            else if (it.path().filename() == file_name) {
                return it.path().u8string();
            }
        }
        return {};
    }

//just initialize the root_directory, which then we are going to analise
    SF::SF(const std::u8string &root_directory) : m_root{root_directory} {}

    std::u8string SF::FindPath(const std::u8string &file_name) {
        auto res = ScanRootDirectory(file_name);
        if (!res.empty()) return res;
        // now we need to initialize threads
        //ThreadPool pool(static_cast<int>(m_root_subdirs.size()));
        BS::thread_pool pool(static_cast<int>(m_root_subdirs.size()));
        //pool.init();
        //make tests to find out the best threads number
        for (const_path &path_: m_root_subdirs) {
            //pool.submit(&Operator::ScanDirectory, path_, file_name);
            pool.push_task(&Operator::ScanDirectory, path_, file_name);
        }
        return Operator::path_function_result;
    }

    std::u8string SF::tempFindPath(const std::u8string &file_name) {
        auto res = ScanRootDirectory(file_name);
        if (!res.empty()) return res;
        Operator::ScanDirectory(m_root_subdirs.front(), file_name);
        return Operator::path_function_result;
    }
}