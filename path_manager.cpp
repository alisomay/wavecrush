#include "path_manager.hpp"
    
    fs::path Path_Manager::get_in_path(){
    	return in_path;
    };
    fs::path Path_Manager::get_crushed_path(){
    	return crushed_path;
    };
    fs::path Path_Manager::set_crushed_path(std::string path){
    	crushed_path = path;
    	return crushed_path;
    };
    fs::path Path_Manager::set_crushed_path(char* path){
    	crushed_path = path;
    	return crushed_path;
    };