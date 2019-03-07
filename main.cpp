//
//  main.cpp
//  Created by Ali Somay. 
// 

#include <iostream>


#include <string>
#include <cstring>
#include <vector>

#include "wavecrush.hpp"

using namespace std;


int main( int argc, char* argv[] ) {

   Wave_Crush wc; 
   // std::vector<unsigned char> file_in_memory = wc.read_file(argv[1]);
   // you can organise the arguments array later.
   
   wc.parse(argv[1]);
   // wc.parse(file_in_memory);
   
   
   cout << "FILE SIZE : " << wc.get_overall_size() << " BYTES. ( +8 to include \"RIFF\" and size bytes in the size. )" <<  endl;              
   cout << "LENGTH OF FMT : " << wc.get_length_of_fmt() << " BYTES. ( +8 to include \"fmt \" and size bytes in the size. )" <<  endl;                              
   cout << "ENCODING : " << wc.get_format_type() << endl;                   
   cout << "CHANNELS : " <<  wc.get_channels() << endl;                        
   cout << "SAMPLE RATE (blocks per second) : " <<  wc.get_sample_rate() << endl;                    
   cout << "BYTE RATE (SampleRate * NumChannels * BitsPerSample/8) : " <<  wc.get_byterate() << endl;                       
   cout << "BLOCK ALIGN (NumChannels * BitsPerSample/8) : " << wc.get_block_align() << endl;                   
   cout << "BITS PER SAMPLE : " << wc.get_bits_per_sample() << " BITS." << endl;                
   // cout << "SIZE OF THE JUNK CHUNK : " << wc.get_junk_size() << " BYTES. ( +8 to include \"JUNK\" and size bytes in the size. )" << endl;
   cout << "SIZE OF THE DATA CHUNK : " << wc.get_data_size() << " BYTES. ( +8 to include \"data\" and size bytes in the size. )" << endl;                       
   cout << endl;
   // cout << "--INFORMATION ABOUT STRIPPED DATA CHUNK--" << endl;
   // cout << endl;
   // cout << "BYTESIZE OF ALL SAMPLES : " << wc.get_all_samples_size() << " BYTES." << endl;
   // cout << "BYTESIZE OF LEFT CHANNEL SAMPLES : " << wc.get_left_channel_samples_size() << " BYTES." << endl;
   // cout << "BYTESIZE OF RIGHT CHANNEL SAMPLES : " << wc.get_right_channel_samples_size() << " BYTES." << endl;

   // // wc.crush(file_in_memory);

   //  wc.write_file("/Users/vallahiboyle/Desktop/wavecrush/invtry.wav", wc.get_all_samples());
  
    return 0;

}



