#include "Love2dCode.h"

#include "Context.h"
#include "StagePanel.h"

using namespace etexpacker;
using namespace ebuilder;

Love2dCode::Love2dCode(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
	
}

void Love2dCode::resolve()
{
	std::string sw = wxString::FromDouble(Context::Instance()->width),
		sh = wxString::FromDouble(Context::Instance()->height);

	lua::TableAssign ta(m_gen, "quads");

	std::vector<d2d::Sprite*> sprites;
	Context::Instance()->stage->TraverseSprites(d2d::FetchAllVisitor<d2d::Sprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::Sprite* sprite = sprites[i];

		const d2d::Symbol& symbol = sprite->GetSymbol();
		const d2d::Vector& pos = sprite->GetPosition();

		std::string name = d2d::FileHelper::GetFilename(symbol.GetFilepath());

		std::string x, y, w, h, px, py, a;
		if (sprite->GetAngle() != 0)
		{
			x = wxString::FromDouble(pos.x - symbol.GetSize().Height() * 0.5f);
			y = wxString::FromDouble(pos.y - symbol.GetSize().Width() * 0.5f);
			w = wxString::FromDouble(symbol.GetSize().Height());
			h = wxString::FromDouble(symbol.GetSize().Width());
			a = "1.57";
		}
		else
		{
			x = wxString::FromDouble(pos.x - symbol.GetSize().Width() * 0.5f);
			y = wxString::FromDouble(pos.y - symbol.GetSize().Height() * 0.5f);
			w = wxString::FromDouble(symbol.GetSize().Width());
			h = wxString::FromDouble(symbol.GetSize().Height());
			a = "0";
		}
		px = wxString::FromDouble(symbol.GetSize().Width() * 0.5f);
		py = wxString::FromDouble(480 - symbol.GetSize().Height() * 0.5f);

		// q = love.graphics.newQuad(x, y, w, h, sw, sh)
		std::string aq = lua::assign("q", lua::call("", "love.graphics.newQuad", 6, x, y, w, h, sw, sh));
		std::string ax = lua::assign("x", px);
		std::string ay = lua::assign("y", py);
		std::string aa = lua::assign("a", a);

		// name = { q = love.graphics.newQuad(x, y, w, h, sw, sh), 	x = px, y = py, a = a },
		m_gen.line(lua::assign(name, "{ " + aq + ", " + ax + ", " + ay + ", " + aa + " }") + ",");		
	}
}