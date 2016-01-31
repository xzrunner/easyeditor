#ifndef _LIBRESPACKER_UNPACKE_FROM_LUA_H_
#define _LIBRESPACKER_UNPACKE_FROM_LUA_H_



namespace librespacker
{

class UnpackFromLua
{
public:
	static void Unpack(const std::string& filepath, 
		const std::vector<ee::Image*>& images);

}; // UnpackFromLua

}

#endif // _LIBRESPACKER_UNPACKE_FROM_LUA_H_