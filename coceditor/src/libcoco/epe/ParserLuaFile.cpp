#include "ParserLuaFile.h"

#include <ee/Image.h>
#include <ee/ImageClip.h>
#include <ee/ImageSaver.h>
#include <ee/NullSprite.h>
#include <ee/NullSymbol.h>
#include <ee/FileType.h>
#include <ee/ImageSymbol.h>
#include <ee/ImageSprite.h>
#include <ee/SpriteFactory.h>
#include <ee/trans_color.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyimage.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

#include <sstream>

namespace ecoco
{
namespace epe
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
	luaL_openlibs(L);
	lua_settop(L,0);

	luaL_dofile(L, filename.c_str());
	// for old format
	if (lua_isstring(L, -1))
	{
		lua_pop(L, 1);

		const char* script = 
			"local all = {} \n"
			"local function picture(tbl) \n"
			"  tbl[\"type\"] = \"picture\" \n"
			"  table.insert(all, tbl) \n"
			"end \n"
			"local function animation(tbl) \n"
			"  tbl[\"type\"] = \"animation\" \n"
			"  table.insert(all, tbl) \n"
			"end \n"
			"local function texture(n) \n"
			"end \n"
			"local _env = _ENV \n"
			"_env.picture = picture \n"
			"_env.animation = animation \n"
			"_env.texture = texture \n"
			"function foo(filename) \n"
			"  f = assert(loadfile(filename , \"t\", _env)) \n"
			"  f() \n"
			"  return all \n"
			"end \n"
			;

		int err = luaL_dostring(L, script);
		if (err) {
			const char *msg = lua_tostring(L,-1);
			std::cout << msg << std::endl;
		}
		lua_getglobal(L, "foo");
		lua_pushstring(L, filename.c_str());
		err = lua_pcall(L, 1, 1, 0);
		if (err) {
			const char *msg = lua_tostring(L,-1);
			std::cout << msg << std::endl;
		}
	}

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
	transPicToFiles(texfilenames, outfloder);
	transAniToFiles(outfloder);
}

void ParserLuaFile::transToMemory(const std::vector<std::string>& texfilenames)
{
	transPicToMemory(texfilenames);
	transAniToMemory();
}

void ParserLuaFile::getAllSymbols(std::vector<ee::Symbol*>& symbols) const
{
	symbols.reserve(m_mapSymbols.size());
	std::map<int, ee::Symbol*>::const_iterator itr = m_mapSymbols.begin();
	for ( ; itr != m_mapSymbols.end(); ++itr)
		symbols.push_back(itr->second);
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

		part->init();
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
				if (lua_type(L, -1) != LUA_TNIL) {
					item->color = lua_tonumber(L, -1);
				}
				lua_pop(L, 1);

				lua_getfield(L, -1, "add");
				if (lua_type(L, -1) != LUA_TNIL) {
					item->add = lua_tonumber(L, -1);
				}
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

void ParserLuaFile::transPicToFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder)
{
	// pictures
	std::vector<ee::Image*> images;
	images.resize(texfilenames.size());
	for (int i = 0, n = texfilenames.size(); i < n; ++i)
		images[i] = ee::ImageMgr::Instance()->GetItem(texfilenames[i]);

	// Picture to easycomplex
	std::map<int, Picture*>::iterator itr = m_mapPictures.begin();
	for (int i = 0; itr != m_mapPictures.end(); ++itr, ++i)
	{
		std::cout << "pic: [" << i << "/" << m_mapPictures.size() << "]" << std::endl;

		Picture* pic = itr->second;

		ecomplex::Symbol* symbol = new ecomplex::Symbol;
		for (int i = 0, n = pic->parts.size(); i < n; ++i)
		{
			Picture::Part* part = pic->parts[i];

			const ee::ImageData* image = images[part->tex]->GetImageData();
			ee::ImageClip clip(*image);

			const uint8_t* pixels = clip.Clip(part->xmin, part->xmax, part->ymin, part->ymax);
			if (pixels) 
			{
				int width = part->xmax-part->xmin,
					height = part->ymax-part->ymin;
				std::string outfile = outfloder + "\\" + part->filename;
				if (!wxFileExists(outfile))
					ee::ImageSaver::StoreToFile(pixels, width, height, 4, outfile, ee::ImageSaver::e_png);

				std::string outpath = outfile + ".png";
				ee::Sprite* sprite = new ee::NullSprite(new ee::NullSymbol(outpath, width, height));
				part->transform(sprite);
				symbol->m_sprites.push_back(sprite);
			}
		}

		std::stringstream ss;
		if (symbol->name.empty()) {
			ss << itr->first;
		} else {
			ss << symbol->name;
		}
		std::string filename = outfloder + "\\" + ss.str() 
			+ "_" + ee::FileType::GetTag(ee::FileType::e_complex) + ".json";
		ecomplex::FileStorer::Store(filename.c_str(), symbol);

		pic->filename = filename;
		pic->width = symbol->GetSize().Width();
		pic->height = symbol->GetSize().Height();

		delete symbol;
	}
}

