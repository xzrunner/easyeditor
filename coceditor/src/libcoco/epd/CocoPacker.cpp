#include "CocoPacker.h"
#include "../Utility.h"

#include <ee/ImageSymbol.h>
#include <ee/ImageSprite.h>
#include <ee/FontBlankSymbol.h>
#include <ee/FontBlankSprite.h>
#include <ee/Exception.h>
#include <ee/SymbolMgr.h>
#include <ee/Image.h>
#include <ee/Math2D.h>

#include <easybuilder.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyicon.h>
#include <epbin.h>

#include <gum/trans_color.h>
#include <sprite2/S2_Sprite.h>
#include <sprite2/MeshTriangle.h>
#include <SM_Process.h>

#include <queue>

namespace ecoco
{
namespace epd
{

namespace lua = ebuilder::lua;

CocoPacker::CocoPacker(const std::vector<const ee::Symbol*>& syms, 
					   const TextureMgr& tex_mgr)
	: m_gen(new ebuilder::CodeGenerator)
	, m_parser(syms, tex_mgr)
	, m_scale(1)
	, m_id(0)
	, m_img_ider(NULL)
{
}

CocoPacker::CocoPacker(const std::vector<const ee::Symbol*>& syms, 
					   const TextureMgr& tex_mgr, 
					   const std::string& img_id_file, 
					   float scale)
	: m_gen(new ebuilder::CodeGenerator)
	, m_parser(syms, tex_mgr)
	, m_scale(scale)
	, m_id(0)
{
	m_img_ider = new epbin::ImageIDer(img_id_file);
}

CocoPacker::~CocoPacker()
{
	delete m_img_ider;
	delete m_gen;
}

void CocoPacker::Parser()
{
	m_parser.Parser();
	ResolveSymbols();
}

void CocoPacker::Output(const char* filepath) const
{
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath);
	std::locale::global(std::locale("C"));
	fout << m_gen->toText() << std::endl;
	fout.close();
}

void CocoPacker::ResolveSymbols()
{
	m_gen->line("texture("+ee::StringHelper::ToString(m_parser.GetTexSize())+")");

	const std::vector<const ee::Symbol*>& syms = m_parser.GetSymbolSet().GetOrdered();
	for (size_t i = 0, n = syms.size(); i < n; ++i)
	{
		const ee::Symbol* sym = syms[i];

		std::cout << ee::StringHelper::Format("[%d/%d] file: %s\n", i, n, sym->GetFilepath().c_str());

		if (const ee::ImageSymbol* image = dynamic_cast<const ee::ImageSymbol*>(sym))
		{
		}
		else if (const ee::FontBlankSymbol* font = dynamic_cast<const ee::FontBlankSymbol*>(sym))
		{
		}
		else if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(sym))
		{
			const std::vector<s2::Sprite*>& children = complex->GetChildren();			
			for (size_t i = 0, n = children.size(); i < n; ++i)
			{
				ee::Sprite* spr = dynamic_cast<ee::Sprite*>(children[i]);
				if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(spr))
				{
					m_mapSpriteID.insert(std::make_pair(spr, m_id++));
					sm::vec2 sz = image->GetSymbol()->GetBounding().Size();
					if (sz.x == 4 && sz.y == 4)
						ParserPicture(image, e_bothfix);
					else
						ParserPicture(image);
				}
				else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(spr))
				{
					m_mapSpriteID.insert(std::make_pair(spr, m_id++));
				}
				else if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(spr))
				{
					ParserIcon(icon);
				}
				else if (emesh::Sprite* mesh = dynamic_cast<emesh::Sprite*>(spr))
				{
					// todo multi mesh!
					std::map<const ee::Symbol*, std::vector<SpriteID> >::iterator itr
						= m_map_symbol2ids.find(dynamic_cast<const ee::Symbol*>(mesh->GetSymbol()));
					if (itr != m_map_symbol2ids.end()) {
						bool find = false;
						for (int i = 0, n = itr->second.size(); i < n; ++i) {
							SpriteID id = itr->second[i];
							if (mesh->GetSpeed() == static_cast<emesh::Sprite*>(id.spr)->GetSpeed()) {
								m_mapSpriteID.insert(std::make_pair(spr, id.id));
								find = true;
								break;
							}
						}
						if (!find) {
							if (!itr->second.empty()) {
								// todo symbol id 会重复!
								int zz = 0;
							}

							SpriteID id;
							id.spr = mesh;
							id.id = m_id;
							itr->second.push_back(id);

							// mesh's pictures id
							// todo for x
							m_mapSpriteID.insert(std::make_pair(spr, m_id));
							int size = ParserMesh(mesh);
							m_id += size;
						}
					} else {
						SpriteID id;
						id.spr = mesh;
						id.id = m_id;

						std::vector<SpriteID> ids;
						ids.push_back(id);

						m_map_symbol2ids.insert(std::make_pair(dynamic_cast<const ee::Symbol*>(mesh->GetSymbol()), ids));

						// mesh's pictures id
						// todo for x
						m_mapSpriteID.insert(std::make_pair(spr, m_id));
						int size = ParserMesh(mesh);
						m_id += size;
					}
				} 
				else if (eterrain2d::Sprite* ocean = dynamic_cast<eterrain2d::Sprite*>(spr)) 
				{
					m_mapSpriteID.insert(std::make_pair(ocean, m_id));
					int size = ParserTerrain2D(ocean);
					m_id += size;
				}
				else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(spr))
				{
					std::map<const ee::Symbol*, std::vector<SpriteID> >::iterator itr
						= m_map_symbol2ids.find(dynamic_cast<const ee::Symbol*>(tex->GetSymbol()));
					if (itr != m_map_symbol2ids.end()) 
					{
						bool find = false;
						for (int i = 0, n = itr->second.size(); i < n; ++i) 
						{
							SpriteID id = itr->second[i];
							m_mapSpriteID.insert(std::make_pair(spr, id.id));
							find = true;
							break;
						}
						if (!find) 
						{
							SpriteID id;
							id.spr = tex;
							id.id = m_id;
							itr->second.push_back(id);

							m_mapSpriteID.insert(std::make_pair(spr, m_id));
							int size = ParserTexture(tex);
							m_id += size;
						}
					} 
					else 
					{
						SpriteID id;
						id.spr = tex;
						id.id = m_id;

						std::vector<SpriteID> ids;
						ids.push_back(id);

						m_map_symbol2ids.insert(std::make_pair(dynamic_cast<const ee::Symbol*>(tex->GetSymbol()), ids));

						m_mapSpriteID.insert(std::make_pair(spr, m_id));
						int size = ParserTexture(tex);
						m_id += size;
					}
				}
			}

			m_mapSymbolID.insert(std::make_pair(sym, m_id++));
			ParserComplex(complex);
		}
		else if (const eanim::Symbol* anim = dynamic_cast<const eanim::Symbol*>(sym))
		{
			////////////////////////////////////////////////////////////////////////////
			//// version 1: parser all sprs to picture
			////////////////////////////////////////////////////////////////////////////

			//for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			//{
			//	eanim::Symbol::Layer* layer = anim->m_layers[i];
			//	for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
			//	{
			//		eanim::Symbol::Frame* frame = layer->frames[j];
			//		for (size_t k = 0, l = frame->sprs.size(); k < l; ++k)
			//		{
			//			ee::Sprite* spr = frame->sprs[k];
			//			if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(spr))
			//			{
			//				m_mapSpriteID.insert(std::make_pair(spr, m_id++));
			//				resolvePicture(image);
			//			}
			//			else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(spr))
			//			{
			//				m_mapSpriteID.insert(std::make_pair(spr, m_id++));
			//			}
			//		}
			//	}
			//}

			//m_mapSymbolID.insert(std::make_pair(sym, m_id++));
			//resolveAnimation(anim);

			//////////////////////////////////////////////////////////////////////////
			// version 2: use mat instead of each picture
			//////////////////////////////////////////////////////////////////////////

			std::set<const ee::ImageSymbol*, ee::SymbolCmp> unique;
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
						else if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(spr))
							ParserIcon(icon);
					}
				}
			}

			std::set<const ee::ImageSymbol*, ee::SymbolCmp>::iterator itr = unique.begin();
			for ( ; itr != unique.end(); ++itr)
			{
				std::map<const ee::Symbol*, int>::iterator itrFind = m_mapSymbolID.find(*itr);
				if (itrFind == m_mapSymbolID.end())
				{
					m_mapSymbolID.insert(std::make_pair(*itr, m_id++));
					if ((*itr)->GetBounding().Size().x <= 4)
						ParserPicture(*itr, e_xfix);
					else
						ParserPicture(*itr);
				}
			}

			for (int i = 0, n = anim->GetMaxFrameIdx(); i < n; ++i)
			{
				std::vector<s2::Sprite*> sprs;
				anim->CreateFrameSprites(i + 1, sprs);
				if (sprs.empty()) {
					continue;
				}
				for (int i = 0, n = sprs.size(); i < n; ++i)
				{
					if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(sprs[i])) {
						int id = QueryIconID(icon);
						if (id == -1) {
							ParserIcon(icon);
						}
					}
				}
				for_each(sprs.begin(), sprs.end(), cu::RemoveRefFunctor<s2::Sprite>());
			}

			m_mapSymbolID.insert(std::make_pair(sym, m_id++));
			ParserAnimation(anim);
		}
		else if (const escale9::Symbol* patch9 = dynamic_cast<const escale9::Symbol*>(sym))
		{
			const s2::Scale9& s9 = patch9->GetScale9();
			std::vector<s2::Sprite*> grids;
			s9.GetGrids(grids);
 			for (int i = 0, n = grids.size(); i < n; ++i)
 			{
 				s2::Sprite* grid = grids[i];
 				if (ee::ImageSprite* image = dynamic_cast<ee::ImageSprite*>(grid))
 				{
 					PicFixType tsrc = e_null, tscreen = e_null;
 					switch (s9.GetType())
 					{
					case s2::S9_9GRID:
 							if (grid == s9.GetGrid(s2::S9_MID_CENTER)) 
 							{
 								tsrc = e_bothfix;
 								tscreen = e_bothfix;
 							}
 							else if (grid == s9.GetGrid(s2::S9_MID_LEFT)) tsrc = e_yfix;
 							else if (grid == s9.GetGrid(s2::S9_MID_RIGHT)) tsrc = e_yfix;
 							else if (grid == s9.GetGrid(s2::S9_DOWN_CENTER)) tsrc = e_xfix;
 							else if (grid == s9.GetGrid(s2::S9_TOP_CENTER)) tsrc = e_xfix;
 							break;
 						case s2::S9_9GRID_HOLLOW:
 							if (grid == s9.GetGrid(s2::S9_MID_LEFT)) tsrc = e_yfix;
 							else if (grid == s9.GetGrid(s2::S9_MID_RIGHT)) tsrc = e_yfix;
 							else if (grid == s9.GetGrid(s2::S9_DOWN_CENTER)) tsrc = e_xfix;
 							else if (grid == s9.GetGrid(s2::S9_TOP_CENTER)) tsrc = e_xfix;
 							break;
 						case s2::S9_3GRID_HORI:
 							if (grid == s9.GetGrid(s2::S9_MID_CENTER)) 
 							{
 								tsrc = e_xfix;
 								tscreen = e_xfix;
 							}
 							break;
 						case s2::S9_3GRID_VERT:
 							if (grid == s9.GetGrid(s2::S9_MID_CENTER)) 
 							{
 								tsrc = e_yfix;
 								tscreen = e_yfix;
 							}
 							break;
 						case s2::S9_6GRID_UPPER:
							if (grid == s9.GetGrid(s2::S9_MID_CENTER)) 
 							{
 								tsrc = e_bothfix;
 								tscreen = e_bothfix;
 							}
 							else if (grid == s9.GetGrid(s2::S9_MID_LEFT)) tsrc = e_yfix;
 							else if (grid == s9.GetGrid(s2::S9_MID_RIGHT)) tsrc = e_yfix;
 							else if (grid == s9.GetGrid(s2::S9_TOP_CENTER)) tsrc = e_xfix;
 							break;
 					}
 
					m_mapSpriteID.insert(std::make_pair(dynamic_cast<ee::Sprite*>(grid), m_id++));
 					ParserPicture(image, tsrc, tscreen);
 				}
 				else if (ee::FontBlankSprite* font = dynamic_cast<ee::FontBlankSprite*>(grid))
 				{
 					m_mapSpriteID.insert(std::make_pair(dynamic_cast<ee::Sprite*>(grid), m_id++));
 				}
 			}
 
 			m_mapSymbolID.insert(std::make_pair(sym, m_id++));
 			ParserScale9(patch9);
		}
		else if (const escale9::Symbol* patch9 = dynamic_cast<const escale9::Symbol*>(sym))
		{
		}
		else if (const emesh::Symbol* mesh = dynamic_cast<const emesh::Symbol*>(sym))
		{
			// mesh's id
			m_mapSymbolID.insert(std::make_pair(mesh, m_id++));
		}
		else if (const eterrain2d::Symbol* ocean = dynamic_cast<const eterrain2d::Symbol*>(sym))
		{
			m_mapSymbolID.insert(std::make_pair(ocean, m_id++));
		}
		else if (const etexture::Symbol* tex = dynamic_cast<const etexture::Symbol*>(sym))
		{
			m_mapSymbolID.insert(std::make_pair(tex, m_id++));
		}
	}
}

