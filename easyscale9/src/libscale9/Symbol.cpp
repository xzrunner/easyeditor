#include "Symbol.h"
#include "Sprite.h"
#include "config.h"

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + wxVariant(id++);

	memset(m_sprites, 0, sizeof(void*) * 9);

	m_width = m_height = 0;

	m_type = e_null;
}

Symbol::~Symbol()
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->Release();
		}
	}
	memset(m_sprites, 0, sizeof(int) * 9);
}

void Symbol::reloadTexture() const
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->getSymbol().reloadTexture();
		}
	}
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	if (sprite) {
		const Sprite* spr = static_cast<const Sprite*>(sprite);
		spr->Draw(mt, mul, add, r_trans, g_trans, b_trans);
	} else {
		DrawScale9(m_type, m_sprites, mt, mul, add, r_trans, g_trans, b_trans);
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	if (sprite) {
		const Sprite* spr = static_cast<const Sprite*>(sprite);
		float w, h;
		spr->GetSize(w, h);
		return d2d::Rect(w, h);
	} else {
		return d2d::Rect(m_width, m_height);
	}
}

void Symbol::DrawScale9(Type type,
						 d2d::ISprite* const sprites[3][3],
						 const d2d::Matrix& mt,
						 const d2d::Colorf& mul, 
						 const d2d::Colorf& add,
						 const d2d::Colorf& r_trans,
						 const d2d::Colorf& g_trans,
						 const d2d::Colorf& b_trans)
{
	switch (type)
	{
	case e_9Grid:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (!sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(sprites[i][j], mt, mul, add, r_trans, g_trans, b_trans);
			}
		}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				if (!sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(sprites[i][j], mt, mul, add, r_trans, g_trans, b_trans);
			}
		}
		break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i) {
			if (!sprites[1][i]) continue;
			d2d::SpriteDraw::drawSprite(sprites[1][i], mt, mul, add, r_trans, g_trans, b_trans);
		}
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i) {
			if (!sprites[i][1]) continue;
			d2d::SpriteDraw::drawSprite(sprites[i][1], mt, mul, add, r_trans, g_trans, b_trans);
		}
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (!sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(sprites[i][j], mt, mul, add, r_trans, g_trans, b_trans);
			}
		}
		break;
	}	
}

//void Symbol::composeFromSprites(d2d::ISprite* sprites[3][3], float width, float height)
//{
//	m_type = getType(sprites);
//	if (m_type == e_null) return;
//
//	m_width = width;
//	m_height = height;
//	switch (m_type)
//	{
//	case e_9Grid:
//		for (size_t i = 0; i < 3; ++i)
//			for (size_t j = 0; j < 3; ++j)
//			{
//				if (m_sprites[i][j]) m_sprites[i][j]->Release();
//				m_sprites[i][j] = sprites[i][j]->clone();
//			}
//		break;
//	case e_9GridHollow:
//		for (size_t i = 0; i < 3; ++i)
//			for (size_t j = 0; j < 3; ++j)
//			{
//				if (i == 1 && j == 1) continue;
//				if (m_sprites[i][j]) m_sprites[i][j]->Release();
//				m_sprites[i][j] = sprites[i][j]->clone();
//			}
//			break;
//	case e_3GridHor:
//		for (size_t i = 0; i < 3; ++i)
//		{
//			if (m_sprites[1][i]) m_sprites[1][i]->Release();
//			m_sprites[1][i] = sprites[1][i]->clone();
//		}
//		m_height = m_sprites[1][0]->getSymbol().getSize().yLength();
//		break;
//	case e_3GridVer:
//		for (size_t i = 0; i < 3; ++i)
//		{
//			if (m_sprites[i][1]) m_sprites[i][1]->Release();
//			m_sprites[i][1] = sprites[i][1]->clone();
//		}
//		m_width = m_sprites[0][1]->getSymbol().getSize().xLength();
//		break;
//	case e_6GridUpper:
//		for (size_t i = 1; i < 3; ++i)
//			for (size_t j = 0; j < 3; ++j)
//			{
//				if (m_sprites[i][j]) m_sprites[i][j]->Release();
//				m_sprites[i][j] = sprites[i][j]->clone();
//			} 
//		break;
//	}
//
//	composeFromSprites();
//}

// void Symbol::resize(float width, float height) const
// {
// 	m_width = width;
// 	m_height = height;
// 
// 	composeFromSprites();
// }

