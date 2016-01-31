#ifndef _EASYCOCO_UTILITY_H_
#define _EASYCOCO_UTILITY_H_



namespace libcoco
{

class Utility
{
public:
	static void GroupSpritesFromTag(const std::vector<ee::Sprite*>& src, 
		std::map<std::string, std::vector<ee::Sprite*> >& dst, std::vector<ee::Sprite*>& others);

}; // Utility

}

#endif // _EASYCOCO_UTILITY_H_