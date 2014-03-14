#include "ParserLuaFile.h"

#include <sstream>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyimage.h>

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
};

namespace libcoco
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
	transPicToFiles(texfilenames, outfloder);
	transAniToFiles(outfloder);
}

void ParserLuaFile::transToMemory(const std::vector<std::string>& texfilenames)
{
	transPicToMemory(texfilenames);
	transAniToMemory();
}

void ParserLuaFile::getAllSymbols(std::vector<d2d::ISymbol*>& symbols) const
{
	symbols.reserve(m_mapSymbols.size());
	std::map<int, d2d::ISymbol*>::const_iterator itr = m_mapSymbols.begin();
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

void ParserLuaFile::transPicToFiles(const std::vector<std::string>& texfilenames, const std::string& outfloder)
{
	// pictures
	std::vector<d2d::Image*> images;
	images.resize(texfilenames.size());
	for (int i = 0, n = texfilenames.size(); i < n; ++i)
		images[i] = d2d::ImageMgr::Instance()->getItem(texfilenames[i]);

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
			d2d::Image* image = images[part->tex];
			eimage::ImageProcessor processor(image);
			const unsigned char* pixels = processor.clip(part->xmin, part->xmax, part->ymin, part->ymax);
			if (pixels) 
			{
				int width = part->xmax-part->xmin,
					height = part->ymax-part->ymin;
				std::string outfile = outfloder + "\\" + part->filename;
				if (!wxFileExists(outfile))
					d2d::ImageSaver::storeToFile(pixels, width, height, outfile, d2d::ImageSaver::e_png);

				std::string outpath = outfile + ".png";
				d2d::ISprite* sprite = new d2d::NullSprite(new d2d::NullSymbol(outpath, width, height));
				part->transform(sprite);
				symbol->m_sprites.push_back(sprite);
			}
		}

		std::stringstream ss;
		ss << itr->first;
		std::string filename = outfloder + "\\" + ss.str() 
			+ "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_complex) + ".json";
		ecomplex::FileSaver::store(filename.c_str(), symbol);

		pic->filename = filename;
		pic->width = symbol->getSize().xLength();
		pic->height = symbol->getSize().yLength();

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

		anim::Symbol* symbol = new anim::Symbol;
		anim::Symbol::Layer* layer = new anim::Symbol::Layer;
		symbol->name = ani->export_name;
		symbol->m_fps = 30;
		for (int i = 0, n = ani->frames.size(); i < n; ++i)
		{
//				std::cout << "frame: [" << i << "/" << ani->frames.size() << "]" << std::endl;

			anim::Symbol::Frame* frame = new anim::Symbol::Frame;
			frame->index = i;
			frame->bClassicTween = false;
			for (int j = 0, m = ani->frames[i].size(); j < m; ++j)
			{
//					std::cout << "item: [" << j << "/" << ani->frames[i].size() << "]" << std::endl;

				Animation::Item* item = ani->frames[i][j];
				std::map<int, Picture*>::iterator itr = m_mapPictures.find(ani->component[item->index]);
				if (itr != m_mapPictures.end())
				{
					Picture* pic = itr->second;
					d2d::ISprite* sprite = new d2d::NullSprite(new d2d::NullSymbol(pic->filename, pic->width, pic->height));
					item->transform(sprite);
					frame->sprites.push_back(sprite);
				}
				else
				{
					std::map<int, Animation*>::iterator itr = m_mapAnims.find(ani->component[item->index]);
					assert(itr != m_mapAnims.end());
					Animation* ani = itr->second;
					d2d::ISprite* sprite = new d2d::NullSprite(new d2d::NullSymbol(ani->filename));
					item->transform(sprite);
					frame->sprites.push_back(sprite);
				}
			}
			layer->frames.push_back(frame);
		}
		symbol->m_layers.push_back(layer);

		std::stringstream ss;
		ss << itr->first;
		std::string filename = outfloder + "\\" + ss.str() 
			+ "_" + d2d::FileNameParser::getFileTag(d2d::FileNameParser::e_anim) + ".json";
		anim::FileSaver::store(filename.c_str(), *symbol);

		ani->filename = filename;

		delete symbol;
	}
}

