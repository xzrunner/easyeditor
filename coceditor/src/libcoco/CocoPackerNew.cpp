#include "CocoPackerNew.h"
#include "TexturePacker.h"

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

namespace libcoco
{

namespace lua = ebuilder::lua;

CocoPackerNew::CocoPackerNew(const TexturePacker& tex)
	: m_tex(tex)
	, m_id(0)
{
}

void CocoPackerNew::pack(const std::vector<const d2d::ISymbol*>& symbols)
{
	lua::TableAssign ta(m_gen, "return", false, false);

	for (int i = 0, n = symbols.size(); i < n; ++i)
	{
		const d2d::ISymbol* symbol = symbols[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol))
		{
			for (size_t i = 0, n = complex->m_sprites.size(); i < n; ++i)
			{
				d2d::ISprite* sprite = complex->m_sprites[i];
				if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
					resolvePicture(image);
				}
				else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
					resolveFont(font);
				}
			}

			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			resolveAnimation(complex);
		}
		else if (const anim::Symbol* anim = dynamic_cast<const anim::Symbol*>(symbol))
		{
			std::set<const d2d::ImageSymbol*> unique;
			for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			{
				anim::Symbol::Layer* layer = anim->m_layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					anim::Symbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
					{
						d2d::ISprite* sprite = frame->sprites[k];
						if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
							unique.insert(&image->getSymbol());
						else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(sprite))
							m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
					}
				}
			}

			std::set<const d2d::ImageSymbol*>::iterator itr = unique.begin();
			for ( ; itr != unique.end(); ++itr)
			{
				std::map<const d2d::ISymbol*, int>::iterator itrFind = m_mapSymbolID.find(*itr);
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

 			std::vector<d2d::ISprite*> sprites;
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
 				d2d::ISprite* sprite = sprites[i];
 				if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
 				{
 					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
 					resolvePicture(image);
 				}
 				else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(sprite))
 				{
 					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
 				}
 			}
 
 			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
 			resolveAnimation(patch9);
		}
	}
}

void CocoPackerNew::storeToFile(const std::string& filename)
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filename.c_str());
	std::locale::global(std::locale("C"));	
//	assert(!fout.fail());
	fout << m_gen.toText() << std::endl;
	fout.close();
}

void CocoPackerNew::resolvePicture(const d2d::ImageSprite* sprite)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"picture\"") + ",");

	// id
	{
		std::map<const d2d::ISprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		std::string sid = wxString::FromDouble(itr->second);
		m_gen.line(lua::assign("id", sid.c_str()) + ",");
	}

	// tex
	// todo: specify only 1 texture
	std::string assignTex = lua::assign("tex", wxString::FromDouble(1).ToStdString());
	const d2d::Rect* pr = m_tex.query(sprite->getSymbol().getImage());

	// src
	int x0 = pr->xMin, y0 = pr->yMax;
	int x1 = pr->xMin, y1 = pr->yMin;
	int x2 = pr->xMax, y2 = pr->yMin;
	int x3 = pr->xMax, y3 = pr->yMax;

	std::string sx0 = wxString::FromDouble(x0), sy0 = wxString::FromDouble(y0);
	std::string sx1 = wxString::FromDouble(x1), sy1 = wxString::FromDouble(y1);
	std::string sx2 = wxString::FromDouble(x2), sy2 = wxString::FromDouble(y2);
	std::string sx3 = wxString::FromDouble(x3), sy3 = wxString::FromDouble(y3);
	std::string assignSrc = lua::assign("src", lua::tableassign("", 8, sx0.c_str(), sy0.c_str(), 
		sx1.c_str(), sy1.c_str(), sx2.c_str(), sy2.c_str(), sx3.c_str(), sy3.c_str()));

	// screen
	const float hw = pr->xLength() * 0.5f,
		hh = pr->yLength() * 0.5f;
	d2d::Vector screen[4];
	screen[0].set(-hw, hh);
	screen[1].set(-hw, -hh);
	screen[2].set(hw, -hh);
	screen[3].set(hw, hh);
	// 1. scale
	for (size_t i = 0; i < 4; ++i)
		screen[i].x *= sprite->getScale().x;
	for (size_t i = 0; i < 4; ++i)
		screen[i].y *= sprite->getScale().y;
	// 2. rotate
	for (size_t i = 0; i < 4; ++i)
	{
		d2d::Vector rot = d2d::Math::rotateVector(screen[i], sprite->getAngle());
		screen[i] = rot;
	}
	// 3. translate
	d2d::Vector center = sprite->getCenter();
 	for (size_t i = 0; i < 4; ++i)
 		screen[i] += center;
	//for (size_t i = 0; i < 4; ++i)
	//{
	//	screen[i].x += center.x;
	//	screen[i].y -= center.y;
	//}
	// 4. mirror
	bool xMirror, yMirror;
	sprite->getMirror(xMirror, yMirror);
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

	sx0 = wxString::FromDouble(screen[0].x);
	sy0 = wxString::FromDouble(screen[0].y);
	sx1 = wxString::FromDouble(screen[1].x);
	sy1 = wxString::FromDouble(screen[1].y);
	sx2 = wxString::FromDouble(screen[2].x);
	sy2 = wxString::FromDouble(screen[2].y);
	sx3 = wxString::FromDouble(screen[3].x);
	sy3 = wxString::FromDouble(screen[3].y);
	std::string assignScreen = lua::assign("screen", lua::tableassign("", 8, sx0.c_str(), sy0.c_str(), 
		sx1.c_str(), sy1.c_str(), sx2.c_str(), sy2.c_str(), sx3.c_str(), sy3.c_str()));

	lua::tableassign(m_gen, "", 3, assignTex.c_str(), assignSrc.c_str(), assignScreen.c_str());
}

