   
#include "wav_crusher.hpp"

std::vector<unsigned char> Wav_Crusher::range_shuffler(std::vector<unsigned char> samples){
    


	#define PRM parameter_structs.rangeshuffler_params

	std::cout << PRM.mode<<std::endl;
	std::cout << PRM.op<<std::endl;
	std::cout << PRM.algorithm<<std::endl;
	std::cout << PRM.div<<std::endl;
	std::cout << PRM.mul<<std::endl;
	std::cout << PRM.ms<<std::endl;
    
    auto sr = parsed_data.get_sample_rate();
    
    int ms_to_samp = sr/1000;

    int offset;
    
    switch(PRM.mode){
	case 0 :
		if(PRM.op == "div"){
			defcase:
			offset = std::round(sr / PRM.div);
		}
		else if(PRM.op == "mul"){

			offset = std::round(sr * PRM.mul);

		}
		else{
			offset = std::round(sr / PRM.div);

		}	    	
	break;
	case 1 :
		offset = ms_to_samp * PRM.ms;
	break;
	default :  
		goto defcase;
	}
    std::cout <<std::endl;
    std::cout <<std::endl;
    std::cout << offset << std::endl;
    std::cout <<std::endl;
    std::cout <<std::endl;

    //Slice

    std::vector<std::vector<unsigned char>>sliced_samps;
    
    for(std::vector<unsigned char>::iterator it = samples.begin(); it < samples.end(); it+=offset) {
      if(it+offset < samples.end()){
      std::vector<unsigned char> vec(it,it+offset);

      sliced_samps.push_back(vec);
      }
      else{
      	std::vector<unsigned char> rem_vec(it,samples.end());
      	sliced_samps.push_back(rem_vec);

      }
      
    }
     //std::cout << a << std::endl;
    //Shuffle
    if(PRM.algorithm == "reverse"){
    	std::reverse( sliced_samps.begin(),sliced_samps.end() );
 

    }
    else if(PRM.algorithm == "random"){
    	std::random_shuffle( sliced_samps.begin(),sliced_samps.end() );
 

    }
    else if(PRM.algorithm == "swap"){
        for(std::vector<std::vector<unsigned char>>::iterator it = sliced_samps.begin(); it < sliced_samps.end(); it=it+2) { 
    		std::swap(*it,*(it+1)); 
    	}

	}
    else{
       //defaultcase
       std::reverse( sliced_samps.begin(),sliced_samps.end() );

    }

    
    //consolidate
    std::vector<unsigned char> s;
	size_t total_size{ 0 };
	
    for (auto const& items: sliced_samps){
	    total_size += items.size();
	}
	s.reserve(total_size);
	std::cout << total_size << "   " << samples.size() << std::endl;
	for (auto& items: sliced_samps){    
    	std::move(items.begin(), items.end(), std::back_inserter(s));
	}


    /*for(std::vector<unsigned char>::iterator it = samples.begin(); it != samples.end();) {
       

       if(it < samples.end()){
       std::random_shuffle(it+std::round(sample_rate/12),it+std::round(sample_rate/6));

       if(it < samples.end()){
       it+=std::round(sample_rate/6); 
       }
       }

    }*/


   	return s;

}