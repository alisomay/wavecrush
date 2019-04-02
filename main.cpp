//
//  main.cpp
//  Created by Ali Somay. 
// 

#include <iostream>

#include "cxxopts.hpp"

#include "path_manager.hpp"
#include "parsed_data.hpp"
#include "wav_parser.hpp"
#include "wav_crusher.hpp"

#include "parameter_structs.hpp"

int main( int argc, char* argv[] ) {

  try
  {
    
    Parameter_Structs parameter_structs;
 
    cxxopts::Options options(argv[0], " - example, wavecrush --rangeshuffler -sr -d 2 --reverse [file path] ");
    options
      .positional_help("[optional arguments]")
      .show_positional_help();

    options.add_options()
      ("help", "Print help")
      ("i,input", "Input", cxxopts::value<std::string>())
	;
    
    options.add_options("processor")
    ("p,processor", "choose processor", cxxopts::value<std::string>());

    options.add_options("algorithm type")
    ("a,algorithm", "choose algorithm", cxxopts::value<std::string>(parameter_structs.rangeshuffler_params.algorithm));
	
	options.add_options("algorithm")    
    ("mode", "choose mode", cxxopts::value<int>(parameter_structs.rangeshuffler_params.mode), "[0 or 1]")
    ("div", "For specifiying values", cxxopts::value<int>(parameter_structs.rangeshuffler_params.div), "[non-zero]")
    ("mul", "For specifiying values", cxxopts::value<int>(parameter_structs.rangeshuffler_params.mul), "[non-zero]")
    ("ms", "For specifiying values", cxxopts::value<int>(parameter_structs.rangeshuffler_params.ms), "[non-zero]");

    options.parse_positional({"input", "output", "positional"});

    auto result = options.parse(argc, argv);
  

    if (result.count("help"))
    {
      std::cout << options.help({"", "processor","algorithm type", "algorithm"}) << std::endl;
      exit(0);
    }

if (result.count("input"))
{
	std::cout << "Input = " << result["input"].as<std::string>()
    << std::endl;
	
	if (result.count("processor") )
    {
		if ("rangeshuffler" == result["processor"].as<std::string>())
		{
			parameter_structs.rangeshuffler_params.algorithm = result["algorithm"].as<std::string>();
			std::cout << "algorithm = " << result["algorithm"].as<std::string>()
        	<< std::endl;
				
			if (result.count("mode")){

				parameter_structs.rangeshuffler_params.mode = result["mode"].as<int>();
				
				if (parameter_structs.rangeshuffler_params.mode  ==  0){
	
					if (result.count("div")){
				    parameter_structs.rangeshuffler_params.op = "div";
					parameter_structs.rangeshuffler_params.div = result["div"].as<int>();
					}
					else if(result.count("mul")){

				    parameter_structs.rangeshuffler_params.op = "mul";
					parameter_structs.rangeshuffler_params.mul = result["mul"].as<int>();
					}
					else{
					}
				}
	  		    else if (parameter_structs.rangeshuffler_params.mode  == 1){
	  		    	
	  		    	if (result.count("ms")){
					parameter_structs.rangeshuffler_params.div = result["ms"].as<int>();
					}
					else{
					}
				}
	  			else{

	  			}
			}
			else{

			}
		}
		else{
		}	  
	}
	else{
	}
}	
else{

	std::cout << "enter a file path" << std::endl;
    exit(1);
}



       std::cout << "Arguments remain = " << argc << std::endl;

       Path_Manager path_manager(result["input"].as<std::string>());

	   Parsed_Data parsed_data;  

	   Wav_Parser wav_parser(path_manager,parsed_data);

	   wav_parser.parse();
	   parsed_data.report();
	   
	   
	   Wav_Crusher wav_crusher(path_manager,parsed_data,parameter_structs); 
	   
	   auto crushed_samples = wav_crusher.range_shuffler(parsed_data.get_all_samples());
	   
	   wav_parser.write_file(crushed_samples);

	   exit(0);

  } catch (const cxxopts::OptionException& e)
  {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

}



