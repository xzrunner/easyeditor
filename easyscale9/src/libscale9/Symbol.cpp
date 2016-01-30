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
	: d2d::Symbol(symbol)
	, m_data(symbol.m_data)
{
}

void Symbol::ReloadTexture() const
{
	m_data.ReloadTexture();
}

void Symbol::Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color, 
				  const d2d::Sprite* spr, const d2d::Sprite* root) const
{
	m_data.Draw(mt, color, spr, root);
}

d2d::Rect Symbol::GetSize(const d2d::Sprite* sprite/* = NULL*/) const
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

void Symbol::ComposeFromSprites(d2d::Sprite* sprites[3][3], float width, float height)
{
	m_data.Compose(sprites, width, height);
}

void Symbol::LoadResources()
{
	m_data.LoadFromFile(m_filepath);
}

}