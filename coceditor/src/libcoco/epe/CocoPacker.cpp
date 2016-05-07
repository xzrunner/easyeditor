#include "CocoPacker.h"
#include "TexturePacker.h"

#include "../Utility.h"

#include <ee/ImageSprite.h>
#include <ee/FontBlankSprite.h>
#include <ee/trans_color.h>
#include <ee/Exception.h>
#include <ee/Math2D.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

namespace ecoco
{
namespace epe
{

namespace lua = ebuilder::lua;

CocoPacker::CocoPacker(const TexturePacker& tex)
	: m_tex(tex)
	, m_id(0)
{
}

void CocoPacker::pack(const std::vector<const ee::Symbol*>& symbols)
{
	lua::TableAssign ta(m_gen, "return", false, false);

	for (int i = 0, n = symbols.size(); i < n; ++i)
	{
		const ee::Symbol* symbol = symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol))
		{
			for (size_t i = 0, n = complex->m_sprites.size(); i < n; ++i)
			{
				ee::Sprite* sprite = complex->m_sprites[i];
				if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
					resolvePicture(image);
				}
				else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
					resolveFont(font);
				}
			}

			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			resolveAnimation(complex);
		}
		else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(symbol))
		{
			std::set<const ee::ImageSymbol*> unique;
			for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			{
				eanim::Symbol::Layer* layer = anim->m_layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					eanim::Symbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
					{
						ee::Sprite* sprite = frame->sprites[k];
						if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(sprite))
							unique.insert(&image->GetSymbol());
						else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(sprite))
							m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
					}
				}
			}

			std::set<const ee::ImageSymbol*>::iterator itr = unique.begin();
			for ( ; itr != unique.end(); ++itr)
			{
				std::map<const ee::Symbol*, int>::iterator itrFind = m_mapSymbolID.find(*itr);
				if (itrFind == m_mapSymbolID.end())
				{
					m_mapSymbolID.insert(std::make_pair(*itr, m_id++));
					resolvePicture(*itr);
				}
			}

			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			resolveAnimation(anim);
		}
		else if (const escale9::Symbol* patch9 = dynamic_cast<const escale9::Symbol*>(symbol))
		{
			const escale9::Scale9Data& data = patch9->GetScale9Data();

 			std::vector<ee::Sprite*> sprites;
 			switch (data.GetType())
 			{
 			case escale9::e_9Grid:
 				for (size_t i = 0; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j)
 						sprites.push_back(data.GetSprite(i, j));
 				break;
 			case escale9::e_9GridHollow:
 				for (size_t i = 0; i < 3; ++i) {
 					for (size_t j = 0; j < 3; ++j) {
 						if (i == 1 && j == 1) continue;
 						sprites.push_back(data.GetSprite(i, j));
 					}
 				}
 				break;
 			case escale9::e_3GridHor:
 				for (size_t i = 0; i < 3; ++i)
 					sprites.push_back(data.GetSprite(1, i));
 				break;
 			case escale9::e_3GridVer:
 				for (size_t i = 0; i < 3; ++i)
 					sprites.push_back(data.GetSprite(i, 1));
 				break;
 			case escale9::e_6GridUpper:
 				for (size_t i = 1; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j)
 						sprites.push_back(data.GetSprite(i, j));
 				break;
 			}
 
 			for (size_t i = 0, n = sprites.size(); i < n; ++i)
 			{
 				ee::Sprite* sprite = sprites[i];
 				if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(sprite))
 				{
 					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
 					resolvePicture(image);
 				}
 				else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(sprite))
 				{
 					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
 				}
 			}
 
 			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
 			resolveAnimation(patch9);
		}
	}
}

void CocoPacker::storeToFile(const std::string& filename)
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filename.c_str());
	std::locale::global(std::locale("C"));	
//	assert(!fout.fail());
	fout << m_gen.toText() << std::endl;
	fout.close();
}

