#include "Utility.h"

namespace libcoco
{

void Utility::GroupSpritesFromTag(const std::vector<d2d::Sprite*>& src, 
								  std::map<std::string, std::vector<d2d::Sprite*> >& dst,
								  std::vector<d2d::Sprite*>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		d2d::Sprite* sprite = src[i];
		if (sprite->tag.empty())
		{
			others.push_back(sprite);
		}
		else
		{
			std::vector<std::string> tags;
			d2d::StringHelper::Split(sprite->tag, ";", tags);
			bool is_action = false;
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				if (tags[i].find("=") != std::string::npos) {
					continue;
				}

				is_action = true;

				std::map<std::string, std::vector<d2d::Sprite*> >::iterator itr = 
					dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<d2d::Sprite*> sprites;
					sprites.push_back(sprite);
					dst.insert(std::make_pair(tags[i], sprites));
				}
				else
				{
					itr->second.push_back(sprite);
				}
			}

			if (!is_action) {
				others.push_back(sprite);
			}
		}
	}
}

}