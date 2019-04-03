
#include <iostream>

#include "cxxopts.hpp"

#include "path_manager.hpp"
#include "parsed_data.hpp"
#include "wav_parser.hpp"
#include "wav_crusher.hpp"

#include "parameter_structs.hpp"
#include "list_info.hpp"

int main( int argc, char* argv[] ) {

  try
  {
    
    Parameter_Structs parameter_structs;
 
    cxxopts::Options options(argv[0], " - example, wavecrush -p rangeshuffler -a random --mode 0 --div 15 [input file path]");
    /*options
      .positional_help("[optional arguments]");
      .show_positional_help();*/

	    options.add_options()
	    ("help", "print help")
	    ("list", "detailed help about processors and usage of them. --list all, lists everything wavecrush has with detail. --list p lists processors, --list [name of processor] lists info about that processor.",cxxopts::value<std::string>())
	    ("i,input", "input file", cxxopts::value<std::string>())
	;
    
    options.add_options("operation")
	    ("p,processor", "choose processor", cxxopts::value<std::string>())
	    ("a,algorithm", "choose an algorithm available for that processor to run", cxxopts::value<std::string>(parameter_structs.rangeshuffler_params.algorithm))
    ;

    
    options.add_options("rangeshuffler parameters")    
	    ("mode", "choose mode", cxxopts::value<int>(parameter_structs.rangeshuffler_params.mode), "[0 or 1]")
	    //mode 0 
	    ("div", "division factor", cxxopts::value<int>(parameter_structs.rangeshuffler_params.div), "[non-zero]")
	    ("mul", "multiplication factor", cxxopts::value<int>(parameter_structs.rangeshuffler_params.mul), "[non-zero]")
	    //mode 1
	    ("ms", "milisecconds", cxxopts::value<int>(parameter_structs.rangeshuffler_params.ms), "[non-zero]")
	    ("so", "swap offset", cxxopts::value<int>(parameter_structs.rangeshuffler_params.so), "[non-zero]")
	    ("ss", "swap step", cxxopts::value<int>(parameter_structs.rangeshuffler_params.ss), "[non-zero]")
    ;

    options.parse_positional({"input"/*, "output", "positional"*/});
	auto result = options.parse(argc, argv);
  
    if (result.count("list"))
    {
	  list_info::list(result["list"].as<std::string>());
      exit(0);
    }
	if (result.count("help"))
    {
      std::cout << std::endl;  
      std::cout << "wavecrush operates on WAV files. It takes in a file, runs the algorithms which user choses"<< std::endl;
      std::cout << "on the \"data\" chunk of the file and writes the modified file to the folder which the program" << std::endl;
      std::cout << "is located." << std::endl;
      std::cout << std::endl;
      std::cout << options.help({"", "operation"}) << std::endl;
      exit(0);
    
    }

if (result.count("input"))
{
	if (result.count("processor") )
    {
		if ("rangeshuffler" == result["processor"].as<std::string>())
		{
			parameter_structs.rangeshuffler_params.algorithm = result["algorithm"].as<std::string>();
			if(parameter_structs.rangeshuffler_params.algorithm == "swap")	{
				if (result.count("so")){
					parameter_structs.rangeshuffler_params.div = result["so"].as<int>();
				}
				if (result.count("ss")){
					parameter_structs.rangeshuffler_params.div = result["ss"].as<int>();

				}
			}
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
				}
	  			else{

	  			}
			}
		}
		else{
		}	  
	}
	else{
		std::cout << "processor is missing in the arguments" << std::endl;
	    exit(1);
	}
}	
else{

	std::cout << "please enter a valid file path" << std::endl;
    exit(1);
}

       Path_Manager path_manager(result["input"].as<std::string>());

	   Parsed_Data parsed_data;  

	   Wav_Parser wav_parser(path_manager,parsed_data);

	   wav_parser.parse();
	   parsed_data.report();
	   
	   
	   Wav_Crusher wav_crusher(path_manager,parsed_data,parameter_structs); 
	   
	   auto crushed_samples = wav_crusher.range_shuffler(parsed_data.get_all_samples());
	   
	   wav_parser.write_file(crushed_samples);
       
       std::cout << std::endl;
       std::cout << "input file successfully crushed!" << std::endl;
       std::cout << std::endl;
	   exit(0);

  } catch (const cxxopts::OptionException& e)
  {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }

}



