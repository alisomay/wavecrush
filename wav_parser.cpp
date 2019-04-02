#include "wav_parser.hpp"

unsigned int Wav_Parser::pack4chars_ltob(char c4, char c3, char c2, char c1) {
    return (
    	       (unsigned int)(((unsigned char)c1) << 24)
            |  (unsigned int)(((unsigned char)c2) << 16)
            |  (unsigned int)(((unsigned char)c3) << 8)
            |  (unsigned int)((unsigned char)c4)
           
           );
}

std::vector<unsigned char> Wav_Parser::unpack_unsigned_int_to_chars_btol_4bytes(unsigned int integer) {
    std::vector<unsigned char> chars(4);
    
    chars[3] = (unsigned char)(integer >> 24) ;
    chars[2] = (unsigned char)(integer >> 16);
    chars[1] = (unsigned char)(integer >> 8);
    chars[0] = (unsigned char)integer;

    return chars;
}
std::vector<unsigned char> Wav_Parser::unpack_unsigned_int_to_chars_btol_2bytes(unsigned int integer) {
    //00 00 00 01 --> 01 00 00 00 
    std::vector<unsigned char> chars(2);
    
    chars[0] = (unsigned char)integer;
    chars[1] = (unsigned char)(integer >> 8);

    return chars;
}
std::vector<unsigned char> Wav_Parser::unpack_unsigned_int_to_chars_4bytes(unsigned int integer) {
    std::vector<unsigned char> chars(4);
    
    chars[0] = (unsigned char)(integer >> 24);
    chars[1] = (unsigned char)(integer >> 16);
    chars[2] = (unsigned char)(integer >> 8);
    chars[3] = (unsigned char)integer;

    return chars;
}


unsigned int Wav_Parser::pack3chars_ltob(char c3, char c2, char c1) {
    return (
    		   (unsigned int)(((unsigned char)0) << 24)
            |  (unsigned int)(((unsigned char)c1) << 16)
            |  (unsigned int)(((unsigned char)c2) << 8)
            |  (unsigned int)((unsigned char)c3)

           );
}

unsigned int Wav_Parser::pack2chars_ltob(char c2, char c1) {
    return (
    		   (unsigned int)(((unsigned char)0) << 24)
            |  (unsigned int)(((unsigned char)0) << 16)
            |  (unsigned int)(((unsigned char)c1) << 8)
            |  (unsigned int)((unsigned char)c2)

           );
}

std::string Wav_Parser::read_x_bytes_to_string(std::ifstream& file,int x){

			std::string str;
	        
	        while(true){
		    int LENGTH = x;
		    str.resize(LENGTH);
		    file.read(str.data(), LENGTH); //c++17
		    str.resize(file.gcount());
		    if(file.gcount() != LENGTH){ continue; }else{ break; }
	        }

	        return str;
}
std::vector<unsigned char> Wav_Parser::read_x_bytes_to_vector(std::ifstream& file, int x){

			std::vector<unsigned char> vec(x);
	        
	        while(true){
			    int LENGTH = x;
			    file.read((char*) &vec[0], LENGTH); 
			    if(file.gcount() != LENGTH){ continue; }else{ break; }
	        }

	        return vec;
}
void Wav_Parser::read_and_fill_x_bytes_to_vector(const std::vector<unsigned char>& target_vec,std::ifstream& file, int offset, int length){
	        
	        while(true){
			    file.read((char*) &target_vec[offset], length); 
			    if(file.gcount() != length){ continue; }else{ break; }
	        }

}

