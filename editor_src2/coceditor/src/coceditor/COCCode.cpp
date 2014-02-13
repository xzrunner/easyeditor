#include "COCCode.h"
#include "COCParser.h"

#include "Context.h"
#include "StagePanel.h"

#include <queue>

#include <easyanim.h>

namespace coceditor
{

namespace lua = ebuilder::lua;

COCCode::COCCode(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
	, m_id(0)
{
}

void COCCode::resolve()
{
	COCParser parser;
	parser.parser();

	resolveFromParser(parser);
}

void COCCode::resolveFromParser(const COCParser& parser)
{
	size_t size = 0;
	const TexturesMgr& texMgr = Context::Instance()->texMgr;
	for (size_t i = 0; i < TexturesMgr::NUM; ++i)
		if (texMgr.textures[i])
			++size;
	m_gen.line("texture("+wxString::FromDouble(size).ToStdString()+")");

	for (size_t i = 0, n = parser.m_outList.size(); i < n; ++i)
	{
		const d2d::ISymbol* symbol = parser.m_outList[i];
		if (const d2d::ImageSymbol* image = dynamic_cast<const d2d::ImageSymbol*>(symbol))
		{
		}
		else if (const d2d::FontBlankSymbol* font = dynamic_cast<const d2d::FontBlankSymbol*>(symbol))
		{
		}
		else if (const complex::Symbol* complex = dynamic_cast<const complex::Symbol*>(symbol))
		{
			for (size_t i = 0, n = complex->m_sprites.size(); i < n; ++i)
			{
				d2d::ISprite* sprite = complex->m_sprites[i];
				if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));

					if (image->getSymbol().getWidth() == 4 && image->getSymbol().getHeight() == 4)
						resolvePicture(image, parser, e_bothfix);
					else
						resolvePicture(image, parser);
				}
				else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
				}
			}

			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			resolveAnimation(complex);
		}
		else if (const d2d::AnimSymbol* anim = dynamic_cast<const d2d::AnimSymbol*>(symbol))
		{
			////////////////////////////////////////////////////////////////////////////
			//// version 1: parser all sprites to picture
			////////////////////////////////////////////////////////////////////////////

			//for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			//{
			//	d2d::AnimSymbol::Layer* layer = anim->m_layers[i];
			//	for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
			//	{
			//		d2d::AnimSymbol::Frame* frame = layer->frames[j];
			//		for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
			//		{
			//			d2d::ISprite* sprite = frame->sprites[k];
			//			if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
			//			{
			//				m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
			//				resolvePicture(image, parser);
			//			}
			//			else if (d2d::FontSprite* font = dynamic_cast<d2d::FontSprite*>(sprite))
			//			{
			//				m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
			//			}
			//		}
			//	}
			//}

			//m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			//resolveAnimation(anim);

			//////////////////////////////////////////////////////////////////////////
			// version 2: use mat instead of each picture
			//////////////////////////////////////////////////////////////////////////

			std::set<const d2d::ImageSymbol*> unique;
			for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			{
				d2d::AnimSymbol::Layer* layer = anim->m_layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					d2d::AnimSymbol::Frame* frame = layer->frames[j];
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
					if ((*itr)->getWidth() <= 4)
						resolvePicture(*itr, parser, e_xfix);
					else
						resolvePicture(*itr, parser);
				}
			}

			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			resolveAnimation(anim);
		}
		else if (const d2d::Patch9Symbol* patch9 = dynamic_cast<const d2d::Patch9Symbol*>(symbol))
		{
			std::vector<d2d::ISprite*> sprites;
			switch (patch9->type())
			{
			case d2d::Patch9Symbol::e_9Grid:
				for (size_t i = 0; i < 3; ++i)
					for (size_t j = 0; j < 3; ++j)
						sprites.push_back(patch9->m_sprites[i][j]);
				break;
			case d2d::Patch9Symbol::e_9GridHollow:
				for (size_t i = 0; i < 3; ++i) {
					for (size_t j = 0; j < 3; ++j) {
						if (i == 1 && j == 1) continue;
						sprites.push_back(patch9->m_sprites[i][j]);
					}
				}
				break;
			case d2d::Patch9Symbol::e_3GridHor:
				for (size_t i = 0; i < 3; ++i)
					sprites.push_back(patch9->m_sprites[1][i]);
				break;
			case d2d::Patch9Symbol::e_3GridVer:
				for (size_t i = 0; i < 3; ++i)
					sprites.push_back(patch9->m_sprites[i][1]);
				break;
			case d2d::Patch9Symbol::e_6GridUpper:
				for (size_t i = 1; i < 3; ++i)
					for (size_t j = 0; j < 3; ++j)
						sprites.push_back(patch9->m_sprites[i][j]);
				break;
			}

			for (size_t i = 0, n = sprites.size(); i < n; ++i)
			{
				d2d::ISprite* sprite = sprites[i];
				if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
				{
					PicFixType tsrc = e_null, tscreen = e_null;
					switch (patch9->type())
					{
						case d2d::Patch9Symbol::e_9Grid:
							if (sprite == patch9->m_sprites[1][1]) 
							{
								tsrc = e_bothfix;
								tscreen = e_bothfix;
							}
							else if (sprite == patch9->m_sprites[1][0]) tsrc = e_yfix;
							else if (sprite == patch9->m_sprites[1][2]) tsrc = e_yfix;
							else if (sprite == patch9->m_sprites[0][1]) tsrc = e_xfix;
							else if (sprite == patch9->m_sprites[2][1]) tsrc = e_xfix;
							break;
						case d2d::Patch9Symbol::e_9GridHollow:
							if (sprite == patch9->m_sprites[1][0]) tsrc = e_yfix;
							else if (sprite == patch9->m_sprites[1][2]) tsrc = e_yfix;
							else if (sprite == patch9->m_sprites[0][1]) tsrc = e_xfix;
							else if (sprite == patch9->m_sprites[2][1]) tsrc = e_xfix;
							break;
						case d2d::Patch9Symbol::e_3GridHor:
							if (sprite == patch9->m_sprites[1][1]) 
							{
								tsrc = e_xfix;
								tscreen = e_xfix;
							}
							break;
						case d2d::Patch9Symbol::e_3GridVer:
							if (sprite == patch9->m_sprites[1][1]) 
							{
								tsrc = e_yfix;
								tscreen = e_yfix;
							}
							break;
						case d2d::Patch9Symbol::e_6GridUpper:
							if (sprite == patch9->m_sprites[1][1]) 
							{
								tsrc = e_bothfix;
								tscreen = e_bothfix;
							}
							else if (sprite == patch9->m_sprites[1][0]) tsrc = e_yfix;
							else if (sprite == patch9->m_sprites[1][2]) tsrc = e_yfix;
							else if (sprite == patch9->m_sprites[2][1]) tsrc = e_xfix;
							break;
					}

					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
					resolvePicture(image, parser, tsrc, tscreen);
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

void COCCode::resolvePicture(const d2d::ImageSprite* sprite, const COCParser& parser,
							 PicFixType tsrc /*= e_null*/, PicFixType tscreen/* = e_null*/)
{
	tsrc = tscreen = e_null;

	std::map<const d2d::ISymbol*, COCParser::Picture*>::const_iterator itr 
		= parser.m_mapSymbolPicture.find(&sprite->getSymbol());
	assert(itr != parser.m_mapSymbolPicture.end());
	if (itr == parser.m_mapSymbolPicture.end())
		throw d2d::Exception("Error! COCCode::resolvePicture L232");

	COCParser::Picture* picture = itr->second;

	lua::TableAssign ta(m_gen, "picture", false, false);

	// id
	{
		std::map<const d2d::ISprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		if (itr == m_mapSpriteID.end())
			throw d2d::Exception("Error! COCCode::resolvePicture L243");

		std::string sid = wxString::FromDouble(itr->second);
		m_gen.line(lua::assign("id", sid.c_str()) + ",");
	}

	// tex
	std::string assignTex = lua::assign("tex", wxString::FromDouble(picture->tex).ToStdString());

	// src
	int x0 = picture->scr[0].x, y0 = picture->scr[0].y;
	int x1 = picture->scr[1].x, y1 = picture->scr[1].y;
	int x2 = picture->scr[2].x, y2 = picture->scr[2].y;
	int x3 = picture->scr[3].x, y3 = picture->scr[3].y;

	if (tsrc == e_bothfix)
	{
		++x0; ++x1; --x2; --x3;
		--y1; --y2; ++y0; ++y3;
	}
	else if (tsrc == e_yfix || tsrc == e_xfix)
	{
		if (picture->entry->rotated)
		{
			if (abs(x0 - x1) < abs(y1 - y2))
			{
				--x0; --x3; ++x1; ++x2;
			}
			else
			{
				++y0; ++y1; --y2; --y3;
			}
		}
		else
		{
			if (abs(x1 - x2) < abs(y1 - y0))
			{
				++x0; ++x1; --x2; --x3;
			}
			else
			{
				--y1; --y2; ++y0; ++y3;
			}
		}
	}

	std::string sx0 = wxString::FromDouble(x0), sy0 = wxString::FromDouble(y0);
	std::string sx1 = wxString::FromDouble(x1), sy1 = wxString::FromDouble(y1);
	std::string sx2 = wxString::FromDouble(x2), sy2 = wxString::FromDouble(y2);
	std::string sx3 = wxString::FromDouble(x3), sy3 = wxString::FromDouble(y3);
	std::string assignSrc = lua::assign("src", lua::tableassign("", 8, sx0.c_str(), sy0.c_str(), 
		sx1.c_str(), sy1.c_str(), sx2.c_str(), sy2.c_str(), sx3.c_str(), sy3.c_str()));

	// screen
	const float hw = picture->entry->spriteSourceSize.w * 0.5f / Context::Instance()->scale,
		hh = picture->entry->spriteSourceSize.h * 0.5f / Context::Instance()->scale;
	d2d::Vector screen[4];
	screen[0].set(-hw, hh);
	screen[1].set(-hw, -hh);
	screen[2].set(hw, -hh);
	screen[3].set(hw, hh);
 	for (size_t i = 0; i < 4; ++i)
 		screen[i] += picture->offset / Context::Instance()->scale;
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
	// fix
	const int FIX = 6;
	if (tsrc == e_bothfix)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			if (screen[i].x < 0) screen[i].x -= FIX;
			else screen[i].x += FIX;
			if (screen[i].y < 0) screen[i].y -= FIX;
			else screen[i].y += FIX;
		}
	}
	else if (tsrc == e_xfix)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			if (screen[i].x < 0) screen[i].x -= FIX;
			else screen[i].x += FIX;
		}
	}
	else if (tsrc == e_yfix)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			if (screen[i].y < 0) screen[i].y -= FIX;
			else screen[i].y += FIX;
		}
	}

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

