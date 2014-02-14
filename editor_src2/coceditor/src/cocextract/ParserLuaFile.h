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

		void transToEasyFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder);

	private:
		void parserPic(lua_State* L, int id);
		void parserAni(lua_State* L, int id);

		void transPicFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder);
		void transAniFiles(const std::string& outfloder);

	private:
		struct Picture
		{
			struct Part
			{
				int tex;
				d2d::Vector src[4];
				d2d::Vector dst[4];

				int xmin, xmax, ymin, ymax;
				std::string filename;

				void init();

				void transform(d2d::ISprite* sprite) const;

			private:
				std::string dstMode() const;
				// 0 1
				// 3 2
				static int nodeMode(const d2d::Vector& center, const d2d::Vector& node);
				static int findInMode(int mode[4], int query);
			};

			std::vector<Part*> parts;

			std::string filename;
			int width, height;

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

			std::string filename;

			~Animation()
			{
				for (int i = 0, n = frames.size(); i < n; ++i)
					for_each(frames[i].begin(), frames[i].end(), DeletePointerFunctor<Item>());
			}
		};

	private:
		std::map<int, Picture*> m_mapPictures;
		std::map<int, Animation*> m_mapAnims;

//		std::map<std::string, Picture::Part*> m_mapPart;

	}; // ParserLuaFile
}