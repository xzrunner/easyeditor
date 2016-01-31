#include "Love2dCode.h"

#include "Context.h"
#include "StagePanel.h"

using namespace formation;
using namespace ebuilder;

Love2dCode::Love2dCode(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
}

void Love2dCode::resolve()
{
	Context* context = Context::Instance();

	std::vector<ee::ISprite*> sprites;
	Context::Instance()->stage->traverseSprites(ee::FetchAllVisitor<ee::ISprite>(sprites));

	std::sort(sprites.begin(), sprites.end(), ee::SpriteCmp(ee::SpriteCmp::e_y));

	lua::TableAssign ta(m_gen, "");

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ee::ISprite* sprite = sprites[i];

		std::string name = ee::FilenameTools::getFilename(sprite->getSymbol().getFilepath());
		name = lua::assign("name", "\""+name+"\"");

		std::string time = wxString::FromDouble(sprite->getPosition().y / context->height * context->time, 2);
		time = lua::assign("time", time);

		std::string pos = wxString::FromDouble(sprite->getPosition().x / context->width, 3);
		pos = lua::assign("pos", pos);

		lua::tableassign(m_gen, "", 3, name.c_str(), time.c_str(), pos.c_str());
	}
}