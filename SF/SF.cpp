#include "SF.h"
namespace Operator {
    namespace {
        std::mutex res_mutex;
        std::mutex file_mutex;
        std::u8string path_function_result{};

        bool OperateDirectory(const_path &path_, const std::u8string &file_name) {
            {
                std::scoped_lock file_lock(file_mutex);
                std::ofstream out("out.txt", std::ios_base::app);
                out << path_ << std::endl;
                out.close();
            }
            if(std::scoped_lock lock(res_mutex); !path_function_result.empty()){
                return true;
            }
            std::vector<path> subdirectories;
            for(const_path& _path: iterator(path_)){
                if(is_directory(_path)){
                    subdirectories.push_back(_path);
                }
                if(_path.filename().u8string() == file_name){
                    std::scoped_lock lock(res_mutex);
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
        std::ofstream out("out.txt", std::ios_base::app);
        //Scan the m_root directory to find all the subdirectories
        for (auto &it: iterator(m_root)) {
            out << it.path() << std::endl;
            if (is_directory(it.path())) {
                m_root_subdirs.push_back(it.path());
            }
                //If we find the answer here, we don't have to initialize the threads
            else if (it.path().filename() == file_name) {
                out.close();
                return it.path().u8string();
            }
        }
        out.close();
        return {};
    }

    //just initialize the root_directory, which then we are going to analise
    SF::SF(const std::u8string &root_directory) : m_root{root_directory} {}

    std::u8string SF::FindPath(const std::u8string &file_name) {
        std::ofstream out("out.txt", std::ios_base::trunc);
        out.close();
        auto res = ScanRootDirectory(file_name);
        if (!res.empty()) return res;
        // now we need to initialize threads

        BS::thread_pool pool(static_cast<int>(8));
        //make tests to find out the best threads number
        for (const_path &path_: m_root_subdirs) {
            //pool.submit(&Operator::ScanDirectory, path_, file_name);
            auto fut = pool.submit(&Operator::ScanDirectory, path_, file_name);
            fut.wait();
        }
        return Operator::path_function_result;
    }
    std::string SF::FindPath(const std::string &file_name){
        return "";
    }

}