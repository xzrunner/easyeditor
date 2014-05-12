#include "RandomPasteOP.h"
#include "RandomPasteCMPT.h"

#include "Context.h"
#include "StagePanel.h"

using namespace formation;

RandomPasteOP::RandomPasteOP(d2d::EditPanel* editPanel, RandomPasteCMPT* cmpt)
	: d2d::DrawRectangleOP(editPanel)
	, m_cmpt(cmpt)
{
}

bool RandomPasteOP::onMouseLeftUp(int x, int y)
{
	d2d::Vector p0 = m_firstPos, p1 = m_currPos;

	if (d2d::DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	if (!p0.isValid() || !p1.isValid()) return false;

	std::vector<d2d::ISymbol*> symbols;
	Context::Instance()->library->traverse(d2d::FetchAllVisitor<d2d::ISymbol>(symbols));
	std::map<wxString, int>::iterator itr = m_cmpt->m_mapNameToNum.begin();
	for ( ; itr != m_cmpt->m_mapNameToNum.end(); ++itr)
	{
		d2d::ISymbol* symbol = NULL;
		for (size_t i = 0, n = symbols.size(); i < n; ++i)
		{
			if (d2d::FilenameTools::getFilename(symbols[i]->getFilepath()) == itr->first)
			{
				symbol = symbols[i];
				break;
			}
		}

		assert(symbol);
		for (size_t i = 0; i < itr->second; ++i)
		{
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			float x = p0.x + (p1.x - p0.x) * d2d::Random::getNum0To1(),
				y = p0.y + (p1.y - p0.y) * d2d::Random::getNum0To1();
			sprite->setTransform(d2d::Vector(x, y), sprite->getAngle());
			Context::Instance()->stage->insertSprite(sprite);
		}
	}

	return false;
}