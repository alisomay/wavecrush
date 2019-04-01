#include "wavecrush.hpp"

#include <fstream>
#include <algorithm>
#include <iterator>
#include <regex>

#include "crushalgorithms.hpp"

namespace fs = std::filesystem;



    fs::path Wave_Crush::get_in_path(){
    	return in_path;
    };
    fs::path Wave_Crush::get_crushed_path(){
    	return crushed_path;
    };
    fs::path Wave_Crush::set_in_path(std::string path){
    	in_path = path;
    	return in_path;
    };
    fs::path Wave_Crush::set_in_path(char* path){
    	in_path = path;
    	return in_path;
    };
    fs::path Wave_Crush::set_crushed_path(std::string path){
    	crushed_path = path;
    	return crushed_path;
    };
    fs::path Wave_Crush::set_crushed_path(char* path){
    	crushed_path = path;
    	return crushed_path;

    };


unsigned int Wave_Crush::pack4chars_ltob(char c4, char c3, char c2, char c1) {
    return (
    	       (unsigned int)(((unsigned char)c1) << 24)
            |  (unsigned int)(((unsigned char)c2) << 16)
            |  (unsigned int)(((unsigned char)c3) << 8)
            |  (unsigned int)((unsigned char)c4)
           
           );
}

std::vector<unsigned char> Wave_Crush::unpack_unsigned_int_to_chars_btol_4bytes(unsigned int integer) {
    std::vector<unsigned char> chars(4);
    
    chars[3] = (unsigned char)(integer >> 24) ;
    chars[2] = (unsigned char)(integer >> 16);
    chars[1] = (unsigned char)(integer >> 8);
    chars[0] = (unsigned char)integer;

    return chars;
}
std::vector<unsigned char> Wave_Crush::unpack_unsigned_int_to_chars_btol_2bytes(unsigned int integer) {
    //00 00 00 01 --> 01 00 00 00 
    std::vector<unsigned char> chars(2);
    
    chars[0] = (unsigned char)integer;
    chars[1] = (unsigned char)(integer >> 8);

    return chars;
}
std::vector<unsigned char> Wave_Crush::unpack_unsigned_int_to_chars_4bytes(unsigned int integer) {
    std::vector<unsigned char> chars(4);
    
    chars[0] = (unsigned char)(integer >> 24);
    chars[1] = (unsigned char)(integer >> 16);
    chars[2] = (unsigned char)(integer >> 8);
    chars[3] = (unsigned char)integer;

    return chars;
}


unsigned int Wave_Crush::pack3chars_ltob(char c3, char c2, char c1) {
    return (
    		   (unsigned int)(((unsigned char)0) << 24)
            |  (unsigned int)(((unsigned char)c1) << 16)
            |  (unsigned int)(((unsigned char)c2) << 8)
            |  (unsigned int)((unsigned char)c3)

           );
}

unsigned int Wave_Crush::pack2chars_ltob(char c2, char c1) {
    return (
    		   (unsigned int)(((unsigned char)0) << 24)
            |  (unsigned int)(((unsigned char)0) << 16)
            |  (unsigned int)(((unsigned char)c1) << 8)
            |  (unsigned int)((unsigned char)c2)

           );
}




