#include "CocoPacker.h"
#include "TexturePacker.h"

#include "../Utility.h"

#include <ee/ImageSprite.h>
#include <ee/ImageSymbol.h>
#include <ee/FontBlankSprite.h>
#include <ee/Exception.h>

#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>

#include <sprite2/S2_Sprite.h>
#include <sprite2/RenderColor.h>
#include <gum/trans_color.h>
#include <SM_Calc.h>

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

void CocoPacker::pack(const std::vector<const ee::Symbol*>& syms)
{
	lua::TableAssign ta(m_gen, "return", false, false);

	for (int i = 0, n = syms.size(); i < n; ++i)
	{
		const ee::Symbol* sym = syms[i];
		if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(sym))
		{
			const std::vector<s2::Sprite*>& children = complex->GetAllChildren();
			for (int i = 0, n = children.size(); i < n; ++i) {
				ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
				if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(child))
				{
					m_mapSpriteID.insert(std::make_pair(child, m_id++));
					resolvePicture(image);
				}
				else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(child))
				{
					m_mapSpriteID.insert(std::make_pair(child, m_id++));
					resolveFont(font);
				}
			}
			m_mapSymbolID.insert(std::make_pair(sym, m_id++));
			resolveAnimation(complex);
		}
		else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(sym))
		{
			std::set<const ee::ImageSymbol*> unique;
			const std::vector<s2::AnimSymbol::Layer*>& layers = anim->GetLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				s2::AnimSymbol::Layer* layer = layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					s2::AnimSymbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprs.size(); k < l; ++k)
					{
						ee::Sprite* spr = dynamic_cast<ee::Sprite*>(frame->sprs[k]);
						if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(spr))
							unique.insert(dynamic_cast<const ee::ImageSymbol*>(image->GetSymbol()));
						else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(spr))
							m_mapSpriteID.insert(std::make_pair(spr, m_id++));
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

			m_mapSymbolID.insert(std::make_pair(sym, m_id++));
			resolveAnimation(anim);
		}
		else if (const escale9::Symbol* patch9 = dynamic_cast<const escale9::Symbol*>(sym))
		{
			std::vector<s2::Sprite*> grids;
			patch9->GetScale9().GetGrids(grids);
 			for (int i = 0, n = grids.size(); i < n; ++i)
 			{
 				s2::Sprite* grid = grids[i];
 				if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(grid))
 				{
 					m_mapSpriteID.insert(std::make_pair(image, m_id++));
 					resolvePicture(image);
 				}
 				else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(grid))
 				{
 					m_mapSpriteID.insert(std::make_pair(font, m_id++));
 				}
 			}
 
 			m_mapSymbolID.insert(std::make_pair(sym, m_id++));
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

void CocoPacker::resolvePicture(const ee::ImageSprite* spr)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"picture\"") + ",");

	// id
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		assert(itr != m_mapSpriteID.end());
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen.line(lua::assign("id", sid) + ",");
	}

	// tex
	// todo: specify only 1 texture
	std::string assignTex = lua::assign("tex", ee::StringHelper::ToString(1));
	const sm::rect* pr = m_tex.Query(dynamic_cast<const ee::ImageSymbol*>(spr->GetSymbol())->GetImage());

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
	sm::vec2 sz = pr->Size();
	const float hw = sz.x * 0.5f, 
		        hh = sz.y * 0.5f;
	sm::vec2 screen[4];
	screen[0].Set(-hw, hh);
	screen[1].Set(-hw, -hh);
	screen[2].Set(hw, -hh);
	screen[3].Set(hw, hh);
	// 1. scale
	for (size_t i = 0; i < 4; ++i)
		screen[i].x *= spr->GetScale().x;
	for (size_t i = 0; i < 4; ++i)
		screen[i].y *= spr->GetScale().y;
	// 2. rotate
	for (size_t i = 0; i < 4; ++i)
	{
		sm::vec2 rot = sm::rotate_vector(screen[i], spr->GetAngle());
		screen[i] = rot;
	}
	// 3. translate
	sm::vec2 center = spr->GetCenter();
 	for (size_t i = 0; i < 4; ++i)
 		screen[i] += center;
	//for (size_t i = 0; i < 4; ++i)
	//{
	//	screen[i].x += center.x;
	//	screen[i].y -= center.y;
	//}
	// 4. mirror
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

