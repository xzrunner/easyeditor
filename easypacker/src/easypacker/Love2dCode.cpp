#include "Love2dCode.h"

#include "Context.h"
#include "StagePanel.h"

using namespace epacker;
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

	std::vector<d2d::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		d2d::ISprite* sprite = sprites[i];

		const d2d::ISymbol& symbol = sprite->getSymbol();
		const d2d::Vector& pos = sprite->getPosition();

		std::string name = d2d::FilenameTools::getFilename(symbol.getFilepath());

		std::string x, y, w, h, px, py, a;
		if (sprite->getAngle() != 0)
		{
			x = wxString::FromDouble(pos.x - symbol.getSize().yLength() * 0.5f);
			y = wxString::FromDouble(pos.y - symbol.getSize().xLength() * 0.5f);
			w = wxString::FromDouble(symbol.getSize().yLength());
			h = wxString::FromDouble(symbol.getSize().xLength());
			a = "1.57";
		}
		else
		{
			x = wxString::FromDouble(pos.x - symbol.getSize().xLength() * 0.5f);
			y = wxString::FromDouble(pos.y - symbol.getSize().yLength() * 0.5f);
			w = wxString::FromDouble(symbol.getSize().xLength());
			h = wxString::FromDouble(symbol.getSize().yLength());
			a = "0";
		}
		px = wxString::FromDouble(symbol.getSize().xLength() * 0.5f);
		py = wxString::FromDouble(480 - symbol.getSize().yLength() * 0.5f);

		// q = love.graphics.newQuad(x, y, w, h, sw, sh)
		std::string aq = lua::assign("q", lua::call("", "love.graphics.newQuad", 6, x.c_str(), y.c_str(), w.c_str(), h.c_str(), sw.c_str(), sh.c_str()));
		std::string ax = lua::assign("x", px.c_str());
		std::string ay = lua::assign("y", py.c_str());
		std::string aa = lua::assign("a", a.c_str());

		// name = { q = love.graphics.newQuad(x, y, w, h, sw, sh), 	x = px, y = py, a = a },
		m_gen.line(lua::assign(name, "{ " + aq + ", " + ax + ", " + ay + ", " + aa + " }") + ",");		
	}
}