void CocoPacker::ParserPicture(const ee::ImageSprite* spr, PicFixType tsrc, PicFixType tscreen)
{
	tsrc = tscreen = e_null;

	const ee::Symbol* ee_sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
	TPParser::Picture* picture = m_parser.FindPicture(ee_sym);
	if (!picture) {
		std::string str = "\""+ee_sym->GetFilepath()+"\""+" not in the texpacker file 0!";
		throw ee::Exception(str);
	}

	lua::TableAssign ta(*m_gen, "picture", false, false);

	// id
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		if (itr == m_mapSpriteID.end()) {
			std::string str = "\""+ee_sym->GetFilepath()+"\""+" not in the m_mapSpriteID!";
			throw ee::Exception(str);
		}
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen->line(lua::assign("id", sid) + ",");
	}

	// tex
	std::string assignTex = lua::assign("tex", ee::StringHelper::ToString(picture->tex));

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

	if (m_img_ider) {
		int id = m_img_ider->Query(picture->filename);
		x0 = x1 = x2 = x3 = -id;
		y0 = y1 = y2 = y3 = -id;
	}

	std::string sx0 = ee::StringHelper::ToString(x0), sy0 = ee::StringHelper::ToString(y0);
	std::string sx1 = ee::StringHelper::ToString(x1), sy1 = ee::StringHelper::ToString(y1);
	std::string sx2 = ee::StringHelper::ToString(x2), sy2 = ee::StringHelper::ToString(y2);
	std::string sx3 = ee::StringHelper::ToString(x3), sy3 = ee::StringHelper::ToString(y3);
	std::string assignSrc = lua::assign("src", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	// screen
	const float hw = picture->entry->sprite_source_size.w * 0.5f / picture->invscale * m_scale,
		hh = picture->entry->sprite_source_size.h * 0.5f / picture->invscale * m_scale;
	sm::vec2 screen[4];
	screen[0].Set(-hw, hh);
	screen[1].Set(-hw, -hh);
	screen[2].Set(hw, -hh);
	screen[3].Set(hw, hh);
	
	// 1. shear
	float sx = spr->GetShear().x,
		sy = spr->GetShear().y;
	screen[0].x += sx * hh;
	screen[3].x += sx * hh;
	screen[1].x -= sx * hh;
	screen[2].x -= sx * hh;
	screen[2].y += sy * hw;
	screen[3].y += sy * hw;
	screen[1].y -= sy * hw;
	screen[0].y -= sy * hw;
	// 2. mirror
 	// 3. scale
 	for (size_t i = 0; i < 4; ++i)
 		screen[i].x *= spr->GetScale().x;
 	for (size_t i = 0; i < 4; ++i)
 		screen[i].y *= spr->GetScale().y;
 	// 4. rotate
 	for (size_t i = 0; i < 4; ++i)
 	{
 		sm::vec2 rot = sm::rotate_vector(screen[i], spr->GetAngle());
 		screen[i] = rot;
 	}
 	// 5. translate
 	sm::vec2 offset = picture->offset;
 	offset.x *= spr->GetScale().x / picture->invscale;
 	offset.y *= spr->GetScale().y / picture->invscale;
	if (spr->GetScale().x < 0) {
		offset.x = -offset.x;
	}
	if (spr->GetScale().y < 0) {
		offset.y = -offset.y;
	}
 	sm::vec2 center = spr->GetCenter();
 	center += sm::rotate_vector(offset, spr->GetAngle());
 	for (size_t i = 0; i < 4; ++i)
 		screen[i] += center;

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

	sx0 = ee::StringHelper::ToString(screen[0].x);
	sy0 = ee::StringHelper::ToString(screen[0].y);
	sx1 = ee::StringHelper::ToString(screen[1].x);
	sy1 = ee::StringHelper::ToString(screen[1].y);
	sx2 = ee::StringHelper::ToString(screen[2].x);
	sy2 = ee::StringHelper::ToString(screen[2].y);
	sx3 = ee::StringHelper::ToString(screen[3].x);
	sy3 = ee::StringHelper::ToString(screen[3].y);
	std::string assignScreen = lua::assign("screen", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	lua::tableassign(*m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

void CocoPacker::ParserPicture(const ee::ImageSymbol* sym, PicFixType tsrc)
{
	tsrc = e_null;

	TPParser::Picture* picture = m_parser.FindPicture(sym);
	if (!picture) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the texpacker file 1!";

// 		printf("symbol pointer: %p \n", sym);
// 		m_parser.DebugInfo();

		throw ee::Exception(str);
	}

	lua::TableAssign ta(*m_gen, "picture", false, false);

	// id
	{
		std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
		if (itr == m_mapSymbolID.end()) {
			std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 0!";
			throw ee::Exception(str);
		}
		std::string sid = ee::StringHelper::ToString(itr->second);
		m_gen->line(lua::assign("id", sid) + ",");
	}

	// tex
	std::string assignTex = lua::assign("tex", ee::StringHelper::ToString(picture->tex));

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

	if (m_img_ider) {
		int id = m_img_ider->Query(picture->filename);
		x0 = x1 = x2 = x3 = -id;
		y0 = y1 = y2 = y3 = -id;
	}

	std::string sx0 = ee::StringHelper::ToString(x0), sy0 = ee::StringHelper::ToString(y0);
	std::string sx1 = ee::StringHelper::ToString(x1), sy1 = ee::StringHelper::ToString(y1);
	std::string sx2 = ee::StringHelper::ToString(x2), sy2 = ee::StringHelper::ToString(y2);
	std::string sx3 = ee::StringHelper::ToString(x3), sy3 = ee::StringHelper::ToString(y3);
	std::string assignSrc = lua::assign("src", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	// screen
	const float hw = picture->entry->sprite_source_size.w * 0.5f / picture->invscale * m_scale,
		hh = picture->entry->sprite_source_size.h * 0.5f / picture->invscale * m_scale;
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
	std::string assignScreen = lua::assign("screen", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	lua::tableassign(*m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

int CocoPacker::ParserIcon(const eicon::Sprite* spr)
{
	int id = -1;

	const eicon::Symbol* sym = dynamic_cast<const eicon::Symbol*>(spr->GetSymbol());
	float proc = spr->GetProcess();

	std::map<const eicon::Symbol*, std::map<float, int> >::iterator itr_symbol
		= m_map_icon2ids.find(sym);
	if (itr_symbol != m_map_icon2ids.end()) 
	{
		std::map<float, int>::iterator itr_process 
			= itr_symbol->second.find(proc);
		if (itr_process != itr_symbol->second.end()) {
			id = itr_process->second;
			m_mapSpriteID.insert(std::make_pair(spr, id));
		} else {
			id = m_id++;
			ParserIcon(sym, proc, id);
			itr_symbol->second.insert(std::make_pair(proc, id));
			m_mapSpriteID.insert(std::make_pair(spr, id));
		}
	} 
	else 
	{
		id = m_id++;
		ParserIcon(sym, proc, id);
		std::map<float, int> procs;
		procs.insert(std::make_pair(proc, id));
		m_map_icon2ids.insert(make_pair(sym, procs));
		m_mapSpriteID.insert(std::make_pair(spr, id));
	}

	return id;
}

void CocoPacker::ParserIcon(const eicon::Symbol* sym, float process, int id)
{
	const ee::Symbol* img_sym = dynamic_cast<const ee::Symbol*>(sym->GetIcon()->GetImage());
	TPParser::Picture* picture = m_parser.FindPicture(img_sym);
	if (!picture) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the texpacker file 2!";
		throw ee::Exception(str);
	}

	lua::TableAssign ta(*m_gen, "picture", false, false);

	// id
	{
		std::string sid = ee::StringHelper::ToString(id);
		m_gen->line(lua::assign("id", sid) + ",");
	}

	// tex
	std::string assignTex = lua::assign("tex", ee::StringHelper::ToString(picture->tex));

	// src
	sm::vec2 node[4];
	sym->GetIcon()->GenTexcoords(process, node);
	int left = picture->scr[1].x, bottom = picture->scr[1].y;
	int width = picture->scr[2].x - left,
		height = picture->scr[0].y - bottom;
	std::string sx[4], sy[4];
	for (int i = 0; i < 4; ++i) {
		double x = left + width * node[i].x,
			y = bottom + height * node[i].y;
		sx[i] = ee::StringHelper::ToString(x);
		sy[i] = ee::StringHelper::ToString(y);
	}

	std::string assignSrc = lua::assign("src", 
		lua::tableassign("", 8, sx[0], sy[0], sx[1], sy[1], sx[2], sy[2], sx[3], sy[3]));

	// screen
	const float hw = picture->entry->sprite_source_size.w * 0.5f / picture->invscale * m_scale,
		hh = picture->entry->sprite_source_size.h * 0.5f / picture->invscale * m_scale;
	sm::vec2 screen[4];
	for (int i = 0; i < 4; ++i) {
		screen[i].x = -hw + hw * 2 * node[i].x;
		screen[i].y = -hh + hh * 2 * node[i].y;
	}

	// flip y
	for (size_t i = 0; i < 4; ++i)
		screen[i].y = -screen[i].y;
	// finish: scale
	const float SCALE = 16;
	for (size_t i = 0; i < 4; ++i)
		screen[i] *= SCALE;
	//
	for (int i = 0; i < 4; ++i) {
		sx[i] = ee::StringHelper::ToString(screen[i].x);
		sy[i] = ee::StringHelper::ToString(screen[i].y);
	}

	std::string assignScreen = lua::assign("screen", 
		lua::tableassign("", 8, sx[0], sy[0], sx[1], sy[1], sx[2], sy[2], sx[3], sy[3]));

	lua::tableassign(*m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

void CocoPacker::ParserComplex(const ecomplex::Symbol* sym)
{
	lua::TableAssign ta(*m_gen, "animation", false, false);

	ParserSymbolBase(sym);

	// clipbox
	const sm::rect& cb = sym->GetScissor();
	if (cb.xmin != 0 || cb.xmax != 0 || cb.ymin != 0 || cb.ymax != 0)
	{
		std::string width = ee::StringHelper::ToString(cb.xmax - cb.xmin);
		std::string height = ee::StringHelper::ToString(cb.ymax - cb.ymin);
		std::string left = ee::StringHelper::ToString(cb.xmin);
		std::string top = ee::StringHelper::ToString(-cb.ymax);
		lua::tableassign(*m_gen, "clipbox", 4, width, height, left, top);
	}

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;
	{
		lua::TableAssign ta(*m_gen, "component", true);
		const std::vector<s2::Sprite*>& children = sym->GetChildren();
		for (int i = 0, n = children.size(); i < n; ++i) {
			ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
			ParserSpriteForComponent(child, ids, unique, order);
		}
	}

	// children
	std::map<std::string, std::vector<ee::Sprite*> > map_actions;
	std::vector<ee::Sprite*> others;
	Utility::GroupSpritesFromTag(sym->GetChildren(), map_actions, others);
	if (!map_actions.empty())
	{
		std::map<std::string, std::vector<ee::Sprite*> >::iterator itr;
		for (itr = map_actions.begin(); itr != map_actions.end(); ++itr)
		{
			lua::TableAssign ta(*m_gen, "", true);
			m_gen->line(lua::assign("action", "\"" + itr->first + "\"") + ",");
			// frame 0
			{
				lua::TableAssign ta(*m_gen, "", true);
				const std::vector<s2::Sprite*>& children = sym->GetChildren();
				for (size_t i = 0, n = itr->second.size(); i < n; ++i)
				{
					int idx = -1;
					for (idx = 0; idx < children.size(); ++idx) {
						ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[idx]);
						if (child == itr->second[i])
							break;
					}
					ParserSpriteForFrame(itr->second[i], idx, ids, order);
				}
			}
		}
	}
	if (!others.empty())
	{
 		lua::TableAssign ta(*m_gen, "", true);
 		// frame 0
 		{
 			lua::TableAssign ta(*m_gen, "", true);
			const std::vector<s2::Sprite*>& children = sym->GetChildren();
  			for (size_t i = 0, n = others.size(); i < n; ++i)
			{
				int idx = -1;
				for (idx = 0; idx < children.size(); ++idx) {
					ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);					
					if (child == others[i])
						break;
				}
 				ParserSpriteForFrame(others[i], idx, ids, order);
			}
 		}
	}
}

void CocoPacker::ParserAnimation(const eanim::Symbol* sym)
{
	lua::TableAssign ta(*m_gen, "animation", false, false);

	ParserSymbolBase(sym);

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;

	// component for icon's tween
	std::map<int, int> map_id2idx;
	int comp_idx = order.size();

	{
		lua::TableAssign ta(*m_gen, "component", true);
		const std::vector<s2::AnimSymbol::Layer*>& layers = sym->GetLayers();
		for (size_t i = 0, n = sym->GetMaxFrameIdx(); i < n; ++i)
		{			
			for (size_t j = 0, m = layers.size(); j < m; ++j)
			{
				s2::AnimSymbol::Layer* layer = layers[j];
				if (i < layer->frames.size())
				{
					s2::AnimSymbol::Frame* frame = layer->frames[i];
					for (size_t k = 0, l = frame->sprs.size(); k < l; ++k) {
						ee::Sprite* spr = dynamic_cast<ee::Sprite*>(frame->sprs[k]);
						ParserSpriteForComponent(spr, ids, unique, order);
					}
				}
			}
		}

		// component for icon's tween
		comp_idx = order.size();
		for (int i = 0, n = sym->GetMaxFrameIdx(); i < n; ++i)
		{
			std::vector<s2::Sprite*> sprs;
			sym->CreateFrameSprites(i + 1, sprs);
			if (sprs.empty()) {
				continue;
			}
			for (int i = 0, n = sprs.size(); i < n; ++i)
			{
				if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(sprs[i])) {
					int id = QueryIconID(icon);
					assert(id != -1);
					std::map<int, int>::iterator itr_comp = map_id2idx.find(id);
					if (itr_comp == map_id2idx.end()) {
						std::string assign_id = lua::assign("id", ee::StringHelper::ToString(id));
						lua::tableassign(*m_gen, "", 1, assign_id);
						map_id2idx.insert(std::make_pair(id, comp_idx++));
					}
				}
			}
			for_each(sprs.begin(), sprs.end(), cu::RemoveRefFunctor<s2::Sprite>());
		}
	}	
 	// children
 	{
 		lua::TableAssign ta(*m_gen, "", true);
 		// frames
 		int index = 0;
 		for (size_t i = 1, n = sym->GetMaxFrameIdx(); i <= n; ++i)
 		{
 			lua::TableAssign ta(*m_gen, "", true);

			std::vector<s2::Sprite*> sprs;
			sym->CreateFrameSprites(i, sprs);
			for (size_t j = 0, m = sprs.size(); j < m; ++j)
				ParserSpriteForFrame(dynamic_cast<ee::Sprite*>(sprs[j]), order, map_id2idx);
			for_each(sprs.begin(), sprs.end(), cu::RemoveRefFunctor<s2::Sprite>());
 		}
 	}
}

void CocoPacker::ParserScale9(const escale9::Symbol* sym)
{
 	lua::TableAssign ta(*m_gen, "animation", false, false);
 
 	ParserSymbolBase(sym);

	std::vector<s2::Sprite*> grids;
	sym->GetScale9().GetGrids(grids);
 
 	// component
 	std::vector<int> ids;
 	std::map<int, std::vector<std::string> > unique;
 	std::vector<std::pair<int, std::string> > order;
 	{
 		lua::TableAssign ta(*m_gen, "component", true);
		for (int i = 0, n = grids.size(); i < n; ++i) {
			ParserSpriteForComponent(dynamic_cast<ee::Sprite*>(grids[i]), ids, unique, order);			
		}
 	}
 	// children
 	{
 		lua::TableAssign ta(*m_gen, "", true);
 		// frame 0
 		{
 			lua::TableAssign ta(*m_gen, "", true);
			for (int i = 0, n = grids.size(); i < n; ++i) {
				ParserSpriteForFrame(dynamic_cast<ee::Sprite*>(grids[i]), i, ids, order);				
			}
 		}
 	}
}

void CocoPacker::CalSrcFromUV(sm::vec2 src[4], TPParser::Picture* picture)
{
	if (picture->entry->rotated)
	{
		float w = fabs(picture->scr[1].y - picture->scr[2].y);
		float h = fabs(picture->scr[0].x - picture->scr[1].x);
		for (int i = 0; i < 4; ++i)
		{
			float u = src[i].x, v = src[i].y;
			src[i].x = picture->scr[1].x + v * h;
//			src[i].y = w - (picture->scr[1].y + (1-u) * w);
			src[i].y = picture->scr[1].y + (1-u) * w;
		}
	}
	else
	{
		float w = fabs(picture->scr[1].x - picture->scr[2].x);
		float h = fabs(picture->scr[0].y - picture->scr[1].y);
		for (int i = 0; i < 4; ++i)
		{
			float u = src[i].x, v = src[i].y;
			src[i].x = picture->scr[0].x + u * w;
//			src[i].y = h - (picture->scr[0].y + v * h);
			src[i].y = picture->scr[0].y + v * h;
		}
	}
}

void CocoPacker::CalSrcFromUVFixed(sm::vec2 src[4], TPParser::Picture* picture)
{
	if (picture->entry->rotated)
	{
		float w = fabs(picture->scr[1].y - picture->scr[2].y);
		float h = fabs(picture->scr[0].x - picture->scr[1].x);
		for (int i = 0; i < 4; ++i)
		{
			float u = src[i].x, v = src[i].y;
			src[i].x = picture->scr[2].x + v * h;
			src[i].y = picture->scr[2].y + (1-u) * w;
		}
	}
	else
	{
		float w = fabs(picture->scr[1].x - picture->scr[2].x);
		float h = fabs(picture->scr[0].y - picture->scr[1].y);
		for (int i = 0; i < 4; ++i)
		{
			float u = src[i].x, v = src[i].y;
			src[i].x = picture->scr[1].x + u * w;
			src[i].y = picture->scr[1].y + v * h;
		}
	}
}

int CocoPacker::ParserMesh(const emesh::Sprite* spr)
{
	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
	ee::Symbol* img_symbol = ee::SymbolMgr::Instance()->FetchSymbol(sym->GetFilepath());
	TPParser::Picture* picture = m_parser.FindPicture(img_symbol);
	if (!picture) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the texpacker file 3!";
		throw ee::Exception(str);
	}

	//////////////////////////////////////////////////////////////////////////
	// pictures
	//////////////////////////////////////////////////////////////////////////
	// id
	std::map<const ee::Sprite*, int>::iterator itr_sprite = m_mapSpriteID.find(spr);
	if (itr_sprite == m_mapSpriteID.end()) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the m_mapSpriteID!";
		throw ee::Exception(str);
	}
	int curr_id = itr_sprite->second;
	// tex
	std::string assign_tex = lua::assign("tex", ee::StringHelper::ToString(picture->tex));

	int frame = 1;
	if (spr->GetSpeed().y != 0) {
		frame = std::fabs(std::floor(1.0f / spr->GetSpeed().y));
	}
	std::vector<int> frame_size;
	sm::vec2 speed = spr->GetSpeed();
	emesh::Mesh* shape = const_cast<emesh::Mesh*>(dynamic_cast<const emesh::Mesh*>(dynamic_cast<const emesh::Symbol*>(sym)->GetMesh()));
	// 打包emesh::Strip做的流水
	if (dynamic_cast<emesh::Strip*>(shape))
	{
		for (int i = 0; i < frame; ++i)
		{
			// todo 只是具体strip的情况，不支持mesh的旋转
			const std::vector<s2::MeshTriangle*>& tris = shape->GetTriangles();

			int quad_size = tris.size() / 2;
			frame_size.push_back(quad_size);

			for (int j = 0; j < quad_size; ++j)
			{
				s2::MeshTriangle* right_down = tris[j*2];
				s2::MeshTriangle* left_up = tris[j*2+1];

				// id
				lua::TableAssign ta(*m_gen, "picture", false, false);
				m_gen->line(lua::assign("id", ee::StringHelper::ToString(curr_id++)) + ",");

				// src
				sm::vec2 src[4];
				src[0] = left_up->nodes[2]->uv;
				src[1] = left_up->nodes[0]->uv;
				src[2] = right_down->nodes[1]->uv;
				src[3] = right_down->nodes[2]->uv;
				CalSrcFromUV(src, picture);
				std::string sx0 = ee::StringHelper::ToString(src[0].x), sy0 = ee::StringHelper::ToString(src[0].y);
				std::string sx1 = ee::StringHelper::ToString(src[1].x), sy1 = ee::StringHelper::ToString(src[1].y);
				std::string sx2 = ee::StringHelper::ToString(src[2].x), sy2 = ee::StringHelper::ToString(src[2].y);
				std::string sx3 = ee::StringHelper::ToString(src[3].x), sy3 = ee::StringHelper::ToString(src[3].y);
				std::string assign_src = lua::assign("src", 
					lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));		

				// screen
				sm::vec2 screen[4];
				screen[0] = left_up->nodes[2]->xy;
				screen[1] = left_up->nodes[0]->xy;
				screen[2] = right_down->nodes[1]->xy;
				screen[3] = right_down->nodes[2]->xy;
				// 			// translate
				// 			for (size_t i = 0; i < 4; ++i)
				// 				screen[i] += spr->getPosition();
				// flip y
				for (size_t i = 0; i < 4; ++i)
					screen[i].y = -screen[i].y;
				// scale 16
				const float SCALE = 16;
				for (size_t i = 0; i < 4; ++i)
					screen[i] *= SCALE; 
				std::string dx0 = ee::StringHelper::ToString(screen[0].x); std::string dy0 = ee::StringHelper::ToString(screen[0].y);
				std::string dx1 = ee::StringHelper::ToString(screen[1].x); std::string dy1 = ee::StringHelper::ToString(screen[1].y);
				std::string dx2 = ee::StringHelper::ToString(screen[2].x); std::string dy2 = ee::StringHelper::ToString(screen[2].y);
				std::string dx3 = ee::StringHelper::ToString(screen[3].x); std::string dy3 = ee::StringHelper::ToString(screen[3].y);
				std::string assign_screen = lua::assign("screen", 
					lua::tableassign("", 8, dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3));

				lua::tableassign(*m_gen, "", 3, assign_tex, assign_src, assign_screen);
			}
			shape->OffsetUV(speed.x, speed.y);
		}
	}
	// 打包普通emesh::Mesh
	else if (dynamic_cast<emesh::Network*>(shape))
	{
		const std::vector<s2::MeshTriangle*>& tris = shape->GetTriangles();
		frame_size.push_back(tris.size());
		for (int i = 0, n = tris.size(); i < n; ++i)
		{
			s2::MeshTriangle* tri = tris[i];

			// id
			lua::TableAssign ta(*m_gen, "picture", false, false);
			m_gen->line(lua::assign("id", ee::StringHelper::ToString(curr_id++)) + ",");

			// src
			sm::vec2 src[4];
			src[0] = tri->nodes[0]->uv;
			src[1] = tri->nodes[1]->uv;
			src[2] = tri->nodes[2]->uv;
			src[3] = tri->nodes[2]->uv;
			CalSrcFromUV(src, picture);
			std::string sx0 = ee::StringHelper::ToString(src[0].x), sy0 = ee::StringHelper::ToString(src[0].y);
			std::string sx1 = ee::StringHelper::ToString(src[1].x), sy1 = ee::StringHelper::ToString(src[1].y);
			std::string sx2 = ee::StringHelper::ToString(src[2].x), sy2 = ee::StringHelper::ToString(src[2].y);
			std::string sx3 = ee::StringHelper::ToString(src[3].x), sy3 = ee::StringHelper::ToString(src[3].y);
			std::string assign_src = lua::assign("src", 
				lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));		

			// screen
			sm::vec2 screen[4];
			screen[0] = tri->nodes[0]->xy;
			screen[1] = tri->nodes[1]->xy;
			screen[2] = tri->nodes[2]->xy;
			screen[3] = tri->nodes[2]->xy;
			// 			// translate
			// 			for (size_t i = 0; i < 4; ++i)
			// 				screen[i] += spr->getPosition();
			// flip y
			for (size_t i = 0; i < 4; ++i)
				screen[i].y = -screen[i].y;
			// scale 16
			const float SCALE = 16;
			for (size_t i = 0; i < 4; ++i)
				screen[i] *= SCALE; 
			std::string dx0 = ee::StringHelper::ToString(screen[0].x); std::string dy0 = ee::StringHelper::ToString(screen[0].y);
			std::string dx1 = ee::StringHelper::ToString(screen[1].x); std::string dy1 = ee::StringHelper::ToString(screen[1].y);
			std::string dx2 = ee::StringHelper::ToString(screen[2].x); std::string dy2 = ee::StringHelper::ToString(screen[2].y);
			std::string dx3 = ee::StringHelper::ToString(screen[3].x); std::string dy3 = ee::StringHelper::ToString(screen[3].y);
			std::string assign_screen = lua::assign("screen", 
				lua::tableassign("", 8, dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3));

			lua::tableassign(*m_gen, "", 3, assign_tex, assign_src, assign_screen);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// animation
	//////////////////////////////////////////////////////////////////////////
	lua::TableAssign ta(*m_gen, "animation", false, false);
	// export
	if (!sym->name.empty())
		m_gen->line(lua::assign("export", "\""+sym->name+"\"")+",");
 	// id
 	std::map<const ee::Symbol*, int>::iterator itr_mesh_symbol = m_mapSymbolID.find(sym);
 	if (itr_mesh_symbol == m_mapSymbolID.end()) {
 		std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 1!";
 		throw ee::Exception(str);
 	}
 	std::string sid = ee::StringHelper::ToString(itr_mesh_symbol->second);
 	m_gen->line(lua::assign("id", sid) + ",");
	// component
	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (int id = itr_sprite->second; id < curr_id; ++id)
		{
			std::string assign_id = lua::assign("id", ee::StringHelper::ToString(id));
			lua::tableassign(*m_gen, "", 1, assign_id);
		}
	}
	// frames
	int id = 0;
	{
		lua::TableAssign ta(*m_gen, "", true);
		
		for (int i = 0; i < frame; ++i)
		{
			lua::TableAssign ta(*m_gen, "", true);
			for (int j = 0; j < frame_size[i]; ++j)
			{
				std::vector<std::string> params;
				std::string assign_index = lua::assign("index", ee::StringHelper::ToString(id++));
				params.push_back(assign_index);
				GetColorAssignParams(spr, params);
				lua::tableassign(*m_gen, "", params);
			}
		}
	}

	return id;
}

int CocoPacker::ParserTerrain2D(const eterrain2d::Sprite* spr)
{
	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
	std::map<const ee::Symbol*, int>::iterator itr_mesh_symbol = m_mapSymbolID.find(sym);
	if (itr_mesh_symbol == m_mapSymbolID.end()) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 2!";
		throw ee::Exception(str);
	}

	if (m_terrain2d_ids.find(itr_mesh_symbol->second) != m_terrain2d_ids.end()) {
		return 0;
	} else {
		m_terrain2d_ids.insert(itr_mesh_symbol->second);
	}

	// only use one texture in the sprite

	const std::vector<eterrain2d::OceanMesh*> oceans = static_cast<const eterrain2d::Symbol*>(sym)->GetOceans();
	assert(oceans.size() == 1);
	eterrain2d::OceanMesh* ocean = oceans[0];
	const ee::ImageSymbol* img = ocean->GetImage0();
	ee::Symbol* img_symbol = ee::SymbolMgr::Instance()->FetchSymbol(img->GetFilepath());
	TPParser::Picture* picture = m_parser.FindPicture(img_symbol);
	if (!picture) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the texpacker file 4!";
		throw ee::Exception(str);
	}

	//////////////////////////////////////////////////////////////////////////
	// pictures
	//////////////////////////////////////////////////////////////////////////
	// id
	std::map<const ee::Sprite*, int>::iterator itr_sprite = m_mapSpriteID.find(spr);
	if (itr_sprite == m_mapSpriteID.end()) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the m_mapSpriteID!";
		throw ee::Exception(str);
	}
	int curr_id = itr_sprite->second;
	// tex
	std::string assign_tex = lua::assign("tex", ee::StringHelper::ToString(picture->tex));

	static const float FPS = 30;

	int frame = (int)(fabs(FPS / ocean->GetUVMoveSpeed().y));

	for (int i = 0; i < oceans.size(); ++i) {
		eterrain2d::OceanMesh* ocean = oceans[i];
		ocean->Build();
	}

	std::vector<int> frames_count;
	for (int i = 0; i < frame; ++i)
	{
		int img_count = 0;

		for (int i = 0; i < oceans.size(); ++i) {
			eterrain2d::OceanMesh* ocean = oceans[i];
			ocean->OpenWave(false);
			ocean->OpenBlend(false);
			const std::vector<eterrain2d::MeshShape*>& meshes = ocean->GetMeshes();

			// id
			lua::TableAssign ta(*m_gen, "picture", false, false);
			m_gen->line(lua::assign("id", ee::StringHelper::ToString(curr_id++)) + ",");

			

			++img_count;

			for (int j = 0; j < meshes.size(); ++j) {
				const eterrain2d::MeshShape* mesh = meshes[j];
				const std::vector<s2::MeshTriangle*>& tris = mesh->GetTriangles();
				for (int k = 0; k < tris.size(); ++k) {
					// same with 打包普通emesh::Mesh

					s2::MeshTriangle* tri = tris[k];

					// src
					sm::vec2 src[4];
					src[0] = tri->nodes[0]->uv;
					src[1] = tri->nodes[1]->uv;
					src[2] = tri->nodes[2]->uv;
					src[3] = tri->nodes[2]->uv;
					CalSrcFromUVFixed(src, picture);
					std::string sx0 = ee::StringHelper::ToString(src[0].x), sy0 = ee::StringHelper::ToString(src[0].y);
					std::string sx1 = ee::StringHelper::ToString(src[1].x), sy1 = ee::StringHelper::ToString(src[1].y);
					std::string sx2 = ee::StringHelper::ToString(src[2].x), sy2 = ee::StringHelper::ToString(src[2].y);
					std::string sx3 = ee::StringHelper::ToString(src[3].x), sy3 = ee::StringHelper::ToString(src[3].y);
					std::string assign_src = lua::assign("src", lua::tableassign("", 8, sx0, sy0, 
						sx1, sy1, sx2, sy2, sx3, sy3));		

					// screen
					sm::vec2 screen[4];
					screen[0] = tri->nodes[0]->xy;
					screen[1] = tri->nodes[1]->xy;
					screen[2] = tri->nodes[2]->xy;
					screen[3] = tri->nodes[2]->xy;
					// 			// translate
					// 			for (size_t i = 0; i < 4; ++i)
					// 				screen[i] += spr->getPosition();
					// flip y
					for (size_t i = 0; i < 4; ++i)
						screen[i].y = -screen[i].y;
					// scale 16
					const float SCALE = 16;
					for (size_t i = 0; i < 4; ++i)
						screen[i] *= SCALE; 
					std::string dx0 = ee::StringHelper::ToString(screen[0].x); std::string dy0 = ee::StringHelper::ToString(screen[0].y);
					std::string dx1 = ee::StringHelper::ToString(screen[1].x); std::string dy1 = ee::StringHelper::ToString(screen[1].y);
					std::string dx2 = ee::StringHelper::ToString(screen[2].x); std::string dy2 = ee::StringHelper::ToString(screen[2].y);
					std::string dx3 = ee::StringHelper::ToString(screen[3].x); std::string dy3 = ee::StringHelper::ToString(screen[3].y);
					std::string assign_screen = lua::assign("screen", lua::tableassign("", 8, dx0, dy0, 
						dx1, dy1, dx2, dy2, dx3, dy3));

					lua::tableassign(*m_gen, "", 3, assign_tex, assign_src, assign_screen);
				}
			}

			ocean->Update(1 / FPS);
		}

 		frames_count.push_back(img_count);
	}

	//////////////////////////////////////////////////////////////////////////
	// animation
	//////////////////////////////////////////////////////////////////////////

	// copy from ParserMesh

	lua::TableAssign ta(*m_gen, "animation", false, false);
	// export
	if (!sym->name.empty())
		m_gen->line(lua::assign("export", "\""+sym->name+"\"")+",");

	std::string sid = ee::StringHelper::ToString(itr_mesh_symbol->second);
	m_gen->line(lua::assign("id", sid) + ",");
	// component
	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (int id = itr_sprite->second; id < curr_id; ++id)
		{
			std::string assign_id = lua::assign("id", ee::StringHelper::ToString(id));
			lua::tableassign(*m_gen, "", 1, assign_id);
		}
	}
	// frames
	int id = 0;
	{
		lua::TableAssign ta(*m_gen, "", true);

		for (int i = 0; i < frame; ++i)
		{
			lua::TableAssign ta(*m_gen, "", true);
			for (int j = 0; j < frames_count[i]; ++j)
			{
				std::vector<std::string> params;
				std::string assign_index = lua::assign("index", ee::StringHelper::ToString(id++));
				params.push_back(assign_index);
				GetColorAssignParams(spr, params);
				lua::tableassign(*m_gen, "", params);
			}
		}
	}

	return id;
}

