#ifndef _LIBRESPACKER_RES_UNPACKER_H_
#define _LIBRESPACKER_RES_UNPACKER_H_



namespace librespacker
{

class ResUnpacker
{
public:

	void UnpackLua(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

	void UnpackBin(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

}; // ResUnpacker

}

#endif // _LIBRESPACKER_RES_UNPACKER_H_