void CocoPackerNew::resolvePicture(const d2d::ImageSymbol* symbol)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"picture\"") + ",");

	// id
	{
		std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
		assert(itr != m_mapSymbolID.end());
		std::string sid = wxString::FromDouble(itr->second);
		m_gen.line(lua::assign("id", sid.c_str()) + ",");
	}

	// tex
	// todo: specify only 1 texture
	std::string assignTex = lua::assign("tex", wxString::FromDouble(1).ToStdString());
	const d2d::Rect* pr = m_tex.query(symbol->getImage());

	// src
	int x0 = pr->xMin, y0 = pr->yMax;
	int x1 = pr->xMin, y1 = pr->yMin;
	int x2 = pr->xMax, y2 = pr->yMin;
	int x3 = pr->xMax, y3 = pr->yMax;

	std::string sx0 = wxString::FromDouble(x0), sy0 = wxString::FromDouble(y0);
	std::string sx1 = wxString::FromDouble(x1), sy1 = wxString::FromDouble(y1);
	std::string sx2 = wxString::FromDouble(x2), sy2 = wxString::FromDouble(y2);
	std::string sx3 = wxString::FromDouble(x3), sy3 = wxString::FromDouble(y3);
	std::string assignSrc = lua::assign("src", lua::tableassign("", 8, sx0.c_str(), sy0.c_str(), 
		sx1.c_str(), sy1.c_str(), sx2.c_str(), sy2.c_str(), sx3.c_str(), sy3.c_str()));

	// screen
	const float hw = pr->xLength() * 0.5f,
		hh = pr->yLength() * 0.5f;
	d2d::Vector screen[4];
	screen[0].set(-hw, hh);
	screen[1].set(-hw, -hh);
	screen[2].set(hw, -hh);
	screen[3].set(hw, hh);

	// flip y
	for (size_t i = 0; i < 4; ++i)
		screen[i].y = -screen[i].y;
	// finish: scale
	const float SCALE = 16;
	for (size_t i = 0; i < 4; ++i)
		screen[i] *= SCALE;
	//
	sx0 = wxString::FromDouble(screen[0].x);
	sy0 = wxString::FromDouble(screen[0].y);
	sx1 = wxString::FromDouble(screen[1].x);
	sy1 = wxString::FromDouble(screen[1].y);
	sx2 = wxString::FromDouble(screen[2].x);
	sy2 = wxString::FromDouble(screen[2].y);
	sx3 = wxString::FromDouble(screen[3].x);
	sy3 = wxString::FromDouble(screen[3].y);
	std::string assignScreen = lua::assign("screen", lua::tableassign("", 8, sx0.c_str(), sy0.c_str(), 
		sx1.c_str(), sy1.c_str(), sx2.c_str(), sy2.c_str(), sx3.c_str(), sy3.c_str()));

	lua::tableassign(m_gen, "", 3, assignTex.c_str(), assignSrc.c_str(), assignScreen.c_str());
}