void ParserLuaFile::transAniToFiles(const std::string& outfloder)
{
	// to Animation
	std::map<int, Animation*>::iterator itr = m_mapAnims.begin();
	for (int i = 0; itr != m_mapAnims.end(); ++itr, ++i)
	{
		std::cout << "ani: " << itr->first << " [" << i << "/" << m_mapAnims.size() << "]" << std::endl;

		Animation* ani = itr->second;
		if (ani->frames.size() > 1) {
			transAniToAnimationFile(outfloder, itr->first, ani);
		} else {
			transAniToComplexFile(outfloder, itr->first, ani);
		}
	}
}

void ParserLuaFile::transAniToAnimationFile(const std::string& outfloder, int id, Animation* ani)
{
	eanim::Symbol* symbol = new eanim::Symbol;
	eanim::Symbol::Layer* layer = new eanim::Symbol::Layer;
	symbol->name = ani->export_name;
	symbol->setFPS(30);
	for (int i = 0, n = ani->frames.size(); i < n; ++i)
	{
		//				std::cout << "frame: [" << i << "/" << ani->frames.size() << "]" << std::endl;

		eanim::Symbol::Frame* frame = new eanim::Symbol::Frame;
		frame->index = i+1;
		frame->bClassicTween = false;
		for (int j = 0, m = ani->frames[i].size(); j < m; ++j)
		{
			//					std::cout << "item: [" << j << "/" << ani->frames[i].size() << "]" << std::endl;

			Animation::Item* item = ani->frames[i][j];
			ee::Sprite* sprite = NULL;
			std::map<int, Picture*>::iterator itr = m_mapPictures.find(ani->component[item->index]);
			if (itr != m_mapPictures.end())
			{
				Picture* pic = itr->second;
				sprite = new ee::NullSprite(new ee::NullSymbol(pic->filename, pic->width, pic->height));
			}
			else
			{
				std::map<int, Animation*>::iterator itr = m_mapAnims.find(ani->component[item->index]);
				assert(itr != m_mapAnims.end());
				Animation* ani = itr->second;
				sprite = new ee::NullSprite(new ee::NullSymbol(ani->filename));
			}
			item->transform(sprite);
			frame->sprites.push_back(sprite);
		}
		layer->frames.push_back(frame);
	}
	symbol->m_layers.push_back(layer);

	std::stringstream ss;
	if (symbol->name.empty()) {
		ss << id;
	} else {
		ss << symbol->name;
	}
	std::string filename = outfloder + "\\" + ss.str() 
		+ "_" + ee::FileType::GetTag(ee::FileType::e_anim) + ".json";
	eanim::FileSaver::Store(filename.c_str(), *symbol);

	ani->filename = filename;

	delete symbol;
}

