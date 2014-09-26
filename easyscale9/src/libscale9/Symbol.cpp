#include "Symbol.h"
#include "Sprite.h"
#include "config.h"

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + wxVariant(id++);

	memset(m_sprites, 0, sizeof(int) * 9);

	m_width = m_height = 0;

	m_type = e_null;
}

Symbol::~Symbol()
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->release();
		}
	memset(m_sprites, 0, sizeof(int) * 9);
}

void Symbol::reloadTexture() const
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->getSymbol().reloadTexture();
		}
}

void Symbol::draw(const d2d::Screen& scr,
				  const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	const Sprite* scale9 = dynamic_cast<const Sprite*>(sprite);
// 	bool isResize = scale9 && ((scale9->width != m_width) || (scale9->height != m_height));
// 	float w = m_width, h = m_height;
// 	if (isResize) resize(scale9->width, scale9->height);

	switch (m_type)
	{
	case e_9Grid:
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (!m_sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(scr, m_sprites[i][j], mt, mul, add);
			}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (i == 1 && j == 1) continue;
				if (!m_sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(scr, m_sprites[i][j], mt, mul, add);
			}
			break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i)
		{
			if (!m_sprites[1][i]) continue;
			d2d::SpriteDraw::drawSprite(scr, m_sprites[1][i], mt, mul, add);
		}
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i)
		{
			if (!m_sprites[i][1]) continue;
			d2d::SpriteDraw::drawSprite(scr, m_sprites[i][1], mt, mul, add);
		}
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (!m_sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(scr, m_sprites[i][j], mt, mul, add);
			}
	}

//	if (isResize) resize(w, h);
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return d2d::Rect(m_width, m_height);
}

void Symbol::composeFromSprites(d2d::ISprite* sprites[3][3],
									  float width, float height)
{
	m_type = getType(sprites);
	if (m_type == e_null) return;

	m_width = width;
	m_height = height;
	switch (m_type)
	{
	case e_9Grid:
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (m_sprites[i][j]) m_sprites[i][j]->release();
				m_sprites[i][j] = sprites[i][j]->clone();
			}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (i == 1 && j == 1) continue;
				if (m_sprites[i][j]) m_sprites[i][j]->release();
				m_sprites[i][j] = sprites[i][j]->clone();
			}
			break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i)
		{
			if (m_sprites[1][i]) m_sprites[1][i]->release();
			m_sprites[1][i] = sprites[1][i]->clone();
		}
		m_height = m_sprites[1][0]->getSymbol().getSize().yLength();
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i)
		{
			if (m_sprites[i][1]) m_sprites[i][1]->release();
			m_sprites[i][1] = sprites[i][1]->clone();
		}
		m_width = m_sprites[0][1]->getSymbol().getSize().xLength();
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (m_sprites[i][j]) m_sprites[i][j]->release();
				m_sprites[i][j] = sprites[i][j]->clone();
			} 
		break;
	}

	composeFromSprites();
}

void Symbol::resize(float width, float height) const
{
	m_width = width;
	m_height = height;

	composeFromSprites();
}

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
			for (size_t i = 0; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					initSprite(adapter.m_data[i*3+j], &m_sprites[i][j], dir);
			break;
		case e_9GridHollow:
			for (size_t i = 0; i < 3; ++i) {
				for (size_t j = 0; j < 3; ++j) {
					if (i == 1 && j == 1) continue;
					if (i > 1 || i == 1 && j > 1)
						initSprite(adapter.m_data[i*3+j-1], &m_sprites[i][j], dir);
					else
						initSprite(adapter.m_data[i*3+j], &m_sprites[i][j], dir);
				}
			}
			break;
		case e_3GridHor:
			for (size_t i = 0; i < 3; ++i)
				initSprite(adapter.m_data[i], &m_sprites[1][i], dir);
			break;
		case e_3GridVer:
			for (size_t i = 0; i < 3; ++i)
				initSprite(adapter.m_data[i], &m_sprites[i][1], dir);
			break;
		case e_6GridUpper:
			for (size_t i = 1; i < 3; ++i)
				for (size_t j = 0; j < 3; ++j)
					initSprite(adapter.m_data[index++], &m_sprites[i][j], dir);
			break;
		}

		composeFromSprites();
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
				symbol->release();
				sprite->load(val);

				m_sprites[i][j] = sprite;
			}
		}
	}
}

