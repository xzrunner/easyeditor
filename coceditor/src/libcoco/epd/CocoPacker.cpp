#include "CocoPacker.h"
#include "../Utility.h"

#include <queue>

#include <easybuilder.h>
#include <easycomplex.h>
#include <easyanim.h>
#include <easyscale9.h>
#include <easymesh.h>
#include <easyterrain2d.h>
#include <easytexture.h>
#include <easyicon.h>
#include <epbin.h>

namespace libcoco
{
namespace epd
{

namespace lua = ebuilder::lua;

CocoPacker::CocoPacker(const std::vector<const d2d::Symbol*>& symbols, 
					   const TextureMgr& tex_mgr)
	: m_gen(new ebuilder::CodeGenerator)
	, m_parser(symbols, tex_mgr)
	, m_scale(1)
	, m_id(0)
	, m_img_ider(NULL)
{
}

CocoPacker::CocoPacker(const std::vector<const d2d::Symbol*>& symbols, 
					   const TextureMgr& tex_mgr, 
					   const std::string& img_id_file, 
					   float scale)
	: m_gen(new ebuilder::CodeGenerator)
	, m_parser(symbols, tex_mgr)
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
	m_gen->line("texture("+wxString::FromDouble(m_parser.GetTexSize()).ToStdString()+")");

	const std::vector<const d2d::Symbol*>& symbols = m_parser.GetSymbolSet().GetOrdered();
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		const d2d::Symbol* symbol = symbols[i];

		wxString msg;
		msg.Printf("[%d/%d] file: %s\n", i, n, symbol->GetFilepath());
		std::cout << msg;

		if (const d2d::ImageSymbol* image = dynamic_cast<const d2d::ImageSymbol*>(symbol))
		{
		}
		else if (const d2d::FontBlankSymbol* font = dynamic_cast<const d2d::FontBlankSymbol*>(symbol))
		{
		}
		else if (const ecomplex::Symbol* complex = dynamic_cast<const ecomplex::Symbol*>(symbol))
		{
			for (size_t i = 0, n = complex->m_sprites.size(); i < n; ++i)
			{
				d2d::Sprite* sprite = complex->m_sprites[i];
				if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));

					if (image->GetSymbol().GetSize().Width() == 4 && image->GetSymbol().GetSize().Height() == 4)
						ParserPicture(image, e_bothfix);
					else
						ParserPicture(image);
				}
				else if (d2d::FontBlankSprite* font = dynamic_cast<d2d::FontBlankSprite*>(sprite))
				{
					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
				}
				else if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(sprite))
				{
					ParserIcon(icon);
				}
				else if (emesh::Sprite* mesh = dynamic_cast<emesh::Sprite*>(sprite))
				{
					// todo multi mesh!
					std::map<const d2d::Symbol*, std::vector<SpriteID> >::iterator itr
						= m_map_symbol2ids.find(&mesh->GetSymbol());
					if (itr != m_map_symbol2ids.end()) {
						bool find = false;
						for (int i = 0, n = itr->second.size(); i < n; ++i) {
							SpriteID id = itr->second[i];
							if (mesh->GetSpeed() == static_cast<emesh::Sprite*>(id.sprite)->GetSpeed()) {
								m_mapSpriteID.insert(std::make_pair(sprite, id.id));
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
							id.sprite = mesh;
							id.id = m_id;
							itr->second.push_back(id);

							// mesh's pictures id
							// todo for x
							m_mapSpriteID.insert(std::make_pair(sprite, m_id));
							int size = ParserMesh(mesh);
							m_id += size;
						}
					} else {
						SpriteID id;
						id.sprite = mesh;
						id.id = m_id;

						std::vector<SpriteID> ids;
						ids.push_back(id);

						m_map_symbol2ids.insert(std::make_pair(&mesh->GetSymbol(), ids));

						// mesh's pictures id
						// todo for x
						m_mapSpriteID.insert(std::make_pair(sprite, m_id));
						int size = ParserMesh(mesh);
						m_id += size;
					}
				} 
				else if (eterrain2d::Sprite* ocean = dynamic_cast<eterrain2d::Sprite*>(sprite)) 
				{
					m_mapSpriteID.insert(std::make_pair(ocean, m_id));
					int size = ParserTerrain2D(ocean);
					m_id += size;
				}
				else if (etexture::Sprite* tex = dynamic_cast<etexture::Sprite*>(sprite))
				{
					std::map<const d2d::Symbol*, std::vector<SpriteID> >::iterator itr
						= m_map_symbol2ids.find(&tex->GetSymbol());
					if (itr != m_map_symbol2ids.end()) 
					{
						bool find = false;
						for (int i = 0, n = itr->second.size(); i < n; ++i) 
						{
							SpriteID id = itr->second[i];
							m_mapSpriteID.insert(std::make_pair(sprite, id.id));
							find = true;
							break;
						}
						if (!find) 
						{
							SpriteID id;
							id.sprite = tex;
							id.id = m_id;
							itr->second.push_back(id);

							m_mapSpriteID.insert(std::make_pair(sprite, m_id));
							int size = ParserTexture(tex);
							m_id += size;
						}
					} 
					else 
					{
						SpriteID id;
						id.sprite = tex;
						id.id = m_id;

						std::vector<SpriteID> ids;
						ids.push_back(id);

						m_map_symbol2ids.insert(std::make_pair(&tex->GetSymbol(), ids));

						m_mapSpriteID.insert(std::make_pair(sprite, m_id));
						int size = ParserTexture(tex);
						m_id += size;
					}
				}
			}

			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			ParserComplex(complex);
		}
		else if (const libanim::Symbol* anim = dynamic_cast<const libanim::Symbol*>(symbol))
		{
			////////////////////////////////////////////////////////////////////////////
			//// version 1: parser all sprites to picture
			////////////////////////////////////////////////////////////////////////////

			//for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			//{
			//	libanim::Symbol::Layer* layer = anim->m_layers[i];
			//	for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
			//	{
			//		libanim::Symbol::Frame* frame = layer->frames[j];
			//		for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
			//		{
			//			d2d::Sprite* sprite = frame->sprites[k];
			//			if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
			//			{
			//				m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
			//				resolvePicture(image);
			//			}
			//			else if (d2d::FontBlankSprite* font = dynamic_cast<d2d::FontBlankSprite*>(sprite))
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

			std::set<const d2d::ImageSymbol*, d2d::SymbolCmp> unique;
			for (size_t i = 0, n = anim->m_layers.size(); i < n; ++i)
			{
				libanim::Symbol::Layer* layer = anim->m_layers[i];
				for (size_t j = 0, m = layer->frames.size(); j < m; ++j)
				{
					libanim::Symbol::Frame* frame = layer->frames[j];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
					{
						d2d::Sprite* sprite = frame->sprites[k];
						if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
							unique.insert(&image->GetSymbol());
						else if (d2d::FontBlankSprite* font = dynamic_cast<d2d::FontBlankSprite*>(sprite))
							m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
						else if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(sprite))
							ParserIcon(icon);
					}
				}
			}

			std::set<const d2d::ImageSymbol*, d2d::SymbolCmp>::iterator itr = unique.begin();
			for ( ; itr != unique.end(); ++itr)
			{
				std::map<const d2d::Symbol*, int>::iterator itrFind = m_mapSymbolID.find(*itr);
				if (itrFind == m_mapSymbolID.end())
				{
					m_mapSymbolID.insert(std::make_pair(*itr, m_id++));
					if ((*itr)->GetSize().xLength() <= 4)
						ParserPicture(*itr, e_xfix);
					else
						ParserPicture(*itr);
				}
			}

			for (int i = 0, n = anim->getMaxFrameIndex(); i < n; ++i)
			{
				std::vector<d2d::Sprite*> sprites;
				libanim::Utility::GetCurrSprites(anim, i + 1, sprites);
				if (sprites.empty()) {
					continue;
				}
				for (int i = 0, n = sprites.size(); i < n; ++i)
				{
					if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(sprites[i])) {
						int id = QueryIconID(icon);
						if (id == -1) {
							ParserIcon(icon);
						}
					}
				}
				for_each(sprites.begin(), sprites.end(), d2d::ReleaseObjectFunctor<d2d::Sprite>());
			}

			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
			ParserAnimation(anim);
		}
		else if (const escale9::Symbol* patch9 = dynamic_cast<const escale9::Symbol*>(symbol))
		{
 			std::vector<d2d::Sprite*> sprites;
			const escale9::Scale9Data& data = patch9->GetScale9Data();
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
 				d2d::Sprite* sprite = sprites[i];
 				if (d2d::ImageSprite* image = dynamic_cast<d2d::ImageSprite*>(sprite))
 				{
 					PicFixType tsrc = e_null, tscreen = e_null;
 					switch (data.GetType())
 					{
 						case escale9::e_9Grid:
 							if (sprite == data.GetSprite(1, 1)) 
 							{
 								tsrc = e_bothfix;
 								tscreen = e_bothfix;
 							}
 							else if (sprite == data.GetSprite(1, 0)) tsrc = e_yfix;
 							else if (sprite == data.GetSprite(1, 2)) tsrc = e_yfix;
 							else if (sprite == data.GetSprite(0, 1)) tsrc = e_xfix;
 							else if (sprite == data.GetSprite(2, 1)) tsrc = e_xfix;
 							break;
 						case escale9::e_9GridHollow:
 							if (sprite == data.GetSprite(1, 0)) tsrc = e_yfix;
 							else if (sprite == data.GetSprite(1, 2)) tsrc = e_yfix;
 							else if (sprite == data.GetSprite(0, 1)) tsrc = e_xfix;
 							else if (sprite == data.GetSprite(2, 1)) tsrc = e_xfix;
 							break;
 						case escale9::e_3GridHor:
 							if (sprite == data.GetSprite(1, 1)) 
 							{
 								tsrc = e_xfix;
 								tscreen = e_xfix;
 							}
 							break;
 						case escale9::e_3GridVer:
 							if (sprite == data.GetSprite(1, 1)) 
 							{
 								tsrc = e_yfix;
 								tscreen = e_yfix;
 							}
 							break;
 						case escale9::e_6GridUpper:
 							if (sprite == data.GetSprite(1, 1)) 
 							{
 								tsrc = e_bothfix;
 								tscreen = e_bothfix;
 							}
 							else if (sprite == data.GetSprite(1, 0)) tsrc = e_yfix;
 							else if (sprite == data.GetSprite(1, 2)) tsrc = e_yfix;
 							else if (sprite == data.GetSprite(2, 1)) tsrc = e_xfix;
 							break;
 					}
 
 					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
 					ParserPicture(image, tsrc, tscreen);
 				}
 				else if (d2d::FontBlankSprite* font = dynamic_cast<d2d::FontBlankSprite*>(sprite))
 				{
 					m_mapSpriteID.insert(std::make_pair(sprite, m_id++));
 				}
 			}
 
 			m_mapSymbolID.insert(std::make_pair(symbol, m_id++));
 			ParserScale9(patch9);
		}
		else if (const escale9::Symbol* patch9 = dynamic_cast<const escale9::Symbol*>(symbol))
		{
		}
		else if (const emesh::Symbol* mesh = dynamic_cast<const emesh::Symbol*>(symbol))
		{
			// mesh's id
			m_mapSymbolID.insert(std::make_pair(mesh, m_id++));
		}
		else if (const eterrain2d::Symbol* ocean = dynamic_cast<const eterrain2d::Symbol*>(symbol))
		{
			m_mapSymbolID.insert(std::make_pair(ocean, m_id++));
		}
		else if (const etexture::Symbol* tex = dynamic_cast<const etexture::Symbol*>(symbol))
		{
			m_mapSymbolID.insert(std::make_pair(tex, m_id++));
		}
	}
}

