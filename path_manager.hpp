
#include <filesystem> //c++17
#include <string>
#include <regex>

namespace fs = std::filesystem;

#ifndef path_manager_hpp
#define path_manager_hpp

class Path_Manager {

private:
	const fs::path in_path;
	fs::path crushed_path;

public:

	Path_Manager() = default;
	Path_Manager(std::string path) : in_path(path) {
    
    if( !fs::exists(in_path) ){
		throw std::invalid_argument( "This file path is not valid or accessible. There is also a possibility that you havent gave the program enough arguments." );
	}
    
    crushed_path = std::regex_replace(in_path.filename().string(), std::regex(".wav"), "_crushed.wav");
    crushed_path = fs::current_path() / crushed_path;
	
	fs::copy(in_path,get_crushed_path(),fs::copy_options::overwrite_existing);

	}
	~Path_Manager() = default;
    

    fs::path get_in_path();
    fs::path get_crushed_path();
    fs::path set_crushed_path(std::string);
    fs::path set_crushed_path(char*);


};

#endif /* path_manager_hpp */