#ifndef parameter_structs_hpp
#define parameter_structs_hpp

#include <string> 

struct Parameter_Structs {
	
	struct Rangeshuffler_Params {

	std::string algorithm = "reverse";
	int mode = 0; 
	int div = 1; 
	int mul = 1;
	std::string op = "div";
	int ms = 1000; 
	int so = 1;
	int ss = 1;


	} rangeshuffler_params;

};


#endif /* parameter_structs_hpp */

