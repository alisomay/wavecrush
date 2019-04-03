#include "parsed_data.hpp"

unsigned int Parsed_Data::get_overall_size(){
	return header.overall_size;
}             
unsigned int Parsed_Data::get_length_of_fmt(){
	return header.length_of_fmt;
}                
unsigned int Parsed_Data::get_format_type(){
	return header.format_type;
}                 
unsigned int Parsed_Data::get_channels(){
	return header.channels;
}                      
unsigned int Parsed_Data::get_sample_rate(){
	return header.sample_rate;
}                  
unsigned int Parsed_Data::get_byterate(){
	return header.byterate;
}                     
unsigned int Parsed_Data::get_block_align(){
	return header.block_align;
}                 
unsigned int Parsed_Data::get_bits_per_sample(){
	return header.bits_per_sample;
}              
unsigned int Parsed_Data::get_data_size(){
	return header.data_size;
} 
unsigned int Parsed_Data::get_all_samples_size(){
	return data_chunk.all_samples.size();
};                 
unsigned int Parsed_Data::get_left_channel_samples_size(){
	return data_chunk.left_channel_samples.size();
};              
unsigned int Parsed_Data::get_right_channel_samples_size(){
	return data_chunk.right_channel_samples.size();
};

std::vector<unsigned char> Parsed_Data::get_all_samples(){
	return data_chunk.all_samples;
};                 
std::vector<unsigned char> Parsed_Data::get_left_channel_samples(){
	return data_chunk.left_channel_samples;
};              
std::vector<unsigned char> Parsed_Data::get_right_channel_samples(){
	return data_chunk.right_channel_samples;
};

void Parsed_Data::report(){
   std::cout << "FILE SIZE : " << get_overall_size() << " BYTES. ( +8 to include \"RIFF\" and size bytes in the size. )" <<  std::endl;              
   std::cout << "LENGTH OF FMT : " << get_length_of_fmt() << " BYTES. ( +8 to include \"fmt \" and size bytes in the size. )" <<  std::endl;                              
   std::cout << "ENCODING : " << get_format_type() << std::endl;                   
   std::cout << "CHANNELS : " << get_channels() << std::endl;                        
   std::cout << "SAMPLE RATE (blocks per second) : " << get_sample_rate() << std::endl;                    
   std::cout << "BYTE RATE (SampleRate * NumChannels * BitsPerSample/8) : " << get_byterate() << std::endl;                       
   std::cout << "BLOCK ALIGN (NumChannels * BitsPerSample/8) : " << get_block_align() << std::endl;                   
   std::cout << "BITS PER SAMPLE : " << get_bits_per_sample() << " BITS." << std::endl;                
   // std::cout << "SIZE OF THE JUNK CHUNK : " << get_junk_size() << " BYTES. ( +8 to include \"JUNK\" and size bytes in the size. )" << std::endl;
   std::cout << "SIZE OF THE DATA CHUNK : " << get_data_size() << " BYTES. ( +8 to include \"data\" and size bytes in the size. )" << std::endl;                       
   std::cout << std::endl;
   
   std::cout << "--INFORMATION ABOUT STRIPPED DATA CHUNK--" << std::endl;
   std::cout << std::endl;
   std::cout << "BYTESIZE OF ALL SAMPLES : " << get_all_samples_size() << " BYTES." << std::endl;
   std::cout << "BYTESIZE OF LEFT CHANNEL SAMPLES : " << get_left_channel_samples_size() << " BYTES." << std::endl;
   std::cout << "BYTESIZE OF RIGHT CHANNEL SAMPLES : " << get_right_channel_samples_size() << " BYTES." << std::endl;

};