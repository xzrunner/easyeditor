#pragma once

#include <string>
#include <drag2d.h>

struct lua_State;

namespace libcoco
{

class ParserLuaFile
{
public:
	~ParserLuaFile();

	void parser(const std::string& filename);

	void transToEasyFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder);
	void transToMemory(const std::vector<std::string>& texfilenames);

	void getAllSymbols(std::vector<d2d::ISymbol*>& symbols) const;

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

			void transform(d2d::ISprite* sprite) const;

			Item() {
				color = "0xffffffff";
				add = "0x0";
				memset(&mat[0], 0, sizeof(int) * 6);
				mat[0] = mat[3] = 1024;
			}
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
	void parserPic(lua_State* L, int id);
	void parserAni(lua_State* L, int id);

	void transPicToFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder);
	void transAniToFiles(const std::string& outfloder);
	void transAniToAnimationFile(const std::string& outfloder, int id, Animation* ani);
	void transAniToComplexFile(const std::string& outfloder, int id, Animation* ani);

	void transPicToMemory(const std::vector<std::string>& texfilenames);
	void transAniToMemory();
	void transAniToAnimationMemory(int id, Animation* ani);
	void transAniToComplexMemory(int id, Animation* ani);

private:
	std::map<int, Picture*> m_mapPictures;
	std::map<int, Animation*> m_mapAnims;

	std::map<int, d2d::ISymbol*> m_mapSymbols;

}; // ParserLuaFile

}