void COCCode::resolvePicture(const d2d::ImageSymbol* symbol, const COCParser& parser,
							 PicFixType tsrc/* = e_null*/)
{
	tsrc = e_null;

	std::map<const d2d::ISymbol*, COCParser::Picture*>::const_iterator itr 
		= parser.m_mapSymbolPicture.find(symbol);
	assert(itr != parser.m_mapSymbolPicture.end());
	if (itr == parser.m_mapSymbolPicture.end())
		throw d2d::Exception("Error! COCCode::resolvePicture L390");

	COCParser::Picture* picture = itr->second;

	lua::TableAssign ta(m_gen, "picture", false, false);

	// id
	{
		std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
		assert(itr != m_mapSymbolID.end());
		if (itr == m_mapSymbolID.end())
			throw d2d::Exception("Error! COCCode::resolvePicture L401");

		std::string sid = wxString::FromDouble(itr->second);
		m_gen.line(lua::assign("id", sid.c_str()) + ",");
	}

	// tex
	std::string assignTex = lua::assign("tex", wxString::FromDouble(picture->tex).ToStdString());

	// src
	int x0 = picture->scr[0].x, y0 = picture->scr[0].y;
	int x1 = picture->scr[1].x, y1 = picture->scr[1].y;
	int x2 = picture->scr[2].x, y2 = picture->scr[2].y;
	int x3 = picture->scr[3].x, y3 = picture->scr[3].y;

	if (tsrc == e_bothfix)
	{
		++x0; ++x1; --x2; --x3;
		--y1; --y2; ++y0; ++y3;
	}
	else if (tsrc == e_yfix || tsrc == e_xfix)
	{
		if (picture->entry->rotated)
		{
			if (abs(x0 - x1) < abs(y1 - y2))
			{
				--x0; --x3; ++x1; ++x2;
			}
			else
			{
				++y0; ++y1; --y2; --y3;
			}
		}
		else
		{
			if (abs(x1 - x2) < abs(y1 - y0))
			{
				++x0; ++x1; --x2; --x3;
			}
			else
			{
				--y1; --y2; ++y0; ++y3;
			}
		}
	}

	std::string sx0 = wxString::FromDouble(x0), sy0 = wxString::FromDouble(y0);
	std::string sx1 = wxString::FromDouble(x1), sy1 = wxString::FromDouble(y1);
	std::string sx2 = wxString::FromDouble(x2), sy2 = wxString::FromDouble(y2);
	std::string sx3 = wxString::FromDouble(x3), sy3 = wxString::FromDouble(y3);
	std::string assignSrc = lua::assign("src", lua::tableassign("", 8, sx0.c_str(), sy0.c_str(), 
		sx1.c_str(), sy1.c_str(), sx2.c_str(), sy2.c_str(), sx3.c_str(), sy3.c_str()));

	// screen
	const float hw = picture->entry->spriteSourceSize.w * 0.5f / Context::Instance()->scale,
		hh = picture->entry->spriteSourceSize.h * 0.5f / Context::Instance()->scale;
	d2d::Vector screen[4];
	screen[0].set(-hw, hh);
	screen[1].set(-hw, -hh);
	screen[2].set(hw, -hh);
	screen[3].set(hw, hh);
	for (size_t i = 0; i < 4; ++i)
		screen[i] += picture->offset / Context::Instance()->scale;

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

void COCCode::resolveAnimation(const complex::Symbol* symbol)
{
	lua::TableAssign ta(m_gen, "animation", false, false);

	resolveAnimationCommon(symbol);

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

			//////////////////////////////////////////////////////////////////////////

// 			std::map<std::string, std::vector<d2d::ISprite*> >::iterator itr = 
// 				map_actions.find(sprite->tag);
// 			if (itr == map_actions.end())
// 			{
// 				std::vector<d2d::ISprite*> sprites;
// 				sprites.push_back(sprite);
// 				map_actions.insert(std::make_pair(sprite->tag, sprites));
// 			}
// 			else
// 			{
// 				itr->second.push_back(sprite);
// 			}
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

//void COCCode::resolveAnimation(const d2d::AnimSymbol* symbol)
//{
//	lua::TableAssign ta(m_gen, "animation", false, false);
//
//	resolveAnimationCommon(symbol);
//
// 	// component
// 	std::vector<int> ids;
// 	std::set<int> unique;
// 	{
// 		lua::TableAssign ta(m_gen, "component", true);
// 		for (size_t i = 0, n = symbol->getMaxFrameIndex(); i < n; ++i)
// 		{
// 			for (size_t j = 0, m = symbol->m_layers.size(); j < m; ++j)
// 			{
// 				d2d::AnimSymbol::Layer* layer = symbol->m_layers[j];
// 				if (i < layer->frames.size())
// 				{
// 					d2d::AnimSymbol::Frame* frame = layer->frames[i];
// 					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
// 						resolveSpriteForComponent(frame->sprites[k], ids, unique);
// 				}
// 			}
// 		}
// 	}
// 	// children
// 	{
// 		lua::TableAssign ta(m_gen, "", true);
// 		// frames
// 		int index = 0;
// 		for (size_t i = 0, n = symbol->getMaxFrameIndex(); i < n; ++i)
// 		{
// 			lua::TableAssign ta(m_gen, "", true);
// 			for (size_t j = 0, m = symbol->m_layers.size(); j < m; ++j)
// 			{
// 				d2d::AnimSymbol::Layer* layer = symbol->m_layers[j];
// 				if (i < layer->frames.size())
// 				{
// 					d2d::AnimSymbol::Frame* frame = layer->frames[i];
// 					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k, ++index)
// 						resolveSpriteForFrame(frame->sprites[k], index, ids, unique);
// 				}
// 			}
// 		}
// 	}
//}

void COCCode::resolveAnimation(const d2d::AnimSymbol* symbol)
{
	lua::TableAssign ta(m_gen, "animation", false, false);

	resolveAnimationCommon(symbol);

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
				d2d::AnimSymbol::Layer* layer = symbol->m_layers[j];
				if (i < layer->frames.size())
				{
					d2d::AnimSymbol::Frame* frame = layer->frames[i];
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
			libanim::Tools::getCurrSprites(symbol, i, sprites);
			for (size_t j = 0, m = sprites.size(); j < m; ++j)
				resolveSpriteForFrame(sprites[j], order);
 		}
 	}
}

void COCCode::resolveAnimation(const d2d::Patch9Symbol* symbol)
{
	lua::TableAssign ta(m_gen, "animation", false, false);

	resolveAnimationCommon(symbol);

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;
	{
		lua::TableAssign ta(m_gen, "component", true);
		if (symbol->type() == d2d::Patch9Symbol::e_9Grid)
			for (size_t i = 0; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					resolveSpriteForComponent(symbol->m_sprites[i][j], ids, unique, order);
		else if (symbol->type() == d2d::Patch9Symbol::e_9GridHollow)
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					if (i == 1 && j == 1) continue;
					resolveSpriteForComponent(symbol->m_sprites[i][j], ids, unique, order);
				}
			}
		else if (symbol->type() == d2d::Patch9Symbol::e_3GridHor)
			for (size_t i = 0; i < 3; ++i)
				resolveSpriteForComponent(symbol->m_sprites[1][i], ids, unique, order);
		else if (symbol->type() == d2d::Patch9Symbol::e_3GridVer)
			for (size_t i = 0; i < 3; ++i)
				resolveSpriteForComponent(symbol->m_sprites[i][1], ids, unique, order);
		else if (symbol->type() == d2d::Patch9Symbol::e_6GridUpper)
			for (size_t i = 1; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					resolveSpriteForComponent(symbol->m_sprites[i][j], ids, unique, order);
	}
	// children
	{
		lua::TableAssign ta(m_gen, "", true);
		// frame 0
		{
			lua::TableAssign ta(m_gen, "", true);
			int index = 0;
			if (symbol->type() == d2d::Patch9Symbol::e_9Grid)
				for (size_t i = 0; i < 3; ++i)
					for (size_t j = 0; j < 3; ++j, ++index)
						resolveSpriteForFrame(symbol->m_sprites[i][j], index, ids, order);
			else if (symbol->type() == d2d::Patch9Symbol::e_9GridHollow)
				for (size_t i = 0; i < 3; ++i) {
					for (size_t j = 0; j < 3; ++j, ++index) {
						if (i == 1 && j == 1) { 
							--index;
							continue;
						}
						resolveSpriteForFrame(symbol->m_sprites[i][j], index, ids, order);
					}
				}
			else if (symbol->type() == d2d::Patch9Symbol::e_3GridHor)
				for (size_t i = 0; i < 3; ++i)
					resolveSpriteForFrame(symbol->m_sprites[1][i], i, ids, order);
			else if (symbol->type() == d2d::Patch9Symbol::e_3GridVer)
				for (size_t i = 0; i < 3; ++i)
					resolveSpriteForFrame(symbol->m_sprites[i][1], i, ids, order);
			else if (symbol->type() == d2d::Patch9Symbol::e_6GridUpper)
				for (size_t i = 1; i < 3; ++i)
					for (size_t j = 0; j < 3; ++j, ++index)
						resolveSpriteForFrame(symbol->m_sprites[i][j], index, ids, order);
		}
	}
}

void COCCode::resolveAnimationCommon(const d2d::ISymbol* symbol)
{
	// export
	if (!symbol->name.empty())
		m_gen.line(lua::assign("export", "\""+symbol->name+"\"")+",");

	// id
	std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
	assert(itr != m_mapSymbolID.end());
	if (itr == m_mapSymbolID.end())
		throw d2d::Exception("Error! COCCode::resolveAnimationCommon L666");

	std::string sid = wxString::FromDouble(itr->second);
	m_gen.line(lua::assign("id", sid.c_str()) + ",");
}

void COCCode::resolveSpriteForComponent(const d2d::ISprite* sprite, std::vector<int>& ids, 
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
			// AnimSymbol's sprites store unique

			std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->getSymbol());
			assert(itr != m_mapSymbolID.end());
			if (itr == m_mapSymbolID.end())
				throw d2d::Exception("Error! COCCode::resolveSpriteForComponent L694");

			id = itr->second;
		}
	}
	else if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(sprite))
	{
		isFont = true;
		std::map<const d2d::ISprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		assert(itr != m_mapSpriteID.end());
		if (itr == m_mapSpriteID.end())
			throw d2d::Exception("Error! COCCode::resolveSpriteForComponent L705");

		id = itr->second;
	}
	else
	{
		std::map<const d2d::ISymbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->getSymbol());
		assert(itr != m_mapSymbolID.end());
		if (itr == m_mapSymbolID.end())
			throw d2d::Exception("Error! COCCode::resolveSpriteForComponent L715");

		id = itr->second;
	}

	ids.push_back(id);

	std::map<int, std::vector<std::string> >::iterator itr = unique.find(id);
	if (unique.find(id) == unique.end())
	{
		if (isFont)
		{
			const d2d::FontSprite* font = static_cast<const d2d::FontSprite*>(sprite);
			bool isNullNode = font->font.empty() && font->color == d2d::Colorf(0, 0, 0, 0);
			if (isNullNode)
			{
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				lua::tableassign(m_gen, "", 1, aName.c_str());
			}
			else
			{
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				std::string aFont = lua::assign("font", "\""+font->font+"\"");
				std::string aColor = lua::assign("color", transColor(font->color, d2d::PT_ARGB));
				std::string aAlign = lua::assign("align", wxString::FromDouble(font->align).ToStdString());
				std::string aSize = lua::assign("size", wxString::FromDouble(font->size).ToStdString());
				std::string aWidth = lua::assign("width", wxString::FromDouble(font->width).ToStdString());
				std::string aHeight = lua::assign("height", wxString::FromDouble(font->height).ToStdString());

				lua::tableassign(m_gen, "", 7, aName.c_str(), aFont.c_str(), aColor.c_str(), aAlign.c_str(), 
					aSize.c_str(), aWidth.c_str(), aHeight.c_str());
			}
		}
		else
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

//		if (!sprite->name.empty())
//			order.push_back(std::make_pair(id, sprite->name));
	}
}