std::vector<unsigned char> Wave_Crush::read_file(fs::path file_path) {
    // open the file:
    std::ifstream file(file_path, std::ios::binary);

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

void Wave_Crush::parse(fs::path file_path){

	//copy file
    set_crushed_path( std::regex_replace(file_path.filename().string(), std::regex(".wav"), "_crushed.wav") );
    set_crushed_path( fs::current_path() / get_crushed_path() );
	fs::copy(file_path,get_crushed_path(),fs::copy_options::overwrite_existing);


    // open the file:
    std::streampos file_size;
    std::ifstream file(crushed_path, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    chunk_info = collect_chunk_info(file,file_size,chunk_types);
    parse_fmt(file);

    file.seekg(chunk_info.find("data")->second.first+8, std::ios::beg);
    read_samples(file);

    
// std::cout <<"WO" << data_chunk.all_samples.size() << std::endl;
    std::cout << std::endl;
	
	// for(auto elem : chunk_info)
	// {
 //   		std::cout << elem.first << " " << elem.second.first << " " << elem.second.second << " " << std::endl;
	// }
	for(auto elem : insertation_order){

		std::cout << chunk_info.find(elem)->first << " " << chunk_info.find(elem)->second.first << " " << chunk_info.find(elem)->second.second << " " << std::endl;
	}

    std::cout << std::endl;
	std::cout << "-END-" << std::endl;
	std::cout << std::endl;

	


}

std::vector<unsigned char> Wave_Crush::crusher(){

auto samples = get_all_samples();
auto sample_rate = get_sample_rate();
return crush_algorithms::range_shuffler(samples,sample_rate);

}

bool Wave_Crush::write_file(fs::path crushed_path, std::vector<unsigned char>& crushed_samples){

	//reconstruct file from the bits that you have restored
	//and put the new data chunk instead of the old one
	//if the size of the datachunk change, change the info in the file 
	//accordingly



 //    int vector_size = (header.overall_size + 8) - ( chunk_info.find("data")->second.second + 8 ) + data_chunk.size();
 //    int file_size = header.overall_size  - chunk_info.find("data")->second.second  + data_chunk.size();
 //    const std::vector<unsigned char> out_vec(vector_size);

	// for(auto elem : insertation_order){
	// 	//start inserting
	// 	//
	// 	std::cout << chunk_info.find(elem)->first << " " << chunk_info.find(elem)->second.first << " " << chunk_info.find(elem)->second.second << " " << std::endl;
	



	// }

    //maybe no vector in the middle and check and write to file directly? 
    //make a copy and modify?


    
    std::ios_base::sync_with_stdio(false);
    auto crushed_wav = std::fstream(crushed_path, std::ios::in | std::ios::out | std::ios::binary);
    
    //here do the format changes
    
    //here overwrite the data part 
    std::cout << chunk_info.find("data")->second.first+8 << std::endl;
    crushed_wav.seekp(chunk_info.find("data")->second.first+8, std::ios::beg);

    auto pos = crushed_wav.tellg();
    std::cout << "The file pointer is now at location " << pos << std::endl;
    //business logic for different cases
    
    if(crushed_samples.size() != chunk_info.find("data")->second.second){
    	 std::cout <<  "NOOOOOOOO" << std::endl;
    }
    else{

    crushed_wav.write(reinterpret_cast<const char*>(&crushed_samples[0]), crushed_samples.size()*sizeof(unsigned char));
    crushed_wav.close();

	}

	return 0;
};


// Wave_Crush::Wave_Crush(void) {};
// Wave_Crush::~Wave_Crush(void) {};


unsigned int Wave_Crush::get_overall_size(){
	return header.overall_size;
}             
unsigned int Wave_Crush::get_length_of_fmt(){
	return header.length_of_fmt;
}                
unsigned int Wave_Crush::get_format_type(){
	return header.format_type;
}                 
unsigned int Wave_Crush::get_channels(){
	return header.channels;
}                      
unsigned int Wave_Crush::get_sample_rate(){
	return header.sample_rate;
}                  
unsigned int Wave_Crush::get_byterate(){
	return header.byterate;
}                     
unsigned int Wave_Crush::get_block_align(){
	return header.block_align;
}                 
unsigned int Wave_Crush::get_bits_per_sample(){
	return header.bits_per_sample;
}              
unsigned int Wave_Crush::get_data_size(){
	return header.data_size;
} 
unsigned int Wave_Crush::get_junk_size(){
	return header.junk_size;
}

unsigned int Wave_Crush::get_all_samples_size(){
	return data_chunk.all_samples.size();
};                 
unsigned int Wave_Crush::get_left_channel_samples_size(){
	return data_chunk.left_channel_samples.size();
};              
unsigned int Wave_Crush::get_right_channel_samples_size(){
	return data_chunk.right_channel_samples.size();
};

std::vector<unsigned char> Wave_Crush::get_all_samples(){
	return data_chunk.all_samples;
};                 
std::vector<unsigned char> Wave_Crush::get_left_channel_samples(){
	return data_chunk.left_channel_samples;
};              
std::vector<unsigned char> Wave_Crush::get_right_channel_samples(){
	return data_chunk.right_channel_samples;
};




std::vector<unsigned char> Wave_Crush::crush(struct crush_parameters crush_parameters){

//first seperate channels
//idea not to seperate channels
//math operations
//fades
//lets continue :)

return data_chunk.all_samples;
};



std::string Wave_Crush::read_x_bytes_to_string(std::ifstream& file,int x){

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
std::vector<unsigned char> Wave_Crush::read_x_bytes_to_vector(std::ifstream& file, int x){

			std::vector<unsigned char> vec(x);
	        
	        while(true){
			    int LENGTH = x;
			    file.read((char*) &vec[0], LENGTH); 
			    if(file.gcount() != LENGTH){ continue; }else{ break; }
	        }

	        return vec;
}
void Wave_Crush::read_and_fill_x_bytes_to_vector(const std::vector<unsigned char>& target_vec,std::ifstream& file, int offset, int length){
	        
	        while(true){
			    file.read((char*) &target_vec[offset], length); 
			    if(file.gcount() != length){ continue; }else{ break; }
	        }

}


std::map< std::string,std::pair<int,unsigned int>>  Wave_Crush::collect_chunk_info(std::ifstream& file, std::streampos& file_size, std::vector<std::string>& chunk_types){

	// read the data:
    std::vector<unsigned char> read_bytes;
    int i{0};

    std::string riff_string( read_x_bytes_to_string(file,4) );
    if(riff_string=="RIFF"){
    	header.riff[0] = 'R';
    	header.riff[1] = 'I';
    	header.riff[2] = 'F';
    	header.riff[3] = 'F';
    }

    file.seekg(i+=4);
    read_bytes = read_x_bytes_to_vector(file,sizeof(read_bytes));
    header.overall_size = pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]);


    chunk_info.insert(std::make_pair(riff_string,std::make_pair(0,pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]))));

    insertation_order.push_back(riff_string);
       
    file.seekg(i+=4);
    
    std::string wave_string( read_x_bytes_to_string(file,4) );

    if(wave_string=="WAVE"){
    	header.wave[0] = 'W';
    	header.wave[1] = 'A';
    	header.wave[2] = 'W';
    	header.wave[3] = 'E';
    }
    
    chunk_info.insert(std::make_pair(wave_string,std::make_pair(8,0) ) );
    insertation_order.push_back(wave_string);
  
    
    file.seekg(i+=4);
    
    for(;i<file_size;){
	    
	    file.seekg(i);
        std::string query( read_x_bytes_to_string(file,4) );
       
	    if( std::find(chunk_types.begin(), chunk_types.end(), query) != chunk_types.end() ){
	    	
	    	file.seekg(i+=4);
	    	read_bytes = read_x_bytes_to_vector(file,sizeof(read_bytes));

		    unsigned int size = pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]);
		    
		    if(chunk_info.find(query) == chunk_info.end()){
			
			    chunk_info.insert(std::make_pair(query,std::make_pair(i-4,size) ) );
			    insertation_order.push_back(query);
		    
		    }
	        
	        i+=4;

		    i+=chunk_info[query].second;		    
	    	
	    }
	    else{

	    	i+=4;
	    }
    
    }

    return chunk_info;
};

