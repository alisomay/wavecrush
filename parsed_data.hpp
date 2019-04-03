
#include <vector>
#include <map>
#include <string>
#include <iostream>

#ifndef parsed_data_hpp
#define parsed_data_hpp

class Parsed_Data {

	public:

	Parsed_Data() = default;
	~Parsed_Data() = default;

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


	    unsigned char data_chunk_header[4]{'d','a','t','a'};        // DATA string or FLLR string
	    unsigned int data_size{0};                     // NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
	    

	} header;


    struct data_chunk {
	   
	   std::vector<unsigned char> all_samples{};
	   std::vector<unsigned char> left_channel_samples{};
	   std::vector<unsigned char> right_channel_samples{};

	} data_chunk;

	std::vector<std::string> insertation_order;
	std::map< std::string,std::pair<int,unsigned int> > chunk_info; 


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

    void report();
	
}; 

#endif /* parsed_data_hpp */