#include "Utility.h"

#include <ee/Sprite.h>
#include <ee/StringHelper.h>

#include <sprite2/S2_Sprite.h>

namespace ecoco
{

void Utility::GroupSpritesFromTag(const std::vector<s2::Sprite*>& src, 
								  std::map<std::string, std::vector<ee::Sprite*> >& dst,
								  std::vector<ee::Sprite*>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		ee::Sprite* sprite = static_cast<ee::Sprite*>(src[i]->GetUD());
		if (sprite->GetTag().empty())
		{
			others.push_back(sprite);
		}
		else
		{
			std::vector<std::string> tags;
			ee::StringHelper::Split(sprite->GetTag(), ";", tags);
			bool is_action = false;
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				if (tags[i].find("=") != std::string::npos) {
					continue;
				}

				is_action = true;

				std::map<std::string, std::vector<ee::Sprite*> >::iterator itr = 
					dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<ee::Sprite*> sprites;
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