void ParserLuaFile::transAniToComplexFile(const std::string& outfloder, int id, Animation* ani)
{
	assert(ani->frames.size() == 1);

	ecomplex::Symbol* symbol = new ecomplex::Symbol;
	symbol->name = ani->export_name;
	for (int i = 0, n = ani->frames[0].size(); i < n; ++i)
	{
		Animation::Item* item = ani->frames[0][i];
		ee::Sprite* sprite = NULL;
		std::map<int, Picture*>::iterator itr = m_mapPictures.find(ani->component[item->index]);
		if (itr != m_mapPictures.end())
		{
			Picture* pic = itr->second;
			sprite = new ee::NullSprite(new ee::NullSymbol(pic->filename, pic->width, pic->height));
		}
		else
		{
			std::map<int, Animation*>::iterator itr = m_mapAnims.find(ani->component[item->index]);
			assert(itr != m_mapAnims.end());
			Animation* ani = itr->second;
			sprite = new ee::NullSprite(new ee::NullSymbol(ani->filename));
		}
		item->transform(sprite);
		symbol->m_sprites.push_back(sprite);
	}

	std::stringstream ss;
	if (symbol->name.empty()) {
		ss << id;
	} else {
		ss << symbol->name;
	}
	std::string filename = outfloder + "\\" + ss.str() 
		+ "_" + ee::FileType::GetTag(ee::FileType::e_complex) + ".json";
	ecomplex::FileStorer::Store(filename.c_str(), symbol);

	ani->filename = filename;

	delete symbol;
}

void ParserLuaFile::transPicToMemory(const std::vector<std::string>& texfilenames)
{
	// pictures
	std::vector<ee::Image*> images;
	images.resize(texfilenames.size());
	for (int i = 0, n = texfilenames.size(); i < n; ++i)
		images[i] = ee::ImageMgr::Instance()->GetItem(texfilenames[i]);

	// Picture to easycomplex
	std::map<int, Picture*>::iterator itr = m_mapPictures.begin();
	for (int i = 0; itr != m_mapPictures.end(); ++itr, ++i)
	{
		std::cout << "pic: [" << i << "/" << m_mapPictures.size() << "]" << std::endl;

		Picture* pic = itr->second;

		ecomplex::Symbol* symbol = new ecomplex::Symbol;
		for (int i = 0, n = pic->parts.size(); i < n; ++i)
		{
			Picture::Part* part = pic->parts[i];

			ee::ImageSymbol* image = new ee::ImageSymbol(images[part->tex], texfilenames[part->tex]);
			ee::Rect r;
			r.xmin = part->xmin;
			r.xmax = part->xmax;
			r.ymin = part->ymin;
			r.ymax = part->ymax;
			image->SetRegion(r);

			ee::Sprite* sprite = new ee::ImageSprite(image);
			part->transform(sprite);
			symbol->m_sprites.push_back(sprite);
		}

		// todo filepath
//		symbol->InitThumbnail();
		m_mapSymbols.insert(std::make_pair(itr->first, symbol));
	}
}

void ParserLuaFile::transAniToMemory()
{
	std::map<int, Animation*>::iterator itr = m_mapAnims.begin();
	for (int i = 0; itr != m_mapAnims.end(); ++itr, ++i)
	{
		std::cout << "ani: " << itr->first << " [" << i << "/" << m_mapAnims.size() << "]" << std::endl;

		Animation* ani = itr->second;
		if (ani->frames.size() > 1) {
			transAniToAnimationMemory(itr->first, ani);
		} else {
			transAniToComplexMemory(itr->first, ani);
		}
	}
}

void ParserLuaFile::transAniToAnimationMemory(int id, Animation* ani)
{
	eanim::Symbol* symbol = new eanim::Symbol;
	eanim::Symbol::Layer* layer = new eanim::Symbol::Layer;
	symbol->name = ani->export_name;
	symbol->setFPS(30);
	for (int i = 0, n = ani->frames.size(); i < n; ++i)
	{
		// std::cout << "frame: [" << i << "/" << ani->frames.size() << "]" << std::endl;

		eanim::Symbol::Frame* frame = new eanim::Symbol::Frame;
		frame->index = i + 1;
		frame->bClassicTween = false;
		for (int j = 0, m = ani->frames[i].size(); j < m; ++j)
		{
			// std::cout << "item: [" << j << "/" << ani->frames[i].size() << "]" << std::endl;

			Animation::Item* item = ani->frames[i][j];
			ee::Sprite* sprite = NULL;
			std::map<int, Picture*>::iterator itr = m_mapPictures.find(ani->component[item->index]);
			if (itr != m_mapPictures.end())
			{
				std::map<int, ee::Symbol*>::iterator itr 
					= m_mapSymbols.find(ani->component[item->index]);
				assert(itr != m_mapSymbols.end());
				sprite = ee::SpriteFactory::Instance()->Create(itr->second);
			}
			else
			{
				std::map<int, ee::Symbol*>::iterator itr 
					= m_mapSymbols.find(ani->component[item->index]);
				assert(itr != m_mapSymbols.end());
				sprite = ee::SpriteFactory::Instance()->Create(itr->second);
			}
			item->transform(sprite);
			frame->sprites.push_back(sprite);
		}
		layer->frames.push_back(frame);
	}
	symbol->m_layers.push_back(layer);

	// todo filepath
	//symbol->InitThumbnail();
	m_mapSymbols.insert(std::make_pair(id, symbol));
}

