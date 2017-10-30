#include "Utility.h"

#include <ee/Sprite.h>
#include <ee/StringHelper.h>

#include <sprite2/Sprite.h>

namespace ecoco
{

void Utility::GroupSpritesFromTag(const CU_VEC<s2::SprPtr>& src, 
								  std::map<std::string, std::vector<ee::SprPtr> >& dst,
								  std::vector<ee::SprPtr>& others)
{
	for (int i = 0, n = src.size(); i < n; ++i)
	{
		auto& spr = std::dynamic_pointer_cast<ee::Sprite>(src[i]);
		if (spr->GetTag().empty())
		{
			others.push_back(spr);
		}
		else
		{
			std::vector<std::string> tags;
			ee::StringHelper::Split(spr->GetTag(), ";", tags);
			bool is_action = false;
			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				if (tags[i].find("=") != std::string::npos) {
					continue;
				}

				is_action = true;

				std::map<std::string, std::vector<ee::SprPtr> >::iterator itr = 
					dst.find(tags[i]);
				if (itr == dst.end())
				{
					std::vector<ee::SprPtr> sprs;
					sprs.push_back(spr);
					dst.insert(std::make_pair(tags[i], sprs));
				}
				else
				{
					itr->second.push_back(spr);
				}
			}

			if (!is_action) {
				others.push_back(spr);
			}
		}
	}
}

}