void Symbol::composeFromSprites() const
{
	if (m_type == e_9Grid)
	{
		const float w0 = m_sprites[0][0]->getSymbol().getSize().xLength(),
			w2 = m_sprites[0][2]->getSymbol().getSize().xLength(),
			w1 = m_width - w0 - w2;
		const float h0 = m_sprites[0][0]->getSymbol().getSize().yLength(),
			h2 = m_sprites[0][2]->getSymbol().getSize().yLength(),
			h1 = m_height - h0 - h2;

		stretch(m_sprites[0][0], d2d::Vector(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		stretch(m_sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		stretch(m_sprites[0][2], d2d::Vector(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		stretch(m_sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		stretch(m_sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, h1);
		stretch(m_sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		stretch(m_sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		stretch(m_sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		stretch(m_sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (m_type == e_9GridHollow)
	{
		const float w0 = m_sprites[0][0]->getSymbol().getSize().xLength(),
			w2 = m_sprites[0][2]->getSymbol().getSize().xLength(),
			w1 = m_width - w0 - w2;
		const float h0 = m_sprites[0][0]->getSymbol().getSize().yLength(),
			h2 = m_sprites[0][2]->getSymbol().getSize().yLength(),
			h1 = m_height - h0 - h2;

		stretch(m_sprites[0][0], d2d::Vector(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		stretch(m_sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		stretch(m_sprites[0][2], d2d::Vector(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		stretch(m_sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		stretch(m_sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		stretch(m_sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		stretch(m_sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		stretch(m_sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (m_type == e_6GridUpper)
	{
		const float w0 = m_sprites[2][0]->getSymbol().getSize().xLength(),
			w2 = m_sprites[2][2]->getSymbol().getSize().xLength(),
			w1 = m_width - w0 - w2;
		const float h2 = m_sprites[2][0]->getSymbol().getSize().yLength(),
			h1 = m_height - h2;

		stretch(m_sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		stretch(m_sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, h1);
		stretch(m_sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		stretch(m_sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		stretch(m_sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		stretch(m_sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (m_type == e_3GridHor)
	{
		const float w0 = m_sprites[1][0]->getSymbol().getSize().xLength(),
			w2 = m_sprites[1][2]->getSymbol().getSize().xLength(),
			w1 = m_width - w0 - w2; 

		stretch(m_sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, m_height);
		stretch(m_sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, m_height);
		stretch(m_sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, m_height);
	}
	else if (m_type == e_3GridVer)
	{
		const float h0 = m_sprites[0][1]->getSymbol().getSize().yLength(),
			h2 = m_sprites[2][1]->getSymbol().getSize().yLength(),
			h1 = m_height - h0 - h2;

		stretch(m_sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), m_width, h0);
		stretch(m_sprites[1][1], d2d::Vector(0.0f, 0.0f), m_width, h1);
		stretch(m_sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), m_width, h2);
	}
}

Symbol::Type Symbol::getType(d2d::ISprite* sprites[3][3]) const
{
	{
		Type type = e_9Grid;
		for (size_t i = 0; i < 3 && type == e_9Grid; ++i)
			for (size_t j = 0; j < 3 && type == e_9Grid; ++j)
				if (!sprites[i][j]) type = e_null;
		if (type == e_9Grid) return type;
	}
	{
		Type type = e_9GridHollow;
		for (size_t i = 0; i < 3 && type == e_9GridHollow; ++i) {
			for (size_t j = 0; j < 3 && type == e_9GridHollow; ++j) {
				if (i == 1 && j == 1) continue;
				if (!sprites[i][j]) type = e_null;
			}
		}
		if (type == e_9GridHollow) return type;
	}
	{
		Type type = e_6GridUpper;
		for (size_t i = 1; i < 3 && type == e_6GridUpper; ++i)
			for (size_t j = 0; j < 3 && type == e_6GridUpper; ++j)
				if (!sprites[i][j]) type = e_null;
		if (type == e_6GridUpper) return type;
	}
	{
		Type type = e_3GridHor;
		for (size_t i = 0; i < 3 && type == e_3GridHor; ++i)
			if (!sprites[1][i]) type = e_null;
		if (type == e_3GridHor) return type;
	}
	{
		Type type = e_3GridVer;
		for (size_t i = 0; i < 3 && type == e_3GridVer; ++i)
			if (!sprites[i][1]) type = e_3GridVer;
		if (type == e_3GridVer) return type;
	}
	return e_null;
}

void Symbol::stretch(d2d::ISprite* sprite, const d2d::Vector& center, 
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

void Symbol::initSprite(const FileLoader::Entry& entry, d2d::ISprite** pSprite,
						const std::string& dir)
{
	std::string filepath = entry.filepath;
	if (!d2d::FilenameTools::isExist(filepath))
		filepath = d2d::FilenameTools::getAbsolutePath(dir, filepath);

	ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
	symbol->release();

	sprite->name = entry.name;

	sprite->setTransform(entry.pos, entry.angle);
	sprite->setScale(entry.xscale, entry.yscale);
	sprite->setShear(entry.xshear, entry.yshear);
	sprite->setMirror(entry.xMirror, entry.yMirror);

	*pSprite = sprite;
}

}