std::vector<unsigned char> Wav_Parser::read_file() {
    
    in_path = path_manager.get_in_path();


    // open the file:
    std::ifstream file(in_path, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos file_size;

    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::vector<unsigned char> vec;
    vec.reserve(file_size);

    // read the data:
    vec.insert(vec.begin(),
               std::istream_iterator<unsigned char>(file),
               std::istream_iterator<unsigned char>());

    return vec;
}

void Wav_Parser::parse(){

	crushed_path = path_manager.get_crushed_path();

    // open the file:
    std::streampos file_size;
    std::ifstream file(crushed_path, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    collect_chunk_info(file,file_size);
    parse_fmt(file);

    file.seekg(parsed_data.chunk_info.find("data")->second.first+8, std::ios::beg);
    read_samples(file);

    std::cout << std::endl;
	
	for(auto elem : parsed_data.insertation_order){

		std::cout << parsed_data.chunk_info.find(elem)->first << " " << parsed_data.chunk_info.find(elem)->second.first << " " << parsed_data.chunk_info.find(elem)->second.second << " " << std::endl;
	}

    std::cout << std::endl;
	std::cout << "-END-" << std::endl;
	std::cout << std::endl;

}


bool Wav_Parser::write_file(std::vector<unsigned char>& crushed_samples){

	//reconstruct file from the bits that you have restored
	//and put the new data chunk instead of the old one
	//if the size of the datachunk change, change the info in the file 
	//accordingly

    //maybe no vector in the middle and check and write to file directly? 
    //make a copy and modify?
   

	crushed_path = path_manager.get_crushed_path();

    std::ios_base::sync_with_stdio(false);
    auto crushed_wav = std::fstream(crushed_path, std::ios::in | std::ios::out | std::ios::binary);
    
    //here do the format changes
    
    //here overwrite the data part 
    std::cout << parsed_data.chunk_info.find("data")->second.first+8 << std::endl;
    crushed_wav.seekp(parsed_data.chunk_info.find("data")->second.first+8, std::ios::beg);

    auto pos = crushed_wav.tellg();
    std::cout << "The file pointer is now at location " << pos << std::endl;
    //business logic for different cases
    
    if(crushed_samples.size() != parsed_data.chunk_info.find("data")->second.second){
    	 std::cout <<  "NOOOOOOOO" << std::endl;
    }
    else{

    crushed_wav.write(reinterpret_cast<const char*>(&crushed_samples[0]), crushed_samples.size()*sizeof(unsigned char));
    crushed_wav.close();

	}

	return 0;
};

void Wav_Parser::collect_chunk_info(std::ifstream& file, std::streampos& file_size){

	// read the data:
    std::vector<unsigned char> read_bytes;
    int i{0};

    std::string riff_string( read_x_bytes_to_string(file,4) );
    if(riff_string=="RIFF"){
    	parsed_data.header.riff[0] = 'R';
    	parsed_data.header.riff[1] = 'I';
    	parsed_data.header.riff[2] = 'F';
    	parsed_data.header.riff[3] = 'F';
    }

    file.seekg(i+=4);
    read_bytes = read_x_bytes_to_vector(file,sizeof(read_bytes));
    parsed_data.header.overall_size = pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]);


    parsed_data.chunk_info.insert(std::make_pair(riff_string,std::make_pair(0,pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]))));

    parsed_data.insertation_order.push_back(riff_string);
       
    file.seekg(i+=4);
    
    std::string wave_string( read_x_bytes_to_string(file,4) );

    if(wave_string=="WAVE"){
    	parsed_data.header.wave[0] = 'W';
    	parsed_data.header.wave[1] = 'A';
    	parsed_data.header.wave[2] = 'W';
    	parsed_data.header.wave[3] = 'E';
    }
    
    parsed_data.chunk_info.insert(std::make_pair(wave_string,std::make_pair(8,0) ) );
    parsed_data.insertation_order.push_back(wave_string);
  
    
    file.seekg(i+=4);
    
    for(;i<file_size;){
	    
	    file.seekg(i);
        std::string query( read_x_bytes_to_string(file,4) );
       
	    if( std::find(chunk_types.begin(), chunk_types.end(), query) != chunk_types.end() ){
	    	
	    	file.seekg(i+=4);
	    	read_bytes = read_x_bytes_to_vector(file,sizeof(read_bytes));

		    unsigned int size = pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]);
		    
		    if(parsed_data.chunk_info.find(query) == parsed_data.chunk_info.end()){
			
			    parsed_data.chunk_info.insert(std::make_pair(query,std::make_pair(i-4,size) ) );
			    parsed_data.insertation_order.push_back(query);
		    
		    }
	        
	        i+=4;

		    i+=parsed_data.chunk_info[query].second;		    
	    	
	    }
	    else{

	    	i+=4;
	    }
    
    }

    
};

int Wav_Parser::parse_fmt(std::ifstream& file){

	if(parsed_data.chunk_info["fmt "].second) {

		// std::cout << "hey!" << std::endl;
		std::vector<unsigned char> fmt;
		file.seekg(parsed_data.chunk_info["fmt "].first);
		fmt = read_x_bytes_to_vector(file,parsed_data.chunk_info["fmt "].second+8);
		
		for(unsigned int i{0}; i < sizeof(parsed_data.header.fmt_chunk_marker); i++){
	    parsed_data.header.fmt_chunk_marker[i] = fmt[i];
		}
		parsed_data.header.length_of_fmt = pack4chars_ltob(  fmt[4],fmt[5],fmt[6],fmt[7]   );
	    parsed_data.header.format_type = pack2chars_ltob(   fmt[8],fmt[9]  );
	    parsed_data.header.channels = pack2chars_ltob(   fmt[10],fmt[11]   );
	    parsed_data.header.sample_rate = pack4chars_ltob(   fmt[12],fmt[13],fmt[14],fmt[15] );
	    parsed_data.header.byterate = pack4chars_ltob(  fmt[16],fmt[17],fmt[18],fmt[19]   );
	    parsed_data.header.block_align = pack2chars_ltob(   fmt[20],fmt[21]  );
	    parsed_data.header.bits_per_sample = pack2chars_ltob( fmt[22],fmt[23]  );
	    if(parsed_data.chunk_info["fmt "].second > 16){
	        parsed_data.header.cbSize = pack2chars_ltob(   fmt[24],fmt[25]  );
	        if(parsed_data.chunk_info["fmt "].second > 18 && parsed_data.chunk_info["fmt "].second <= 40){
	        	parsed_data.header.wValidBitsPerSample = pack2chars_ltob(   fmt[26],fmt[27]  );
	        	parsed_data.header.dwChannelMask = pack4chars_ltob(   fmt[28],fmt[29],fmt[30],fmt[31]  );
	        	for(unsigned int i{0}; i < sizeof(parsed_data.header.fmt_chunk_marker); i++){
			    parsed_data.header.SubFormat[i] = fmt[i];
				}
				
	        }
	        else{
	        	return 1;
	        }
	    }

        return 0;
	}
	else{
		return 1;
	}

}

int Wav_Parser::read_samples(std::ifstream& file){

parsed_data.data_chunk.all_samples = read_x_bytes_to_vector(file,parsed_data.chunk_info.find("data")->second.second);


 //    //seperate channels for further modification
    if(parsed_data.header.channels == 2){
    auto i = parsed_data.data_chunk.all_samples.begin();
    
    unsigned int n{0};
	        while ( n < parsed_data.header.data_size){
		        	if(n % (parsed_data.header.bits_per_sample / 8) /* would give bytes per sample.*/ ){
		        	//left channel
	                parsed_data.data_chunk.left_channel_samples.push_back((*i));
		            }
		            else{
		            //right channel
		            parsed_data.data_chunk.right_channel_samples.push_back((*i));
		            }
		        	
		        	n++;
		        	++i;
	        	}
	}

return 0;
}