void ParserLuaFile::transAniToComplexMemory(int id, Animation* ani)
{
	assert(ani->frames.size() == 1);

	ecomplex::Symbol* symbol = new ecomplex::Symbol;
	symbol->name = ani->export_name;
	for (int i = 0, n = ani->frames[0].size(); i < n; ++i)
	{
		// std::cout << "item: [" << j << "/" << ani->frames[i].size() << "]" << std::endl;

		Animation::Item* item = ani->frames[0][i];
		ee::Sprite* sprite = NULL;
		std::map<int, Picture*>::iterator itr = m_mapPictures.find(ani->component[item->index]);
		if (itr != m_mapPictures.end())
		{
			std::map<int, ee::Symbol*>::iterator itr 
				= m_mapSymbols.find(ani->component[item->index]);
			assert(itr != m_mapSymbols.end());
			sprite = ee::SpriteFactory::Instance()->Create(itr->second);
		}
		else
		{
			std::map<int, ee::Symbol*>::iterator itr 
				= m_mapSymbols.find(ani->component[item->index]);
			assert(itr != m_mapSymbols.end());
			sprite = ee::SpriteFactory::Instance()->Create(itr->second);
		}
		item->transform(sprite);
		symbol->m_sprites.push_back(sprite);
	}

	// todo filepath
	//symbol->InitThumbnail();
	m_mapSymbols.insert(std::make_pair(id, symbol));
}

//////////////////////////////////////////////////////////////////////////
// class ParserLuaFile::Picture::Part
//////////////////////////////////////////////////////////////////////////

void ParserLuaFile::Picture::Part::init()
{
	xmin = ymin = INT_MAX;
	xmax = ymax = INT_MIN;
	for (int i = 0; i < 4; ++i)
	{
		if (src[i].x < xmin)
			xmin = src[i].x;
		else if (src[i].x > xmax)
			xmax = src[i].x;
		if (src[i].y < ymin)
			ymin = src[i].y;
		else if (src[i].y > ymax)
			ymax = src[i].y;
	}

	std::stringstream ss;
	ss << tex << "_" << xmin << "_" << xmax << "_" << ymin << "_" << ymax;
	filename = ss.str();
}

void ParserLuaFile::Picture::Part::transform(ee::Sprite* sprite) const
{
	bool xMirror = false, yMirror = false;
	float angle = 0;
	bool xy_swap = false;

	std::set<int> buf;
	for (int i = 0; i < 4; ++i) {
		buf.insert(dst[i].x);
		buf.insert(dst[i].y);
	}
	float pre_rotate = 0;
	ee::Vector _dst[4];
	memcpy(&_dst[0].x, &dst[0].x, sizeof(ee::Vector)*4);
	if (buf.size() != 4)
	{
		ee::Vector center(0, 0);
		for (int i = 0; i < 4; ++i) {
			center += dst[i];
		}
		center /= 4;

		ee::Vector other = (dst[0] + dst[1]) * 0.5f;
		float angle = ee::Math2D::GetLineAngle(center, other);
		for (int i = 0; i < 4; ++i) {
			_dst[i] = center + ee::Math2D::RotateVector(dst[i] - center, -angle);
		}
		pre_rotate = -angle;
	}

	std::string mode = dstMode(_dst);
	// 0123 0321 
	// 1032 1230
	// 2103 2301
	// 3012 3210
	if (mode == "0123")
		;
	else if (mode == "0321")
	{
		yMirror = true;
		angle = -ee::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "1032")
	{
		xMirror = true;
	}
	else if (mode == "1230")
	{
		angle = -ee::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "2103")
	{
		xMirror = true;
		angle = -ee::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "2301")
	{
		angle = ee::PI;
	}
	else if (mode == "3012")
	{
		angle = ee::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "3210")
	{
		xMirror = true;
		angle = ee::PI;
	}

	ee::Vector scenter = (src[0] + src[1] + src[2] + src[3]) * 0.25f, 
		dcenter = (_dst[0] + _dst[1] + _dst[2] + _dst[3]) * 0.25f;
	float sw = fabs(src[0].x - scenter.x), sh = fabs(src[0].y - scenter.y);
	float dw = fabs(_dst[0].x - dcenter.x), dh = fabs(_dst[0].y - dcenter.y);

	float sx, sy;
	if (xy_swap) 
	{
		sx = dw / 16.0f / sh;
		sy = dh / 16.0f / sw;
	} 
	else
	{
		sx = dw / 16.0f / sw;
		sy = dh / 16.0f / sh;
	}
	sprite->SetScale(ee::Vector(sx, sy));

	sprite->SetMirror(xMirror, yMirror);
	angle = -angle;
	sprite->SetTransform(ee::Vector(dcenter.x / 16, - dcenter.y / 16), pre_rotate + angle);
}

