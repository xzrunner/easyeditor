#include "Scale9Data.h"
#include "Sprite.h"

namespace escale9
{

Scale9Data::Scale9Data()
	: m_width(0)
	, m_height(0)
	, m_type(e_null)
{
	memset(m_sprites, 0, sizeof(m_sprites));
}

Scale9Data::Scale9Data(const Scale9Data& data)
	: m_width(data.m_width)
	, m_height(data.m_height)
	, m_type(data.m_type)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			d2d::ISprite* src = data.m_sprites[i][j];
			if (src) {
				m_sprites[i][j] = src->clone();
			} else {
				m_sprites[i][j] = NULL;
			}
		}
	}

	ResizeScale9(m_type, m_sprites, m_width, m_height);
}

Scale9Data::~Scale9Data()
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (m_sprites[i][j]) {
				m_sprites[i][j]->Release();
			}
		}
	}
}

void Scale9Data::ReloadTexture() const
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (m_sprites[i][j]) {
				m_sprites[i][j]->getSymbol().reloadTexture();
			}
		}
	}
}

void Scale9Data::Draw(const d2d::Matrix& mt,
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

void Scale9Data::GetSize(float& width, float& height) const
{
	width = m_width;
	height = m_height;
}

void Scale9Data::Resize(float width, float height)
{
	if (m_width == width && m_height == height) {
		return;
	}

	m_width = width;
	m_height = height;
	ResizeScale9(m_type, m_sprites, m_width, m_height);	
}

void Scale9Data::Compose(d2d::ISprite* sprites[3][3], float width, float height)
{
	m_type = CheckType(sprites);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (m_sprites[i][j]) {
				m_sprites[i][j]->Release();
			}

			d2d::ISprite* src = sprites[i][j];
			if (src) {
				m_sprites[i][j] = src->clone();
			} else {
				m_sprites[i][j] = NULL;
			}
		}
	}

	Resize(width, height);
}

// todo 
void Scale9Data::LoadFromFile(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Json::Value spr_val = value["sprite"];

	std::string dir = d2d::FilenameTools::getFileDir(filepath);

	m_width = value["width"].asInt();
	m_height = value["height"].asInt();

	int index = 0;

	m_type = Scale9Type(value["type"].asInt());
	switch (m_type)
	{
	case e_9Grid:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				InitSprite(spr_val[i*3+j], &m_sprites[i][j], dir);
			}
		}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				if (i > 1 || i == 1 && j > 1)
					InitSprite(spr_val[i*3+j-1], &m_sprites[i][j], dir);
				else
					InitSprite(spr_val[i*3+j], &m_sprites[i][j], dir);
			}
		}
		break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i) {
			InitSprite(spr_val[i], &m_sprites[1][i], dir);
		}
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i) {
			InitSprite(spr_val[i], &m_sprites[i][1], dir);
		}
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				InitSprite(spr_val[index++], &m_sprites[i][j], dir);
			}
		}
		break;
	}

	ResizeScale9(m_type, m_sprites, m_width, m_height);
}

void Scale9Data::DrawScale9(Scale9Type type,
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
				d2d::SpriteRenderer::Instance()->Draw(sprites[i][j], mt, mul, add, r_trans, g_trans, b_trans);
			}
		}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				if (!sprites[i][j]) continue;
				d2d::SpriteRenderer::Instance()->Draw(sprites[i][j], mt, mul, add, r_trans, g_trans, b_trans);
			}
		}
		break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i) {
			if (!sprites[1][i]) continue;
			d2d::SpriteRenderer::Instance()->Draw(sprites[1][i], mt, mul, add, r_trans, g_trans, b_trans);
		}
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i) {
			if (!sprites[i][1]) continue;
			d2d::SpriteRenderer::Instance()->Draw(sprites[i][1], mt, mul, add, r_trans, g_trans, b_trans);
		}
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (!sprites[i][j]) continue;
				d2d::SpriteRenderer::Instance()->Draw(sprites[i][j], mt, mul, add, r_trans, g_trans, b_trans);
			}
		}
		break;
	}	
}