void ParserLuaFile::transPicToMemory(const std::vector<std::string>& texfilenames)
{
	// pictures
	std::vector<d2d::Image*> images;
	images.resize(texfilenames.size());
	for (int i = 0, n = texfilenames.size(); i < n; ++i)
		images[i] = d2d::ImageMgr::Instance()->getItem(texfilenames[i]);

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

			d2d::ImageSymbol* image = new d2d::ImageSymbol(images[part->tex], texfilenames[part->tex]);
			d2d::Rect r;
			r.xMin = part->xmin;
			r.xMax = part->xmax;
			r.yMin = part->ymin;
			r.yMax = part->ymax;
			image->setRegion(r);

			d2d::ISprite* sprite = new d2d::ImageSprite(image);
			part->transform(sprite);
			symbol->m_sprites.push_back(sprite);
		}

		symbol->refresh();
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

		anim::Symbol* symbol = new anim::Symbol;
		anim::Symbol::Layer* layer = new anim::Symbol::Layer;
		symbol->name = ani->export_name;
		symbol->m_fps = 30;
		for (int i = 0, n = ani->frames.size(); i < n; ++i)
		{
			// std::cout << "frame: [" << i << "/" << ani->frames.size() << "]" << std::endl;

			anim::Symbol::Frame* frame = new anim::Symbol::Frame;
			frame->index = i;
			frame->bClassicTween = false;
			for (int j = 0, m = ani->frames[i].size(); j < m; ++j)
			{
				// std::cout << "item: [" << j << "/" << ani->frames[i].size() << "]" << std::endl;

				Animation::Item* item = ani->frames[i][j];
				std::map<int, Picture*>::iterator itr = m_mapPictures.find(ani->component[item->index]);
				if (itr != m_mapPictures.end())
				{
					std::map<int, d2d::ISymbol*>::iterator itr 
						= m_mapSymbols.find(ani->component[item->index]);
					assert(itr != m_mapSymbols.end());
					frame->sprites.push_back(d2d::SpriteFactory::Instance()->create(itr->second));
				}
				else
				{
					std::map<int, d2d::ISymbol*>::iterator itr 
						= m_mapSymbols.find(ani->component[item->index]);
					assert(itr != m_mapSymbols.end());
					d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(itr->second);
					item->transform(sprite);
					frame->sprites.push_back(sprite);
				}
			}
			layer->frames.push_back(frame);
		}
		symbol->m_layers.push_back(layer);

		symbol->refresh();
		m_mapSymbols.insert(std::make_pair(itr->first, symbol));
	}
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

void ParserLuaFile::Picture::Part::transform(d2d::ISprite* sprite) const
{
	bool xMirror = false, yMirror = false;
	float angle = 0;
	bool xy_swap = false;

	std::string mode = dstMode();
	// 0123 0321 
	// 1032 1230
	// 2103 2301
	// 3012 3210
	if (mode == "0123")
		;
	else if (mode == "0321")
	{
		xMirror = true;
		angle = d2d::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "1032")
	{
		xMirror = true;
	}
	else if (mode == "1230")
	{
		angle = d2d::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "2103")
	{
		xMirror = true;
		angle = - d2d::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "2301")
	{
		angle = d2d::PI;
	}
	else if (mode == "3012")
	{
		angle = - d2d::PI * 0.5f;
		xy_swap = true;
	}
	else if (mode == "3210")
	{
		xMirror = true;
		angle = d2d::PI;
	}

	d2d::Vector scenter = (src[0] + src[1] + src[2] + src[3]) * 0.25f, 
		dcenter = (dst[0] + dst[1] + dst[2] + dst[3]) * 0.25f;
	float sw = fabs(src[0].x - scenter.x), sh = fabs(src[0].y - scenter.y);
	float dw = fabs(dst[0].x - dcenter.x), dh = fabs(dst[0].y - dcenter.y);
	float sx = dw / 16.0f / sw,
		  sy = dh / 16.0f / sh;

	if (xy_swap)
		sprite->setScale(sy, sx);
	else
		sprite->setScale(sx, sy);
	sprite->setMirror(xMirror, yMirror);
	sprite->setTransform(d2d::Vector(dcenter.x / 16, - dcenter.y / 16), angle);
}

std::string ParserLuaFile::Picture::Part::dstMode() const
{
	int sm[4], dm[4];
	d2d::Vector scenter = (src[0] + src[1] + src[2] + src[3]) * 0.25f, 
		dcenter = (dst[0] + dst[1] + dst[2] + dst[3]) * 0.25f;
	for (int i = 0; i < 4; ++i)
		sm[i] = nodeMode(scenter, src[i]);
	for (int i = 0; i < 4; ++i)
		dm[i] = nodeMode(dcenter, dst[i]);
	
	std::stringstream ss;
	for (int i = 0; i < 4; ++i)
		ss << dm[findInMode(sm, i)];
	return ss.str();
}

int ParserLuaFile::Picture::Part::nodeMode(const d2d::Vector& center, const d2d::Vector& node)
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

void ParserLuaFile::Animation::Item::transform(d2d::ISprite* sprite) const
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
		// coc的数据颜色都是argb格式
		sprite->multiCol = d2d::transColor(color, d2d::PT_ARGB);
		sprite->addCol = d2d::transColor(add, d2d::PT_ARGB);

		float x = mat[4] / 16.0f,
			y = -mat[5] / 16.0f;
		assert(mat[0] != 0 && mat[3] != 0);
		float ang1 = atan(-(float)mat[2]/mat[0]),
			ang2 = atan((float)mat[1]/mat[3]);
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
				sx = -mat[2]/1024.0f/sin(ang1);
				sy = mat[1]/1024.0f/sin(ang1);
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
		sprite->rotate(angle);
		sprite->translate(d2d::Vector(x, y));
		const d2d::Vector& scale = sprite->getScale();
		sprite->setScale(scale.x * sx, scale.y * sy);
		sprite->setShear(kx, ky);
	}
}

}