void CocoPacker::resolvePicture(const ee::ImageSymbol* sym)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"picture\"") + ",");

	// id
	{
		std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
		assert(itr != m_mapSymbolID.end());
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen.line(lua::assign("id", sid) + ",");
	}

	// tex
	// todo: specify only 1 texture
	std::string assignTex = lua::assign("tex", ee::StringHelper::ToString(1));
	const sm::rect* pr = m_tex.Query(sym->GetImage());

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
	sm::vec2 sz = pr->Size();
	const float hw = sz.x * 0.5f,
		        hh = sz.y * 0.5f;
	sm::vec2 screen[4];
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

void CocoPacker::resolveFont(const ee::FontBlankSprite* spr)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"label\"") + ",");

	// id
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		assert(itr != m_mapSpriteID.end());
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen.line(lua::assign("id", sid) + ",");
	}

	std::string aFont = lua::assign("font", "\""+spr->font+"\"");
	std::string aColor = lua::assign("color", gum::color2str(spr->font_color, s2::ARGB));
//	std::string aAlign = lua::assign("align", ee::StringHelper::ToString(spr->align));
	std::string aAlign = lua::assign("align", ee::StringHelper::ToString(spr->align_hori));
	std::string aSize = lua::assign("size", ee::StringHelper::ToString(spr->size));
	std::string aWidth = lua::assign("width", ee::StringHelper::ToString(spr->width));
	std::string aHeight = lua::assign("height", ee::StringHelper::ToString(spr->height));
	std::string aEdge = lua::assign("noedge", "false");

	m_gen.line(aFont + ", " + aColor + ", " + aAlign + ", " + aSize + ", " + aWidth + ", " + aHeight + ", " + aEdge);
}

void CocoPacker::resolveAnimation(const ecomplex::Symbol* sym)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"animation\"") + ",");

	// export
	if (!sym->name.empty())
		m_gen.line(lua::assign("export", "\""+sym->name+"\"")+",");

	// id
	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
	assert(itr != m_mapSymbolID.end());
	std::string sid = ee::StringHelper::ToString(itr->second);
	m_gen.line(lua::assign("id", sid) + ",");

	// clipbox
	const sm::rect& cb = sym->GetScissor();
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
		const std::vector<s2::Sprite*>& children = sym->GetAllChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
			resolveSpriteForComponent(child, ids, unique, order);
		}
	}

	// children
	std::map<std::string, std::vector<ee::Sprite*> > map_actions;
	std::vector<ee::Sprite*> others;
	const std::vector<s2::Sprite*>& children = sym->GetAllChildren();
	Utility::GroupSpritesFromTag(children, map_actions, others);
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
					for (idx = 0; idx < children.size(); ++idx) {
						ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[idx]);
						if (child == itr->second[i])
							break;
					}
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
				for (idx = 0; idx < children.size(); ++idx) {
					ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[idx]);
					if (child == others[i])
						break;
				}
				resolveSpriteForFrame(others[i], idx, ids, order);
			}
		}
	}
}

void CocoPacker::resolveAnimation(const libanim::Symbol* sym)
{
	lua::TableAssign ta(m_gen, "", true, false);

	// type
	m_gen.line(lua::assign("type", "\"animation\"") + ",");

	// export
	if (!sym->name.empty())
		m_gen.line(lua::assign("export", "\""+sym->name+"\"")+",");

	// id
	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
	assert(itr != m_mapSymbolID.end());
	std::string sid = ee::StringHelper::ToString(itr->second);
	m_gen.line(lua::assign("id", sid) + ",");

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;
	{
		lua::TableAssign ta(m_gen, "component", true);
		const std::vector<s2::AnimSymbol::Layer*>& layers = sym->GetLayers();
		for (size_t i = 0, n = sym->GetMaxFrameIdx(); i < n; ++i)
		{
			for (size_t j = 0, m = layers.size(); j < m; ++j)
			{
				s2::AnimSymbol::Layer* layer = layers[j];
				if (i < layer->frames.size())
				{
					s2::AnimSymbol::Frame* frame = layer->frames[i];
					for (size_t k = 0, l = frame->sprs.size(); k < l; ++k)
						resolveSpriteForComponent(dynamic_cast<ee::Sprite*>(frame->sprs[k]), ids, unique, order);
				}
			}
		}
	}
	// children
	{
		lua::TableAssign ta(m_gen, "", true);
		// frames
		int index = 0;
		for (size_t i = 1, n = sym->GetMaxFrameIdx(); i <= n; ++i)
		{
			lua::TableAssign ta(m_gen, "", true);

			std::vector<s2::Sprite*> sprs;
			sym->CreateFrameSprites(i, sprs);
			for (size_t j = 0, m = sprs.size(); j < m; ++j)
				resolveSpriteForFrame(dynamic_cast<ee::Sprite*>(sprs[j]), order);
			for_each(sprs.begin(), sprs.end(), cu::RemoveRefFunctor<s2::Sprite>());
		}
	}
}