void CocoPacker::resolvePicture(const ee::ImageSprite* sprite)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"picture\"") + ",");

	// id
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen.line(lua::assign("id", sid) + ",");
	}

	// tex
	// todo: specify only 1 texture
	std::string assignTex = lua::assign("tex", ee::StringHelper::ToString(1));
	const ee::Rect* pr = m_tex.Query(sprite->GetSymbol().GetImage());

	// src
	int x0 = pr->xmin, y0 = pr->ymax;
	int x1 = pr->xmin, y1 = pr->ymin;
	int x2 = pr->xmax, y2 = pr->ymin;
	int x3 = pr->xmax, y3 = pr->ymax;

	std::string sx0 = ee::StringHelper::ToString(x0), sy0 = ee::StringHelper::ToString(y0);
	std::string sx1 = ee::StringHelper::ToString(x1), sy1 = ee::StringHelper::ToString(y1);
	std::string sx2 = ee::StringHelper::ToString(x2), sy2 = ee::StringHelper::ToString(y2);
	std::string sx3 = ee::StringHelper::ToString(x3), sy3 = ee::StringHelper::ToString(y3);
	std::string assignSrc = lua::assign("src", lua::tableassign("", 8, sx0, sy0, 
		sx1, sy1, sx2, sy2, sx3, sy3));

	// screen
	const float hw = pr->Width() * 0.5f,
		hh = pr->Height() * 0.5f;
	ee::Vector screen[4];
	screen[0].Set(-hw, hh);
	screen[1].Set(-hw, -hh);
	screen[2].Set(hw, -hh);
	screen[3].Set(hw, hh);
	// 1. scale
	for (size_t i = 0; i < 4; ++i)
		screen[i].x *= sprite->GetScale().x;
	for (size_t i = 0; i < 4; ++i)
		screen[i].y *= sprite->GetScale().y;
	// 2. rotate
	for (size_t i = 0; i < 4; ++i)
	{
		ee::Vector rot = ee::Math2D::RotateVector(screen[i], sprite->GetAngle());
		screen[i] = rot;
	}
	// 3. translate
	ee::Vector center = sprite->GetCenter();
 	for (size_t i = 0; i < 4; ++i)
 		screen[i] += center;
	//for (size_t i = 0; i < 4; ++i)
	//{
	//	screen[i].x += center.x;
	//	screen[i].y -= center.y;
	//}
	// 4. mirror
	bool xMirror, yMirror;
	sprite->GetMirror(xMirror, yMirror);
	if (xMirror)
	{
		std::swap(screen[0].x, screen[3].x);
		std::swap(screen[1].x, screen[2].x);
	}
	if (yMirror)
	{
		std::swap(screen[1].y, screen[0].y);
		std::swap(screen[2].y, screen[3].y);
	}
 	// flip y
 	for (size_t i = 0; i < 4; ++i)
 		screen[i].y = -screen[i].y;
	// finish: scale
	const float SCALE = 16;
	for (size_t i = 0; i < 4; ++i)
		screen[i] *= SCALE;

	sx0 = ee::StringHelper::ToString(screen[0].x);
	sy0 = ee::StringHelper::ToString(screen[0].y);
	sx1 = ee::StringHelper::ToString(screen[1].x);
	sy1 = ee::StringHelper::ToString(screen[1].y);
	sx2 = ee::StringHelper::ToString(screen[2].x);
	sy2 = ee::StringHelper::ToString(screen[2].y);
	sx3 = ee::StringHelper::ToString(screen[3].x);
	sy3 = ee::StringHelper::ToString(screen[3].y);
	std::string assignScreen = lua::assign("screen", lua::tableassign("", 8, sx0, sy0, 
		sx1, sy1, sx2, sy2, sx3, sy3));

	lua::tableassign(m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

void CocoPacker::resolvePicture(const ee::ImageSymbol* symbol)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"picture\"") + ",");

	// id
	{
		std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
		assert(itr != m_mapSymbolID.end());
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen.line(lua::assign("id", sid) + ",");
	}

	// tex
	// todo: specify only 1 texture
	std::string assignTex = lua::assign("tex", ee::StringHelper::ToString(1));
	const ee::Rect* pr = m_tex.Query(symbol->GetImage());

	// src
	int x0 = pr->xmin, y0 = pr->ymax;
	int x1 = pr->xmin, y1 = pr->ymin;
	int x2 = pr->xmax, y2 = pr->ymin;
	int x3 = pr->xmax, y3 = pr->ymax;

	std::string sx0 = ee::StringHelper::ToString(x0), sy0 = ee::StringHelper::ToString(y0);
	std::string sx1 = ee::StringHelper::ToString(x1), sy1 = ee::StringHelper::ToString(y1);
	std::string sx2 = ee::StringHelper::ToString(x2), sy2 = ee::StringHelper::ToString(y2);
	std::string sx3 = ee::StringHelper::ToString(x3), sy3 = ee::StringHelper::ToString(y3);
	std::string assignSrc = lua::assign("src", lua::tableassign("", 8, sx0, sy0, 
		sx1, sy1, sx2, sy2, sx3, sy3));

	// screen
	const float hw = pr->Width() * 0.5f,
		hh = pr->Height() * 0.5f;
	ee::Vector screen[4];
	screen[0].Set(-hw, hh);
	screen[1].Set(-hw, -hh);
	screen[2].Set(hw, -hh);
	screen[3].Set(hw, hh);

	// flip y
	for (size_t i = 0; i < 4; ++i)
		screen[i].y = -screen[i].y;
	// finish: scale
	const float SCALE = 16;
	for (size_t i = 0; i < 4; ++i)
		screen[i] *= SCALE;
	//
	sx0 = ee::StringHelper::ToString(screen[0].x);
	sy0 = ee::StringHelper::ToString(screen[0].y);
	sx1 = ee::StringHelper::ToString(screen[1].x);
	sy1 = ee::StringHelper::ToString(screen[1].y);
	sx2 = ee::StringHelper::ToString(screen[2].x);
	sy2 = ee::StringHelper::ToString(screen[2].y);
	sx3 = ee::StringHelper::ToString(screen[3].x);
	sy3 = ee::StringHelper::ToString(screen[3].y);
	std::string assignScreen = lua::assign("screen", lua::tableassign("", 8, sx0, sy0, 
		sx1, sy1, sx2, sy2, sx3, sy3));

	lua::tableassign(m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

void CocoPacker::resolveFont(const ee::FontBlankSprite* sprite)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"label\"") + ",");

	// id
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen.line(lua::assign("id", sid) + ",");
	}

	std::string aFont = lua::assign("font", "\""+sprite->font+"\"");
	std::string aColor = lua::assign("color", TransColor(sprite->font_color, ee::PT_ARGB));