void CocoPackerNew::resolveFont(const d2d::FontSprite* sprite)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"label\"") + ",");

	// id
	{
		std::map<const d2d::ISprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		std::string sid = wxString::FromDouble(itr->second);
		m_gen.line(lua::assign("id", sid.c_str()) + ",");
	}

	std::string aFont = lua::assign("font", "\""+sprite->font+"\"");
	std::string aColor = lua::assign("color", transColor(sprite->color, d2d::PT_ARGB));
//	std::string aAlign = lua::assign("align", wxString::FromDouble(sprite->align).ToStdString());
	std::string aAlign = lua::assign("align", wxString::FromDouble(sprite->align_hori).ToStdString());
	std::string aSize = lua::assign("size", wxString::FromDouble(sprite->size).ToStdString());
	std::string aWidth = lua::assign("width", wxString::FromDouble(sprite->width).ToStdString());
	std::string aHeight = lua::assign("height", wxString::FromDouble(sprite->height).ToStdString());
	std::string aEdge = lua::assign("noedge", "false");

	m_gen.line(aFont + ", " + aColor + ", " + aAlign + ", " + aSize + ", " + aWidth + ", " + aHeight + ", " + aEdge);
}

void CocoPackerNew::resolveAnimation(const ecomplex::Symbol* symbol)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"animation\"") + ",");

	// export
	if (!symbol->name.empty())
		m_gen.line(lua::assign("export", "\""+symbol->name+"\"")+",");

	// id
	std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
	assert(itr != m_mapSymbolID.end());
	std::string sid = wxString::FromDouble(itr->second);
	m_gen.line(lua::assign("id", sid.c_str()) + ",");

	// clipbox
	const d2d::Rect& cb = symbol->m_clipbox;
	if (cb.xMin != 0 || cb.xMax != 0 || cb.yMin != 0 || cb.yMax != 0)
	{
		std::string width = wxString::FromDouble(cb.xMax - cb.xMin);
		std::string height = wxString::FromDouble(cb.yMax - cb.yMin);
		std::string left = wxString::FromDouble(cb.xMin);
		std::string top = wxString::FromDouble(-cb.yMax);
		lua::tableassign(m_gen, "clipbox", 4, width.c_str(), height.c_str(), left.c_str(), top.c_str());
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
	std::map<std::string, std::vector<d2d::ISprite*> > map_actions;
	std::vector<d2d::ISprite*> others;
	for (int i = 0, n = symbol->m_sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = symbol->m_sprites[i];
		if (sprite->tag.empty())
			others.push_back(sprite);
		else
		{
			std::vector<std::string> tags;

			// split
			char * cstr = new char [sprite->tag.length()+1];
			std::strcpy(cstr, sprite->tag.c_str());
			char * p = std::strtok(cstr,";");
			while (p!=0)
			{
				tags.push_back(std::string(p));
				p = strtok(NULL,";");
			}
			delete[] cstr;

			for (int i = 0, n = tags.size(); i < n; ++i)
			{
				std::map<std::string, std::vector<d2d::ISprite*> >::iterator itr = 
					map_actions.find(tags[i]);
				if (itr == map_actions.end())
				{
					std::vector<d2d::ISprite*> sprites;
					sprites.push_back(sprite);
					map_actions.insert(std::make_pair(tags[i], sprites));
				}
				else
				{
					itr->second.push_back(sprite);
				}
			}
		}
	}

	if (!map_actions.empty())
	{
		std::map<std::string, std::vector<d2d::ISprite*> >::iterator itr;
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

void CocoPackerNew::resolveAnimation(const anim::Symbol* symbol)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"animation\"") + ",");

	// export
	if (!symbol->name.empty())
		m_gen.line(lua::assign("export", "\""+symbol->name+"\"")+",");

	// id
	std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
	assert(itr != m_mapSymbolID.end());
	std::string sid = wxString::FromDouble(itr->second);
	m_gen.line(lua::assign("id", sid.c_str()) + ",");

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
				anim::Symbol::Layer* layer = symbol->m_layers[j];
				if (i < layer->frames.size())
				{
					anim::Symbol::Frame* frame = layer->frames[i];
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

			std::vector<d2d::ISprite*> sprites;
			anim::Tools::getCurrSprites(symbol, i, sprites);
			for (size_t j = 0, m = sprites.size(); j < m; ++j)
				resolveSpriteForFrame(sprites[j], order);
		}
	}
}