void CocoPacker::resolveAnimation(const escale9::Symbol* sym)
{
 	lua::TableAssign ta(m_gen, "", true, false);
 
	s2::SCALE9_TYPE type = sym->GetScale9().GetType();

 	// type
 	m_gen.line(lua::assign("type", "\"animation\"") + ",");
 
 	// export
 	if (!sym->name.empty())
 		m_gen.line(lua::assign("export", "\""+sym->name+"\"")+",");
 
 	// id
 	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
 	assert(itr != m_mapSymbolID.end());
 	std::string sid = ee::StringHelper::ToString(itr->second);
 	m_gen.line(lua::assign("id", sid) + ",");
 
	std::vector<s2::Sprite*> grids;
	sym->GetScale9().GetGrids(grids);

 	// component
 	std::vector<int> ids;
 	std::map<int, std::vector<std::string> > unique;
 	std::vector<std::pair<int, std::string> > order;
 	{
 		lua::TableAssign ta(m_gen, "component", true);
		for (int i = 0, n = grids.size(); i < n; ++i) {
			resolveSpriteForComponent(dynamic_cast<const ee::Sprite*>(grids[i]), ids, unique, order);			
		}
 	}
 	// children
 	{
 		lua::TableAssign ta(m_gen, "", true);
 		// frame 0
 		{
 			lua::TableAssign ta(m_gen, "", true);
 			int index = 0;
			for (int i = 0, n = grids.size(); i < n; ++i) {
				resolveSpriteForFrame(dynamic_cast<const ee::Sprite*>(grids[i]), index, ids, order);			
			}
 		}
 	}
}

void CocoPacker::resolveSpriteForComponent(const ee::Sprite* spr, std::vector<int>& ids, 
										   std::map<int, std::vector<std::string> >& unique, 
										   std::vector<std::pair<int, std::string> >& order)
{
	int id;

	bool isFont = false;

	if (const ee::ImageSprite* image = dynamic_cast<const ee::ImageSprite*>(spr))
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		if (itr != m_mapSpriteID.end())
		{
			id = itr->second;
		}
		else
		{
			// libanim::Symbol's sprs store unique
			std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
			assert(itr != m_mapSymbolID.end());
			id = itr->second;
		}
	}
	else if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr))
	{
		isFont = true;
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		assert(itr != m_mapSpriteID.end());
		id = itr->second;
	}
	else
	{
		std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
		assert(itr != m_mapSymbolID.end());
		id = itr->second;
	}

	ids.push_back(id);

	const std::string& name = spr->GetName();

	std::map<int, std::vector<std::string> >::iterator itr = unique.find(id);
	if (unique.find(id) == unique.end())
	{
		std::string aID = lua::assign("id", ee::StringHelper::ToString(id));
		if (!name.empty() && name[0] != '_')
		{
			std::string aName = lua::assign("name", "\""+name+"\"");
			lua::tableassign(m_gen, "", 2, aName, aID);
		}
		else
		{
			lua::tableassign(m_gen, "", 1, aID);
		}

		std::vector<std::string> names;
		names.push_back(name);
		unique.insert(std::make_pair(id, names));
		order.push_back(std::make_pair(id, name));
	}
	else
	{
		int i = 0;
		for (int n = itr->second.size(); i < n; ++i)
			if (itr->second[i] == name)
				break;
		if (i == itr->second.size() && !isFont)
		{
			std::string aID = lua::assign("id", ee::StringHelper::ToString(id));
			if (!name.empty() && name[0] != '_')
			{
				std::string aName = lua::assign("name", "\""+name+"\"");
				lua::tableassign(m_gen, "", 2, aName, aID);
			}
			else
			{
				lua::tableassign(m_gen, "", 1, aID);
			}

			order.push_back(std::make_pair(id, name));
		}
		itr->second.push_back(name);
	}
}