//	std::string aAlign = lua::assign("align", ee::StringHelper::ToString(sprite->align));
	std::string aAlign = lua::assign("align", ee::StringHelper::ToString(sprite->align_hori));
	std::string aSize = lua::assign("size", ee::StringHelper::ToString(sprite->size));
	std::string aWidth = lua::assign("width", ee::StringHelper::ToString(sprite->width));
	std::string aHeight = lua::assign("height", ee::StringHelper::ToString(sprite->height));
	std::string aEdge = lua::assign("noedge", "false");

	m_gen.line(aFont + ", " + aColor + ", " + aAlign + ", " + aSize + ", " + aWidth + ", " + aHeight + ", " + aEdge);
}

void CocoPacker::resolveAnimation(const ecomplex::Symbol* symbol)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"animation\"") + ",");

	// export
	if (!symbol->name.empty())
		m_gen.line(lua::assign("export", "\""+symbol->name+"\"")+",");

	// id
	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
	assert(itr != m_mapSymbolID.end());
	std::string sid = ee::StringHelper::ToString(itr->second);
	m_gen.line(lua::assign("id", sid) + ",");

	// clipbox
	const ee::Rect& cb = symbol->m_clipbox;
	if (cb.xmin != 0 || cb.xmax != 0 || cb.ymin != 0 || cb.ymax != 0)
	{
		std::string width = ee::StringHelper::ToString(cb.xmax - cb.xmin);
		std::string height = ee::StringHelper::ToString(cb.ymax - cb.ymin);
		std::string left = ee::StringHelper::ToString(cb.xmin);
		std::string top = ee::StringHelper::ToString(-cb.ymax);
		lua::tableassign(m_gen, "clipbox", 4, width, height, left, top);
	}

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;
	{
		lua::TableAssign ta(m_gen, "component", true);
		for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
			resolveSpriteForComponent(symbol->m_sprites[i], ids, unique, order);
	}

	// children
	std::map<std::string, std::vector<ee::Sprite*> > map_actions;
	std::vector<ee::Sprite*> others;
	Utility::GroupSpritesFromTag(symbol->m_sprites, map_actions, others);
	if (!map_actions.empty())
	{
		std::map<std::string, std::vector<ee::Sprite*> >::iterator itr;
		for (itr = map_actions.begin(); itr != map_actions.end(); ++itr)
		{
			lua::TableAssign ta(m_gen, "", true);
			m_gen.line(lua::assign("action", "\"" + itr->first + "\"") + ",");
			// frame 0
			{
				lua::TableAssign ta(m_gen, "", true);
				for (size_t i = 0, n = itr->second.size(); i < n; ++i)
				{
					int idx = -1;
					for (idx = 0; idx < symbol->m_sprites.size(); ++idx)
						if (symbol->m_sprites[idx] == itr->second[i])
							break;
					resolveSpriteForFrame(itr->second[i], idx, ids, order);
				}
			}
		}
	}
	if (!others.empty())
	{
		lua::TableAssign ta(m_gen, "", true);
		// frame 0
		{
			lua::TableAssign ta(m_gen, "", true);
			for (size_t i = 0, n = others.size(); i < n; ++i)
			{
				int idx = -1;
				for (idx = 0; idx < symbol->m_sprites.size(); ++idx)
					if (symbol->m_sprites[idx] == others[i])
						break;
				resolveSpriteForFrame(others[i], idx, ids, order);
			}
		}
	}
}

