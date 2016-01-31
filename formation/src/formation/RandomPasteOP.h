#ifndef FORMATION_RANDOM_PASTE_OP_H
#define FORMATION_RANDOM_PASTE_OP_H



namespace formation
{
	class RandomPasteCMPT;

	class RandomPasteOP : public ee::DrawRectangleOP
	{
	public:
		RandomPasteOP(ee::EditPanel* editPanel, RandomPasteCMPT* cmpt);

		virtual bool onMouseLeftUp(int x, int y);

	private:
		RandomPasteCMPT* m_cmpt;

	}; // RandomPasteOP
}

#endif // FORMATION_RANDOM_PASTE_OP_H