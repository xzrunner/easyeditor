#ifndef _LIBRESPACKER_UNPACKE_FROM_BIN_H_
#define _LIBRESPACKER_UNPACKE_FROM_BIN_H_



namespace librespacker
{

class UnpackFromBin
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

private:
	static void Unpack(uint8_t** ptr, const std::vector<ee::Image*>& images);

}; // UnpackFromBin

}

#endif // _LIBRESPACKER_UNPACKE_FROM_BIN_H_