void CocoPacker::resolveAnimation(const eanim::Symbol* symbol)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"animation\"") + ",");

	// export
	if (!symbol->name.empty())
		m_gen.line(lua::assign("export", "\""+symbol->name+"\"")+",");

	// id
	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
	assert(itr != m_mapSymbolID.end());
	std::string sid = ee::StringHelper::ToString(itr->second);
	m_gen.line(lua::assign("id", sid) + ",");

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;
	{
		lua::TableAssign ta(m_gen, "component", true);
		for (size_t i = 0, n = symbol->getMaxFrameIndex(); i < n; ++i)
		{
			for (size_t j = 0, m = symbol->m_layers.size(); j < m; ++j)
			{
				eanim::Symbol::Layer* layer = symbol->m_layers[j];
				if (i < layer->frames.size())
				{
					eanim::Symbol::Frame* frame = layer->frames[i];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
						resolveSpriteForComponent(frame->sprites[k], ids, unique, order);
				}
			}
		}
	}
	// children
	{
		lua::TableAssign ta(m_gen, "", true);
		// frames
		int index = 0;
		for (size_t i = 1, n = symbol->getMaxFrameIndex(); i <= n; ++i)
		{
			lua::TableAssign ta(m_gen, "", true);

			std::vector<ee::Sprite*> sprites;
			eanim::Utility::GetCurrSprites(symbol, i, sprites);
			for (size_t j = 0, m = sprites.size(); j < m; ++j)
				resolveSpriteForFrame(sprites[j], order);
		}
	}
}

