#ifndef FORMATION_RANDOM_PASTE_OP_H
#define FORMATION_RANDOM_PASTE_OP_H

#include <drag2d.h>

namespace formation
{
	class RandomPasteCMPT;

	class RandomPasteOP : public d2d::DrawRectangleOP
	{
	public:
		RandomPasteOP(d2d::EditPanel* editPanel, RandomPasteCMPT* cmpt);

		virtual bool onMouseLeftUp(int x, int y);

	private:
		RandomPasteCMPT* m_cmpt;

	}; // RandomPasteOP
}

#endif // FORMATION_RANDOM_PASTE_OP_H