void CocoPackerNew::resolveAnimation(const escale9::Symbol* symbol)
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
 	std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
 	assert(itr != m_mapSymbolID.end());
 	std::string sid = wxString::FromDouble(itr->second);
 	m_gen.line(lua::assign("id", sid.c_str()) + ",");
 
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

void CocoPackerNew::resolveSpriteForComponent(const d2d::ISprite* sprite, std::vector<int>& ids, 
										   std::map<int, std::vector<std::string> >& unique, 
										   std::vector<std::pair<int, std::string> >& order)
{
	int id;

	bool isFont = false;

	if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(sprite))
	{
		std::map<const d2d::ISprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		if (itr != m_mapSpriteID.end())
		{
			id = itr->second;
		}
		else
		{
			// anim::Symbol's sprites store unique
			std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->getSymbol());
			assert(itr != m_mapSymbolID.end());
			id = itr->second;
		}
	}
	else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(sprite))
	{
		isFont = true;
		std::map<const d2d::ISprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		id = itr->second;
	}
	else
	{
		std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->getSymbol());
		assert(itr != m_mapSymbolID.end());
		id = itr->second;
	}

	ids.push_back(id);

	std::map<int, std::vector<std::string> >::iterator itr = unique.find(id);
	if (unique.find(id) == unique.end())
	{
		std::string aID = lua::assign("id", wxString::FromDouble(id).ToStdString());
		if (!sprite->name.empty() && sprite->name[0] != '_')
		{
			std::string aName = lua::assign("name", "\""+sprite->name+"\"");
			lua::tableassign(m_gen, "", 2, aName.c_str(), aID.c_str());
		}
		else
		{
			lua::tableassign(m_gen, "", 1, aID.c_str());
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
			std::string aID = lua::assign("id", wxString::FromDouble(id).ToStdString());
			if (!sprite->name.empty() && sprite->name[0] != '_')
			{
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				lua::tableassign(m_gen, "", 2, aName.c_str(), aID.c_str());
			}
			else
			{
				lua::tableassign(m_gen, "", 1, aID.c_str());
			}

			order.push_back(std::make_pair(id, sprite->name));
		}
		itr->second.push_back(sprite->name);
	}
}

void CocoPackerNew::resolveSpriteForFrame(const d2d::ISprite* sprite, int index,
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

	if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(sprite))
		resolveSpriteForFrameFont(font, cindex);
	else
		resolveSpriteForFrameImage(sprite, cindex);
}

void CocoPackerNew::resolveSpriteForFrame(const d2d::ISprite* sprite, const std::vector<std::pair<int, std::string> >& order)
{
	std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->getSymbol());
	assert(itr != m_mapSymbolID.end());
	if (itr == m_mapSymbolID.end())
		throw d2d::Exception("Error! COCCode::resolveSpriteForFrame L822");

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
			throw d2d::Exception("Error! COCCode::resolveSpriteForFrame L845");

		resolveSpriteForFrame(sprite, cindex, true);
}

void CocoPackerNew::resolveSpriteForFrame(const d2d::ISprite* sprite, int id, bool forceMat)
{
	std::string assignIndex = lua::assign("index", wxString::FromDouble(id).ToStdString());

	float mat[6];
	transToMat(sprite, mat, forceMat);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = wxString::FromDouble(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0].c_str(), m[1].c_str(), m[2].c_str(), 
		m[3].c_str(), m[4].c_str(), m[5].c_str());
	std::string assignMat = lua::assign("mat", smat);

	if (sprite->multiCol != d2d::Colorf(1,1,1,1) || sprite->addCol != d2d::Colorf(0,0,0,0))
	{
		std::string assignColor = lua::assign("color", d2d::transColor(sprite->multiCol, d2d::PT_BGRA));
		std::string assignAdd = lua::assign("add", d2d::transColor(sprite->addCol, d2d::PT_ABGR));
		lua::tableassign(m_gen, "", 4, assignIndex.c_str(), assignColor.c_str(), assignAdd.c_str(), assignMat.c_str());
	}
	else
		lua::tableassign(m_gen, "", 2, assignIndex.c_str(), assignMat.c_str());
}

