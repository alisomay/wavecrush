
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <filesystem> //c++17


namespace fs = std::filesystem;


class Wave_Crush {

public:

	Wave_Crush() = default;
	~Wave_Crush() = default;

    struct crush_parameters {
	   
	   

	} crush_parameters;


	std::vector<unsigned char> read_file(fs::path); 
	bool write_file(fs::path, std::vector<unsigned char>&);

	void parse(fs::path);
    std::vector<unsigned char> crusher();

	void parse_old(std::vector<unsigned char>);
	std::vector<unsigned char> crush(struct crush_parameters);

	unsigned int get_overall_size();              
    unsigned int get_length_of_fmt();                
	unsigned int get_format_type();                 
    unsigned int get_channels();                      
	unsigned int get_sample_rate();                  
	unsigned int get_byterate();                     
	unsigned int get_block_align();                 
	unsigned int get_bits_per_sample();              
    unsigned int get_data_size();                     
	unsigned int get_junk_size();

	unsigned int get_all_samples_size();                 
	unsigned int get_left_channel_samples_size();              
    unsigned int get_right_channel_samples_size();     

    std::vector<unsigned char> get_all_samples();                 
	std::vector<unsigned char> get_left_channel_samples();              
    std::vector<unsigned char> get_right_channel_samples();                  

    fs::path get_in_path();
    fs::path get_crushed_path();
    fs::path set_in_path(std::string);
    fs::path set_in_path(char*);
    fs::path set_crushed_path(std::string);
    fs::path set_crushed_path(char*);

    


private:

fs::path in_path;
fs::path crushed_path;

//NOT NEEDED ANYMORE

//HEADER BYTES
std::vector<unsigned char> header_bytes{};
std::vector<unsigned char> bext_data{};
//JUNK PART
std::vector<unsigned char> junk_data{}; //does not include "JUNK" and size bytes
//THE BYTES AFTER SAMPLE DATA FINISHED
std::vector<unsigned char> bytes_after_sample_data{};
// WAVE file header format

//NOT NEEDED ANYMORE
	
	
	//NOT NEEDED ANYMORE
	int bext_index{0};
	int real_bext_size{0};
    int junk_index{0};
    int data_index{0};
    int leftover_index{0};
    int leftover_size{0};
    int real_junk_size{0};
    //NOT NEEDED ANYMORE
    


    std::map< std::string,std::pair<int,unsigned int>> 
    collect_chunk_info(std::ifstream&, std::streampos&, std::vector<std::string>&);
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
	//crush algorithms
	//
	


	//
	struct header {
	   
	    unsigned char riff[4]{0,0,0,0};                      // RIFF string
	    unsigned int overall_size{0}   ;               // overall size of file in bytes
	    unsigned char wave[4]{0,0,0,0};                      // WAVE string
	    unsigned char fmt_chunk_marker[4]{0,0,0,0};          // fmt string with trailing null char
	    unsigned int length_of_fmt{0};                 // length of the format data
	    unsigned int format_type{0};                   // format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law
	    unsigned int channels{0};                      // no.of channels
	    unsigned int sample_rate{0};                   // sampling rate (blocks per second)
	    unsigned int byterate{0};                      // SampleRate * NumChannels * BitsPerSample/8
	    unsigned int block_align{0};                   // NumChannels * BitsPerSample/8
	    unsigned int bits_per_sample{0};               // bits per sample, 8- 8bits, 16- 16 bits etc
	    unsigned int cbSize{0}; 	                   //2 	Size of the extension (0 or 22)
		unsigned int wValidBitsPerSample{0}; 		   //2 	Number of valid bits
		unsigned int dwChannelMask{0}; 				   //4 	Speaker position mask
		unsigned char SubFormat[16]; 				   //16 GUID, including the data format code





	    unsigned char data_chunk_header[4]{0,0,0,0};        // DATA string or FLLR string
	    unsigned int data_size{0};                     // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
	    

	    //NOT NEEDED ANYMORE
	    unsigned char JUNK[4]{0,0,0,0};
	    unsigned int junk_size{0};
	    unsigned char bext_chunk_header[4]{0,0,0,0};
	    unsigned int bext_size{0};
	    //NOT NEEDED ANYMORE

	} header;

    struct data_chunk {
	   
	   std::vector<unsigned char> all_samples{};
	   std::vector<unsigned char> left_channel_samples{};
	   std::vector<unsigned char> right_channel_samples{};

	} data_chunk;

    std::vector<std::string> chunk_types{
    "JUNK","fmt ","data","bext",
    "cue ","LIST","minf","elm1",
    "slnt","fact","plst","labl","note",
    "adtl","ltxt","file"};
    std::map< std::string,std::pair<int,unsigned int> > chunk_info; 

    //
    std::vector<std::string> insertation_order;

};