void CocoPacker::resolveAnimation(const escale9::Symbol* symbol)
{
 	lua::TableAssign ta(m_gen, "", true, false);
 
	const escale9::Scale9Data& data = symbol->GetScale9Data();
	escale9::Scale9Type type = data.GetType();

 	// type
 	m_gen.line(lua::assign("type", "\"animation\"") + ",");
 
 	// export
 	if (!symbol->name.empty())
 		m_gen.line(lua::assign("export", "\""+symbol->name+"\"")+",");
 
 	// id
 	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
 	assert(itr != m_mapSymbolID.end());
 	std::string sid = ee::StringHelper::ToString(itr->second);
 	m_gen.line(lua::assign("id", sid) + ",");
 
 	// component
 	std::vector<int> ids;
 	std::map<int, std::vector<std::string> > unique;
 	std::vector<std::pair<int, std::string> > order;
 	{
 		lua::TableAssign ta(m_gen, "component", true);
 		if (type == escale9::e_9Grid)
 			for (size_t i = 0; i < 3; ++i)
 				for (size_t j = 0; j < 3; ++j)
 					resolveSpriteForComponent(data.GetSprite(i, j), ids, unique, order);
 		else if (type == escale9::e_9GridHollow)
 			for (size_t i = 0; i < 3; ++i) {
 				for (size_t j = 0; j < 3; ++j) {
 					if (i == 1 && j == 1) continue;
 					resolveSpriteForComponent(data.GetSprite(i, j), ids, unique, order);
 				}
 			}
 		else if (type == escale9::e_3GridHor)
 			for (size_t i = 0; i < 3; ++i)
 				resolveSpriteForComponent(data.GetSprite(1, i), ids, unique, order);
 		else if (type == escale9::e_3GridVer)
 			for (size_t i = 0; i < 3; ++i)
 				resolveSpriteForComponent(data.GetSprite(i, 1), ids, unique, order);
 		else if (type == escale9::e_6GridUpper)
 			for (size_t i = 1; i < 3; ++i)
 				for (size_t j = 0; j < 3; ++j)
 					resolveSpriteForComponent(data.GetSprite(i, j), ids, unique, order);
 	}
 	// children
 	{
 		lua::TableAssign ta(m_gen, "", true);
 		// frame 0
 		{
 			lua::TableAssign ta(m_gen, "", true);
 			int index = 0;
 			if (type == escale9::e_9Grid)
 				for (size_t i = 0; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j, ++index)
 						resolveSpriteForFrame(data.GetSprite(i, j), index, ids, order);
 			else if (type == escale9::e_9GridHollow)
 				for (size_t i = 0; i < 3; ++i) {
 					for (size_t j = 0; j < 3; ++j, ++index) {
 						if (i == 1 && j == 1) { 
 							--index;
 							continue;
 						}
 						resolveSpriteForFrame(data.GetSprite(i, j), index, ids, order);
 					}
 				}
 			else if (type == escale9::e_3GridHor)
 				for (size_t i = 0; i < 3; ++i)
 					resolveSpriteForFrame(data.GetSprite(1, i), i, ids, order);
 			else if (type == escale9::e_3GridVer)
 				for (size_t i = 0; i < 3; ++i)
 					resolveSpriteForFrame(data.GetSprite(i, 1), i, ids, order);
 			else if (type == escale9::e_6GridUpper)
 				for (size_t i = 1; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j, ++index)
 						resolveSpriteForFrame(data.GetSprite(i, j), index, ids, order);
 		}
 	}
}