// todo 
void Symbol::loadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	if (value["sprite new"].isNull())
	{
		FileLoader adapter;
		adapter.load(m_filepath.c_str());

		std::string dir = d2d::FilenameTools::getFileDir(m_filepath);

		m_width = adapter.width;
		m_height = adapter.height;

		int index = 0;

		m_type = Type(adapter.type);
		switch (m_type)
		{
		case e_9Grid:
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					InitSprite(adapter.m_data[i*3+j], &m_sprites[i][j], dir);
				}
			}
			break;
		case e_9GridHollow:
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					if (i == 1 && j == 1) continue;
					if (i > 1 || i == 1 && j > 1)
						InitSprite(adapter.m_data[i*3+j-1], &m_sprites[i][j], dir);
					else
						InitSprite(adapter.m_data[i*3+j], &m_sprites[i][j], dir);
				}
			}
			break;
		case e_3GridHor:
			for (size_t i = 0; i < 3; ++i) {
				InitSprite(adapter.m_data[i], &m_sprites[1][i], dir);
			}
			break;
		case e_3GridVer:
			for (size_t i = 0; i < 3; ++i) {
				InitSprite(adapter.m_data[i], &m_sprites[i][1], dir);
			}
			break;
		case e_6GridUpper:
			for (size_t i = 1; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					InitSprite(adapter.m_data[index++], &m_sprites[i][j], dir);
				}
			}
			break;
		}
	}
	else
	{
		m_width = value["width"].asDouble();
		m_height = value["height"].asDouble();
		m_type = Type(value["type"].asInt());

		std::string dir = d2d::FilenameTools::getFileDir(m_filepath);
		int idx = 0;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				Json::Value val = value["sprite new"][idx++];
				if (val.isNull()) {
					continue;
				}

				std::string filepath = val["filepath"].asString();
				if (!d2d::FilenameTools::isExist(filepath))
					filepath = d2d::FilenameTools::getAbsolutePath(dir, filepath);

				ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
				d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
				symbol->Release();
				sprite->load(val);
				
				m_sprites[i][j] = sprite;
			}
		}
	}

	Sprite::ResizeScale9(m_type, m_sprites, m_width, m_height);
}

// Symbol::Type Symbol::getType(d2d::ISprite* sprites[3][3]) const
// {
// 	{
// 		Type type = e_9Grid;
// 		for (size_t i = 0; i < 3 && type == e_9Grid; ++i)
// 			for (size_t j = 0; j < 3 && type == e_9Grid; ++j)
// 				if (!sprites[i][j]) type = e_null;
// 		if (type == e_9Grid) return type;
// 	}
// 	{
// 		Type type = e_9GridHollow;
// 		for (size_t i = 0; i < 3 && type == e_9GridHollow; ++i) {
// 			for (size_t j = 0; j < 3 && type == e_9GridHollow; ++j) {
// 				if (i == 1 && j == 1) continue;
// 				if (!sprites[i][j]) type = e_null;
// 			}
// 		}
// 		if (type == e_9GridHollow) return type;
// 	}
// 	{
// 		Type type = e_6GridUpper;
// 		for (size_t i = 1; i < 3 && type == e_6GridUpper; ++i)
// 			for (size_t j = 0; j < 3 && type == e_6GridUpper; ++j)
// 				if (!sprites[i][j]) type = e_null;
// 		if (type == e_6GridUpper) return type;
// 	}
// 	{
// 		Type type = e_3GridHor;
// 		for (size_t i = 0; i < 3 && type == e_3GridHor; ++i)
// 			if (!sprites[1][i]) type = e_null;
// 		if (type == e_3GridHor) return type;
// 	}
// 	{
// 		Type type = e_3GridVer;
// 		for (size_t i = 0; i < 3 && type == e_3GridVer; ++i)
// 			if (!sprites[i][1]) type = e_3GridVer;
// 		if (type == e_3GridVer) return type;
// 	}
// 	return e_null;
// }

void Symbol::InitSprite(const FileLoader::Entry& entry, d2d::ISprite** pSprite, 
						const std::string& dir)
{
 	std::string filepath = entry.filepath;
	if (!d2d::FilenameTools::isExist(filepath)) {
 		filepath = d2d::FilenameTools::getAbsolutePath(dir, filepath);
	}
 
 	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
 	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
 	symbol->Release();
 
 	sprite->name = entry.name;

	sprite->rotate(entry.angle);
 	sprite->setMirror(entry.xMirror, entry.yMirror);
 
 	*pSprite = sprite;
}


}