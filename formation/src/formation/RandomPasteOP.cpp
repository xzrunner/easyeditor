#include "RandomPasteOP.h"
#include "RandomPasteCMPT.h"

#include "Context.h"
#include "StagePanel.h"

using namespace formation;

RandomPasteOP::RandomPasteOP(ee::EditPanel* editPanel, RandomPasteCMPT* cmpt)
	: ee::DrawRectangleOP(editPanel)
	, m_cmpt(cmpt)
{
}

bool RandomPasteOP::onMouseLeftUp(int x, int y)
{
	ee::Vector p0 = m_firstPos, p1 = m_currPos;

	if (ee::DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	if (!p0.isValid() || !p1.isValid()) return false;

	std::vector<ee::ISymbol*> symbols;
	Context::Instance()->library->traverse(ee::FetchAllVisitor<ee::ISymbol>(symbols));
	std::map<wxString, int>::iterator itr = m_cmpt->m_mapNameToNum.begin();
	for ( ; itr != m_cmpt->m_mapNameToNum.end(); ++itr)
	{
		ee::ISymbol* symbol = NULL;
		for (size_t i = 0, n = symbols.size(); i < n; ++i)
		{
			if (ee::FilenameTools::getFilename(symbols[i]->getFilepath()) == itr->first)
			{
				symbol = symbols[i];
				break;
			}
		}

		assert(symbol);
		for (size_t i = 0; i < itr->second; ++i)
		{
			ee::ISprite* sprite = ee::SpriteFactory::Instance()->create(symbol);
			float x = p0.x + (p1.x - p0.x) * ee::Random::getNum0To1(),
				y = p0.y + (p1.y - p0.y) * ee::Random::getNum0To1();
			sprite->setTransform(ee::Vector(x, y), sprite->getAngle());
			Context::Instance()->stage->insertSprite(sprite);
		}
	}

	return false;
}