std::string ParserLuaFile::Picture::Part::dstMode(const ee::Vector _dst[4]) const
{
	int sm[4], dm[4];
	ee::Vector scenter = (src[0] + src[1] + src[2] + src[3]) * 0.25f, 
		dcenter = (_dst[0] + _dst[1] + _dst[2] + _dst[3]) * 0.25f;
	for (int i = 0; i < 4; ++i)
		sm[i] = nodeMode(scenter, src[i]);
	for (int i = 0; i < 4; ++i)
		dm[i] = nodeMode(dcenter, _dst[i]);
	
	std::stringstream ss;
	for (int i = 0; i < 4; ++i)
		ss << dm[findInMode(sm, i)];
	return ss.str();
}

int ParserLuaFile::Picture::Part::nodeMode(const ee::Vector& center, const ee::Vector& node)
{
	if (node.x < center.x)
	{
		if (node.y < center.y)
			return 3;
		else
			return 0;
	}
	else
	{
		if (node.y < center.y)
			return 2;
		else
			return 1;
	}
}

int ParserLuaFile::Picture::Part::findInMode(int mode[4], int query)
{
	for (int i = 0; i < 4; ++i)
		if (mode[i] == query)
			return i;
	assert(0);
	return -1;
}

void ParserLuaFile::Animation::Item::transform(ee::Sprite* sprite) const
{
	bool valid = false;
	for (int i = 0; i < 6; ++i)
	{
		if (mat[i] != 0)
		{
			valid = true;
			break;
		}
	}

	if (is_full && valid)
	{
		sprite->color.multi = ee::TransColor(color, ee::PT_BGRA);
		sprite->color.add = ee::TransColor(add, ee::PT_ABGR);

		float x = mat[4] / 16.0f,
			y = mat[5] / 16.0f;
		float ang1, ang2;
		if (mat[0] == 0) {
			ang1 = ee::PI * 0.5f;
		} else {
			ang1 = atan(-(float)mat[2]/mat[0]);
		}
		if (mat[3] == 0) {
			ang2 = ee::PI * 0.5f;
		} else {
			ang2 = atan((float)mat[1]/mat[3]);
		}
 		float angle;
 		float sx, sy, kx, ky;
 		if (fabs(ang1 - ang2) < 0.00001f)
 		{
 			if (ang1 == 0)
 			{
 				sx = mat[0]/1024.0f/cos(ang1);
 				sy = mat[3]/1024.0f/cos(ang1);
 			}
 			else
 			{
 				sx = mat[1]/1024.0f/sin(ang1);
				sy = -mat[2]/1024.0f/sin(ang1);
 			}
 			angle = ang1;
 			kx = ky = 0;
 		}
 		else
 		{
 			// no rotate
 			sx = mat[0]/1024.0f;
 			sy = mat[3]/1024.0f;
 			kx = float(mat[2])/mat[3];
 			ky = float(mat[1])/mat[0];
 			angle = 0;
 		}
		// for y
		y = -y;
		angle = -angle;

		sprite->Rotate(angle);
		sprite->Translate(ee::Vector(x, y));
		const ee::Vector& scale = sprite->GetScale();
		sprite->SetScale(ee::Vector(scale.x * sx, scale.y * sy));
		sprite->SetShear(ee::Vector(kx, ky));
	}
}

}
}