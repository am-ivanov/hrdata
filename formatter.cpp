
#include "hrdata/hrdata.h"

#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " input.hrdata" << std::endl;
		exit(-1);
	}
	try 
	{
		hrdata::Node n = hrdata::parseFile(argv[1]);
		n.emit(std::cout);
	} 
	catch (const hrdata::Exception& exc) {
		std::cerr << exc.what() << std::endl;
		exit(-1);
	}
	return 0;
}
