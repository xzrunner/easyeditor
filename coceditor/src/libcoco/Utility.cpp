#include "Utility.h"

namespace libcoco
{

void Utility::GroupSpritesFromTag(const std::vector<d2d::ISprite*>& src, 
								  std::map<std::string, std::vector<d2d::ISprite*> >& dst,
								  std::vector<d2d::ISprite*>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = src[i];
		if (sprite->tag.empty())
			others.push_back(sprite);
		else
		{
			std::vector<std::string> tags;
			d2d::StringTools::Split(sprite->tag, ";", tags);
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				std::map<std::string, std::vector<d2d::ISprite*> >::iterator itr = 
					dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<d2d::ISprite*> sprites;
					sprites.push_back(sprite);
					dst.insert(std::make_pair(tags[i], sprites));
				}
				else
				{
					itr->second.push_back(sprite);
				}
			}
		}
	}
}

}