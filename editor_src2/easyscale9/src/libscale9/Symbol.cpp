#include "Symbol.h"
#include "Sprite.h"

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = wxT("scale9") + wxVariant(id++);

	memset(m_sprites, 0, sizeof(int) * 9);

	m_width = m_height = 0;

	const float SCALE = 0.15f;
	const float WIDTH = 800, HEIGHT = 480;
	m_bitmap = new d2d::Bitmap(
		new wxBitmap(WIDTH * SCALE, HEIGHT * SCALE)
		);

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

void Symbol::loadFromTextFile(std::ifstream& fin)
{
}

void Symbol::storeToTextFile(std::ofstream& fout) const
{
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

void Symbol::draw(const d2d::Colorf& mul, const d2d::Colorf& add,
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
				d2d::SpriteDraw::drawSprite(m_sprites[i][j], mul, add);
			}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (i == 1 && j == 1) continue;
				if (!m_sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(m_sprites[i][j], mul, add);
			}
			break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i)
		{
			if (!m_sprites[1][i]) continue;
			d2d::SpriteDraw::drawSprite(m_sprites[1][i], mul, add);
		}
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i)
		{
			if (!m_sprites[i][1]) continue;
			d2d::SpriteDraw::drawSprite(m_sprites[i][1], mul, add);
		}
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
			{
				if (!m_sprites[i][j]) continue;
				d2d::SpriteDraw::drawSprite(m_sprites[i][j], mul, add);
			}
	}

//	if (isResize) resize(w, h);
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	return d2d::Rect(m_width, m_height);
}

void Symbol::refresh()
{
	ISymbol::refresh();
	refreshThumbnail();
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

void Symbol::loadResources()
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

void Symbol::refreshThumbnail()
{
	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.SetBackground(wxBrush(*wxWHITE));
	memDC.Clear();

	switch (m_type)
	{
	case e_9Grid:
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
				d2d::SpriteDraw::drawSprite(m_sprites[i][j], memDC);
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				d2d::SpriteDraw::drawSprite(m_sprites[i][j], memDC);
			}
		}
		break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i)
			d2d::SpriteDraw::drawSprite(m_sprites[1][i], memDC);
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i)
			d2d::SpriteDraw::drawSprite(m_sprites[i][1], memDC);
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
				d2d::SpriteDraw::drawSprite(m_sprites[i][j], memDC);
		break;
	}

	memDC.SelectObject(wxNullBitmap);
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
	assert(sprite->getSymbol().getSize().xLength() != 0
		&& sprite->getSymbol().getSize().yLength() != 0);

	sprite->setTransform(center, sprite->getAngle());
	const float sw = sprite->getSymbol().getSize().xLength(),
		sh = sprite->getSymbol().getSize().yLength();

	const float times = sprite->getAngle() / d2d::PI;
	if (times - (int)(times + 0.01f) < 0.3f)
		sprite->setScale(width / sw, height / sh);
	else
		sprite->setScale(height / sw, width / sh);
}

void Symbol::initSprite(const FileLoader::Entry& entry, d2d::ISprite** pSprite,
						const std::string& dir)
{
	std::string filepath = entry.filepath;
	if (!d2d::FilenameTools::isExist(filepath))
		filepath = d2d::FilenameTools::getAbsolutePath(dir, filepath);

	ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
	d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);

	sprite->name = entry.name;

	sprite->setTransform(entry.pos, entry.angle);
	sprite->setScale(entry.xscale, entry.yscale);
	sprite->setShear(entry.xshear, entry.yshear);
	sprite->setMirror(entry.xMirror, entry.yMirror);

	*pSprite = sprite;
}

}