void Scale9Data::ResizeScale9(Scale9Type type, d2d::ISprite* const sprites[3][3],
							  float width, float height)
{
	if (type == e_9Grid) 
	{
		const float w0 = sprites[0][0]->getSymbol().getSize().xLength(),
			w2 = sprites[0][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2;
		const float h0 = sprites[0][0]->getSymbol().getSize().yLength(),
			h2 = sprites[0][2]->getSymbol().getSize().yLength(),
			h1 = height - h0 - h2;

		ResizeSprite(sprites[0][0], d2d::Vector(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		ResizeSprite(sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		ResizeSprite(sprites[0][2], d2d::Vector(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, h1);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);		
	}
	else if (type == e_9GridHollow)
	{
		const float w0 = sprites[0][0]->getSymbol().getSize().xLength(),
			w2 = sprites[0][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2;
		const float h0 = sprites[0][0]->getSymbol().getSize().yLength(),
			h2 = sprites[0][2]->getSymbol().getSize().yLength(),
			h1 = height - h0 - h2;

		ResizeSprite(sprites[0][0], d2d::Vector(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		ResizeSprite(sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		ResizeSprite(sprites[0][2], d2d::Vector(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (type == e_6GridUpper)
	{
		const float w0 = sprites[2][0]->getSymbol().getSize().xLength(),
			w2 = sprites[2][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2;
		const float h2 = sprites[2][0]->getSymbol().getSize().yLength(),
			h1 = height - h2;

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, h1);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (type == e_3GridHor)
	{
		const float w0 = sprites[1][0]->getSymbol().getSize().xLength(),
			w2 = sprites[1][2]->getSymbol().getSize().xLength(),
			w1 = width - w0 - w2; 

		ResizeSprite(sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, height);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, height);
		ResizeSprite(sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, height);
	}
	else if (type == e_3GridVer)
	{
		const float h0 = sprites[0][1]->getSymbol().getSize().yLength(),
			h2 = sprites[2][1]->getSymbol().getSize().yLength(),
			h1 = height - h0 - h2;

		ResizeSprite(sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), width, h0);
		ResizeSprite(sprites[1][1], d2d::Vector(0.0f, 0.0f), width, h1);
		ResizeSprite(sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), width, h2);
	}	
}

void Scale9Data::ResizeSprite(d2d::ISprite* sprite, const d2d::Vector& center, 
							  float width, float height)
{
	const d2d::ImageSymbol& symbol = dynamic_cast<const d2d::ImageSymbol&>(sprite->getSymbol());
	int w = symbol.getImage()->originWidth(),
		h = symbol.getImage()->originHeight();
	assert(w != 0 && h != 0);

	sprite->setTransform(center, sprite->getAngle());
	const float times = sprite->getAngle() / d2d::PI;
	if (times - (int)(times + 0.01f) < 0.3f)
		sprite->setScale(width / w, height / h);
	else
		sprite->setScale(height / w, width / h);

	sprite->translate(d2d::Math::rotateVector(sprite->getOffset(), sprite->getAngle()) - sprite->getOffset());
}

Scale9Type Scale9Data::CheckType(d2d::ISprite* sprites[3][3])
{
	Scale9Type type = e_null;

	do {
		// 9 grid
		type = e_9Grid;
		for (int i = 0; i < 3 && type == e_9Grid; ++i) {
			for (int j = 0; j < 3 && type == e_9Grid; ++j) {
				if (!sprites[i][j]) type = e_null;
			}
		}
		if (type != e_null) break;

		// 9 grid hollow
		type = e_9GridHollow;
		for (int i = 0; i < 3 && type == e_9GridHollow; ++i) {
			for (int j = 0; j < 3 && type == e_9GridHollow; ++j) {
				if (i == 1 && j == 1) continue;
				if (!sprites[i][j]) type = e_null;
			}
		}
		if (type != e_null) break;

		// 6 grid upper
		type = e_6GridUpper;
		for (int i = 1; i < 3 && type == e_6GridUpper; ++i) {
			for (int j = 0; j < 3 && type == e_6GridUpper; ++j) {
				if (!sprites[i][j]) type = e_null;
			}
		}
		if (type != e_null) break;

		// 3 grid hor
		type = e_3GridHor;
		for (int i = 0; i < 3 && type == e_3GridHor; ++i) {
			if (!sprites[1][i]) type = e_null;
		}
		if (type != e_null) break;

		// 3 grid ver
		type = e_3GridVer;
		for (int i = 0; i < 3 && type == e_3GridVer; ++i) {
			if (!sprites[i][1]) type = e_null;
		}
		if (type != e_null) break;
	} while (false);

	return type;
}

void Scale9Data::InitSprite(const Json::Value& spr_val, d2d::ISprite** pSprite, 
							const std::string& dir)
{
	wxString filepath = d2d::SymbolSearcher::GetSymbolPath(dir, spr_val);
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	if (!symbol) {
		std::string filepath = spr_val["filepath"].asString();
		throw d2d::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
			dir.c_str(), filepath.c_str());
	}
	d2d::SymbolSearcher::SetSymbolFilepaths(dir, symbol, spr_val);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	sprite->load(spr_val);
	symbol->Release();

	*pSprite = sprite;
}

}