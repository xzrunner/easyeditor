#pragma once

#include <string>
#include <drag2d.h>

struct lua_State;

namespace cocextract
{
	class ParserLuaFile
	{
	public:
		~ParserLuaFile();

		void parser(const std::string& filename);

	private:
		void parserPic(lua_State* L, int id);
		void parserAni(lua_State* L, int id);

	private:
		struct Picture
		{
			struct Part
			{
				int tex;
				d2d::Vector src[4];
				d2d::Vector dst[4];
			};

			std::vector<Part*> parts;

			~Picture()
			{
				for_each(parts.begin(), parts.end(), DeletePointerFunctor<Part>());
			}
		};

		struct Animation
		{
			struct Item
			{
				int index;
				std::string color, add;
				int mat[6];
				bool is_full;
			};

			std::string export_name;
			std::vector<int> component;
			std::vector<std::vector<Item*> > frames;

			~Animation()
			{
				for (int i = 0, n = frames.size(); i < n; ++i)
					for_each(frames[i].begin(), frames[i].end(), DeletePointerFunctor<Item>());
			}
		};

	private:
		std::map<int, Picture*> m_mapPictures;
		std::map<int, Animation*> m_mapAnims;

	}; // ParserLuaFile
}