int CocoPacker::ParserTexture(const etexture::Sprite* spr)
{
	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
	const std::vector<s2::PolygonShape*>& polys = static_cast<const etexture::Symbol*>(sym)->GetPolygons();
	assert(polys.size() == 1);
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(polys[0]);
	assert(poly);
	const eshape::TextureMaterial* material = dynamic_cast<const eshape::TextureMaterial*>(poly->GetMaterial());
	assert(material);
	const ee::ImageSymbol* img_symbol = material->GetImage();
	TPParser::Picture* picture = m_parser.FindPicture(img_symbol);
	if (!picture) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the texpacker file 5!";
		throw ee::Exception(str);
	}

	//////////////////////////////////////////////////////////////////////////
	// pictures
	//////////////////////////////////////////////////////////////////////////
	// id
	std::map<const ee::Sprite*, int>::iterator itr_sprite = m_mapSpriteID.find(spr);
	if (itr_sprite == m_mapSpriteID.end()) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in the m_mapSpriteID!";
		throw ee::Exception(str);
	}
	int curr_id = itr_sprite->second;
	// tex
	std::string assign_tex = lua::assign("tex", ee::StringHelper::ToString(picture->tex));

	const std::vector<sm::vec2>& vertices = material->GetTriangles();
	const std::vector<sm::vec2>& texcoords = material->GetTexcoords();
	assert(vertices.size() == texcoords.size() && vertices.size() % 3 == 0);
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		// id
		lua::TableAssign ta(*m_gen, "picture", false, false);
		m_gen->line(lua::assign("id", ee::StringHelper::ToString(curr_id++)) + ",");
		
		// src
		sm::vec2 src[4];
		for (int j = 0; j < 3; ++j) {
			src[j] = texcoords[i+j];
		}
		src[3] = src[2];
		CalSrcFromUVFixed(src, picture);
		std::string sx0 = ee::StringHelper::ToString(src[0].x), sy0 = ee::StringHelper::ToString(src[0].y);
		std::string sx1 = ee::StringHelper::ToString(src[1].x), sy1 = ee::StringHelper::ToString(src[1].y);
		std::string sx2 = ee::StringHelper::ToString(src[2].x), sy2 = ee::StringHelper::ToString(src[2].y);
		std::string sx3 = ee::StringHelper::ToString(src[3].x), sy3 = ee::StringHelper::ToString(src[3].y);
		std::string assign_src = lua::assign("src", lua::tableassign("", 8, sx0, sy0, 
			sx1, sy1, sx2, sy2, sx3, sy3));		

		// screen
		sm::vec2 screen[4];
		for (int j = 0; j < 3; ++j) {
			screen[j] = vertices[i+j];
		}
		screen[3] = screen[2];
		// flip y
		for (size_t i = 0; i < 4; ++i)
			screen[i].y = -screen[i].y;
		// scale 16
		const float SCALE = 16;
		for (size_t i = 0; i < 4; ++i)
			screen[i] *= SCALE; 
		std::string dx0 = ee::StringHelper::ToString(screen[0].x); std::string dy0 = ee::StringHelper::ToString(screen[0].y);
		std::string dx1 = ee::StringHelper::ToString(screen[1].x); std::string dy1 = ee::StringHelper::ToString(screen[1].y);
		std::string dx2 = ee::StringHelper::ToString(screen[2].x); std::string dy2 = ee::StringHelper::ToString(screen[2].y);
		std::string dx3 = ee::StringHelper::ToString(screen[3].x); std::string dy3 = ee::StringHelper::ToString(screen[3].y);
		std::string assign_screen = lua::assign("screen", lua::tableassign("", 8, dx0, dy0, 
			dx1, dy1, dx2, dy2, dx3, dy3));

		lua::tableassign(*m_gen, "", 3, assign_tex, assign_src, assign_screen);
	}

	//////////////////////////////////////////////////////////////////////////
	// animation
	//////////////////////////////////////////////////////////////////////////
	lua::TableAssign ta(*m_gen, "animation", false, false);
	// export
	if (!sym->name.empty())
		m_gen->line(lua::assign("export", "\""+sym->name+"\"")+",");
	// id
	std::map<const ee::Symbol*, int>::iterator itr_mesh_symbol = m_mapSymbolID.find(sym);
	if (itr_mesh_symbol == m_mapSymbolID.end()) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 3!";
		throw ee::Exception(str);
	}
	std::string sid = ee::StringHelper::ToString(itr_mesh_symbol->second);
	m_gen->line(lua::assign("id", sid) + ",");
	// component
	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (int id = itr_sprite->second; id < curr_id; ++id)
		{
			std::string assign_id = lua::assign("id", ee::StringHelper::ToString(id));
			lua::tableassign(*m_gen, "", 1, assign_id);
		}
	}
	// frames
	int id = 0;
	{
		lua::TableAssign ta0(*m_gen, "", true);
		lua::TableAssign ta1(*m_gen, "", true);
		for (int i = 0; i < vertices.size() / 3; ++i)
		{
			std::vector<std::string> params;
			std::string assign_index = lua::assign("index", ee::StringHelper::ToString(id++));
			params.push_back(assign_index);
			GetColorAssignParams(spr, params);
			lua::tableassign(*m_gen, "", params);
		}
	}

	return id;
}