void CocoPacker::ParserPicture(const d2d::ImageSprite* sprite, PicFixType tsrc, PicFixType tscreen)
{
	tsrc = tscreen = e_null;

	TPParser::Picture* picture = m_parser.FindPicture(&sprite->GetSymbol());
	if (!picture) {
		std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the texpacker file 0!";
		throw d2d::Exception(str);
	}

	lua::TableAssign ta(*m_gen, "picture", false, false);

	// id
	{
		std::map<const d2d::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		if (itr == m_mapSpriteID.end()) {
			std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the m_mapSpriteID!";
			throw d2d::Exception(str);
		}
		std::string sid = wxString::FromDouble(itr->second);
		m_gen->line(lua::assign("id", sid) + ",");
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

	if (m_img_ider) {
		int id = m_img_ider->Query(picture->filename);
		x0 = x1 = x2 = x3 = -id;
		y0 = y1 = y2 = y3 = -id;
	}

	std::string sx0 = wxString::FromDouble(x0), sy0 = wxString::FromDouble(y0);
	std::string sx1 = wxString::FromDouble(x1), sy1 = wxString::FromDouble(y1);
	std::string sx2 = wxString::FromDouble(x2), sy2 = wxString::FromDouble(y2);
	std::string sx3 = wxString::FromDouble(x3), sy3 = wxString::FromDouble(y3);
	std::string assignSrc = lua::assign("src", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	// screen
	const float hw = picture->entry->sprite_source_size.w * 0.5f / picture->invscale * m_scale,
		hh = picture->entry->sprite_source_size.h * 0.5f / picture->invscale * m_scale;
	d2d::Vector screen[4];
	screen[0].Set(-hw, hh);
	screen[1].Set(-hw, -hh);
	screen[2].Set(hw, -hh);
	screen[3].Set(hw, hh);
	
	// 1. shear
	float sx = sprite->GetShear().x,
		sy = sprite->GetShear().y;
	screen[0].x += sx * hh;
	screen[3].x += sx * hh;
	screen[1].x -= sx * hh;
	screen[2].x -= sx * hh;
	screen[2].y += sy * hw;
	screen[3].y += sy * hw;
	screen[1].y -= sy * hw;
	screen[0].y -= sy * hw;
	// 2. mirror
	bool xMirror, yMirror;
	sprite->GetMirror(xMirror, yMirror);
	if (xMirror) {
		for (size_t i = 0; i < 4; ++i)
			screen[i].x = -screen[i].x;
	}
	if (yMirror) {
		for (size_t i = 0; i < 4; ++i)
			screen[i].y = -screen[i].y;
	}
 	// 3. scale
 	for (size_t i = 0; i < 4; ++i)
 		screen[i].x *= sprite->GetScale().x;
 	for (size_t i = 0; i < 4; ++i)
 		screen[i].y *= sprite->GetScale().y;
 	// 4. rotate
 	for (size_t i = 0; i < 4; ++i)
 	{
 		d2d::Vector rot = d2d::Math2D::RotateVector(screen[i], sprite->GetAngle());
 		screen[i] = rot;
 	}
 	// 5. translate
 	d2d::Vector offset = picture->offset;
 	offset.x *= sprite->GetScale().x / picture->invscale;
 	offset.y *= sprite->GetScale().y / picture->invscale;
	if (xMirror) {
		offset.x = -offset.x;
	}
	if (yMirror) {
		offset.y = -offset.y;
	}
 	d2d::Vector center = sprite->GetCenter();
 	center += d2d::Math2D::RotateVector(offset, sprite->GetAngle());
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

	sx0 = wxString::FromDouble(screen[0].x);
	sy0 = wxString::FromDouble(screen[0].y);
	sx1 = wxString::FromDouble(screen[1].x);
	sy1 = wxString::FromDouble(screen[1].y);
	sx2 = wxString::FromDouble(screen[2].x);
	sy2 = wxString::FromDouble(screen[2].y);
	sx3 = wxString::FromDouble(screen[3].x);
	sy3 = wxString::FromDouble(screen[3].y);
	std::string assignScreen = lua::assign("screen", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	lua::tableassign(*m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

void CocoPacker::ParserPicture(const d2d::ImageSymbol* symbol, PicFixType tsrc)
{
	tsrc = e_null;

	TPParser::Picture* picture = m_parser.FindPicture(symbol);
	if (!picture) {
		std::string str = "\""+symbol->GetFilepath()+"\""+" not in the texpacker file 1!";

// 		printf("symbol pointer: %p \n", symbol);
// 		m_parser.DebugInfo();

		throw d2d::Exception(str);
	}

	lua::TableAssign ta(*m_gen, "picture", false, false);

	// id
	{
		std::map<const d2d::Symbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
		if (itr == m_mapSymbolID.end()) {
			std::string str = "\""+symbol->GetFilepath()+"\""+" not in m_mapSymbolID 0!";
			throw d2d::Exception(str);
		}
		std::string sid = wxString::FromDouble(itr->second);
		m_gen->line(lua::assign("id", sid) + ",");
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

	if (m_img_ider) {
		int id = m_img_ider->Query(picture->filename);
		x0 = x1 = x2 = x3 = -id;
		y0 = y1 = y2 = y3 = -id;
	}

	std::string sx0 = wxString::FromDouble(x0), sy0 = wxString::FromDouble(y0);
	std::string sx1 = wxString::FromDouble(x1), sy1 = wxString::FromDouble(y1);
	std::string sx2 = wxString::FromDouble(x2), sy2 = wxString::FromDouble(y2);
	std::string sx3 = wxString::FromDouble(x3), sy3 = wxString::FromDouble(y3);
	std::string assignSrc = lua::assign("src", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	// screen
	const float hw = picture->entry->sprite_source_size.w * 0.5f / picture->invscale * m_scale,
		hh = picture->entry->sprite_source_size.h * 0.5f / picture->invscale * m_scale;
	d2d::Vector screen[4];
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
	sx0 = wxString::FromDouble(screen[0].x);
	sy0 = wxString::FromDouble(screen[0].y);
	sx1 = wxString::FromDouble(screen[1].x);
	sy1 = wxString::FromDouble(screen[1].y);
	sx2 = wxString::FromDouble(screen[2].x);
	sy2 = wxString::FromDouble(screen[2].y);
	sx3 = wxString::FromDouble(screen[3].x);
	sy3 = wxString::FromDouble(screen[3].y);
	std::string assignScreen = lua::assign("screen", 
		lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));

	lua::tableassign(*m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

int CocoPacker::ParserIcon(const eicon::Sprite* sprite)
{
	int id = -1;

	const eicon::Symbol* symbol = &sprite->GetSymbol();
	float proc = sprite->GetProcess();

	std::map<const eicon::Symbol*, std::map<float, int> >::iterator itr_symbol
		= m_map_icon2ids.find(symbol);
	if (itr_symbol != m_map_icon2ids.end()) 
	{
		std::map<float, int>::iterator itr_process 
			= itr_symbol->second.find(proc);
		if (itr_process != itr_symbol->second.end()) {
			id = itr_process->second;
			m_mapSpriteID.insert(std::make_pair(sprite, id));
		} else {
			id = m_id++;
			ParserIcon(symbol, proc, id);
			itr_symbol->second.insert(std::make_pair(proc, id));
			m_mapSpriteID.insert(std::make_pair(sprite, id));
		}
	} 
	else 
	{
		id = m_id++;
		ParserIcon(symbol, proc, id);
		std::map<float, int> procs;
		procs.insert(std::make_pair(proc, id));
		m_map_icon2ids.insert(make_pair(symbol, procs));
		m_mapSpriteID.insert(std::make_pair(sprite, id));
	}

	return id;
}

void CocoPacker::ParserIcon(const eicon::Symbol* symbol, float process, int id)
{
	const d2d::Image* img = symbol->GetIcon()->GetImage();
	d2d::Symbol* img_symbol = d2d::SymbolMgr::Instance()->FetchSymbol(img->GetFilepath());
	TPParser::Picture* picture = m_parser.FindPicture(img_symbol);
	if (!picture) {
		std::string str = "\""+symbol->GetFilepath()+"\""+" not in the texpacker file 2!";
		throw d2d::Exception(str);
	}

	lua::TableAssign ta(*m_gen, "picture", false, false);

	// id
	{
		std::string sid = wxString::FromDouble(id);
		m_gen->line(lua::assign("id", sid) + ",");
	}

	// tex
	std::string assignTex = lua::assign("tex", wxString::FromDouble(picture->tex).ToStdString());

	// src
	d2d::Vector node[4];
	symbol->GetIcon()->GetTexCoords(process, node);
	int left = picture->scr[1].x, bottom = picture->scr[1].y;
	int width = picture->scr[2].x - left,
		height = picture->scr[0].y - bottom;
	std::string sx[4], sy[4];
	for (int i = 0; i < 4; ++i) {
		double x = left + width * node[i].x,
			y = bottom + height * node[i].y;
		sx[i] = wxString::FromDouble(x);
		sy[i] = wxString::FromDouble(y);
	}

	std::string assignSrc = lua::assign("src", 
		lua::tableassign("", 8, sx[0], sy[0], sx[1], sy[1], sx[2], sy[2], sx[3], sy[3]));

	// screen
	const float hw = picture->entry->sprite_source_size.w * 0.5f / picture->invscale * m_scale,
		hh = picture->entry->sprite_source_size.h * 0.5f / picture->invscale * m_scale;
	d2d::Vector screen[4];
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
		sx[i] = wxString::FromDouble(screen[i].x);
		sy[i] = wxString::FromDouble(screen[i].y);
	}

	std::string assignScreen = lua::assign("screen", 
		lua::tableassign("", 8, sx[0], sy[0], sx[1], sy[1], sx[2], sy[2], sx[3], sy[3]));

	lua::tableassign(*m_gen, "", 3, assignTex, assignSrc, assignScreen);
}

void CocoPacker::ParserComplex(const ecomplex::Symbol* symbol)
{
	lua::TableAssign ta(*m_gen, "animation", false, false);

	ParserSymbolBase(symbol);

	// clipbox
	const d2d::Rect& cb = symbol->m_clipbox;
	if (cb.xmin != 0 || cb.xmax != 0 || cb.ymin != 0 || cb.ymax != 0)
	{
		std::string width = wxString::FromDouble(cb.xmax - cb.xmin);
		std::string height = wxString::FromDouble(cb.ymax - cb.ymin);
		std::string left = wxString::FromDouble(cb.xmin);
		std::string top = wxString::FromDouble(-cb.ymax);
		lua::tableassign(*m_gen, "clipbox", 4, width, height, left, top);
	}

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;
	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
			ParserSpriteForComponent(symbol->m_sprites[i], ids, unique, order);
	}

	// children
	std::map<std::string, std::vector<d2d::Sprite*> > map_actions;
	std::vector<d2d::Sprite*> others;
	Utility::GroupSpritesFromTag(symbol->m_sprites, map_actions, others);
	if (!map_actions.empty())
	{
		std::map<std::string, std::vector<d2d::Sprite*> >::iterator itr;
		for (itr = map_actions.begin(); itr != map_actions.end(); ++itr)
		{
			lua::TableAssign ta(*m_gen, "", true);
			m_gen->line(lua::assign("action", "\"" + itr->first + "\"") + ",");
			// frame 0
			{
				lua::TableAssign ta(*m_gen, "", true);
				for (size_t i = 0, n = itr->second.size(); i < n; ++i)
				{
					int idx = -1;
					for (idx = 0; idx < symbol->m_sprites.size(); ++idx)
						if (symbol->m_sprites[idx] == itr->second[i])
							break;
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
  			for (size_t i = 0, n = others.size(); i < n; ++i)
			{
				int idx = -1;
				for (idx = 0; idx < symbol->m_sprites.size(); ++idx)
					if (symbol->m_sprites[idx] == others[i])
						break;
 				ParserSpriteForFrame(others[i], idx, ids, order);
			}
 		}
	}
}

void CocoPacker::ParserAnimation(const libanim::Symbol* symbol)
{
	lua::TableAssign ta(*m_gen, "animation", false, false);

	ParserSymbolBase(symbol);

	// component
	std::vector<int> ids;
	std::map<int, std::vector<std::string> > unique;
	std::vector<std::pair<int, std::string> > order;

	// component for icon's tween
	std::map<int, int> map_id2idx;
	int comp_idx = order.size();

	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (size_t i = 0, n = symbol->getMaxFrameIndex(); i < n; ++i)
		{			
			for (size_t j = 0, m = symbol->m_layers.size(); j < m; ++j)
			{
				libanim::Symbol::Layer* layer = symbol->m_layers[j];
				if (i < layer->frames.size())
				{
					libanim::Symbol::Frame* frame = layer->frames[i];
					for (size_t k = 0, l = frame->sprites.size(); k < l; ++k)
						ParserSpriteForComponent(frame->sprites[k], ids, unique, order);
				}
			}
		}

		// component for icon's tween
		comp_idx = order.size();
		for (int i = 0, n = symbol->getMaxFrameIndex(); i < n; ++i)
		{
			std::vector<d2d::Sprite*> sprites;
			libanim::Utility::GetCurrSprites(symbol, i + 1, sprites);
			if (sprites.empty()) {
				continue;
			}
			for (int i = 0, n = sprites.size(); i < n; ++i)
			{
				if (eicon::Sprite* icon = dynamic_cast<eicon::Sprite*>(sprites[i])) {
					int id = QueryIconID(icon);
					assert(id != -1);
					std::map<int, int>::iterator itr_comp = map_id2idx.find(id);
					if (itr_comp == map_id2idx.end()) {
						std::string assign_id = lua::assign("id", wxString::FromDouble(id).ToStdString());
						lua::tableassign(*m_gen, "", 1, assign_id);
						map_id2idx.insert(std::make_pair(id, comp_idx++));
					}
				}
			}
			for_each(sprites.begin(), sprites.end(), d2d::ReleaseObjectFunctor<d2d::Sprite>());
		}
	}	
 	// children
 	{
 		lua::TableAssign ta(*m_gen, "", true);
 		// frames
 		int index = 0;
 		for (size_t i = 1, n = symbol->getMaxFrameIndex(); i <= n; ++i)
 		{
 			lua::TableAssign ta(*m_gen, "", true);

			std::vector<d2d::Sprite*> sprites;
			libanim::Utility::GetCurrSprites(symbol, i, sprites);
			for (size_t j = 0, m = sprites.size(); j < m; ++j)
				ParserSpriteForFrame(sprites[j], order, map_id2idx);
			for_each(sprites.begin(), sprites.end(), d2d::ReleaseObjectFunctor<d2d::Sprite>());
 		}
 	}
}

void CocoPacker::ParserScale9(const escale9::Symbol* symbol)
{
 	lua::TableAssign ta(*m_gen, "animation", false, false);
 
 	ParserSymbolBase(symbol);
 
	const escale9::Scale9Data& data = symbol->GetScale9Data();
	escale9::Scale9Type type = data.GetType();

 	// component
 	std::vector<int> ids;
 	std::map<int, std::vector<std::string> > unique;
 	std::vector<std::pair<int, std::string> > order;
 	{
 		lua::TableAssign ta(*m_gen, "component", true);
 		if (type == escale9::e_9Grid)
 			for (size_t i = 0; i < 3; ++i)
 				for (size_t j = 0; j < 3; ++j)
 					ParserSpriteForComponent(data.GetSprite(i, j), ids, unique, order);
 		else if (type == escale9::e_9GridHollow)
 			for (size_t i = 0; i < 3; ++i) {
 				for (size_t j = 0; j < 3; ++j) {
 					if (i == 1 && j == 1) continue;
 					ParserSpriteForComponent(data.GetSprite(i, j), ids, unique, order);
 				}
 			}
 		else if (type == escale9::e_3GridHor)
 			for (size_t i = 0; i < 3; ++i)
 				ParserSpriteForComponent(data.GetSprite(1, i), ids, unique, order);
 		else if (type == escale9::e_3GridVer)
 			for (size_t i = 0; i < 3; ++i)
 				ParserSpriteForComponent(data.GetSprite(i, 1), ids, unique, order);
 		else if (type == escale9::e_6GridUpper)
 			for (size_t i = 1; i < 3; ++i)
 				for (size_t j = 0; j < 3; ++j)
 					ParserSpriteForComponent(data.GetSprite(i, j), ids, unique, order);
 	}
 	// children
 	{
 		lua::TableAssign ta(*m_gen, "", true);
 		// frame 0
 		{
 			lua::TableAssign ta(*m_gen, "", true);
 			int index = 0;
 			if (type == escale9::e_9Grid)
 				for (size_t i = 0; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j, ++index)
 						ParserSpriteForFrame(data.GetSprite(i, j), index, ids, order);
 			else if (type == escale9::e_9GridHollow)
 				for (size_t i = 0; i < 3; ++i) {
 					for (size_t j = 0; j < 3; ++j, ++index) {
 						if (i == 1 && j == 1) { 
 							--index;
 							continue;
 						}
 						ParserSpriteForFrame(data.GetSprite(i, j), index, ids, order);
 					}
 				}
 			else if (type == escale9::e_3GridHor)
 				for (size_t i = 0; i < 3; ++i)
 					ParserSpriteForFrame(data.GetSprite(1, i), i, ids, order);
 			else if (type == escale9::e_3GridVer)
 				for (size_t i = 0; i < 3; ++i)
 					ParserSpriteForFrame(data.GetSprite(i, 1), i, ids, order);
 			else if (type == escale9::e_6GridUpper)
 				for (size_t i = 1; i < 3; ++i)
 					for (size_t j = 0; j < 3; ++j, ++index)
 						ParserSpriteForFrame(data.GetSprite(i, j), index, ids, order);
 		}
 	}
}

void CocoPacker::CalSrcFromUV(d2d::Vector src[4], TPParser::Picture* picture)
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

void CocoPacker::CalSrcFromUVFixed(d2d::Vector src[4], TPParser::Picture* picture)
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

int CocoPacker::ParserMesh(const emesh::Sprite* sprite)
{
	d2d::Symbol* img_symbol = d2d::SymbolMgr::Instance()->FetchSymbol(sprite->GetSymbol().getImage()->GetFilepath());
	TPParser::Picture* picture = m_parser.FindPicture(img_symbol);
	if (!picture) {
		std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the texpacker file 3!";
		throw d2d::Exception(str);
	}

	//////////////////////////////////////////////////////////////////////////
	// pictures
	//////////////////////////////////////////////////////////////////////////
	// id
	std::map<const d2d::Sprite*, int>::iterator itr_sprite = m_mapSpriteID.find(sprite);
	if (itr_sprite == m_mapSpriteID.end()) {
		std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the m_mapSpriteID!";
		throw d2d::Exception(str);
	}
	int curr_id = itr_sprite->second;
	// tex
	std::string assign_tex = lua::assign("tex", wxString::FromDouble(picture->tex).ToStdString());

	int frame = 1;
	if (sprite->GetSpeed().y != 0) {
		frame = std::fabs(std::floor(1.0f / sprite->GetSpeed().y));
	}
	std::vector<int> frame_size;
	d2d::Vector speed = sprite->GetSpeed();
	emesh::Shape* shape = const_cast<emesh::Shape*>(sprite->GetSymbol().getShape());
	// 打包emesh::Strip做的流水
	if (dynamic_cast<emesh::Strip*>(shape))
	{
		for (int i = 0; i < frame; ++i)
		{
			// todo 只是具体strip的情况，不支持mesh的旋转
			const std::vector<emesh::Triangle*>& tris = shape->GetTriangles();

			int quad_size = tris.size() / 2;
			frame_size.push_back(quad_size);

			for (int j = 0; j < quad_size; ++j)
			{
				emesh::Triangle* right_down = tris[j*2];
				emesh::Triangle* left_up = tris[j*2+1];

				// id
				lua::TableAssign ta(*m_gen, "picture", false, false);
				m_gen->line(lua::assign("id", wxString::FromDouble(curr_id++).ToStdString()) + ",");

				// src
				d2d::Vector src[4];
				src[0] = left_up->nodes[2]->uv;
				src[1] = left_up->nodes[0]->uv;
				src[2] = right_down->nodes[1]->uv;
				src[3] = right_down->nodes[2]->uv;
				CalSrcFromUV(src, picture);
				std::string sx0 = wxString::FromDouble(src[0].x), sy0 = wxString::FromDouble(src[0].y);
				std::string sx1 = wxString::FromDouble(src[1].x), sy1 = wxString::FromDouble(src[1].y);
				std::string sx2 = wxString::FromDouble(src[2].x), sy2 = wxString::FromDouble(src[2].y);
				std::string sx3 = wxString::FromDouble(src[3].x), sy3 = wxString::FromDouble(src[3].y);
				std::string assign_src = lua::assign("src", 
					lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));		

				// screen
				d2d::Vector screen[4];
				screen[0] = left_up->nodes[2]->xy;
				screen[1] = left_up->nodes[0]->xy;
				screen[2] = right_down->nodes[1]->xy;
				screen[3] = right_down->nodes[2]->xy;
				// 			// translate
				// 			for (size_t i = 0; i < 4; ++i)
				// 				screen[i] += sprite->getPosition();
				// flip y
				for (size_t i = 0; i < 4; ++i)
					screen[i].y = -screen[i].y;
				// scale 16
				const float SCALE = 16;
				for (size_t i = 0; i < 4; ++i)
					screen[i] *= SCALE; 
				std::string dx0 = wxString::FromDouble(screen[0].x); std::string dy0 = wxString::FromDouble(screen[0].y);
				std::string dx1 = wxString::FromDouble(screen[1].x); std::string dy1 = wxString::FromDouble(screen[1].y);
				std::string dx2 = wxString::FromDouble(screen[2].x); std::string dy2 = wxString::FromDouble(screen[2].y);
				std::string dx3 = wxString::FromDouble(screen[3].x); std::string dy3 = wxString::FromDouble(screen[3].y);
				std::string assign_screen = lua::assign("screen", 
					lua::tableassign("", 8, dx0, dy0, dx1, dy1, dx2, dy2, dx3, dy3));

				lua::tableassign(*m_gen, "", 3, assign_tex, assign_src, assign_screen);
			}
			shape->OffsetUV(speed.x, speed.y);
		}
	}
	// 打包普通emesh::Mesh
	else if (dynamic_cast<emesh::Mesh*>(shape))
	{
		const std::vector<emesh::Triangle*>& tris = shape->GetTriangles();
		frame_size.push_back(tris.size());
		for (int i = 0, n = tris.size(); i < n; ++i)
		{
			emesh::Triangle* tri = tris[i];

			// id
			lua::TableAssign ta(*m_gen, "picture", false, false);
			m_gen->line(lua::assign("id", wxString::FromDouble(curr_id++).ToStdString()) + ",");

			// src
			d2d::Vector src[4];
			src[0] = tri->nodes[0]->uv;
			src[1] = tri->nodes[1]->uv;
			src[2] = tri->nodes[2]->uv;
			src[3] = tri->nodes[2]->uv;
			CalSrcFromUV(src, picture);
			std::string sx0 = wxString::FromDouble(src[0].x), sy0 = wxString::FromDouble(src[0].y);
			std::string sx1 = wxString::FromDouble(src[1].x), sy1 = wxString::FromDouble(src[1].y);
			std::string sx2 = wxString::FromDouble(src[2].x), sy2 = wxString::FromDouble(src[2].y);
			std::string sx3 = wxString::FromDouble(src[3].x), sy3 = wxString::FromDouble(src[3].y);
			std::string assign_src = lua::assign("src", 
				lua::tableassign("", 8, sx0, sy0, sx1, sy1, sx2, sy2, sx3, sy3));		

			// screen
			d2d::Vector screen[4];
			screen[0] = tri->nodes[0]->xy;
			screen[1] = tri->nodes[1]->xy;
			screen[2] = tri->nodes[2]->xy;
			screen[3] = tri->nodes[2]->xy;
			// 			// translate
			// 			for (size_t i = 0; i < 4; ++i)
			// 				screen[i] += sprite->getPosition();
			// flip y
			for (size_t i = 0; i < 4; ++i)
				screen[i].y = -screen[i].y;
			// scale 16
			const float SCALE = 16;
			for (size_t i = 0; i < 4; ++i)
				screen[i] *= SCALE; 
			std::string dx0 = wxString::FromDouble(screen[0].x); std::string dy0 = wxString::FromDouble(screen[0].y);
			std::string dx1 = wxString::FromDouble(screen[1].x); std::string dy1 = wxString::FromDouble(screen[1].y);
			std::string dx2 = wxString::FromDouble(screen[2].x); std::string dy2 = wxString::FromDouble(screen[2].y);
			std::string dx3 = wxString::FromDouble(screen[3].x); std::string dy3 = wxString::FromDouble(screen[3].y);
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
	const d2d::Symbol* symbol = &sprite->GetSymbol();
	if (!symbol->name.empty())
		m_gen->line(lua::assign("export", "\""+symbol->name+"\"")+",");
 	// id
 	std::map<const d2d::Symbol*, int>::iterator itr_mesh_symbol = m_mapSymbolID.find(symbol);
 	if (itr_mesh_symbol == m_mapSymbolID.end()) {
 		std::string str = "\""+symbol->GetFilepath()+"\""+" not in m_mapSymbolID 1!";
 		throw d2d::Exception(str);
 	}
 	std::string sid = wxString::FromDouble(itr_mesh_symbol->second);
 	m_gen->line(lua::assign("id", sid) + ",");
	// component
	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (int id = itr_sprite->second; id < curr_id; ++id)
		{
			std::string assign_id = lua::assign("id", wxString::FromDouble(id).ToStdString());
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
				std::string assign_index = lua::assign("index", wxString::FromDouble(id++).ToStdString());
				params.push_back(assign_index);
				GetColorAssignParams(sprite, params);
				lua::tableassign(*m_gen, "", params);
			}
		}
	}

	return id;
}

int CocoPacker::ParserTerrain2D(const eterrain2d::Sprite* sprite)
{
	const d2d::Symbol* symbol = &sprite->GetSymbol();
	std::map<const d2d::Symbol*, int>::iterator itr_mesh_symbol = m_mapSymbolID.find(symbol);
	if (itr_mesh_symbol == m_mapSymbolID.end()) {
		std::string str = "\""+symbol->GetFilepath()+"\""+" not in m_mapSymbolID 2!";
		throw d2d::Exception(str);
	}

	if (m_terrain2d_ids.find(itr_mesh_symbol->second) != m_terrain2d_ids.end()) {
		return 0;
	} else {
		m_terrain2d_ids.insert(itr_mesh_symbol->second);
	}

	// only use one texture in the sprite

	const std::vector<eterrain2d::OceanMesh*> oceans = sprite->GetSymbol().GetOceans();
	assert(oceans.size() == 1);
	eterrain2d::OceanMesh* ocean = oceans[0];
	const d2d::ImageSymbol* img = ocean->GetImage0();
	d2d::Symbol* img_symbol = d2d::SymbolMgr::Instance()->FetchSymbol(img->GetFilepath());
	TPParser::Picture* picture = m_parser.FindPicture(img_symbol);
	if (!picture) {
		std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the texpacker file 4!";
		throw d2d::Exception(str);
	}

	//////////////////////////////////////////////////////////////////////////
	// pictures
	//////////////////////////////////////////////////////////////////////////
	// id
	std::map<const d2d::Sprite*, int>::iterator itr_sprite = m_mapSpriteID.find(sprite);
	if (itr_sprite == m_mapSpriteID.end()) {
		std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the m_mapSpriteID!";
		throw d2d::Exception(str);
	}
	int curr_id = itr_sprite->second;
	// tex
	std::string assign_tex = lua::assign("tex", wxString::FromDouble(picture->tex).ToStdString());

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
			m_gen->line(lua::assign("id", wxString::FromDouble(curr_id++).ToStdString()) + ",");

			

			++img_count;

			for (int j = 0; j < meshes.size(); ++j) {
				const eterrain2d::MeshShape* mesh = meshes[j];
				const std::vector<emesh::Triangle*>& tris = mesh->GetTriangles();
				for (int k = 0; k < tris.size(); ++k) {
					// same with 打包普通emesh::Mesh

					emesh::Triangle* tri = tris[k];

					// src
					d2d::Vector src[4];
					src[0] = tri->nodes[0]->uv;
					src[1] = tri->nodes[1]->uv;
					src[2] = tri->nodes[2]->uv;
					src[3] = tri->nodes[2]->uv;
					CalSrcFromUVFixed(src, picture);
					std::string sx0 = wxString::FromDouble(src[0].x), sy0 = wxString::FromDouble(src[0].y);
					std::string sx1 = wxString::FromDouble(src[1].x), sy1 = wxString::FromDouble(src[1].y);
					std::string sx2 = wxString::FromDouble(src[2].x), sy2 = wxString::FromDouble(src[2].y);
					std::string sx3 = wxString::FromDouble(src[3].x), sy3 = wxString::FromDouble(src[3].y);
					std::string assign_src = lua::assign("src", lua::tableassign("", 8, sx0, sy0, 
						sx1, sy1, sx2, sy2, sx3, sy3));		

					// screen
					d2d::Vector screen[4];
					screen[0] = tri->nodes[0]->xy;
					screen[1] = tri->nodes[1]->xy;
					screen[2] = tri->nodes[2]->xy;
					screen[3] = tri->nodes[2]->xy;
					// 			// translate
					// 			for (size_t i = 0; i < 4; ++i)
					// 				screen[i] += sprite->getPosition();
					// flip y
					for (size_t i = 0; i < 4; ++i)
						screen[i].y = -screen[i].y;
					// scale 16
					const float SCALE = 16;
					for (size_t i = 0; i < 4; ++i)
						screen[i] *= SCALE; 
					std::string dx0 = wxString::FromDouble(screen[0].x); std::string dy0 = wxString::FromDouble(screen[0].y);
					std::string dx1 = wxString::FromDouble(screen[1].x); std::string dy1 = wxString::FromDouble(screen[1].y);
					std::string dx2 = wxString::FromDouble(screen[2].x); std::string dy2 = wxString::FromDouble(screen[2].y);
					std::string dx3 = wxString::FromDouble(screen[3].x); std::string dy3 = wxString::FromDouble(screen[3].y);
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
	if (!symbol->name.empty())
		m_gen->line(lua::assign("export", "\""+symbol->name+"\"")+",");

	std::string sid = wxString::FromDouble(itr_mesh_symbol->second);
	m_gen->line(lua::assign("id", sid) + ",");
	// component
	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (int id = itr_sprite->second; id < curr_id; ++id)
		{
			std::string assign_id = lua::assign("id", wxString::FromDouble(id).ToStdString());
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
				std::string assign_index = lua::assign("index", wxString::FromDouble(id++).ToStdString());
				params.push_back(assign_index);
				GetColorAssignParams(sprite, params);
				lua::tableassign(*m_gen, "", params);
			}
		}
	}

	return id;
}

int CocoPacker::ParserTexture(const etexture::Sprite* sprite)
{
	const std::vector<d2d::Shape*>& shapes = sprite->GetSymbol().GetAllShapes();
	assert(shapes.size() == 1);
	libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shapes[0]);
	assert(poly);
	const libshape::TextureMaterial* material = dynamic_cast<const libshape::TextureMaterial*>(poly->GetMaterial());
	assert(material);
	const d2d::ImageSymbol* img_symbol = material->GetImage();
	TPParser::Picture* picture = m_parser.FindPicture(img_symbol);
	if (!picture) {
		std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the texpacker file 5!";
		throw d2d::Exception(str);
	}

	//////////////////////////////////////////////////////////////////////////
	// pictures
	//////////////////////////////////////////////////////////////////////////
	// id
	std::map<const d2d::Sprite*, int>::iterator itr_sprite = m_mapSpriteID.find(sprite);
	if (itr_sprite == m_mapSpriteID.end()) {
		std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the m_mapSpriteID!";
		throw d2d::Exception(str);
	}
	int curr_id = itr_sprite->second;
	// tex
	std::string assign_tex = lua::assign("tex", wxString::FromDouble(picture->tex).ToStdString());

	const std::vector<d2d::Vector>& vertices = material->GetTriangles();
	const std::vector<d2d::Vector>& texcoords = material->GetTexcoords();
	assert(vertices.size() == texcoords.size() && vertices.size() % 3 == 0);
	for (int i = 0, n = vertices.size(); i < n; i += 3)
	{
		// id
		lua::TableAssign ta(*m_gen, "picture", false, false);
		m_gen->line(lua::assign("id", wxString::FromDouble(curr_id++).ToStdString()) + ",");
		
		// src
		d2d::Vector src[4];
		for (int j = 0; j < 3; ++j) {
			src[j] = texcoords[i+j];
		}
		src[3] = src[2];
		CalSrcFromUVFixed(src, picture);
		std::string sx0 = wxString::FromDouble(src[0].x), sy0 = wxString::FromDouble(src[0].y);
		std::string sx1 = wxString::FromDouble(src[1].x), sy1 = wxString::FromDouble(src[1].y);
		std::string sx2 = wxString::FromDouble(src[2].x), sy2 = wxString::FromDouble(src[2].y);
		std::string sx3 = wxString::FromDouble(src[3].x), sy3 = wxString::FromDouble(src[3].y);
		std::string assign_src = lua::assign("src", lua::tableassign("", 8, sx0, sy0, 
			sx1, sy1, sx2, sy2, sx3, sy3));		

		// screen
		d2d::Vector screen[4];
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
		std::string dx0 = wxString::FromDouble(screen[0].x); std::string dy0 = wxString::FromDouble(screen[0].y);
		std::string dx1 = wxString::FromDouble(screen[1].x); std::string dy1 = wxString::FromDouble(screen[1].y);
		std::string dx2 = wxString::FromDouble(screen[2].x); std::string dy2 = wxString::FromDouble(screen[2].y);
		std::string dx3 = wxString::FromDouble(screen[3].x); std::string dy3 = wxString::FromDouble(screen[3].y);
		std::string assign_screen = lua::assign("screen", lua::tableassign("", 8, dx0, dy0, 
			dx1, dy1, dx2, dy2, dx3, dy3));

		lua::tableassign(*m_gen, "", 3, assign_tex, assign_src, assign_screen);
	}

	//////////////////////////////////////////////////////////////////////////
	// animation
	//////////////////////////////////////////////////////////////////////////
	lua::TableAssign ta(*m_gen, "animation", false, false);
	// export
	const d2d::Symbol* symbol = &sprite->GetSymbol();
	if (!symbol->name.empty())
		m_gen->line(lua::assign("export", "\""+symbol->name+"\"")+",");
	// id
	std::map<const d2d::Symbol*, int>::iterator itr_mesh_symbol = m_mapSymbolID.find(symbol);
	if (itr_mesh_symbol == m_mapSymbolID.end()) {
		std::string str = "\""+symbol->GetFilepath()+"\""+" not in m_mapSymbolID 3!";
		throw d2d::Exception(str);
	}
	std::string sid = wxString::FromDouble(itr_mesh_symbol->second);
	m_gen->line(lua::assign("id", sid) + ",");
	// component
	{
		lua::TableAssign ta(*m_gen, "component", true);
		for (int id = itr_sprite->second; id < curr_id; ++id)
		{
			std::string assign_id = lua::assign("id", wxString::FromDouble(id).ToStdString());
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
			std::string assign_index = lua::assign("index", wxString::FromDouble(id++).ToStdString());
			params.push_back(assign_index);
			GetColorAssignParams(sprite, params);
			lua::tableassign(*m_gen, "", params);
		}
	}

	return id;
}

void CocoPacker::ParserSymbolBase(const d2d::Symbol* symbol)
{
	// export
	if (!symbol->name.empty())
		m_gen->line(lua::assign("export", "\""+symbol->name+"\"")+",");

	// id
	std::map<const d2d::Symbol*, int>::iterator itr = m_mapSymbolID.find(symbol);
	if (itr == m_mapSymbolID.end()) {
		std::string str = "\""+symbol->GetFilepath()+"\""+" not in m_mapSymbolID 4!";
		throw d2d::Exception(str);
	}

	std::string sid = wxString::FromDouble(itr->second);
	m_gen->line(lua::assign("id", sid) + ",");
}

void CocoPacker::ParserSpriteForComponent(const d2d::Sprite* sprite, std::vector<int>& ids, 
										std::map<int, std::vector<std::string> >& unique, 
										std::vector<std::pair<int, std::string> >& order)
{
	int id;

	bool isFont = false;

	if (const d2d::ImageSprite* image = dynamic_cast<const d2d::ImageSprite*>(sprite))
	{
		std::map<const d2d::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		if (itr != m_mapSpriteID.end())
		{
			id = itr->second;
		}
		else
		{
			// libanim::Symbol's sprites store unique

			std::map<const d2d::Symbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->GetSymbol());
			if (itr == m_mapSymbolID.end()) {
				std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in m_mapSymbolID 5!";
				throw d2d::Exception(str);
			}
			id = itr->second;
		}
	}
	else if (const d2d::FontBlankSprite* font = dynamic_cast<const d2d::FontBlankSprite*>(sprite))
	{
		isFont = true;
		std::map<const d2d::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		if (itr == m_mapSpriteID.end()) {
			std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in m_mapSpriteID!";
			throw d2d::Exception(str);
		}
		id = itr->second;
	}
	else if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(sprite))
	{
		std::map<const d2d::Sprite*, int>::iterator itr = m_mapSpriteID.find(sprite);
		if (itr == m_mapSpriteID.end()) {
			std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in m_mapSpriteID!";
			throw d2d::Exception(str);
		}
		id = itr->second;
	}
	else
	{
		std::map<const d2d::Symbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->GetSymbol());
		if (itr == m_mapSymbolID.end()) {
 			std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in m_mapSymbolID 6!";
 			throw d2d::Exception(str);
		}
		id = itr->second;
	}

	ids.push_back(id);

	std::map<int, std::vector<std::string> >::iterator itr = unique.find(id);
	if (unique.find(id) == unique.end())
	{
		if (isFont)
		{
			const d2d::FontBlankSprite* font = dynamic_cast<const d2d::FontBlankSprite*>(sprite);
			bool is_mount_node = font && font->font.empty() && font->font_color == d2d::Colorf(0, 0, 0, 0);
			if (is_mount_node)
			{
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				lua::tableassign(*m_gen, "", 1, aName);
			}
			else
			{
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				std::string aFont = lua::assign("font", "\""+font->font+"\"");
				std::string aColor = lua::assign("color", TransColor(font->font_color, d2d::PT_ARGB));

//				std::string aAlign = lua::assign("align", wxString::FromDouble(font->align).ToStdString());
				int align_hori = font->align_hori;
				int align_vert = font->align_vert;
				int align = align_hori | (align_vert << 4);
				std::string aAlign = lua::assign("align", wxString::FromDouble(align).ToStdString());

				std::string aSize = lua::assign("size", wxString::FromDouble(font->size).ToStdString());
				std::string aWidth = lua::assign("width", wxString::FromDouble(font->width).ToStdString());
				std::string aHeight = lua::assign("height", wxString::FromDouble(font->height).ToStdString());

				lua::tableassign(*m_gen, "", 7, aName, aFont, aColor, aAlign, 
					aSize, aWidth, aHeight);
			}
		}
		else
		{
			bool is_mount_node = false;
			const ecomplex::Sprite* ecomplex = dynamic_cast<const ecomplex::Sprite*>(sprite);
			if (ecomplex && ecomplex->GetSymbol().m_sprites.size() == 1) {
				const d2d::FontBlankSprite* font = dynamic_cast<const d2d::FontBlankSprite*>(ecomplex->GetSymbol().m_sprites[0]);
				is_mount_node = font && font->font.empty() && font->font_color == d2d::Colorf(0, 0, 0, 0);
			}
			if (is_mount_node) {
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				lua::tableassign(*m_gen, "", 1, aName);
			} else {
				std::string aID = lua::assign("id", wxString::FromDouble(id).ToStdString());
				if (!sprite->name.empty() && sprite->name[0] != '_')
				{
					std::string aName = lua::assign("name", "\""+sprite->name+"\"");
					lua::tableassign(*m_gen, "", 2, aName, aID);
				}
				else
				{
					lua::tableassign(*m_gen, "", 1, aID);
				}
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
			bool is_mount_node = false;
			const ecomplex::Sprite* ecomplex = dynamic_cast<const ecomplex::Sprite*>(sprite);
			if (ecomplex && ecomplex->GetSymbol().m_sprites.size() == 1) {
				const d2d::FontBlankSprite* font = dynamic_cast<const d2d::FontBlankSprite*>(ecomplex->GetSymbol().m_sprites[0]);
				is_mount_node = font && font->font.empty() && font->font_color == d2d::Colorf(0, 0, 0, 0);
			}
			if (is_mount_node) {
				std::string aName = lua::assign("name", "\""+sprite->name+"\"");
				lua::tableassign(*m_gen, "", 1, aName);
			} else {
				std::string aID = lua::assign("id", wxString::FromDouble(id).ToStdString());
				if (!sprite->name.empty() && sprite->name[0] != '_')
				{
					std::string aName = lua::assign("name", "\""+sprite->name+"\"");
					lua::tableassign(*m_gen, "", 2, aName, aID);
				}
				else
				{
					lua::tableassign(*m_gen, "", 1, aID);
				}
			}

			order.push_back(std::make_pair(id, sprite->name));
		}
		itr->second.push_back(sprite->name);

//		if (!sprite->name.empty())
//			order.push_back(std::make_pair(id, sprite->name));
	}
}

void CocoPacker::ParserSpriteForFrame(const d2d::Sprite* sprite, int index,
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
	if (cindex == -1) {
		std::string str = sprite->name + " not found in order!";
		throw d2d::Exception(str);
	}	

	if (const d2d::FontBlankSprite* font = dynamic_cast<const d2d::FontBlankSprite*>(sprite))
		ParserFontForFrame(font, cindex);
	else
		ParserImageForFrame(sprite, cindex);

// 	bool forceMat = dynamic_cast<const d2d::FontBlankSprite*>(sprite);
// 	resolveSpriteForFrame(sprite, cindex, forceMat);
}

void CocoPacker::ParserSpriteForFrame(const d2d::Sprite* sprite, 
									  const std::vector<std::pair<int, std::string> >& order,
									  const std::map<int, int>& map_id2idx)
{
	if (const eicon::Sprite* icon = dynamic_cast<const eicon::Sprite*>(sprite)) 
	{
		int id = QueryIconID(icon);
		assert(id != -1);
		std::map<int, int>::const_iterator itr = map_id2idx.find(id);
		assert(itr != map_id2idx.end());
		ParserSpriteForFrame(sprite, itr->second, true);
	} 
	else 
	{
		std::map<const d2d::Symbol*, int>::iterator itr = m_mapSymbolID.find(&sprite->GetSymbol());
		if (itr == m_mapSymbolID.end()) {
			std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in m_mapSymbolID 7!";
			throw d2d::Exception(str);
		}
		int id = itr->second;

		int cindex = -1;
		for (size_t i = 0, n = order.size(); i < n; ++i) {
			if (id == order[i].first && sprite->name == order[i].second) {
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
			std::string str = sprite->name + " not in order!";
			throw d2d::Exception(str);
		}

		ParserSpriteForFrame(sprite, cindex, true);
	}
}

void CocoPacker::ParserSpriteForFrame(const d2d::Sprite* sprite, int id, bool forceMat)
{
	std::vector<std::string> params;

	std::string assignIndex = lua::assign("index", wxString::FromDouble(id).ToStdString());
	params.push_back(assignIndex);

	float mat[6];
	TransToMat(sprite, mat, forceMat);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = wxString::FromDouble(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);
	params.push_back(assignMat);

	GetColorAssignParams(sprite, params);

	lua::tableassign(*m_gen, "", params);
}

void CocoPacker::ParserImageForFrame(const d2d::Sprite* sprite, int id)
{
	std::vector<std::string> params;

	std::string assignIndex = lua::assign("index", wxString::FromDouble(id).ToStdString());
	params.push_back(assignIndex);

	float mat[6];
	TransToMat(sprite, mat, false);

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = wxString::FromDouble(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);
	params.push_back(assignMat);

	if (sprite->clip) {
		params.push_back("clip=true");
	}

	GetColorAssignParams(sprite, params);

	lua::tableassign(*m_gen, "", params);
}

void CocoPacker::ParserFontForFrame(const d2d::FontBlankSprite* sprite, int id)
{
	std::string assignIndex = lua::assign("index", wxString::FromDouble(id).ToStdString());

	float mat[6];
	TransToMat(sprite, mat, true);

	bool isNullNode = sprite->font.empty() && sprite->font_color == d2d::Colorf(0, 0, 0, 0);
	if (!isNullNode)
	{
		// move to left-top
		mat[4] -= floor(sprite->width * 0.5f * 16 + 0.5f);
		mat[5] -= floor(sprite->height * 0.5f * 16 + 0.5f);

		//mat[4] -= floor(sprite->getBounding()->width() * 0.5f * 16 + 0.5f);
		//mat[5] -= floor(sprite->getBounding()->height() * 0.5f * 16 + 0.5f);
	}

	std::string m[6];
	for (size_t i = 0; i < 6; ++i)
		m[i] = wxString::FromDouble(mat[i]);
	std::string smat = lua::tableassign("", 6, m[0], m[1], m[2], 
		m[3], m[4], m[5]);
	std::string assignMat = lua::assign("mat", smat);

	lua::tableassign(*m_gen, "", 2, assignIndex, assignMat);
}

void CocoPacker::TransToMat(const d2d::Sprite* sprite, float mat[6], bool force /*= false*/) const
{
	mat[1] = mat[2] = mat[4] = mat[5] = 0;
	mat[0] = mat[3] = 1;

	if (!force &&
		(dynamic_cast<const d2d::ImageSprite*>(sprite) ||
		dynamic_cast<const d2d::FontBlankSprite*>(sprite)))
	{
	}
	else
	{
		// | 1  ky    | | sx       | |  c  s    | | 1       |
		// | kx  1    | |    sy    | | -s  c    | |    1    |
		// |        1 | |        1 | |        1 | | x  y  1 |
		//     skew        scale        rotate        move

		bool xMirror, yMirror;
		sprite->GetMirror(xMirror, yMirror);

		d2d::Vector center = sprite->GetCenter();
		if (dynamic_cast<const d2d::ImageSprite*>(sprite))
		{
			TPParser::Picture* picture = m_parser.FindPicture(&sprite->GetSymbol());
			if (!picture) {
				std::string str = "\""+sprite->GetSymbol().GetFilepath()+"\""+" not in the texpacker file 6!";
				throw d2d::Exception(str);
			}

			d2d::Vector offset = picture->offset;
			if (xMirror) {
				offset.x = -offset.x;
			}
			if (yMirror) {
				offset.y = -offset.y;
			}
			offset.x *= sprite->GetScale().x / picture->invscale;
			offset.y *= sprite->GetScale().y / picture->invscale;
			center += d2d::Math2D::RotateVector(offset, sprite->GetAngle());
		}
		float sx = sprite->GetScale().x,
			  sy = sprite->GetScale().y;
		if (xMirror) sx = -sx;
		if (yMirror) sy = -sy;

		float c = cos(-sprite->GetAngle()),
			s = sin(-sprite->GetAngle());
		float kx = -sprite->GetShear().x,
			  ky = -sprite->GetShear().y;
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

void CocoPacker::GetColorAssignParams(const d2d::Sprite* sprite, std::vector<std::string>& params) const
{
	if (sprite->color.multi != d2d::Colorf(1,1,1,1) || sprite->color.add != d2d::Colorf(0,0,0,0)) 
	{
		std::string str_multi = lua::assign("color", d2d::TransColor(sprite->color.multi, d2d::PT_BGRA));
		params.push_back(str_multi);
		std::string str_add = lua::assign("add", d2d::TransColor(sprite->color.add, d2d::PT_ABGR));
		params.push_back(str_add);
	}

	if (sprite->color.r != d2d::Colorf(1, 0, 0, 1) || sprite->color.g != d2d::Colorf(0, 1, 0, 1) || sprite->color.b != d2d::Colorf(0, 0, 1, 1))
	{
		std::string str_r = lua::assign("r_map", d2d::TransColor(sprite->color.r, d2d::PT_RGBA));
		params.push_back(str_r);

		std::string str_g = lua::assign("g_map", d2d::TransColor(sprite->color.g, d2d::PT_RGBA));
		params.push_back(str_g);

		std::string str_b = lua::assign("b_map", d2d::TransColor(sprite->color.b, d2d::PT_RGBA));
		params.push_back(str_b);
	}
}

int CocoPacker::QueryIconID(const eicon::Sprite* icon) const
{
	std::map<const eicon::Symbol*, std::map<float, int> >::const_iterator itr
		= m_map_icon2ids.find(&icon->GetSymbol());
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