void COCCode::resolveSpriteForFrame(const d2d::ISprite* sprite, int index,
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
	if (cindex == -1)
		throw d2d::Exception("Error! COCCode::resolveSpriteForFrame L806");

	if (const d2d::FontSprite* font = dynamic_cast<const d2d::FontSprite*>(sprite))
		resolveSpriteForFrameFont(font, cindex);
	else
		resolveSpriteForFrameImage(sprite, cindex);

// 	bool forceMat = dynamic_cast<const d2d::FontSprite*>(sprite);
// 	resolveSpriteForFrame(sprite, cindex, forceMat);
}

void COCCode::resolveSpriteForFrame(const d2d::ISprite* sprite, const std::vector<std::pair<int, std::string> >& order)
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

void COCCode::resolveSpriteForFrame(const d2d::ISprite* sprite, int id, bool forceMat)
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
		std::string assignAdd = lua::assign("add", d2d::transColor(sprite->addCol, d2d::PT_ARGB));
		lua::tableassign(m_gen, "", 4, assignIndex.c_str(), assignColor.c_str(), assignAdd.c_str(), assignMat.c_str());
	}
	else
		lua::tableassign(m_gen, "", 2, assignIndex.c_str(), assignMat.c_str());
}

void COCCode::resolveSpriteForFrameImage(const d2d::ISprite* sprite, int id)
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
		std::string assignAdd = lua::assign("add", d2d::transColor(sprite->addCol, d2d::PT_ARGB));
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

