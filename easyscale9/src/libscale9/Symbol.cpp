#include "Symbol.h"
#include "Sprite.h"
#include "config.h"

namespace escale9
{

Symbol::Symbol()
{
	static int id = 0;
	m_name = FILE_TAG + wxVariant(id++);
}

Symbol::Symbol(const Symbol& symbol)
	: d2d::ISymbol(symbol)
	, m_data(symbol.m_data)
{
}

void Symbol::reloadTexture() const
{
	m_data.ReloadTexture();
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::Colorf& r_trans,
				  const d2d::Colorf& g_trans,
				  const d2d::Colorf& b_trans,
				  const d2d::ISprite* sprite/* = NULL*/) const
{
	m_data.Draw(mt, mul, add, r_trans, g_trans, b_trans, sprite);
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite/* = NULL*/) const
{
	float w, h;
	if (sprite) {
		const Sprite* spr = static_cast<const Sprite*>(sprite);
		spr->GetSize(w, h);
	} else {
		m_data.GetSize(w, h);
	}
	return d2d::Rect(w, h);
}

void Symbol::ResizeScale9(float width, float height)
{
	m_data.Resize(width, height);
}

void Symbol::ComposeFromSprites(d2d::ISprite* sprites[3][3], float width, float height)
{
	m_data.Compose(sprites, width, height);
}

void Symbol::loadResources()
{
	m_data.LoadFromFile(m_filepath.ToStdString());
}

}