void CocoPacker::ParserSymbolBase(const ee::Symbol* sym)
{
	// export
	if (!sym->name.empty())
		m_gen->line(lua::assign("export", "\""+sym->name+"\"")+",");

	// id
	std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
	if (itr == m_mapSymbolID.end()) {
		std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 4!";
		throw ee::Exception(str);
	}

	std::string sid = ee::StringHelper::ToString(itr->second);
	m_gen->line(lua::assign("id", sid) + ",");
}

void CocoPacker::ParserSpriteForComponent(const ee::Sprite* spr, std::vector<int>& ids, 
										std::map<int, std::vector<std::string> >& unique, 
										std::vector<std::pair<int, std::string> >& order)
{
	int id;

	bool isFont = false;

	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
	if (const ee::ImageSprite* image = dynamic_cast<const ee::ImageSprite*>(spr))
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		if (itr != m_mapSpriteID.end())
		{
			id = itr->second;
		}
		else
		{
			// eanim::Symbol's sprs store unique

			std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
			if (itr == m_mapSymbolID.end()) {
				std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 5!";
				throw ee::Exception(str);
			}
			id = itr->second;
		}
	}
	else if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr))
	{
		isFont = true;
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		if (itr == m_mapSpriteID.end()) {
			std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSpriteID!";
			throw ee::Exception(str);
		}
		id = itr->second;
	}
	else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr))
	{
		std::map<const ee::Sprite*, int>::iterator itr = m_mapSpriteID.find(spr);
		if (itr == m_mapSpriteID.end()) {
			std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSpriteID!";
			throw ee::Exception(str);
		}
		id = itr->second;
	}
	else
	{
		std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
		if (itr == m_mapSymbolID.end()) {
 			std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 6!";
 			throw ee::Exception(str);
		}
		id = itr->second;
	}

	ids.push_back(id);

	const std::string& name = spr->GetName();

	std::map<int, std::vector<std::string> >::iterator itr = unique.find(id);
	if (unique.find(id) == unique.end())
	{
		if (isFont)
		{
			const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr);
			bool is_mount_node = font && font->font.empty() && font->font_color == s2::Color(0, 0, 0, 0);
			if (is_mount_node)
			{
				std::string aName = lua::assign("name", "\""+name+"\"");
				lua::tableassign(*m_gen, "", 1, aName);
			}
			else
			{
				std::string aName = lua::assign("name", "\""+name+"\"");
				std::string aFont = lua::assign("font", "\""+font->font+"\"");
				std::string aColor = lua::assign("color", gum::color2str(font->font_color, gum::ARGB));

//				std::string aAlign = lua::assign("align", ee::StringHelper::ToString(font->align));
				int align_hori = font->align_hori;
				int align_vert = font->align_vert;
				int align = align_hori | (align_vert << 4);
				std::string aAlign = lua::assign("align", ee::StringHelper::ToString(align));

				std::string aSize = lua::assign("size", ee::StringHelper::ToString(font->size));
				std::string aWidth = lua::assign("width", ee::StringHelper::ToString(font->width));
				std::string aHeight = lua::assign("height", ee::StringHelper::ToString(font->height));

				lua::tableassign(*m_gen, "", 7, aName, aFont, aColor, aAlign, 
					aSize, aWidth, aHeight);
			}
		}
		else
		{
			bool is_mount_node = false;
			const ecomplex::Sprite* ecomplex = dynamic_cast<const ecomplex::Sprite*>(spr);
			if (ecomplex) {
				const ecomplex::Symbol* comp_sym = dynamic_cast<const ecomplex::Symbol*>(ecomplex->GetSymbol());
				if (comp_sym->GetChildren().size() == 1) {
					const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(comp_sym->GetChildren()[0]);
					is_mount_node = font && font->font.empty() && font->font_color == s2::Color(0, 0, 0, 0);
				}
			}
			if (is_mount_node) {
				std::string aName = lua::assign("name", "\""+name+"\"");
				lua::tableassign(*m_gen, "", 1, aName);
			} else {
				std::string aID = lua::assign("id", ee::StringHelper::ToString(id));
				if (!name.empty() && name[0] != '_')
				{
					std::string aName = lua::assign("name", "\""+name+"\"");
					lua::tableassign(*m_gen, "", 2, aName, aID);
				}
				else
				{
					lua::tableassign(*m_gen, "", 1, aID);
				}
			}
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
			bool is_mount_node = false;
			const ecomplex::Sprite* ecomplex = dynamic_cast<const ecomplex::Sprite*>(spr);
			if (ecomplex) {
				const ecomplex::Symbol* comp_sym = dynamic_cast<const ecomplex::Symbol*>(ecomplex->GetSymbol());
				if (comp_sym->GetChildren().size() == 1) {
					ee::Sprite* child = dynamic_cast<ee::Sprite*>(comp_sym->GetChildren()[0]);				
					const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(child);
					is_mount_node = font && font->font.empty() && font->font_color == s2::Color(0, 0, 0, 0);
				}
			}
			if (is_mount_node) {
				std::string aName = lua::assign("name", "\""+name+"\"");
				lua::tableassign(*m_gen, "", 1, aName);
			} else {
				std::string aID = lua::assign("id", ee::StringHelper::ToString(id));
				if (!name.empty() && name[0] != '_')
				{
					std::string aName = lua::assign("name", "\""+name+"\"");
					lua::tableassign(*m_gen, "", 2, aName, aID);
				}
				else
				{
					lua::tableassign(*m_gen, "", 1, aID);
				}
			}

			order.push_back(std::make_pair(id, name));
		}
		itr->second.push_back(name);

