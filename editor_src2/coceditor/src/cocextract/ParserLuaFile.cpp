#include "ParserLuaFile.h"
#include "ImageHelper.h"

#include <sstream>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace cocextract
{
	ParserLuaFile::~ParserLuaFile()
	{
		{
			std::map<int, Picture*>::iterator itr = m_mapPictures.begin();
			for ( ; itr != m_mapPictures.end(); ++itr)
				delete itr->second;
		}
		{
			std::map<int, Animation*>::iterator itr = m_mapAnims.begin();
			for ( ; itr != m_mapAnims.end(); ++itr)
				delete itr->second;
		}
	}

	void ParserLuaFile::parser(const std::string& filename)
	{
		lua_State *L = luaL_newstate();
		lua_settop(L,0);

		luaL_dofile(L, filename.c_str());

		if(lua_gettop(L) == 1 && lua_istable(L, 1))
		{
			int len = lua_rawlen(L, 1);
			for(int i=1;i <= len; i++)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, 1);

				if (lua_istable(L, -1))
				{
					lua_getfield(L, -1, "type");
					std::string type = lua_tostring(L, -1);
					lua_pop(L,1);

					lua_getfield(L, -1, "id");
					int id = lua_tointeger(L, -1);
					lua_pop(L,1);

					if (type == "picture")
						parserPic(L, id);
					else if (type == "animation")
						parserAni(L, id);
				}
				lua_pop(L,1);
			}
		}

		assert(lua_gettop(L) == 1);
	}

	void ParserLuaFile::transToEasyFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder)
	{
		// pictures
		std::vector<ImageHelper> images;
		images.resize(texfilenames.size());
		for (int i = 0, n = texfilenames.size(); i < n; ++i)
			images[i].loadPPM(texfilenames[i]);

		// clip parts
		std::map<int, Picture*>::iterator itr = m_mapPictures.begin();
		for ( ; itr != m_mapPictures.end(); ++itr)
		{
			Picture* pic = itr->second;
			for (int i = 0, n = pic->parts.size(); i < n; ++i)
			{
				Picture::Part* part = pic->parts[i];
				int xmin = INT_MAX, xmax = INT_MIN, ymin = INT_MAX, ymax = INT_MIN;
				for (int i = 0; i < 4; ++i)
				{
					if (part->src[i].x < xmin)
						xmin = part->src[i].x;
					else if (part->src[i].x > xmax)
						xmax = part->src[i].x;
					if (part->src[i].y < ymin)
						ymin = part->src[i].y;
					else if (part->src[i].y > ymax)
						ymax = part->src[i].y;
				}
				const unsigned char* pixels = images[part->tex].clip(xmin, xmax, ymin, ymax);
				if (pixels) 
				{
					std::stringstream ss;
					ss << xmin << xmax << ymin << ymax;
					std::string outfile = outfloder + "\\" + ss.str() + ".png";
					images[i].writeToFile(pixels, xmax-xmin, ymax-ymin, outfile);
				}
			}
		}
	}

	void ParserLuaFile::parserPic(lua_State* L, int id)
	{
		Picture* pic = new Picture;

		// parts
		int len = lua_rawlen(L, -1);
		pic->parts.reserve(len);
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			assert(lua_istable(L, -1));

			Picture::Part* part = new Picture::Part;

			// tex
			lua_getfield(L, -1, "tex");
			const char* type = lua_typename(L, lua_type(L, -1));
			part->tex = lua_tointeger(L, -1);
			lua_pop(L, 1);

			// src
			lua_getfield(L, -1, "src");
			int len = lua_rawlen(L, -1);
			assert(len == 8);
			float* src = &part->src[0].x;
			for (int i = 1; i <= len; ++i)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, -2);
				src[i-1] = lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);

			// screen
			lua_getfield(L, -1, "screen");
			len = lua_rawlen(L, -1);
			assert(len == 8);
			float* dst = &part->dst[0].x;
			for (int i = 1; i <= len; ++i)
			{
				lua_pushinteger(L, i);
				lua_gettable(L, -2);
				dst[i-1] = lua_tonumber(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);

			pic->parts.push_back(part);

			lua_pop(L, 1);
		}
		
		m_mapPictures.insert(std::make_pair(id, pic));
	}

	void ParserLuaFile::parserAni(lua_State* L, int id)
	{
		Animation* ani = new Animation;

		// export
		lua_getfield(L, -1, "export");
		if (lua_type(L, -1) != LUA_TNIL)
			ani->export_name = lua_tostring(L, -1);
		lua_pop(L, 1);

		// component
		lua_getfield(L, -1, "component");
		int len = lua_rawlen(L, -1);
		ani->component.reserve(len);
		for (int i = 1; i <= len; ++i)
		{
			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			lua_getfield(L, -1, "id");
			ani->component.push_back(lua_tonumber(L, -1));
			lua_pop(L, 2);
		}
		lua_pop(L, 1);

		// frames
		len = lua_rawlen(L, -1);
		assert(len == 1);
		lua_pushinteger(L, 1);
		lua_gettable(L, -2);
		len = lua_rawlen(L, -1);
		ani->frames.reserve(len);
		for (int i = 1; i <= len; ++i)
		{
			std::vector<Animation::Item*> items;

			lua_pushinteger(L, i);
			lua_gettable(L, -2);
			assert(lua_istable(L, -1));
			int len = lua_rawlen(L, -1);
			items.reserve(len);
			for (int i = 1; i <= len; ++i)
			{
				Animation::Item* item = new Animation::Item;

				lua_pushinteger(L, i);
				lua_gettable(L, -2);
				if (lua_isnumber(L, -1))
				{
					item->index = lua_tointeger(L, -1);
					item->is_full = false;
				}
				else if (lua_istable(L, -1))
				{
					lua_getfield(L, -1, "index");
					item->index = lua_tointeger(L, -1);
					lua_pop(L, 1);

					lua_getfield(L, -1, "color");
					if (lua_type(L, -1) != LUA_TNIL)
						item->color = lua_tostring(L, -1);
					lua_pop(L, 1);

					lua_getfield(L, -1, "add");
					if (lua_type(L, -1) != LUA_TNIL)
						item->add = lua_tostring(L, -1);
					lua_pop(L, 1);

					lua_getfield(L, -1, "mat");
					if (lua_type(L, -1) != LUA_TNIL)
					{
						int len = lua_rawlen(L, -1);
						assert(len == 6);
						for (int i = 1; i <= len; ++i)
						{
							lua_pushinteger(L, i);
							lua_gettable(L, -2);
							item->mat[i-1] = lua_tointeger(L, -1);
							lua_pop(L, 1);
						}
					}
					lua_pop(L, 1);

					item->is_full = true;
				}
				else
				{
					assert(0);
				}
				lua_pop(L, 1);

				items.push_back(item);
			}
			lua_pop(L, 1);

			ani->frames.push_back(items);
		}
		lua_pop(L, 1);

		m_mapAnims.insert(std::make_pair(id, ani));
	}
}