int Wave_Crush::parse_fmt(std::ifstream& file){

	if(chunk_info["fmt "].second) {

		// std::cout << "hey!" << std::endl;
		std::vector<unsigned char> fmt;
		file.seekg(chunk_info["fmt "].first);
		fmt = read_x_bytes_to_vector(file,chunk_info["fmt "].second+8);
		
		for(unsigned int i{0}; i < sizeof(header.fmt_chunk_marker); i++){
	    header.fmt_chunk_marker[i] = fmt[i];
		}
		header.length_of_fmt = pack4chars_ltob(  fmt[4],fmt[5],fmt[6],fmt[7]   );
	    header.format_type = pack2chars_ltob(   fmt[8],fmt[9]  );
	    header.channels = pack2chars_ltob(   fmt[10],fmt[11]   );
	    header.sample_rate = pack4chars_ltob(   fmt[12],fmt[13],fmt[14],fmt[15] );
	    header.byterate = pack4chars_ltob(  fmt[16],fmt[17],fmt[18],fmt[19]   );
	    header.block_align = pack2chars_ltob(   fmt[20],fmt[21]  );
	    header.bits_per_sample = pack2chars_ltob( fmt[22],fmt[23]  );
	    if(chunk_info["fmt "].second > 16){
	        header.cbSize = pack2chars_ltob(   fmt[24],fmt[25]  );
	        if(chunk_info["fmt "].second > 18 && chunk_info["fmt "].second <= 40){
	        	header.wValidBitsPerSample = pack2chars_ltob(   fmt[26],fmt[27]  );
	        	header.dwChannelMask = pack4chars_ltob(   fmt[28],fmt[29],fmt[30],fmt[31]  );
	        	for(unsigned int i{0}; i < sizeof(header.fmt_chunk_marker); i++){
			    header.SubFormat[i] = fmt[i];
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

int Wave_Crush::read_samples(std::ifstream& file){

data_chunk.all_samples = read_x_bytes_to_vector(file,chunk_info.find("data")->second.second);


 //    //seperate channels for further modification
    if(header.channels == 2){
    auto i = data_chunk.all_samples.begin();
    
    unsigned int n{0};
	        while ( n < header.data_size){
		        	if(n % (header.bits_per_sample / 8) /* would give bytes per sample.*/ ){
		        	//left channel
	                data_chunk.left_channel_samples.push_back((*i));
		            }
		            else{
		            //right channel
		            data_chunk.right_channel_samples.push_back((*i));
		            }
		        	
		        	n++;
		        	++i;
	        	}
	}

return 0;
}



void Wave_Crush::parse_old(std::vector<unsigned char> file_in_memory){

 //  vector<unsigned char>::iterator i = file_in_memory.begin();

 //  unsigned char junk[4]{'J','U','N','K'};
 //  unsigned char data[4]{'d','a','t','a'};
 //  unsigned char bext[4]{'b','e','x','t'};
 //  unsigned char jcount{0};
 //  unsigned char dcount{0};
 //  unsigned char bcount{0};
  
 //  bool junk_found{false}; 
 //  bool bext_found{false};
 //  bool data_found{false};
 //  bool there_is_junk{false};
 //  bool there_is_bext{false};

 //    //locate junk and data
 //  	for (; i != file_in_memory.end(); i+=4){
         

 //         // std::cout << i - file_in_memory.begin() << std::endl;
  		 

 //  		 for(int j{0}; j < 4; j++){
            
 //            if( junk[j] == (*(i+j)) ){

 //            	jcount++;
 //            }
 //            else{
 //            	break; 
 //            } 
 //            if (jcount==4){
 //            	std::cout << std::distance(file_in_memory.begin(), i) << std::endl;
 //            	junk_found = true;
 //            	jcount=0;
 //            }
             	 
            
	// 	 }
	// 	 for(int k{0}; k < 4; k++){
            
 //            if( bext[k] == (*(i+k)) ){
 //            	bcount++;
 //            }
 //            else{
 //            	break; 
 //            } 
 //            if (bcount==4){

 //            	bext_found = true;
 //            	bcount=0;
 //            }

 //         }
 //         for(int k{0}; k < 4; k++){
            
 //            if( data[k] == (*(i+k)) ){
 //            	dcount++;
 //            }
 //            else{
 //            	break; 
 //            } 
 //            if (dcount==4){

 //            	data_found = true;
 //            	dcount=0;
 //            }

 //         }
 //        if(junk_found){
        
	//         junk_index = std::distance(file_in_memory.begin(), i);

	//         std::cout << "JUNK STARTS AT INDEX : " << junk_index << std::endl;
	        
	//         for(int h{0}; h < 4; h++){ header.JUNK[h] = (*(i + 3 - h));} //JUNK bytes are little endian 
	        
	//         i+=4;

	//         header.junk_size = pack4chars_ltob(   (*i),(*(i+1)),(*(i+2)),(*(i+3))   ); 
	//         real_junk_size = header.junk_size + 8;

	//         i+=4;
	        
	//         int l{0};
	//         while ( l < header.junk_size){
	//         	junk_data.push_back((*i)); //push junk data to vector 
	//         	l++;
	//         	++i;
	//         }
	

	//         junk_found = false;
	//         there_is_junk = true;
 //        }

 //        if(bext_found){
        
	//         bext_index = std::distance(file_in_memory.begin(), i);

	//         std::cout << "bext STARTS AT INDEX : " << junk_index << std::endl;
	        
	//         for(int h{0}; h < 4; h++){ header.bext_chunk_header[h] = (*(i + 3 - h));} //bext bytes are little endian 
	        
	//         i+=4;

	//         header.bext_size = pack4chars_ltob(   (*i),(*(i+1)),(*(i+2)),(*(i+3))   ); 
	//         real_bext_size = header.bext_size + 8;

	//         i+=4;
	        
	//         int l{0};
	//         while ( l < header.bext_size){
	//         	bext_data.push_back((*i)); //push junk data to vector 
	//         	l++;
	//         	++i;
	//         }
	

	//         bext_found = false;
	//         there_is_bext = true;
 //        }
        
 //        if(data_found){
	        

	//         data_index = std::distance(file_in_memory.begin(), i+8); 

	      
	//         if(there_is_junk){
	//         std::cout << std::endl;
	//         std::cout << "ERASING JUNK..." << std::endl;
	//         file_in_memory.erase(file_in_memory.begin() + junk_index,file_in_memory.begin() + junk_index + real_junk_size);
	//         }
	//         if(there_is_bext){
	//         std::cout << std::endl;
	//         std::cout << "ERASING JUNK..." << std::endl;
	//         file_in_memory.erase(file_in_memory.begin() + bext_index - real_junk_size,file_in_memory.begin() + bext_index + real_bext_size - real_junk_size);
	//         }

	//         std::cout << "DATA STARTS AT INDEX : " << data_index << std::endl;
	        
	//         break;
 //        }
  	
 //  	};
  	
  	

 //    i = file_in_memory.begin();
 //    for(int j{0}; j < 4; j++){ header.riff[j] = (*(i+j));}
	// i+=4;
	// header.overall_size = pack4chars_ltob(   (*i),(*(i+1)),(*(i+2)),(*(i+3))   );
	// i+=4;
 // 	for(int j{0}; j < 4; j++){ header.wave[j] = (*(i+j));}
	// i+=4;
	// for(int j{0}; j < 4; j++){ header.fmt_chunk_marker[j] = (*(i+j));}
	// i+=4;
	// header.length_of_fmt = pack4chars_ltob(   (*i),(*(i+1)),(*(i+2)),(*(i+3))   );
 //    i+=4;
 //    header.format_type = pack2chars_ltob(   (*i),(*(i+1))  );
 //    i+=2;
 //    header.channels = pack2chars_ltob(   (*i),(*(i+1))   );
 //    i+=2;
 //    header.sample_rate = pack4chars_ltob(   (*i),(*(i+1)),(*(i+2)),(*(i+3))   );
 //    i+=4;
 //    header.byterate = pack4chars_ltob(   (*i),(*(i+1)),(*(i+2)),(*(i+3))   );
 //    i+=4;
 //    header.block_align = pack2chars_ltob(   (*i),(*(i+1))  );
 //    i+=2;
 //    header.bits_per_sample = pack2chars_ltob(   (*i),(*(i+1))  );
 //    i+=2;
 //    for(int j{0}; j < 4; j++){ header.data_chunk_header[j] = (*(i+j));}
	// i+=4;
	// header.data_size = pack4chars_ltob(   (*i),(*(i+1)),(*(i+2)),(*(i+3))   );     
 //    i+=4;
    
 //    i+=header.data_size;

 //  //   if(std::distance(file_in_memory.begin(), i) < ( (header.overall_size + 8)  -  real_junk_size - real_bext_size) ) { //Cunku overall size nedense ilk 8 bytei saymiyormus
 //  //       leftover_index = std::distance(file_in_memory.begin(), i);
 //  //   	leftover_size  =  ( ( (header.overall_size + 8)  -  real_junk_size - real_bext_size ) - std::distance(file_in_memory.begin(), i) );
 //  //   	// collect the other data here after sound data finishes.
 //  //   	std::cout << "---FILE NOT ENDED AFTER DATA CHUNK---- :: " << leftover_size << " BYTES LEFT, pushing leftovers to a vector..." << std::endl;
	// 	// std::cout << std::endl;
	// 	// std::cout << std::endl;

	// 	// // bytes_after_sample_data.resize(leftover_size);
	//  //    int m{0};
	//  //        while ( m < leftover_size){
	//  //        	bytes_after_sample_data.push_back((*i)); //push leftover data to vector 
	//  //        	m++;
	//  //        	++i;
	        	
	//  //        }
	// 	// // std::cout << bytes_after_sample_data.size() << std::endl;
 //  //   }
 //  //   else{
	// 	// std::cout << "---FILE ENDED AFTER DATA CHUNK----" << std::endl;
	// 	// std::cout << std::endl;
	// 	// std::cout << std::endl;

 //  //   }
    
 //    // strip the sample data from the file and put it in a vector.
 //    int deleted_bytes_before_data = std::distance(file_in_memory.begin(),file_in_memory.begin() + (data_index - real_junk_size - real_bext_size) );
 //    file_in_memory.erase(file_in_memory.begin(),file_in_memory.begin() + (data_index - real_junk_size - real_bext_size) );
 //    if(leftover_size){
 //    file_in_memory.erase(file_in_memory.begin() + (leftover_index - deleted_bytes_before_data), file_in_memory.end() );
 //    }

 //    data_chunk.all_samples = file_in_memory;

 //    //seperate channels for further modification
 //    if(header.channels == 2){
 //    i = data_chunk.all_samples.begin();
    
 //    int n{0};
	//         while ( n < header.data_size){
	// 	        	if(n % (header.bits_per_sample / 8) /* would give bytes per sample.*/ ){
	// 	        	//left channel
	//                 data_chunk.left_channel_samples.push_back((*i));
	// 	            }
	// 	            else{
	// 	            //right channel
	// 	            data_chunk.right_channel_samples.push_back((*i));
	// 	            }
		        	
	// 	        	n++;
	// 	        	++i;
	//         	}
	// }


    

      
}
