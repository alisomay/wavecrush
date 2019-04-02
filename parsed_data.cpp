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
   std::cout << "FILE SIZE : " << header.overall_size << " BYTES. ( +8 to include \"RIFF\" and size bytes in the size. )" <<  std::endl;              
   std::cout << "LENGTH OF FMT : " << header.length_of_fmt << " BYTES. ( +8 to include \"fmt \" and size bytes in the size. )" <<  std::endl;                              
   std::cout << "ENCODING : " << header.format_type << std::endl;                   
   std::cout << "CHANNELS : " <<  header.channels << std::endl;                        
   std::cout << "SAMPLE RATE (blocks per second) : " <<  header.sample_rate << std::endl;                    
   std::cout << "BYTE RATE (SampleRate * NumChannels * BitsPerSample/8) : " <<  header.byterate << std::endl;                       
   std::cout << "BLOCK ALIGN (NumChannels * BitsPerSample/8) : " << header.block_align << std::endl;                   
   std::cout << "BITS PER SAMPLE : " << header.bits_per_sample << " BITS." << std::endl;                
   // std::cout << "SIZE OF THE JUNK CHUNK : " << header.junk_size() << " BYTES. ( +8 to include \"JUNK\" and size bytes in the size. )" << std::endl;
   std::cout << "SIZE OF THE DATA CHUNK : " << header.data_size << " BYTES. ( +8 to include \"data\" and size bytes in the size. )" << std::endl;                       
   std::cout << std::endl;
   // std::cout << "--INFORMATION ABOUT STRIPPED DATA CHUNK--" << std::endl;
   // std::cout << std::endl;
   // std::cout << "BYTESIZE OF ALL SAMPLES : " << header.all_samples_size() << " BYTES." << std::endl;
   // std::cout << "BYTESIZE OF LEFT CHANNEL SAMPLES : " << header.left_channel_samples_size() << " BYTES." << std::endl;
   // std::cout << "BYTESIZE OF RIGHT CHANNEL SAMPLES : " << header.right_channel_samples_size() << " BYTES." << std::endl;

   // // wc.crush(file_in_memory);

   //  wc.write_file("/Users/vallahiboyle/Desktop/wavecrush/invtry.wav", header.all_samples());
};