
#ifndef list_info_hpp
#define list_info_hpp

#include <iostream>
#include <string>

namespace list_info {

void list_processors(){

      std::cout << std::endl;
      std::cout << "	-p rangeshuffler" << std::endl;
      std::cout << std::endl;
      std::cout << "	rangeshuffler slices the data in the file by equal ranges(in samples or seconds)" << std::endl;
      std::cout << "	then shuffles those slices by the chosen algorithm." << std::endl;
      std::cout << std::endl;

}
void list_rangeshuffler(){

      std::cout << std::endl;
      std::cout << "	-p rangeshuffler" << std::endl;
      std::cout << std::endl;
      std::cout << "	rangeshuffler slices the data in the file by equal ranges(in samples or seconds)" << std::endl;
      std::cout << "	then shuffles those slices by the chosen algorithm." << std::endl;
      std::cout << std::endl;
      std::cout << "		-a reverse, random, swap" << std::endl;
      std::cout << "		(default: -a random)" << std::endl;
      std::cout << std::endl;
      std::cout << "		reverse, reverses the slices. random, randomly shuffles the slices." << std::endl;
      std::cout << "		swap, swaps a slice with another slice chosen by the (--so) argument and continues" << std::endl;
      std::cout << "		iterating over the slices in (--ss) steps until it finishes operating on all slices." << std::endl;
      std::cout << std::endl;
      std::cout << "			arguments" << std::endl;
      std::cout << "			(defaults: --mode 0, --div 1, --mul 1, --ms 1000, --so 1, --ss 1)" << std::endl;
      std::cout << std::endl;
      std::cout << "			--mode [0 or 1]" << std::endl;
      std::cout << "			--mode 0 lets you specify the size of range with multiplication or division of samplerate." << std::endl;
      std::cout << "			--mode 1 lets you specify the size of range directly with miliseconds." << std::endl;
	  std::cout << std::endl;
      std::cout << "			--mode 0" << std::endl;
      std::cout << "			--div or --mul followed by a non zero integer. The factor for multiplication or division." << std::endl;
      std::cout << "			(*only for swap algorithm) --so followed by a non zero integer. Choose the offset of the slice to swap." << std::endl;
      std::cout << "			(*only for swap algorithm) --ss followed by a non zero integer. Choose the offset to the next slice to be swapped." << std::endl;
      std::cout << std::endl;
      std::cout << "			--mode 1" << std::endl;
      std::cout << "			--ms followed by a non zero integer. Specify miliseconds for slice ranges." << std::endl;
      std::cout << "			(*only for swap algorithm) --so followed by a non zero integer. Choose the offset of the slice to swap." << std::endl;
      std::cout << "			(*only for swap algorithm) --ss followed by a non zero integer. Choose the offset to the next slice to be swapped." << std::endl;
      std::cout << std::endl;

}
void list(std::string what){
	if(what == "all") {
		list_rangeshuffler();

	}
	else if(what == "p"){
		list_processors();

	}
	else if(what == "rangeshuffler"){
		list_rangeshuffler();
	} 
	else{
		list_rangeshuffler();
	}

}

}

#endif /* list_info_hpp */