//		if (!name.empty())
//			order.push_back(std::make_pair(id, name));
	}
}

void CocoPacker::ParserSpriteForFrame(const ee::Sprite* spr, int index,
									const std::vector<int>& ids, const std::vector<std::pair<int, std::string> >& order)
{
	int id = ids[index];
	int cindex = -1;
	const std::string& name = spr->GetName();
	for (size_t i = 0, n = order.size(); i < n; ++i)
		if (id == order[i].first && name == order[i].second)
		{
			cindex = i;
			break;
		}
	if (cindex == -1) {
		std::string str = name + " not found in order!";
		throw ee::Exception(str);
	}	

	if (const ee::FontBlankSprite* font = dynamic_cast<const ee::FontBlankSprite*>(spr))
		ParserFontForFrame(font, cindex);
	else
		ParserImageForFrame(spr, cindex);

// 	bool forceMat = dynamic_cast<const ee::FontBlankSprite*>(spr);
// 	resolveSpriteForFrame(spr, cindex, forceMat);
}

void CocoPacker::ParserSpriteForFrame(const ee::Sprite* spr, 
									  const std::vector<std::pair<int, std::string> >& order,
									  const std::map<int, int>& map_id2idx)
{
	const std::string& name = spr->GetName();
	if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(spr)) 
	{
		int id = QueryIconID(icon);
		assert(id != -1);
		std::map<int, int>::const_iterator itr = map_id2idx.find(id);
		assert(itr != map_id2idx.end());
		ParserSpriteForFrame(spr, itr->second, true);
	} 
	else 
	{
		const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
		std::map<const ee::Symbol*, int>::iterator itr = m_mapSymbolID.find(sym);
		if (itr == m_mapSymbolID.end()) {
			std::string str = "\""+sym->GetFilepath()+"\""+" not in m_mapSymbolID 7!";
			throw ee::Exception(str);
		}
		int id = itr->second;

		int cindex = -1;
		for (size_t i = 0, n = order.size(); i < n; ++i) {
			if (id == order[i].first && name == order[i].second) {
				cindex = i;
				break;
			}
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

		if (cindex == -1) {
			std::string str = name + " not in order!";
			throw ee::Exception(str);
		}

		ParserSpriteForFrame(spr, cindex, true);
	}
}

