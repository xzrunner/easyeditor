#ifndef _COCO_EPE_PARSER_LUA_FILE_H_
#define _COCO_EPE_PARSER_LUA_FILE_H_

#include <ee/std_functor.h>
#include <ee/Symbol.h>
 
#include <string>
#include <vector>
#include <algorithm>

struct lua_State;

namespace ecoco
{
namespace epe
{

class ParserLuaFile
{
public:
	~ParserLuaFile();

	void parser(const std::string& filename);

	void transToEasyFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder);
	void transToMemory(const std::vector<std::string>& texfilenames);

	void getAllSymbols(std::vector<ee::Symbol*>& symbols) const;

private:
	struct Picture
	{
		struct Part
		{
			int tex;
			sm::vec2 src[4];
			sm::vec2 dst[4];

			int xmin, xmax, ymin, ymax;
			std::string filename;

			void init();

			void transform(ee::Sprite* sprite) const;

		private:
			std::string dstMode(const sm::vec2 dst[4]) const;
			// 0 1
			// 3 2
			static int nodeMode(const sm::vec2& center, const sm::vec2& node);
			static int findInMode(int mode[4], int query);
		};

		std::vector<Part*> parts;

		std::string filename;
		int width, height;

		~Picture()
		{
			for_each(parts.begin(), parts.end(), ee::DeletePointerFunctor<Part>());
		}
	};

	struct Animation
	{
		struct Item
		{
			int index;
			unsigned int color, add;
			int mat[6];
			bool is_full;

			void transform(ee::Sprite* sprite) const;

			Item() {
				color = 0xffffffff;
				add = 0;
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
				for_each(frames[i].begin(), frames[i].end(), ee::DeletePointerFunctor<Item>());
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

	std::map<int, ee::Symbol*> m_mapSymbols;

}; // ParserLuaFile

}
}

#endif // _COCO_EPE_PARSER_LUA_FILE_H_