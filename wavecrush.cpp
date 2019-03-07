#include "wavecrush.hpp"

#include <fstream>
#include <algorithm>

using namespace std;


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




std::vector<unsigned char> Wave_Crush::read_file(const char* file_path) {
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

bool Wave_Crush::write_file(const char* file_path, std::vector<unsigned char> data_chunk){

	//reconstruct file from the bits that you have restored
	//and put the new data chunk instead of the old one
	//if the size of the datachunk change, change the info in the file 
	//accordingly
	   
	   std::vector<unsigned char> chars_to_write;
       std::vector<unsigned char> out_vec;

           for(int i : header.riff){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_4bytes(header.overall_size)){
		    out_vec.push_back(i);	
		   }
		   for(int i : header.wave){
		    out_vec.push_back(i);	
		   }
		   for(int i : header.fmt_chunk_marker){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_4bytes(header.length_of_fmt) ){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_2bytes(header.format_type) ){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_2bytes(header.channels) ){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_4bytes(header.sample_rate) ){
		    out_vec.push_back(i);	
		   }
           for(int i : unpack_unsigned_int_to_chars_btol_4bytes( (header.sample_rate*header.channels*header.bits_per_sample/8)) ){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_2bytes( (header.channels*header.bits_per_sample/8)) ){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_2bytes(header.bits_per_sample) ){
		    out_vec.push_back(i);	
		   }
           for(int i : header.data_chunk_header){
		    out_vec.push_back(i);	
		   }
		   for(int i : unpack_unsigned_int_to_chars_btol_4bytes(header.data_size) ){
		    out_vec.push_back(i);	
		   }
		   for ( int i{0}; i < header.data_size; i++){
			  out_vec.push_back(data_chunk[i]);
		   }


      if(header.junk_size){
      	    
      	    
      	    for(int i : header.JUNK){
      	    out_vec.insert(out_vec.begin() + junk_index, i );
      		}
      		
      		for(int i : unpack_unsigned_int_to_chars_btol_4bytes(header.junk_size) ){
		    out_vec.insert(out_vec.begin() + junk_index + 4, i );
		    }		    
		  	
		  	for ( int i{0}; i < header.junk_size; i++){
			    
		    out_vec.insert(out_vec.begin() + junk_index + 8 + i, junk_data[i]);

		  	}
		   


      }
      if(header.bext_size){
      	    
      	    
      	    for(int i : header.bext_chunk_header){
      	    out_vec.insert(out_vec.begin() + bext_index, i );
      		}
      		
      		for(int i : unpack_unsigned_int_to_chars_btol_4bytes(header.bext_size) ){
		    out_vec.insert(out_vec.begin() + bext_index + 4, i );
		    }		    
		  	
		  	for ( int i{0}; i < header.bext_size; i++){
			    
		    out_vec.insert(out_vec.begin() + bext_index + 8 + i, bext_data[i]);

		  	}
		   


      }

      if(leftover_size){

      	    for(int i : bytes_after_sample_data){
			    out_vec.push_back(i);
			}
	  }
		   




    std::ios_base::sync_with_stdio(false);
    auto crushed_wav = std::fstream(file_path, std::ios::out | std::ios::binary);
    crushed_wav.write(reinterpret_cast<const char*>(&out_vec[0]), out_vec.size()*sizeof(unsigned char));
    crushed_wav.close();
  
    

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

std::vector<unsigned char> Wave_Crush::algo1(){

	std::vector<unsigned char> samps = data_chunk.all_samples;
    //test
    for(int i : samps){
    	samps[i] = samps[i/2];
    }

	return samps;
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
char* Wave_Crush::read_x_bytes_to_char_array(std::ifstream& file, int x){

			char arr[x];
	        
	        while(true){
			    int LENGTH = x;
			    file.read(&arr[0], LENGTH); 
			    if(file.gcount() != LENGTH){ continue; }else{ break; }
	        }

	        return arr;
}

std::map< std::string,std::pair<int,unsigned int>>  Wave_Crush::collect_chunk_info(std::ifstream& file, std::streampos& file_size, std::vector<std::string>& chunk_types){

	// read the data:
    char* read_bytes;
    int i{0};
    
    std::string riff_string( read_x_bytes_to_string(file,4) );
    if(riff_string=="RIFF"){
    	header.riff[0] = 'R';
    	header.riff[1] = 'I';
    	header.riff[2] = 'F';
    	header.riff[3] = 'F';
    }

    file.seekg(i+=4);
    read_bytes = read_x_bytes_to_char_array(file,sizeof(read_bytes));
    
    header.overall_size = pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]);

    chunk_info.insert(std::make_pair(riff_string,std::make_pair(0,pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]))));
    
    file.seekg(i+=4);
    
    std::string wave_string( read_x_bytes_to_string(file,4) );

    if(wave_string=="WAVE"){
    	header.wave[0] = 'W';
    	header.wave[1] = 'A';
    	header.wave[2] = 'W';
    	header.wave[3] = 'E';
    }
    
    chunk_info.insert(std::make_pair(wave_string,std::make_pair(8,0) ) );

    
    file.seekg(i+=4);
    
    for(;i<file_size;){
	    
	    file.seekg(i);
        std::string query( read_x_bytes_to_string(file,4) );

	    if( std::find(chunk_types.begin(), chunk_types.end(), query) != chunk_types.end() ){
	    	
	    	file.seekg(i+=4);
	    	read_bytes = read_x_bytes_to_char_array(file,sizeof(read_bytes));

		    unsigned int size = pack4chars_ltob(read_bytes[0],read_bytes[1],read_bytes[2],read_bytes[3]);
		    chunk_info.insert(std::make_pair(query,std::make_pair(i-4,size) ) );
	        
	        i+=4;

		    i+=chunk_info[query].second;		    
	    	
	    }
	    else{

	    	i+=4;
	    }
    
    }
    return chunk_info;
};