void CocoPacker::ParserSpriteForFrame(const ee::Sprite* spr, int id, bool forceMat)
{
	std::vector<std::string> params;

	std::string assignIndex = lua::assign("index", ee::StringHelper::ToString(id));
	params.push_back(assignIndex);

	float mat[6];
	TransToMat(spr, mat, forceMat);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = ee::StringHelper::ToString(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);
	params.push_back(assignMat);

	GetColorAssignParams(spr, params);

	lua::tableassign(*m_gen, "", params);
}

void CocoPacker::ParserImageForFrame(const ee::Sprite* spr, int id)
{
	std::vector<std::string> params;

	std::string assignIndex = lua::assign("index", ee::StringHelper::ToString(id));
	params.push_back(assignIndex);

	float mat[6];
	TransToMat(spr, mat, false);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = ee::StringHelper::ToString(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);
	params.push_back(assignMat);

	if (spr->IsClip()) {
		params.push_back("clip=true");
	}

	GetColorAssignParams(spr, params);

	lua::tableassign(*m_gen, "", params);
}

void CocoPacker::ParserFontForFrame(const ee::FontBlankSprite* spr, int id)
{
	std::string assignIndex = lua::assign("index", ee::StringHelper::ToString(id));

	float mat[6];
	TransToMat(spr, mat, true);

	bool isNullNode = spr->font.empty() && spr->font_color == s2::Color(0, 0, 0, 0);
	if (!isNullNode)
	{
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

	lua::tableassign(*m_gen, "", 2, assignIndex, assignMat);
}

void CocoPacker::TransToMat(const ee::Sprite* spr, float mat[6], bool force /*= false*/) const
{
	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
	if (!force &&
		(dynamic_cast<const ee::ImageSprite*>(spr) ||
		dynamic_cast<const ee::FontBlankSprite*>(spr)))
	{
	}
	else
	{
		// | 1  ky    | | sx       | |  c  s    | | 1       |
		// | kx  1    | |    sy    | | -s  c    | |    1    |
		// |        1 | |        1 | |        1 | | x  y  1 |
		//     skew        scale        rotate        move

		sm::vec2 center = spr->GetCenter();
		if (dynamic_cast<const ee::ImageSprite*>(spr))
		{
			TPParser::Picture* picture = m_parser.FindPicture(sym);
			if (!picture) {
				std::string str = "\""+sym->GetFilepath()+"\""+" not in the texpacker file 6!";
				throw ee::Exception(str);
			}

			sm::vec2 offset = picture->offset;
			if (spr->GetScale().x < 0) {
				offset.x = -offset.x;
			}
			if (spr->GetScale().y < 0) {
				offset.y = -offset.y;
			}
			offset.x *= spr->GetScale().x / picture->invscale;
			offset.y *= spr->GetScale().y / picture->invscale;
			center += sm::rotate_vector(offset, spr->GetAngle());
		}
		float sx = spr->GetScale().x,
			  sy = spr->GetScale().y;

		float c = cos(-spr->GetAngle()),
			s = sin(-spr->GetAngle());
		float kx = -spr->GetShear().x,
			  ky = -spr->GetShear().y;
		mat[0] = sx*c - ky*sy*s;
		mat[1] = sx*s + ky*sy*c;
		mat[2] = kx*sx*c - sy*s;
		mat[3] = kx*sx*s + sy*c;
		mat[4] = center.x * m_scale;
		mat[5] = center.y * m_scale;
	}

	for (size_t i = 0; i < 4; ++i)
		mat[i] = floor(mat[i] * 1024 + 0.5f);
	for (size_t i = 4; i < 6; ++i)
		mat[i] = floor(mat[i] * 16 + 0.5f);
	// flip y
	mat[5] = -mat[5];
}

void CocoPacker::GetColorAssignParams(const ee::Sprite* spr, std::vector<std::string>& params) const
{
	if (spr->Color().mul != s2::Color(1,1,1,1) || spr->Color().add != s2::Color(0,0,0,0)) 
	{
		std::string str_multi = lua::assign("color", color2int(spr->Color().mul, gum::BGRA));
		params.push_back(str_multi);
		std::string str_add = lua::assign("add", color2int(spr->Color().add, gum::ABGR));
		params.push_back(str_add);
	}

	if (spr->Color().rmap != s2::Color(255, 0, 0, 255) || spr->Color().gmap != s2::Color(0, 255, 0, 255) || spr->Color().bmap != s2::Color(0, 0, 255, 255))
	{
		std::string str_r = lua::assign("r_map", gum::color2str(spr->Color().rmap, gum::RGBA));
		params.push_back(str_r);

		std::string str_g = lua::assign("g_map", gum::color2str(spr->Color().gmap, gum::RGBA));
		params.push_back(str_g);

		std::string str_b = lua::assign("b_map", gum::color2str(spr->Color().bmap, gum::RGBA));
		params.push_back(str_b);
	}
}

int CocoPacker::QueryIconID(const eicon::Sprite* icon) const
{
	std::map<const eicon::Symbol*, std::map<float, int> >::const_iterator itr
		= m_map_icon2ids.find(dynamic_cast<const eicon::Symbol*>(icon->GetSymbol()));
	if (itr == m_map_icon2ids.end()) {
		return -1;
	} else {
		std::map<float, int>::const_iterator itr_id 
			= itr->second.find(icon->GetProcess());
		if (itr_id == itr->second.end()) {
			return -1;
		} else {
			return itr_id->second;
		}
	}
}

}
}