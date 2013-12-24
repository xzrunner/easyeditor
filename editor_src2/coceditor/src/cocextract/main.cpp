#include <iostream>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		std::cerr << "Params: 1. src .lua file; 2. the filepath of export names' list" << std::endl;
		return 1;
	}

	std::string srcfilepath = argv[1];
	std::string listfilepath = argv[2];


}