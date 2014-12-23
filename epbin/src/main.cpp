#include "epbin.h"

#include <assert.h>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	//assert(argc == 6);

	//std::string type = argv[4];
	//epbin::TextureType t;
	//if (type == "png4") {
	//	t = epbin::TT_PNG4;
	//} else if (type == "png8") {
	//	t = epbin::TT_PNG8;
	//} else if (type == "pvr") {
	//	t = epbin::TT_PVR;
	//} else if (type == "pkm") {
	//	t = epbin::TT_PKM;
	//} else {
	//	std::cout << "unknown type " << type << "\n";
	//	return 1;
	//}

	//epbin::BinaryEPP epp(argv[2], argv[3], t, argv[5]);
	//try {
	//	epp.Pack();			
	//} catch (epbin::Exception e) {
	//	std::cout << e.what() << "\n";
	//	return 1;
	//}

	/////////////////////

	assert(argc == 4);
	epbin::BinaryEPD epd(argv[2], argv[3]);
	try {
		epd.Pack(true);			
	} catch (epbin::Exception e) {
		std::cout << e.what() << "\n";
		return 1;
	}

	return 0;
}