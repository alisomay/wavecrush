
#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>

#include "path_manager.hpp"
#include "parsed_data.hpp"


namespace fs = std::filesystem;

#ifndef wav_parser_hpp
#define wav_parser_hpp

class Wav_Parser {

private:

	Path_Manager &path_manager;
	Parsed_Data &parsed_data;
	fs::path in_path;
	fs::path crushed_path;

public:

	Wav_Parser() = default;
	~Wav_Parser() = default;

	Wav_Parser(Path_Manager& path_manager, Parsed_Data& parsed_data) : 
	path_manager(path_manager), 
	parsed_data(parsed_data){};

    friend class Parsed_Data;

    std::vector<std::string> chunk_types{
    "JUNK","fmt ","data","bext",
    "cue ","LIST","minf","elm1",
    "slnt","fact","plst","labl","note",
    "adtl","ltxt","file"};

    std::vector<unsigned char> read_file(); 
    bool write_file(std::vector<unsigned char>&);

    void parse();
    void collect_chunk_info(std::ifstream&, std::streampos&);
    int parse_fmt(std::ifstream&);
    int read_samples(std::ifstream&);
   
	std::string read_x_bytes_to_string(std::ifstream&,int);
    std::vector<unsigned char> read_x_bytes_to_vector(std::ifstream&,int);
    void read_and_fill_x_bytes_to_vector(const std::vector<unsigned char>& ,std::ifstream&, int , int );

    unsigned int pack4chars_ltob(char, char, char, char);
	unsigned int pack3chars_ltob(char, char, char);
	unsigned int pack2chars_ltob(char, char);

    std::vector<unsigned char> unpack_unsigned_int_to_chars_btol_4bytes(unsigned int);
    std::vector<unsigned char> unpack_unsigned_int_to_chars_btol_2bytes(unsigned int);
    std::vector<unsigned char> unpack_unsigned_int_to_chars_4bytes(unsigned int);

};

#endif /* wav_parser_hpp */