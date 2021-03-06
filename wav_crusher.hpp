#include <algorithm>
#include <cmath>
#include <iostream>

#include "path_manager.hpp"
#include "parsed_data.hpp"
#include "parameter_structs.hpp"

#ifndef wav_crusher_hpp
#define wav_crusher_hpp


class Wav_Crusher {

    private:

	Path_Manager &path_manager;
	Parsed_Data &parsed_data;
	Parameter_Structs &parameter_structs;

	public:

	Wav_Crusher() = default;
	~Wav_Crusher() = default;

    Wav_Crusher(Path_Manager& path_manager, Parsed_Data& parsed_data, Parameter_Structs& parameter_structs) : 
	path_manager(path_manager), 
	parsed_data(parsed_data),
	parameter_structs(parameter_structs)
	{};

    std::vector<unsigned char> range_shuffler(std::vector<unsigned char>);

};

#endif /* wav_crusher_hpp */