void CocoPacker::resolveSpriteForComponent(const ee::Sprite* sprite, std::vector<int>& ids, 
										   std::map<int, std::vector<std::string> >& unique, 
										   std::vector<std::pair<int, std::string> >& order)
{
	int id;

	bool isFont = false;

	if (const ee::ImageSprite* image = dynamic_cast<const ee::ImageSprite*>(sprite))
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		if (itr != m_mapSpriteID.end())
		{
			id = itr->second;
		}
		else
		{
			// eanim::Symbol's sprites store unique
			std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->GetSymbol());
			assert(itr != m_mapSymbolID.end());
			id = itr->second;
		}
	}
	else if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(sprite))
	{
		isFont = true;
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		id = itr->second;
	}
	else
	{
		std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->GetSymbol());
		assert(itr != m_mapSymbolID.end());
		id = itr->second;
	}

	ids.push_back(id);

	std::map<int, std::vector<std::string> >::iterator itr = unique.find(id);
	if (unique.find(id) == unique.end())
	{
		std::string aID = lua::assign("id", ee::StringHelper::ToString(id));
		if (!sprite->name.empty() && sprite->name[0] != '_')
		{
			std::string aName = lua::assign("name", "\""+sprite->name+"\"");
			lua::tableassign(m_gen, "", 2, aName, aID);
		}
		else
		{
			lua::tableassign(m_gen, "", 1, aID);
		}

		std::vector<std::string> names;
		names.push_back(sprite->name);
		unique.insert(std::make_pair(id, names));
		order.push_back(std::make_pair(id, sprite->name));
	}
	else
	{
		int i = 0;
		for (int n = itr->second.size(); i < n; ++i)
			if (itr->second[i] == sprite->name)
				break;
		if (i == itr->second.size() && !isFont)
		{
			std::string aID = lua::assign("id", ee::StringHelper::ToString(id));
			if (!sprite->name.empty() && sprite->name[0] != '_')
			{
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				lua::tableassign(m_gen, "", 2, aName, aID);
			}
			else
			{
				lua::tableassign(m_gen, "", 1, aID);
			}

			order.push_back(std::make_pair(id, sprite->name));
		}
		itr->second.push_back(sprite->name);
	}
}

void CocoPacker::resolveSpriteForFrame(const ee::Sprite* sprite, int index,
									   const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order)
{
	int id = ids[index];
	int cindex = -1;
	for (size_t i = 0, n = order.size(); i < n; ++i)
		if (id == order[i].first && sprite->name == order[i].second)
		{
			cindex = i;
			break;
		}
		assert(cindex != -1);

	if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(sprite))
		resolveSpriteForFrameFont(font, cindex);
	else
		resolveSpriteForFrameImage(sprite, cindex);
}

void CocoPacker::resolveSpriteForFrame(const ee::Sprite* sprite, const std::vector<std::pair<int, std::string> >& order)
{
	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->GetSymbol());
	assert(itr != m_mapSymbolID.end());
	if (itr == m_mapSymbolID.end())
		throw ee::Exception("Error! COCCode::resolveSpriteForFrame L822");

	int id = itr->second;
	int cindex = -1;
	for (size_t i = 0, n = order.size(); i < n; ++i)
		if (id == order[i].first && sprite->name == order[i].second)
		{
			cindex = i;
			break;
		}

		if (cindex == -1)
		{
			for (size_t i = 0, n = order.size(); i < n; ++i)
				if (id == order[i].first)
				{
					cindex = i;
					break;
				}
		}

		assert(cindex != -1);
		if (cindex == -1)
			throw ee::Exception("Error! COCCode::resolveSpriteForFrame L845");

		resolveSpriteForFrame(sprite, cindex, true);
}

void CocoPacker::resolveSpriteForFrame(const ee::Sprite* spr, int id, bool forceMat)
{
	std::string assignIndex = lua::assign("index", ee::StringHelper::ToString(id));

	float mat[6];
	transToMat(spr, mat, forceMat);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = ee::StringHelper::ToString(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);

	if (spr->rp->color.multi != ee::Colorf(1,1,1,1) || spr->rp->color.add != ee::Colorf(0,0,0,0))
	{
		std::string assignColor = lua::assign("color", ee::TransColor(spr->rp->color.multi, ee::PT_BGRA));
		std::string assignAdd = lua::assign("add", ee::TransColor(spr->rp->color.add, ee::PT_ABGR));
		lua::tableassign(m_gen, "", 4, assignIndex, assignColor, assignAdd, assignMat);
	}
	else
		lua::tableassign(m_gen, "", 2, assignIndex, assignMat);
}

