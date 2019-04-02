//
//  main.cpp
//  Created by Ali Somay. 
// 

#include "path_manager.hpp"
#include "parsed_data.hpp"
#include "wav_parser.hpp"
#include "wav_crusher.hpp"

int main( int argc, char* argv[] ) {
   
   if( argc < 2 ){
		throw std::invalid_argument( "Please enter a file path." );
   }

   Path_Manager path_manager(argv[1]);
   Parsed_Data parsed_data;  

   
   Wav_Parser wav_parser(path_manager,parsed_data);

   wav_parser.parse();
   parsed_data.report();
   
   
   Wav_Crusher wav_crusher(path_manager,parsed_data); 
   
   auto crushed_samples = wav_crusher.range_shuffler(parsed_data.get_all_samples());
   
   wav_parser.write_file(crushed_samples);

   return 0;

}