void CocoPackerNew::resolveSpriteForFrameImage(const d2d::ISprite* sprite, int id)
{
	std::string assignIndex = lua::assign("index", wxString::FromDouble(id).ToStdString());

	float mat[6];
	transToMat(sprite, mat, false);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = wxString::FromDouble(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0].c_str(), m[1].c_str(), m[2].c_str(), 
		m[3].c_str(), m[4].c_str(), m[5].c_str());
	std::string assignMat = lua::assign("mat", smat);

	if (sprite->multiCol != d2d::Colorf(1,1,1,1) || sprite->addCol != d2d::Colorf(0,0,0,0))
	{
		std::string assignColor = lua::assign("color", d2d::transColor(sprite->multiCol, d2d::PT_BGRA));
		std::string assignAdd = lua::assign("add", d2d::transColor(sprite->addCol, d2d::PT_ABGR));
		if (sprite->clip)
			lua::tableassign(m_gen, "", 5, assignIndex.c_str(), assignColor.c_str(), assignAdd.c_str(), assignMat.c_str(), "clip=true");
		else
			lua::tableassign(m_gen, "", 4, assignIndex.c_str(), assignColor.c_str(), assignAdd.c_str(), assignMat.c_str());
	}
	else
	{
		if (sprite->clip)
			lua::tableassign(m_gen, "", 3, assignIndex.c_str(), assignMat.c_str(), "clip=true");
		else
			lua::tableassign(m_gen, "", 2, assignIndex.c_str(), assignMat.c_str());
	}
}

void CocoPackerNew::resolveSpriteForFrameFont(const d2d::FontSprite* sprite, int id)
{
	std::string assignIndex = lua::assign("index", wxString::FromDouble(id).ToStdString());

	float mat[6];
	transToMat(sprite, mat, true);

	bool isNullNode = sprite->font.empty() && sprite->color == d2d::Colorf(0, 0, 0, 0);
	if (!isNullNode)
	{
		// flip y
 		// move to left-top
  		mat[4] -= (int)(sprite->width * 0.5f * 16 + 0.5f);
  		mat[5] -= (int)(sprite->height * 0.5f * 16 + 0.5f);

		//mat[4] -= (int)(sprite->getBounding()->width() * 0.5f * 16 + 0.5f);
		//mat[5] -= (int)(sprite->getBounding()->height() * 0.5f * 16 + 0.5f);
	}

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = wxString::FromDouble(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0].c_str(), m[1].c_str(), m[2].c_str(), 
		m[3].c_str(), m[4].c_str(), m[5].c_str());
	std::string assignMat = lua::assign("mat", smat);

	lua::tableassign(m_gen, "", 2, assignIndex.c_str(), assignMat.c_str());
}

void CocoPackerNew::transToMat(const d2d::ISprite* sprite, float mat[6], bool force /*= false*/)
{
	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	if (!force &&
		(dynamic_cast<const d2d::ImageSprite*>(sprite) ||
		dynamic_cast<const d2d::FontSprite*>(sprite)))
	{
	}
	else
	{
		// | 1  kx    | | sx       | |  c  s    | | 1       |
		// | ky  1    | |    sy    | | -s  c    | |    1    |
		// |        1 | |        1 | |        1 | | x  y  1 |
		//     skew        scale        rotate        move
		float x = sprite->getCenter().x,
			y = sprite->getCenter().y;
		float sx = sprite->getScale().x,
			sy = sprite->getScale().y;
		bool xMirror, yMirror;
		sprite->getMirror(xMirror, yMirror);
		if (xMirror) sx = -sx;
		if (yMirror) sy = -sy;
		float c = cos(-sprite->getAngle()),
			s = sin(-sprite->getAngle());
		float kx = sprite->getShear().x,
			ky = sprite->getShear().y;
		mat[0] = sx*c - kx*sy*s;
		mat[1] = sx*s + kx*sy*c;
		mat[2] = ky*sx*c - sy*s;
		mat[3] = ky*sx*s + sy*c;
		mat[4] = x;
		mat[5] = y;
	}

	for (size_t i = 0; i < 4; ++i)
		mat[i] = (int)(mat[i] * 1024 + 0.5f);
	for (size_t i = 4; i < 6; ++i)
		mat[i] = (int)(mat[i] * 16 + 0.5f);
	// flip y
	mat[5] = -mat[5];
}

}