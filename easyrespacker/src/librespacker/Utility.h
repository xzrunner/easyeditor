#ifndef _LIBRESPACKER_UTILITY_NEW_H_
#define _LIBRESPACKER_UTILITY_NEW_H_



namespace librespacker
{

class Utility
{
public:
	static bool IsAnchor(const ee::Sprite* spr);

	static bool IsNameValid(const std::string& name);

}; // Utility

}

#endif // _LIBRESPACKER_UTILITY_NEW_H_