bool Wave_Crush::parse_fmt(std::ifstream& file,std::map< std::string,std::pair<int,unsigned int>> chunk_info ){

	if(chunk_info["fmt "].second) {

		cout << "hey!" <<endl;
		char* fmt;
		file.seekg(chunk_info["fmt "].first);
		fmt = read_x_bytes_to_char_array(file,chunk_info["fmt "].second+8);
		
		for(int i{0}; i < sizeof(header.fmt_chunk_marker); i++){
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
	        	for(int i{0}; i < sizeof(header.fmt_chunk_marker); i++){
			    header.SubFormat[i] = fmt[i];
				}
	        }
	        else{
	        	return false;
	        }
	    }


	}
	else{
		return false;
	}

}


void Wave_Crush::parse(const char* file_path){

    // open the file:
    std::streampos file_size;
    std::ifstream file(file_path, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    auto chunk_info = collect_chunk_info(file,file_size,chunk_types);
    parse_fmt(file,chunk_info);

    

    cout << endl;
	
	for(auto elem : chunk_info)
	{
   		std::cout << elem.first << " " << elem.second.first << " " << elem.second.second << " " << endl;
	}

    cout << endl;
	cout << "-END-" << endl;
	cout << endl;

	


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
         

 //         // cout << i - file_in_memory.begin() << endl;
  		 

 //  		 for(int j{0}; j < 4; j++){
            
 //            if( junk[j] == (*(i+j)) ){

 //            	jcount++;
 //            }
 //            else{
 //            	break; 
 //            } 
 //            if (jcount==4){
 //            	cout << std::distance(file_in_memory.begin(), i) << endl;
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

	//         cout << "JUNK STARTS AT INDEX : " << junk_index << endl;
	        
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

	//         cout << "bext STARTS AT INDEX : " << junk_index << endl;
	        
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
	//         cout << endl;
	//         cout << "ERASING JUNK..." << endl;
	//         file_in_memory.erase(file_in_memory.begin() + junk_index,file_in_memory.begin() + junk_index + real_junk_size);
	//         }
	//         if(there_is_bext){
	//         cout << endl;
	//         cout << "ERASING JUNK..." << endl;
	//         file_in_memory.erase(file_in_memory.begin() + bext_index - real_junk_size,file_in_memory.begin() + bext_index + real_bext_size - real_junk_size);
	//         }

	//         cout << "DATA STARTS AT INDEX : " << data_index << endl;
	        
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
 //  //   	cout << "---FILE NOT ENDED AFTER DATA CHUNK---- :: " << leftover_size << " BYTES LEFT, pushing leftovers to a vector..." << endl;
	// 	// cout << endl;
	// 	// cout << endl;

	// 	// // bytes_after_sample_data.resize(leftover_size);
	//  //    int m{0};
	//  //        while ( m < leftover_size){
	//  //        	bytes_after_sample_data.push_back((*i)); //push leftover data to vector 
	//  //        	m++;
	//  //        	++i;
	        	
	//  //        }
	// 	// // cout << bytes_after_sample_data.size() << endl;
 //  //   }
 //  //   else{
	// 	// cout << "---FILE ENDED AFTER DATA CHUNK----" << endl;
	// 	// cout << endl;
	// 	// cout << endl;

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
