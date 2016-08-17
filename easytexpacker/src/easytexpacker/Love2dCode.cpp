#include "Love2dCode.h"

#include "Context.h"
#include "StagePanel.h"

#include <ee/FetchAllVisitor.h>
#include <ee/FileHelper.h>
#include <ee/Symbol.h>
#include <ee/Sprite.h>

using namespace ebuilder;

namespace etexpacker
{

Love2dCode::Love2dCode(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
	
}

void Love2dCode::Resolve()
{
	std::string sw = ee::StringHelper::ToString(Context::Instance()->width),
		sh = ee::StringHelper::ToString(Context::Instance()->height);

	lua::TableAssign ta(m_gen, "quads");

	std::vector<ee::Sprite*> sprs;
	Context::Instance()->stage->TraverseSprites(ee::FetchAllVisitor<ee::Sprite>(sprs));
	for (size_t i = 0, n = sprs.size(); i < n; ++i)
	{
		ee::Sprite* spr = sprs[i];

		const ee::Symbol* sym = dynamic_cast<const ee::Symbol*>(spr->GetSymbol());
		const sm::vec2& pos = spr->GetPosition();

		std::string name = ee::FileHelper::GetFilename(sym->GetFilepath());

		std::string x, y, w, h, px, py, a;
		sm::vec2 sz = sym->GetBounding().Size();
		if (spr->GetAngle() != 0)
		{
			x = ee::StringHelper::ToString(pos.x - sz.y * 0.5f);
			y = ee::StringHelper::ToString(pos.y - sz.x * 0.5f);
			w = ee::StringHelper::ToString(sz.y);
			h = ee::StringHelper::ToString(sz.x);
			a = "1.57";
		}
		else
		{
			x = ee::StringHelper::ToString(pos.x - sz.x * 0.5f);
			y = ee::StringHelper::ToString(pos.y - sz.y * 0.5f);
			w = ee::StringHelper::ToString(sz.x);
			h = ee::StringHelper::ToString(sz.y);
			a = "0";
		}
		px = ee::StringHelper::ToString(sz.x * 0.5f);
		py = ee::StringHelper::ToString(480 - sz.y * 0.5f);

		// q = love.graphics.newQuad(x, y, w, h, sw, sh)
		std::string aq = lua::assign("q", lua::call("", "love.graphics.newQuad", 6, x, y, w, h, sw, sh));
		std::string ax = lua::assign("x", px);
		std::string ay = lua::assign("y", py);
		std::string aa = lua::assign("a", a);

		// name = { q = love.graphics.newQuad(x, y, w, h, sw, sh), 	x = px, y = py, a = a },
		m_gen.line(lua::assign(name, "{ " + aq + ", " + ax + ", " + ay + ", " + aa + " }") + ",");		
	}
}

}