void CocoPacker::resolveSpriteForFrame(const ee::Sprite* spr, int index,
									   const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order)
{
	int id = ids[index];
	int cindex = -1;
	for (size_t i = 0, n = order.size(); i < n; ++i)
		if (id == order[i].first && spr->GetName() == order[i].second)
		{
			cindex = i;
			break;
		}
		assert(cindex != -1);

	if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr))
		resolveSpriteForFrameFont(font, cindex);
	else
		resolveSpriteForFrameImage(spr, cindex);
}

void CocoPacker::resolveSpriteForFrame(const ee::Sprite* spr, const std::vector<std::pair<int, std::string> >& order)
{
	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(dynamic_cast<const ee::Symbol*>(spr->GetSymbol()));
	assert(itr != m_mapSymbolID.end());
	if (itr == m_mapSymbolID.end())
		throw ee::Exception("Error! COCCode::resolveSpriteForFrame L822");

	int id = itr->second;
	int cindex = -1;
	for (size_t i = 0, n = order.size(); i < n; ++i)
		if (id == order[i].first && spr->GetName() == order[i].second)
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

		resolveSpriteForFrame(spr, cindex, true);
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

	const s2::Color& mul = spr->GetColor().GetMul();
	const s2::Color& add = spr->GetColor().GetAdd();
	if (mul != s2::Color(255, 255, 255, 255) || add != s2::Color(0,0,0,0))
	{
		std::string assignColor = lua::assign("color", gum::color2str(mul, s2::BGRA));
		std::string assignAdd = lua::assign("add", gum::color2str(add, s2::ABGR));
		lua::tableassign(m_gen, "", 4, assignIndex, assignColor, assignAdd, assignMat);
	}
	else
	{
		lua::tableassign(m_gen, "", 2, assignIndex, assignMat);
	}
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

	const s2::Color& mul = spr->GetColor().GetMul();
	const s2::Color& add = spr->GetColor().GetAdd();
	if (mul != s2::Color(1,1,1,1) || add != s2::Color(0,0,0,0))
	{
		std::string assignColor = lua::assign("color", gum::color2str(mul, s2::BGRA));
		std::string assignAdd = lua::assign("add", gum::color2str(add, s2::ABGR));
// 		if (spr->IsClip())
// 			lua::tableassign(m_gen, "", 5, assignIndex, assignColor, assignAdd, assignMat, "clip=true");
// 		else
// 			lua::tableassign(m_gen, "", 4, assignIndex, assignColor, assignAdd, assignMat);
	}
	else
	{
// 		if (spr->IsClip())
// 			lua::tableassign(m_gen, "", 3, assignIndex, assignMat, "clip=true");
// 		else
// 			lua::tableassign(m_gen, "", 2, assignIndex, assignMat);
	}
}

void CocoPacker::resolveSpriteForFrameFont(const ee::FontBlankSprite* spr, int id)
{
	std::string assignIndex = lua::assign("index", ee::StringHelper::ToString(id));

	float mat[6];
	transToMat(spr, mat, true);

	bool isNullNode = spr->font.empty() && spr->font_color == s2::Color(0, 0, 0, 0);
	if (!isNullNode)
	{
		// flip y
 		// move to left-top
  		mat[4] -= floor(spr->width * 0.5f * 16 + 0.5f);
  		mat[5] -= floor(spr->height * 0.5f * 16 + 0.5f);

		//mat[4] -= floor(spr->getBounding()->width() * 0.5f * 16 + 0.5f);
		//mat[5] -= floor(spr->getBounding()->height() * 0.5f * 16 + 0.5f);
	}

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = ee::StringHelper::ToString(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);

	lua::tableassign(m_gen, "", 2, assignIndex, assignMat);
}

void CocoPacker::transToMat(const ee::Sprite* spr, float mat[6], bool force /*= false*/)
{
	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	if (!force &&
		(dynamic_cast<const ee::ImageSprite*>(spr) ||
		dynamic_cast<const ee::FontBlankSprite*>(spr)))
	{
	}
	else
	{
		// | 1  kx    | | sx       | |  c  s    | | 1       |
		// | ky  1    | |    sy    | | -s  c    | |    1    |
		// |        1 | |        1 | |        1 | | x  y  1 |
		//     skew        scale        rotate        move
		float x = spr->GetCenter().x,
			y = spr->GetCenter().y;
		float sx = spr->GetScale().x,
			  sy = spr->GetScale().y;
		float c = cos(-spr->GetAngle()),
			s = sin(-spr->GetAngle());
		float kx = spr->GetShear().x,
			ky = spr->GetShear().y;
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