void COCCode::resolveSpriteForFrameFont(const d2d::FontSprite* sprite, int id)
{
	std::string assignIndex = lua::assign("index", wxString::FromDouble(id).ToStdString());

	float mat[6];
	transToMat(sprite, mat, true);

	bool isNullNode = sprite->font.empty() && sprite->color == d2d::Colorf(0, 0, 0, 0);
	if (!isNullNode)
	{
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

void COCCode::transToMat(const d2d::ISprite* sprite, float mat[6], bool force /*= false*/)
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
// 		// translate
// 		d2d::Vector center = sprite->getCenter();
// 		mat[4] = center.x;
// 		mat[5] = center.y;
// 
// 		// scale
// 		mat[0] *= sprite->getScale().x;
// 		mat[3] *= sprite->getScale().y;
// 		mat[1] *= sprite->getScale().y;
// 		mat[2] *= sprite->getScale().x;
// 
// 		// rotate
// 		mat[0] = cos(-sprite->getAngle());
// 		mat[1] = sin(-sprite->getAngle());
// 		mat[2] = -mat[1];
// 		mat[3] = mat[0];
// 
// 		// shear
// 		float m0 = mat[0], m1 = mat[1], m2 = mat[2], m3 = mat[3];
// 		mat[0] = m0 + m1 * sprite->getShear().y;
// 		mat[1] = m0 * sprite->getShear().x + m1;
// 		mat[2] = m2 + m3 * sprite->getShear().y;
// 		mat[3] = m2 * sprite->getShear().x + m3;
// 
// 		// mirror
// 		bool xMirror, yMirror;
// 		sprite->getMirror(xMirror, yMirror);
// 		if (xMirror) mat[0] = -mat[0];
// 		if (yMirror) mat[3] = -mat[3];

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

		mat[0] = c*sx - kx*s*sx;
		mat[1] = s*sy + kx*c*sy;
		mat[2] = ky*c*sx - s*sx;
		mat[3] = ky*s*sy + c*sy;
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
} // coceditor