void CocoPacker::resolveSpriteForFrameImage(const ee::Sprite* spr, int id)
{
	std::string assignIndex = lua::assign("index", ee::StringHelper::ToString(id));

	float mat[6];
	transToMat(spr, mat, false);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = ee::StringHelper::ToString(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);

	if (spr->rp->color.multi != ee::Colorf(1,1,1,1) || spr->rp->color.add != ee::Colorf(0,0,0,0))
	{
		std::string assignColor = lua::assign("color", ee::TransColor(spr->rp->color.multi, ee::PT_BGRA));
		std::string assignAdd = lua::assign("add", ee::TransColor(spr->rp->color.add, ee::PT_ABGR));
		if (spr->clip)
			lua::tableassign(m_gen, "", 5, assignIndex, assignColor, assignAdd, assignMat, "clip=true");
		else
			lua::tableassign(m_gen, "", 4, assignIndex, assignColor, assignAdd, assignMat);
	}
	else
	{
		if (spr->clip)
			lua::tableassign(m_gen, "", 3, assignIndex, assignMat, "clip=true");
		else
			lua::tableassign(m_gen, "", 2, assignIndex, assignMat);
	}
}

void CocoPacker::resolveSpriteForFrameFont(const ee::FontBlankSprite* sprite, int id)
{
	std::string assignIndex = lua::assign("index", ee::StringHelper::ToString(id));

	float mat[6];
	transToMat(sprite, mat, true);

	bool isNullNode = sprite->font.empty() && sprite->font_color == ee::Colorf(0, 0, 0, 0);
	if (!isNullNode)
	{
		// flip y
 		// move to left-top
  		mat[4] -= floor(sprite->width * 0.5f * 16 + 0.5f);
  		mat[5] -= floor(sprite->height * 0.5f * 16 + 0.5f);

		//mat[4] -= floor(sprite->getBounding()->width() * 0.5f * 16 + 0.5f);
		//mat[5] -= floor(sprite->getBounding()->height() * 0.5f * 16 + 0.5f);
	}

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = ee::StringHelper::ToString(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);

	lua::tableassign(m_gen, "", 2, assignIndex, assignMat);
}

void CocoPacker::transToMat(const ee::Sprite* sprite, float mat[6], bool force /*= false*/)
{
	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	if (!force &&
		(dynamic_cast<const ee::ImageSprite*>(sprite) ||
		dynamic_cast<const ee::FontBlankSprite*>(sprite)))
	{
	}
	else
	{
		// | 1  kx    | | sx       | |  c  s    | | 1       |
		// | ky  1    | |    sy    | | -s  c    | |    1    |
		// |        1 | |        1 | |        1 | | x  y  1 |
		//     skew        scale        rotate        move
		float x = sprite->GetCenter().x,
			y = sprite->GetCenter().y;
		float sx = sprite->GetScale().x,
			sy = sprite->GetScale().y;
		bool xMirror, yMirror;
		sprite->GetMirror(xMirror, yMirror);
		if (xMirror) sx = -sx;
		if (yMirror) sy = -sy;
		float c = cos(-sprite->GetAngle()),
			s = sin(-sprite->GetAngle());
		float kx = sprite->GetShear().x,
			ky = sprite->GetShear().y;
		mat[0] = sx*c - kx*sy*s;
		mat[1] = sx*s + kx*sy*c;
		mat[2] = ky*sx*c - sy*s;
		mat[3] = ky*sx*s + sy*c;
		mat[4] = x;
		mat[5] = y;
	}

	for (size_t i = 0; i < 4; ++i)
		mat[i] = floor(mat[i] * 1024 + 0.5f);
	for (size_t i = 4; i < 6; ++i)
		mat[i] = floor(mat[i] * 16 + 0.5f);
	// flip y
	mat[5] = -mat[5];
}

}
}