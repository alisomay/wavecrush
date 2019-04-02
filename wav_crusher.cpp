   
#include "wav_crusher.hpp"

std::vector<unsigned char> Wav_Crusher::range_shuffler(std::vector<unsigned char> samples){
    
    auto sr = parsed_data.get_sample_rate();
    auto offset = std::round(sr/2);
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
    std::reverse( sliced_samps.begin(),sliced_samps.end() );
    
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